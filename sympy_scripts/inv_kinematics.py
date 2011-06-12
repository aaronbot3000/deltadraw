#!/usr/bin/python2

from __future__ import division
from sympy import *

ul_val = 4
ll_val = 6

tx_val = 6
ty_val = 1
tz_val = 6

ul = Symbol('U')
ll = Symbol('L')

tx = Symbol('TX')
ty = Symbol('TY')
tz = Symbol('TZ')

#jx = Symbol('JX')
#jz = Symbol('JZ')

dist = sqrt(tx**2 + ty**2 + tz**2)
alpha = (ll**2 - ul**2 - dist**2) / 2

jx1 = (sqrt(tx**2 * tz**2 * ul**2 + tz**4 * ul**2 - tz**2 * alpha**2) - tx * alpha)/(tx + tz**2)

jx2 = (-sqrt(tx**2 * tz**2 * ul**2 + tz**4 * ul**2 - tz**2 * alpha**2) - tx * alpha)/(tx + tz**2)

jz1 = acos(jx1/ul)
jz2 = acos(jx2/ul)

jx_val1 = jx1.subs([
	(ul, ul_val), 
	(ll, ll_val), 
	(tx, tx_val), 
	(ty, ty_val),
	(tz, tz_val)])

jx_val2 = jx2.subs([
	(ul, ul_val), 
	(ll, ll_val), 
	(tx, tx_val), 
	(ty, ty_val),
	(tz, tz_val)])

print jx_val1.evalf()
print jx_val2.evalf()
