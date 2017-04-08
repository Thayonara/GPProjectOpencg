#pragma once
#include <vector>
using namespace std;

struct Point2D{
	GLdouble x, y;
};
struct Point3D{
	GLdouble x, y, z;
};
struct Point4D{
	GLdouble x, y, z, w;
};
struct Face{
	vector<GLint> vertice;
	vector<GLint> normais;
	vector<GLint> textura;
};
struct Light{
	Point4D cor;
	Point4D position;
};
struct Camera{
	GLdouble rotacX, rotacY, rotacZ;
	GLdouble operations[4][4];
};