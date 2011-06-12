#!/usr/bin/python2

from __future__ import division
from sympy import *

# Generates a range for floats
def frange(start, stop, step):
    r = start
    while r < stop:
        yield r
        r += step

ul_val = 4
ll_val = 7

radius = 6
tx_val = -1
ty_val = 0
tz_val = 8

ul = Symbol('U')
ll = Symbol('L')

tx = Symbol('TX')
ty = Symbol('TY')
tz = Symbol('TZ')

dist = sqrt(tx**2 + ty**2 + tz**2)
alpha = (ll**2 - ul**2 - dist**2) / 2

jx1 = (sqrt(tx**2 * tz**2 * ul**2 + tz**4 * ul**2 - tz**2 * alpha**2) - tx * alpha)/(tx + tz**2)

jx2 = (-sqrt(tx**2 * tz**2 * ul**2 + tz**4 * ul**2 - tz**2 * alpha**2) - tx * alpha)/(tx + tz**2)

jz1 = acos(jx1/ul)
jz2 = acos(jx2/ul)

jx1 = jx1.subs([
    (ul, ul_val), 
    (ll, ll_val),
    (tz, tz_val)])

jx2 = jx2.subs([
    (ul, ul_val), 
    (ll, ll_val),
    (tz, tz_val)])

for theta in frange(0, 2*pi.evalf(), 0.1):
    flag = 0
    tx_val = radius * cos(theta) - 1.5
    ty_val = radius * sin(theta)

    jx_val1 = jx1.subs([
        (tx, tx_val),
        (ty, ty_val)])

    jx_val2 = jx2.subs([
        (tx, tx_val),
        (ty, ty_val)])

    print 'theta %.2f' % (theta * 180 / pi).evalf()
    print 'xpos %.2f' % tx_val.evalf()
    print 'ypos %.2f' % ty_val.evalf()

    if jx_val1.evalf().is_real:
        print 'jx1: %.2f' % jx_val1.evalf()
        flag += 1
    if jx_val2.evalf().is_real:
        print 'jx2: %.2f' % jx_val2.evalf()
        flag += 1
    if not flag:
        print 'NO REAL ANSWER'

    print '\n'
