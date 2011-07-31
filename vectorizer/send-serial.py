#!/usr/bin/python2

from serial import Serial
from vectorize import Vectorizer

#INPUT = '4.1.05.tiff'
INPUT = 'test2.jpg'
#INPUT = 'test4.png'
#INPUT = 'test4.jpg'
#INPUT = 'test5.jpg'
#INPUT = 'test3.png'
#INPUT = 'lena.bmp'
#INPUT = 'test_pattern.jpg'
RES_X = 160

SERIAL_PORT = '/dev/ttyACM0'
BAUD        = 115200

def main():
    global serial, vectorizer
    serial = Serial(SERIAL_PORT, BAUD)
    vect   = Vectorizer()

    polys, height = vect.get_polygons(INPUT, RES_X)
    print height
    con = polys
    pointc = 0
    polyc = 0
    while not con == None:
        pointc += len(con)
        polyc += 1
        con = con.h_next()
    print '\nThe image will contain %d points in %d polygons' %(pointc, polyc)


if __name__ == '__main__':
    main()
