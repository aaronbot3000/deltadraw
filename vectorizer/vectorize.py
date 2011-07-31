#!/usr/bin/python2

import cv, sys

INPUT = 'lena.bmp'
RES_X = 160

class Vectorizer:
    # Vectorization settings
    __canny_lo = 30
    __canny_hi = 70
    __canny_apeture = 0
    __smooth_val = 3
    __poly_acc = 300

    def get_polygons(self, image_name, newX):
        cv.NamedWindow('Canny', cv.CV_WINDOW_NORMAL)
        cv.NamedWindow('Contours', cv.CV_WINDOW_NORMAL)
        cv.CreateTrackbar('smooth', 'Canny', self.__smooth_val, 10, self.__smooth_val_callback)
        cv.CreateTrackbar('apeture', 'Canny', self.__canny_apeture, 2, self.__canny_apeture_callback)
        cv.CreateTrackbar('lowThreshold',  'Canny', self.__canny_lo, 100, self.__canny_lo_callback)
        cv.CreateTrackbar('highThreshold', 'Canny', self.__canny_hi, 100, self.__canny_hi_callback)
        cv.CreateTrackbar('poly acc', 'Contours', self.__poly_acc, 500, self.__poly_acc_callback)

        self.__load_image(image_name, newX)
        self.__smooth_val_callback(self.__smooth_val)

        key = cv.WaitKey()
        while not (key == ord('q') or key == ord('e')):
            key = cv.WaitKey()

        self.__del_most_mem()
        if key == ord('e'):
            sys.exit(0)

        return self.polys_out, self.__newY

    def __load_image(self, image_name, newX):
        orig = cv.LoadImageM(image_name)
        self.__newY = int(float(newX) / orig.cols * orig.rows)
        self.__init_mem(newX, self.__newY)

        cv.Resize(orig, self.res)
        cv.CvtColor(self.res, self.__res_gray, cv.CV_RGB2GRAY)

    def __init_mem(self, newX, newY):
        self.res = cv.CreateMat(newY, newX, cv.CV_8UC3)
        self.__res_gray = cv.CreateMat(newY, newX, cv.CV_8UC1)
        self.res_smooth = cv.CreateMat(newY, newX, cv.CV_8UC1)
        self.canny = cv.CreateMat(newY, newX, cv.CV_8UC1)
        self.contour_in = cv.CreateMat(newY, newX, cv.CV_8UC1)
        self.contour_out = cv.CreateMat(newY, newX, cv.CV_8UC3)
        self.c_storage = cv.CreateMemStorage()
        self.a_storage = cv.CreateMemStorage()

    def __del_most_mem(self):
        del self.__res_gray
        del self.res_smooth
        del self.canny
        del self.contour_in
        del self.contour_out
        del self.contours
        del self.c_storage
        cv.DestroyAllWindows()

    def __refresh_poly(self):
        self.polys_out = cv.ApproxPoly(self.contours, self.a_storage, cv.CV_POLY_APPROX_DP, self.__poly_acc / 100.0, -1)

        # Prints a count of the number of polygons and points in the picture thingy
        con = self.polys_out
        self.pointc = 0
        self.polyc = 0
        while not con == None:
            self.pointc += len(con)
            self.polyc += 1
            con = con.h_next()
        print '\n%d polygons'%self.polyc
        print '%d points'%self.pointc

        cv.Set(self.contour_out, cv.ScalarAll(255))
        cv.DrawContours(self.contour_out, self.polys_out, cv.Scalar(0, 0, 0), cv.Scalar(0, 0, 0), 99)

        cv.ShowImage('Contours', self.contour_out)

    def __refresh_canny(self):
        cv.Canny(self.res_smooth, self.canny, self.__canny_lo, self.__canny_hi, self.__canny_apeture * 2 + 3)
        cv.ShowImage('Canny', self.canny)
        cv.Copy(self.canny, self.contour_in)
        self.contours = cv.FindContours(self.contour_in, self.c_storage, cv.CV_RETR_LIST, cv.CV_CHAIN_APPROX_NONE)
        self.__refresh_poly()

    def __poly_acc_callback(self, value):
        self.__poly_acc = value
        self.__refresh_poly()

    def __smooth_val_callback(self, value):
        self.__smooth_val = value
        cv.Smooth(self.__res_gray, self.res_smooth, cv.CV_GAUSSIAN, self.__smooth_val * 2 + 1, self.__smooth_val * 2 + 1)
        self.__refresh_canny()

    def __canny_lo_callback(self, value):
        self.__canny_lo = value
        self.__refresh_canny()

    def __canny_hi_callback(self, value):
        self.__canny_hi = value
        self.__refresh_canny()

    def __canny_apeture_callback(self, value):
        self.__canny_apeture = value
        self.__refresh_canny()

def main():
    a = Vectorizer()
    a.get_polygons(INPUT, RES_X)
    a.get_polygons(INPUT, RES_X)

if __name__ == '__main__':
    main()
