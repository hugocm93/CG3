//Camera Parameters
eye = [3;3;3]

at = [0;0;0]

up = [0;0;1]

fov = 30

near = 1

far = 15

w = 640

h = 480

//Triangle Parameters
v1 = [1;0;0]

v2 = [0;1;0]

v3 = [0;0;10]

//Camera axis
ze = (eye - at)/norm(eye - at)

xe = cross(up, ze)/norm(cross(up, ze))

ye = cross(ze, xe)

//Frustum bounds
top = near*tan((3.14/180)*fov/2)

bottom = -top

right = top*(w/h)

left = -right

//Matrices

R = [xe(1) xe(2) xe(3) 0; ye(1) ye(2) ye(3) 0; ze(1) ze(2) ze(3) 0; 0 0 0 1]

T = [1 0 0 -eye(1); 0 1 0 -eye(2); 0 0 1 -eye(3); 0 0 0 1]

ModelView = R*T

Projection = [(2*near)/(right-left) 0 (right+left)/(right - left) 0; 0 (2*near)/(top-bottom) (top+bottom)/(top-bottom) 0; 0 0 -(far+near)/(far-near) -(2*far*near)/(far-near); 0 0 -1 0]

//Functions
function [Xw, Yw, Zw]=viewPort(x, y, z)
	Xw = w*((x+1)/2)
	Yw = h*((y+1)/2)
	Zw = 65535*((z+1)/2)
endfunction

function [X, Y, Z]=calcEverything(x, y, z)
	raw = [x;y;z;1]
	temp = Projection * ModelView * raw
	temp = temp/temp(4)

	[X, Y, Z] = viewPort(temp(1), temp(2), temp(3))
endfunction

function [X,Y,Z] = cartesianFromNatural(x, y, z)
	X = x*v1(1) + y*v2(1) + z*v3(1)
	Y = x*v1(2) + y*v2(2) + z*v3(2)
	Z = x*v1(3) + y*v2(3) + z*v3(3)
endfunction


function [X,Y,Z] = NaturalFromCartesian(x, y, z)
	OriginalMatrix = [v1(1) v2(1) v3(1); v1(2) v2(2) v3(2); v1(3) v2(3) v3(3)]

	Inverse = inv(OriginalMatrix)

	aux = Inverse * [x;y;z]

	X = aux(1)
	Y = aux(2)
	Z = aux(3)

endfunction


function [R, G, B]=calcColor(x, y, z)
	[X, Y, Z] = NaturalFromCartesian(x, y, z)

	v4 = [X;Y;Z]
	

	a1 = v4 - [1;0;0]
	d1 = norm(a1)

	a2 = v4 - [0;1;0]
	d2 = norm(a2)

	a3 = v4 - [0;0;1]
	d3 = norm(a3)

	D = d1 + d2 + d3

	w1 = (1 - d1/D)
	w2 = (1 - d2/D)
	w3 = (1 - d3/D)

	c4 = w1*[1;0;0] + w2*[0;1;0] + w3*[0;0;1]

	R = c4(1)*255
	G = c4(2)*255
	B = c4(3)*255
endfunction

function [R, G, B]=calcColor2(x, y, z)
	[X, Y, Z] = NaturalFromCartesian(x, y, z)

	v4 = [X;Y;Z]
	
	a1 = 0.5*norm(cross(v4-[1;0;0], [0;1;0]-[1;0;0]))
	a2 = 0.5*norm(cross(v4-[0;1;0], [0;0;1]-[0;1;0]))
	a3 = 0.5*norm(cross(v4-[0;0;1], [1;0;0]-[0;0;1]))

	A = a1+a2+a3

	c4 = (a2/A)*[1;0;0] + (a3/A)*[0;1;0] + (a1/A)*[0;0;1]

	R = c4(1)*255
	G = c4(2)*255
	B = c4(3)*255
endfunction

function [R, G, B]=calcColor3(x, y, z)
	v4 = [x;y;z]
	
	a1 = 0.5*norm(cross(v4-v1, v2-v1))
	a2 = 0.5*norm(cross(v4-v2, v3-v2))
	a3 = 0.5*norm(cross(v4-v3, v1-v3))

	A = a1+a2+a3

	c4 = (a2/A)*[1;0;0] + (a3/A)*[0;1;0] + (a1/A)*[0;0;1]

	R = c4(1)*255
	G = c4(2)*255
	B = c4(3)*255
endfunction






