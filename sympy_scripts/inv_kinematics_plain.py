#!/usr/bin/python2

from __future__ import division
from sympy import *

xd = Symbol('x_des')
yd = Symbol('y_des')
zd = Symbol('z_des')

l1 = Symbol('l_1')
l2 = Symbol('l_2')

vals = [(xd, 0), (yd, 5), (zd, -10), (l1, 2.5), (l2, 10)]

lower_radius = sqrt(l2**2 - xd**2)

dist = sqrt(yd**2 + zd**2)
inv_dist = 1 / dist;

alpha = (l1**2 - l2**2 + dist**2) * 0.5 * inv_dist;

y1 = (yd * alpha * inv_dist);
z1 = (zd * alpha * inv_dist);

h = sqrt(l1**2 - alpha**2);

y2 = -zd * (h * inv_dist);
z2 =  yd * (h * inv_dist);

angle = atan2(z1 + z2, y1 + y2);
print(angle.subs(vals).evalf())
