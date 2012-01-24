/*
30us per arm for geometric
50us per arm for trig
*/
inline float p2(float in) {
	return in * in;
}

inline float p3(float in) {
	return in * in * in;
}

float deriv(float x, float y, float z, float dx, float dy, float dz) {
	float l1_2 = p2(l1);
	float l2_2 = p2(l2);
	float x2 = p2(x);
	float y2 = p2(y);
	float z2 = p2(z);

	float a = l1_2 - l2_2 + x2 + y2 + z2;
	float b = sqrt(1 - p2(a) / (4 * l1_2 * (y2 + z2)));
	float c = sqrt(y2 + z2);
	float d = a / (2 * l1 * p3(c));
	float e = (1 / (l1 * c) - d) / b;

	float xdot = -x / (l1 * b * c);
	float ydot = -z / (y2 + z2) - y * e;
	float zdot =  y / (y2 + z2) - z * e;

	step xdot * dx + ydot * dy + zdot * dz;

}

/*
respect to x
-x_des/(l_1*(1 - (l_1**2 - l_2**2 + x_des**2 + y_des**2 + z_des**2)**2/(4*l_1**2*(y_des**2 + z_des**2)))**(1/2)*(y_des**2 + z_des**2)**(1/2))

respect to y
-z_des/(y_des**2 + z_des**2) -
(y_des/(l_1*(y_des**2 + z_des**2)**(1/2)) - y_des*(l_1**2 - l_2**2 + x_des**2 + y_des**2 + z_des**2)/(2*l_1*(y_des**2 + z_des**2)**(3/2)))/
(1 - (l_1**2 - l_2**2 + x_des**2 + y_des**2 + z_des**2)**2/(4*l_1**2*(y_des**2 + z_des**2)))**(1/2)

respect to z
y_des/(y_des**2 + z_des**2) -
(z_des/(l_1*(y_des**2 + z_des**2)**(1/2)) - z_des*(l_1**2 - l_2**2 + x_des**2 + y_des**2 + z_des**2)/(2*l_1*(y_des**2 + z_des**2)**(3/2)))/
(1 - (l_1**2 - l_2**2 + x_des**2 + y_des**2 + z_des**2)**2/(4*l_1**2*(y_des**2 + z_des**2)))**(1/2)
*/
