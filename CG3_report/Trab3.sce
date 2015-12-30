//Esse é um script do programa scilab. Ao rodar esse script, algumas variáveis são instanciadas. Utilizei esse programa para fazer todos os cálculos.


//Scilab configuration. Aqui é definido o número de máximo de dígitos que o programa vai utilizar.
format(15)

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

//Essa função determina as coordenadas de tela quando recebe as coordenadas normalizadas
function [Xw, Yw, Zw]=viewPort(x, y, z)
	Xw = w*((x+1)/2)
	Yw = h*((y+1)/2)
	Zw = 65535*((z+1)/2)
endfunction


//Esse função computa todo o processo. Entra as coordenadas do objeto e sai as coordenadas de tela
function [X, Y, Z]=calcEverything(x, y, z)
	raw = [x;y;z;1]
	temp = Projection * ModelView * raw
	temp = temp/temp(4)

	[X, Y, Z] = viewPort(temp(1), temp(2), temp(3))
endfunction


//Essa função calcula a cor em qualquer ponto do triângulo, usando a área como ponderação
function [R, G, B]=calcColor(x, y, z)
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

//Intersection. Aqui são calculados os pontos de interseção com o topo da janela.

P1 = Projection * ModelView * [0;0;10;1]
d1 = -P1(4) + P1(2)

P2 = Projection * ModelView * [0;1;0;1]
d2 = -P2(4) + P2(2)

Pi = (d1/(d1-d2))*P2 - (d2/(d1-d2))*P1

P4 = inv(Projection * ModelView)*Pi

P3 = Projection * ModelView * [1;0;0;1]

d3 = -P3(4) + P3(2)

Pi = (d1/(d1-d3))*P3 - (d2/(d1-d3))*P1

P3 = inv(Projection * ModelView)*Pi


//Exemplo de como achei cada coordenada Vo, Ve, Vc, Vn e Vw

printf('P1\n')

//Primeiramente começamos com a coordenada Vo
Vo = [1;0;0;1]

//Multiplico pela modelView para ter as coordenadas descritas em relação a camera.
Ve = ModelView*Vo

//Multiplico as coordenas descritas pela camera pela matriz projection. Após a multiplicação, o tronco de pirâmide se transforma em um paralelepípedo.
Vc = Projection*Ve

//Para obter as coordenadas normais, divido por w
Vn = Vc/Vc(4)

//Para obter as coordenadas de tela, fiz uma função.
[X, Y, Z] = viewPort(Vn(1), Vn(2), Vn(3))

//Para obter a cor
[R, G, B] = calcColor(Vo(1), Vo(2), Vo(3))

//Fiz esse processo para cada vértice para poder chegar ao resultado visto na planilha que foi enviada no anexo.


printf('P2\n')

Vo = [0;1;0;1]

Ve = ModelView*Vo

Vc = Projection*Ve

Vn = Vc/Vc(4)

[X, Y, Z] = viewPort(Vn(1), Vn(2), Vn(3))

[R, G, B] = calcColor(Vo(1), Vo(2), Vo(3))


printf('P3\n')

Vo = P3

Ve = ModelView*Vo

Vc = Projection*Ve

Vn = Vc/Vc(4)

[X, Y, Z] = viewPort(Vn(1), Vn(2), Vn(3))

[R, G, B] = calcColor(Vo(1), Vo(2), Vo(3))



printf('P4\n')

Vo = P4

Ve = ModelView*Vo

Vc = Projection*Ve

Vn = Vc/Vc(4)

[X, Y, Z] = viewPort(Vn(1), Vn(2), Vn(3))

[R, G, B] = calcColor(Vo(1), Vo(2), Vo(3))




