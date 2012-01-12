#!/usr/bin/python2

from __future__ import division
from sympy import *

xd = Symbol('x_des')
yd = Symbol('y_des')
zd = Symbol('z_des')

dx = Symbol('dx')
dy = Symbol('dy')
dz = Symbol('dz')

l1 = Symbol('l_1')
l2 = Symbol('l_2')

vals = [(xd, 0), (yd, 5), (zd, -10), (l1, 2.5), (l2, 10)]
dvals = [(dx, 5), (dy, 0), (dz, 0)]

r = sqrt(l2**2 - xd**2)
d = sqrt(zd**2 + yd**2)

psi  = atan2(zd, yd)
beta = acos((d**2 + l1**2 - r**2) / (2 * d * l1))

angle = psi + beta

print(angle)

print('respect to x')
dangle_x = diff(angle, xd)
print(dangle_x)

print('respect to y')
dangle_y = diff(angle, yd)
print(dangle_y)

print('respect to z')
dangle_z = diff(angle, zd)
print(dangle_z)

dangle = dangle_x * dx + dangle_y * dy + dangle_z * dz

subbed = angle.subs(vals)
dsubbed = dangle.subs(vals)
dsubbed = dsubbed.subs(dvals)


print('total')
print(dsubbed.evalf())
print(subbed.evalf())
