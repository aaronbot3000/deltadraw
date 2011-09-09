#!/usr/bin/python2

from serial import Serial
from vectorize import Vectorizer
import struct, sys

#INPUT = '4.1.05.tiff'
#INPUT = 'test2.jpg'
#INPUT = 'test_text.png'
#INPUT = 'test4.png'
INPUT = 'test4.jpg'
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
BAUD        = 115200

END_DATA    = struct.pack('<IIb', 0xFFFF1111, 0xFFFF1111, 0x11)

def map_range(a, b1, b2, x1, x2):
    return (float(a - b1) / float(b2 - b1)) * (x2 - x1) + x1

def send_wait_ack(data):
    global serial
    serial.write(data)
    serial.read(1)

def main():
    global serial, MIN_Y, MAX_Y, MIN_X, MAX_X

    if not len(sys.argv) == 3:
        print 'Wrong args'
        sys.exit(0)
    
    serial = Serial(SERIAL_PORT, BAUD)
    vect   = Vectorizer()
    RES_X = int(sys.argv[1])

    polys, RES_Y = vect.get_polygons(sys.argv[2], RES_X)

    if RES_X > RES_Y:
        MIN_Y *= float(RES_Y) / RES_X
        MAX_Y *= float(RES_Y) / RES_X
    elif RES_X < RES_Y:
        MIN_X *= float(RES_X) / RES_Y
        MAX_X *= float(RES_X) / RES_Y

    print '\nThe image will contain %d points in %d polygons' %(vect.pointc, vect.polyc)

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
        next_x = -map_range(cur_p[0][0], 0, RES_X, MIN_X, MAX_X)
        next_y = map_range(cur_p[0][1], 0, RES_Y, MIN_Y, MAX_Y)
        data = struct.pack('<ffb', next_x, next_y, MOVE_POINT);
        send_wait_ack(data)

        # Send the polygon
        for point in cur_p:
            next_x = -map_range(point[0], 0, RES_X, MIN_X, MAX_X)
            next_y = map_range(point[1], 0, RES_Y, MIN_Y, MAX_Y)
            data = struct.pack('<ffb', next_x, next_y, DRAW_POINT);
            send_wait_ack(data)

        next_x = -map_range(cur_p[-1][0], 0, RES_X, MIN_X, MAX_X)
        next_y = map_range(cur_p[-1][1], 0, RES_Y, MIN_Y, MAX_Y)
        data = struct.pack('<ffb', next_x, next_y, MOVE_POINT);
        send_wait_ack(data)

        print 'Sent polygon #%d\n' %poly_count
        poly_count += 1
        cur_p = cur_p.h_next()

    # Send end of transmission
    send_wait_ack(END_DATA)

    raw_input("press enter to continue")
    vect.del_mem()


if __name__ == '__main__':
    main()
