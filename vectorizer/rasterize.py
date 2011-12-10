#!/usr/bin/python2

import cv, sys

TEST_IMAGE = '003Venusaur.png'
MOVE_POINT  = 1
DRAW_POINT  = 2

OVERLAP = 1.09
MAX_DIFF = 0.007

WHITE_THRESH = 0.85
AMP_THRESH = 0.7

class Rasterizer:

    __nwidth = 120

    # inches
    __stroke_width = 0.0115
    __gamma = 2.2

    def __map_range(self, a, b1, b2, x1, x2):
        return (float(a - b1) / float(b2 - b1)) * (x2 - x1) + x1

    def get_lines(self, image_name, MIN_X, MAX_X, MIN_Y, MAX_Y):
        cv.NamedWindow('Output', cv.CV_WINDOW_NORMAL)

        self.__plotw = MAX_X - MIN_X
        self.__ploth = MAX_Y - MIN_Y
        self.__load_image(image_name)
        cv.CreateTrackbar('width', 'Output', self.__nwidth, self.__gray.cols, self.__width_callback)
        cv.CreateTrackbar('gamma', 'Output', 220, 400, self.__gamma_callback)

        self.__width_callback(self.__nwidth)

        key = cv.WaitKey()
        while not (key == ord('q') or key == ord('e')):
            key = cv.WaitKey()
            print key
        if key == ord('e'):
            sys.exit(0)

        return self.__points

    def __fill_pixels(self, r, c):
        fulld = self.__pixw / self.__stroke_width

        # Get the average of the pixels
        value = cv.Get2D(self.__resized, r, c)[0]
        actual = self.__map_range(value, 0, 255, 0, 1) ** (1 / self.__gamma)

        points = []

        if actual > WHITE_THRESH:
            cv.Set2D(self.__output, r, c, 255)
            return []
        elif actual > AMP_THRESH:
            up = self.__pixh * self.__map_range(actual, AMP_THRESH, WHITE_THRESH, (1 - OVERLAP), 0.5)
            down = self.__pixh * self.__map_range(actual, AMP_THRESH, WHITE_THRESH, OVERLAP, 0.5)

            cv.Set2D(self.__output, r, c, int(255 * actual))

            points.append((self.__pixw / 2.0, down))
            points.append((self.__pixw, up))
        else:
            linecount = int(self.__map_range(actual, 0, AMP_THRESH, fulld, 1))
            cv.Set2D(self.__output, r, c, 
                int(AMP_THRESH*255 - ((linecount-1) / float(fulld-1)) ** (1/2.2) * AMP_THRESH*255))

            gap = float(self.__pixw) / linecount
            up = self.__pixh * (1 - OVERLAP)
            down = self.__pixh * OVERLAP

            # Here is where the magic happens
            for i in range(linecount):
                points.append((gap * (i + 0.5), down))
                points.append((gap * (i + 1), up))
        return points

    def __fill_image(self):
        last_was_move = True
        points = []
        for i in range(self.__resized.rows):
        #for i in range(10):
            j = 0
            for j in range(self.__resized.cols):
                value = cv.Get2D(self.__resized, i, j)[0]

                pixels = self.__fill_pixels(i, j)
                shifted_pixel = []
                if pixels:
                    if last_was_move:
                        shifted_pixel.append((-self.__draww/2 + pixels[0][0] + j * self.__pixw, 
                                             -(-self.__drawh/2 + pixels[0][1] + i * self.__pixh),
                                             MOVE_POINT))
                        last_was_move = False
                    shifted = [(-self.__draww/2 + x[0] + j * self.__pixw, 
                               -(-self.__drawh/2 + x[1] + i * self.__pixh),
                               DRAW_POINT) for x in pixels]
                    shifted_pixel.extend(shifted)
                else:
                    if points and not last_was_move:
                        shifted_pixel = [(points[-1][0], points[-1][1], MOVE_POINT)]
                        last_was_move = True
                points.extend(shifted_pixel)

            # Move pen to next line
            if not last_was_move:
                shifted_pixel = [(points[-1][0], points[-1][1], MOVE_POINT)]
                points.extend(shifted_pixel)
                last_was_move = True

        return points
    
    def __load_image(self, image_name):
        orig = cv.LoadImageM(image_name)
        self.__gray = cv.CreateMat(orig.rows, orig.cols, cv.CV_8UC1)
        cv.CvtColor(orig, self.__gray, cv.CV_RGB2GRAY)

        if orig.cols > orig.rows:
            self.__draww = float(self.__plotw)
            self.__drawh = float(orig.rows) / orig.cols * self.__plotw
        else:
            self.__draww = float(orig.cols) / orig.rows * self.__ploth
            self.__drawh = float(self.__ploth)

    def __resize_image(self, newX):
        newY = int(float(newX) / self.__gray.cols * self.__gray.rows)
        if '__resized' in dir(self):
            del self.__resized

        if '__output' in dir(self):
            del self.__output

        self.__resized = cv.CreateMat(newY, newX, cv.CV_8UC1)
        self.__output = cv.CreateMat(newY, newX, cv.CV_8UC1)
        cv.Resize(self.__gray, self.__resized)
        
        self.__pixw = self.__draww / self.__resized.cols
        self.__pixh = self.__drawh / self.__resized.rows

        self.__refill_image()

    def __refill_image(self):
        # Filling image here
        self.__points = self.__fill_image()

        cv.ShowImage('Output', self.__output)


    def __width_callback(self, value):
        self.__resize_image(value + 1)

    def __gamma_callback(self, value):
        self.__gamma = float(value) / 100
        self.__refill_image()

def main():
    a = Rasterizer()
    points = a.get_lines(TEST_IMAGE, -3.75, 3.75, -3.75, 3.75)
    #for i in range(100):
        #print points[i]

if __name__ == '__main__':
    main()
