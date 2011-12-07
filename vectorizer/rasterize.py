#!/usr/bin/python2

import cv, sys

TEST_IMAGE = 'lena.bmp'
MOVE_POINT  = 1
DRAW_POINT  = 2

class Rasterizer:

    __nwidth = 60
    __resized = 0

    # inches
    __stroke_width = 0.0115

    def __map_range(self, a, b1, b2, x1, x2):
        return (float(a - b1) / float(b2 - b1)) * (x2 - x1) + x1

    def get_lines(self, image_name, MIN_X, MAX_X, MIN_Y, MAX_Y):
        cv.NamedWindow('Output', cv.CV_WINDOW_NORMAL)

        self.__plotw = MAX_X - MIN_X
        self.__ploth = MAX_Y - MIN_Y
        self.__load_image(image_name)
        cv.CreateTrackbar('width', 'Output', self.__nwidth, self.__gray.cols, self.__width_callback)

        self.__width_callback(60)

        key = cv.WaitKey()
        while not (key == ord('q') or key == ord('e')):
            key = cv.WaitKey()
            print key

        points = []
        last_was_move = True
        for i in range(self.__resized.rows):
            for j in range(self.__resized.cols):
                pixel = self.__fill_pixel(i, j)
                shifted_pixel = []
                if pixel:
                    if last_was_move:
                        shifted_pixel.append((-self.__draww/2 + pixel[0][0] + j * self.__pixw, 
                                             -(-self.__drawh/2 + pixel[0][1] + i * self.__pixh),
                                             MOVE_POINT))
                        last_was_move = False
                    shifted_pixel.extend([(-self.__draww/2 + x[0] + j * self.__pixw, 
                                      -(-self.__drawh/2 + x[1] + i * self.__pixh),
                                      DRAW_POINT) for x in pixel])
                else:
                    if points and not last_was_move:
                        shifted_pixel = [(points[-1][0], points[-1][1], MOVE_POINT)]
                        last_was_move = True
                points.extend(shifted_pixel)

            if not last_was_move:
                shifted_pixel = [(points[-1][0], points[-1][1], MOVE_POINT)]
                points.extend(shifted_pixel)
                last_was_move = True

        return points

    def __fill_pixel(self, r, c):
        fulld = self.__pixw / self.__stroke_width
        value = cv.Get2D(self.__resized, r, c)[0]
        # Gamma correction
        actual = self.__map_range(value, 0, 255, 0, 1) ** (1 / 2.2)
        density = int(self.__map_range(actual, 0, 1, fulld, 0))
        if not density or density < 0:
            return []

        gap = self.__pixw / density

        points = []
        
        # Here is where the magic happens
        for i in range(density):
            points.append((gap * i, 0))
            points.append((gap * i, self.__pixh))
        return points
    
    def __load_image(self, image_name):
        orig = cv.LoadImageM(image_name)
        self.__gray = cv.CreateMat(orig.rows, orig.cols, cv.CV_8UC1)
        cv.CvtColor(orig, self.__gray, cv.CV_RGB2GRAY)

        if orig.cols > orig.rows:
            self.__draww = self.__plotw
            self.__drawh = float(orig.cols) / orig.rows * self.__plotw
        else:
            self.__draww = float(orig.cols) / orig.rows * self.__ploth
            self.__drawh = self.__ploth

    def __resize_image(self, newX):
        newY = int(float(newX) / self.__gray.cols * self.__gray.rows)
        if (self.__resized):
            del self.__resized

        self.__resized = cv.CreateMat(newY, newX, cv.CV_8UC1)
        self.__gamma_c = cv.CreateMat(newY, newX, cv.CV_8UC1)
        cv.Resize(self.__gray, self.__resized)
        
        self.__pixw = self.__draww / self.__resized.cols
        self.__pixh = self.__drawh / self.__resized.rows
        cv.ShowImage('Output', self.__resized)

    def __width_callback(self, value):
        self.__resize_image(value + 1)

def main():
    a = Rasterizer()
    points = a.get_lines(TEST_IMAGE, -3.5, 3.5, -3.5, 3.5)
    for p in points:
        print p

if __name__ == '__main__':
    main()
