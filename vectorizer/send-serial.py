#!/usr/bin/python2

from serial import Serial
from vectorize import Vectorizer
from rasterize import Rasterizer
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

MAX_X =  3.75
MIN_X = -3.75

MAX_Y =  3.75
MIN_Y = -3.75

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
    
    RES_X = int(sys.argv[1])

    #data   = Vectorizer()
    #points = data.get_polygons(sys.argv[2], RES_X, MIN_X, MAX_X, MIN_Y, MAX_Y)
    data   = Rasterizer()
    points = data.get_lines(sys.argv[2], MIN_X, MAX_X, MIN_Y, MAX_Y)

    serial = Serial(SERIAL_PORT, BAUD)
    serial.flushInput();
    serial.flushOutput();

    print 'Waiting for MCU'

    # Wait until the mcu sends a byte, signaling it's ready
    serial.read(1)

    print 'Starting transmission'

    count = 1
    for cur_p in points:
        next_x = cur_p[0]
        next_y = cur_p[1]
        next_z = cur_p[2]
        data = struct.pack('<ffb', next_x, next_y, next_z);
        send_wait_ack(data)

        print 'Sent point %d of %d\n' %(count, len(points))
        count += 1

    # Send end of transmission
    send_wait_ack(END_DATA)

    raw_input("press enter to continue")

if __name__ == '__main__':
    main()
