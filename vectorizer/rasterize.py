#!/usr/bin/python2

import cv, sys

TEST_IMAGE = 'lena.bmp'

class Rasterizer:
    def map_range(a, b1, b2, x1, x2):
        return (float(a - b1) / float(b2 - b1)) * (x2 - x1) + x1

    def get_lines(self, image_name):
        cv.NamedWindow('Output', cv.CV_WINDOW_NORMAL)
        cv.CreateTrackbar('width', 'Output', self.__nwidth, , self.__width_callback)
        cv.CreateTrackbar('height', 'Output', self.__nheight, , self.__height_callback)


        cv.ShowImage('Contours', self.contour_out)
    
    def __load_image(self, image_name, newX):
        orig = cv.LoadImageM(image_name)
        
        self.__gray = cv.CreateMat(orig.cols, orig.rows, cv.CV_8UC1)

        cv.CvtColor(orig, self.__gray, cv.CV_RGB2GRAY)

def main():
    a = Rasterizer()
    a.get_lines(TEST_IMAGE)

if __name__ == '__main__':
    main()
