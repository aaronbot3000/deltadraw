#!/usr/bin/python2

from __future__ import division
from sympy import *

# Generates a range for floats
def frange(start, stop, step):
    r = start
    while r < stop:
        yield r
        r += step

# Arm lengths
ul_val = 8
ll_val = 10

# Target location
radius = 6 * sqrt(2)
tz_val = 12

offset_matrix = Matrix([-2, 0])

# Symbols
ul = Symbol('U')
ll = Symbol('L')

tx = Symbol('TX')
ty = Symbol('TY')
tz = Symbol('TZ')

arm_theta = Symbol('at')

# Functions
rot_matrix = Matrix([(cos(arm_theta), -sin(arm_theta)), (sin(arm_theta), cos(arm_theta))])

dist = sqrt(tx**2 + ty**2 + tz**2)
alpha = (ll**2 - ul**2 - dist**2) / 2

jx1 = (sqrt(tx**2 * tz**2 * ul**2 + tz**4 * ul**2 - tz**2 * alpha**2) 
        - tx * alpha)/(tx**2 + tz**2)

jx2 = (-sqrt(tx**2 * tz**2 * ul**2 + tz**4 * ul**2 - tz**2 * alpha**2) 
        - tx * alpha)/(tx**2 + tz**2)

jz1 = acos(jx1/ul)
jz2 = acos(jx2/ul)

# Initial substitution
jx1 = jx1.subs([
    (ul, ul_val), 
    (ll, ll_val),
    (tz, tz_val)])

jx2 = jx2.subs([
    (ul, ul_val), 
    (ll, ll_val),
    (tz, tz_val)])

# Making the spiral
for theta in frange(0, 2*pi.evalf(), 0.1):
    print 'theta %.2f' % (theta * 180 / pi).evalf()

    #target = Matrix([radius * theta / (2 * pi) * cos(theta), radius * theta / (2 * pi) * sin(theta)])
    target = Matrix([radius * cos(theta), radius * sin(theta)])

    flag = 0
    target_val = (rot_matrix * target + offset_matrix).subs([(arm_theta, 0)])

    jx_val1 = jx1.subs([
        (tx, target_val[0]),
        (ty, target_val[1])])

    jx_val2 = jx2.subs([
        (tx, target_val[0]),
        (ty, target_val[1])])

    print 'Arm 1'
    print 'xpos %.2f' % target_val[0].evalf()
    print 'ypos %.2f' % target_val[1].evalf()

    if jx_val1.evalf().is_real:
        print 'jx1: %.2f' % jx_val1.evalf()
        flag += 1
    if jx_val2.evalf().is_real:
        print 'jx2: %.2f' % jx_val2.evalf()
        flag += 1
    if not flag:
        print 'NO REAL ANSWER'

    flag = 0
    target_val = (rot_matrix * target + offset_matrix).subs([(arm_theta, 2 * pi / 3)])

    jx_val1 = jx1.subs([
        (tx, target_val[0]),
        (ty, target_val[1])])

    jx_val2 = jx2.subs([
        (tx, target_val[0]),
        (ty, target_val[1])])

    print 'Arm 2'
    print 'xpos %.2f' % target_val[0].evalf()
    print 'ypos %.2f' % target_val[1].evalf()

    if jx_val1.evalf().is_real:
        print 'jx1: %.2f' % jx_val1.evalf()
        flag += 1
    if jx_val2.evalf().is_real:
        print 'jx2: %.2f' % jx_val2.evalf()
        flag += 1
    if not flag:
        print 'NO REAL ANSWER'

    flag = 0
    target_val = (rot_matrix * target + offset_matrix).subs([(arm_theta, 4 * pi / 3)])

    jx_val1 = jx1.subs([
        (tx, target_val[0]),
        (ty, target_val[1])])

    jx_val2 = jx2.subs([
        (tx, target_val[0]),
        (ty, target_val[1])])

    print 'Arm 3'
    print 'xpos %.2f' % target_val[0].evalf()
    print 'ypos %.2f' % target_val[1].evalf()

    if jx_val1.evalf().is_real:
        print 'jx1: %.2f' % jx_val1.evalf()
        flag += 1
    if jx_val2.evalf().is_real:
        print 'jx2: %.2f' % jx_val2.evalf()
        flag += 1
    if not flag:
        print 'NO REAL ANSWER'
    print '\n'
