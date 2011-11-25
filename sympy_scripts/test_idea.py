#!/usr/bin/python2

from __future__ import division
from sympy import *

t = Symbol('t')
sx = Symbol('sx')
sy = Symbol('sy')
sz = Symbol('sz')
dx = Symbol('dx')
dy = Symbol('dy')
dz = Symbol('dz')
lx = sx + t * dx
ly = sy + t * dy
lz = sz + t * dz

arm_ul = 5
arm_ll = 7

lower_radius = sqrt(arm_ll ** 2 - ly ** 2)

dist     = sqrt(lx ** 2 + lz ** 2)
inv_dist = 1 / dist

alpha = (arm_ul - lower_radius ** 2 + dist ** 2) * 0.5 * inv_dist

x1 = (lx * alpha * inv_dist)
z1 = (lz * alpha * inv_dist)

h = sqrt(arm_ul ** 2 - alpha ** 2)

x2 = -lz * (h * inv_dist)
z2 =  lx * (h * inv_dist)

angle = atan2(z1 - z2, x1 - x2)

print (z1 - z2)
