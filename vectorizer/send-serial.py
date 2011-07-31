#!/usr/bin/python2

from serial import Serial
from vectorize import Vectorizer
import struct

#INPUT = '4.1.05.tiff'
#INPUT = 'test2.jpg'
INPUT = 'test_text.png'
#INPUT = 'test4.png'
#INPUT = 'test4.jpg'
#INPUT = 'test5.jpg'
#INPUT = 'test3.png'
#INPUT = 'lena.bmp'
#INPUT = 'test_pattern.jpg'
RES_X = 160

MAX_X =  3
MIN_X = -3

MAX_Y =  3
MIN_Y = -3

MOVE_POINT  = 1
DRAW_POINT  = 2

SERIAL_PORT = '/dev/ttyACM0'
BAUD        = 9600

END_DATA    = struct.pack('<IIb', 0xFFFF1111, 0xFFFF1111, 0x11)

def map_range(a, b1, b2, x1, x2):
    return (float(a - b1) / float(b2 - b1)) * (x2 - x1) + x1

def send_wait_ack(data):
    global serial
    serial.write(data)
    serial.read(1)

def main():
    global serial
    serial = Serial(SERIAL_PORT, BAUD)
    vect   = Vectorizer()

    polys, RES_Y = vect.get_polygons(INPUT, RES_X)
    print RES_Y
    con = polys
    pointc = 0
    polyc = 0
    while con:
        pointc += len(con)
        polyc += 1
        con = con.h_next()
    print '\nThe image will contain %d points in %d polygons' %(pointc, polyc)

    serial.flushInput();
    serial.flushOutput();

    print 'Waiting for MCU'

    # Wait until the mcu sends a byte, signaling it's ready
    serial.read(1)

    print 'Starting transmission'

    cur_p = polys
    poly_count = 1
    while cur_p:
        print 'Starting polygon #%d' %poly_count

        # Move to the first point in the polygon
        next_x = map_range(cur_p[0][0], 0, RES_X, MIN_X, MAX_X)
        next_y = map_range(cur_p[0][1], 0, RES_Y, MIN_Y, MAX_Y)
        data = struct.pack('<ffb', next_x, next_y, MOVE_POINT);
        send_wait_ack(data)

        # Send the polygon
        for point in cur_p:
            next_x = map_range(point[0], 0, RES_X, MIN_X, MAX_X)
            next_y = map_range(point[1], 0, RES_Y, MIN_Y, MAX_Y)
            data = struct.pack('<ffb', next_x, next_y, DRAW_POINT);
            send_wait_ack(data)

        next_x = map_range(cur_p[-1][0], 0, RES_X, MIN_X, MAX_X)
        next_y = map_range(cur_p[-1][1], 0, RES_Y, MIN_Y, MAX_Y)
        data = struct.pack('<ffb', next_x, next_y, MOVE_POINT);
        send_wait_ack(data)

        print 'Sent polygon #%d\n' %poly_count
        poly_count += 1
        cur_p = cur_p.h_next()

    # Send end of transmission
    send_wait_ack(END_DATA)

    raw_input("press any key to continue")


if __name__ == '__main__':
    main()
