#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "GL\glut.h"
#include "fachada.h"

using namespace std;

//Variaveis
fachada glfachada;								
GLdouble width = 800, height = 800;				
GLdouble planeColor[4] = { 0, 1, 1, 0 };		
GLdouble VeloTransCam = 1;
GLdouble VelRotCam = 5;
GLdouble escala = 0.1;					//Parâmetro para escala em objetos
GLdouble veloTranObj = 1;
GLdouble objectRotateVelocity = 1;
int selecObjeto = 0;
GLdouble z_near =0.1, z_far = 100, FOV = 45;	//Campo de visão da câmera
bool director = false, isRotating = false;		
int mouseX, mouseY;								//Posição do mouse
GLdouble mouseSensiblity = 10;					//Rotação da câmera com mouse
GLuint textures[10];							
GLdouble fogDensity = 20;					
GLfloat fogColor[4] = {0, 0, 0, 0};				
void renderFunction(void)
{
	glfachada.selecObjeto = selecObjeto;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);								//Limpa o buffer de cor e profundidade
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (director){
		glViewport(0, 0, width / 2, height);
		gluPerspective(FOV, ((GLfloat)width/2) / (GLfloat)height, z_near, z_far);
	}
	else{
		glViewport(0, 0, width, height);
		gluPerspective(FOV, (GLfloat)width / (GLfloat)height, z_near, z_far);
		
	}
	glMatrixMode(GL_MODELVIEW);														//Seleciona matriz de modelagem
	glLoadIdentity();																//Reseta a matriz de modelagem para o valor padrão
	glfachada.renderLuz();
	glFogf(GL_FOG_END, fogDensity);
	GLdouble translate[4][4];
	fachadaMatriz::identity(translate);
	fachadaMatriz::trans(translate, glfachada.userCamera.operations);
	glLoadMatrixd(translate[0]);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(-10, 0, 0);
	glVertex3d(10, 0, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, -10, 0);
	glVertex3d(0, 10, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, -10);
	glVertex3d(0, 0, 10);
	glEnd();
	glEnable(GL_LIGHTING);
	glfachada.renderObjetos();
	glfachada.planoCartesiano(0.5, -50, 50, -50, 50, -1, planeColor);
	if (director){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(width / 2, 0, width / 2, height);
		gluPerspective(FOV, ((GLfloat)width / 2) / (GLfloat)height, z_near, z_far);
		glMatrixMode(GL_MODELVIEW);										
		glLoadIdentity();												
		glfachada.renderLuz();
		glPopMatrix();
		glDisable(GL_FOG);
		fachadaMatriz::identity(translate);
		fachadaMatriz::trans(translate, glfachada.directorCamera.operations);
		glLoadMatrixd(translate[0]);
		glfachada.planoCartesiano(0.5, -50, 50, -50, 50, -1, planeColor);
		glfachada.renderObjDiretor();
	}	
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(-10, 0, 0);
	glVertex3d(10, 0, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, -10, 0);
	glVertex3d(0, 10, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, -10);
	glVertex3d(0, 0, 10);
	glEnd();
	glEnable(GL_LIGHTING);
	glFlush();																//Pinta 
	glutSwapBuffers();												
}
void reshapeFunction(GLsizei w, GLsizei h){
	width = w;
	height = h;
}
void handleKeyboard(GLubyte key, GLint x, GLint y)
{
	if (key == 27){
		exit(0);
	}
	else if (key == 'w' || key == 'W'){
		glfachada.transCamera(0, 0, VeloTransCam);
	
		renderFunction();
	}
	else if (key == 'a' || key == 'A'){
		glfachada.transCamera(VeloTransCam, 0, 0);
		renderFunction();
	}
	else if (key == 's' || key == 'S'){
		glfachada.transCamera(0, 0, -VeloTransCam);
		renderFunction();
	}
	else if (key == 'd' || key == 'D'){
		glfachada.transCamera(-VeloTransCam, 0, 0);
		renderFunction();
	}
	else if (key == '1'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.transObjeto(selecObjeto, -veloTranObj, 0, 0);
		}
		else{
			glfachada.transLuz(selecObjeto - glfachada.objetos.size(), -veloTranObj, 0, 0);
		}
		renderFunction();
	}
	else if (key == '2'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.transObjeto(selecObjeto, veloTranObj, 0, 0);
		}
		else{
			glfachada.transLuz(selecObjeto - glfachada.objetos.size(), veloTranObj, 0, 0);
		}
		renderFunction();
	}
	else if (key == '3'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.transObjeto(selecObjeto, 0, -veloTranObj, 0);
		}
		else{
			glfachada.transLuz(selecObjeto - glfachada.objetos.size(), 0, -veloTranObj, 0);
		}
		renderFunction();
	}
	else if (key == '4'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.transObjeto(selecObjeto, 0, veloTranObj, 0);
		}
		else{
			glfachada.transLuz(selecObjeto - glfachada.objetos.size(), 0, veloTranObj, 0);
		}
		renderFunction();
	}
	else if (key == '5'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.transObjeto(selecObjeto, 0, 0, veloTranObj);
		}
		else{
			glfachada.transLuz(selecObjeto - glfachada.objetos.size(), 0, 0, veloTranObj);
		}
		renderFunction();
	}
	else if (key == '6'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.transObjeto(selecObjeto, 0, 0, -veloTranObj);
		}
		else{
			glfachada.transLuz(selecObjeto - glfachada.objetos.size(), 0, 0, -veloTranObj);
		}
		renderFunction();
	}
	else if (key == '7'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.rotObjeto(selecObjeto, objectRotateVelocity, 1, 0, 0);
		}
		renderFunction();
	}
	else if (key == '8'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.rotObjeto(selecObjeto, objectRotateVelocity, 0, 1, 0);
		}
		renderFunction();
	}
	else if (key == '9'){
		if (selecObjeto < glfachada.objetos.size()){
			glfachada.rotObjeto(selecObjeto, objectRotateVelocity, 0, 0, 1);
		}
		renderFunction();
	}
	else if (key == '>' || key == '.'){
		selecObjeto++;
		if (selecObjeto >(glfachada.objetos.size() + glfachada.lightNumber - 1)){
			selecObjeto = 0;
		}
		renderFunction();
	}
	else if (key == '<' || key == ','){
		selecObjeto--;
		if (selecObjeto < 0){
			selecObjeto = glfachada.objetos.size() + glfachada.lightNumber - 1;
		}
		renderFunction();
	}
	else if (key == 'l' || key == 'L'){
		glfachada.criarLuz();
		renderFunction();
	}
	else if (key == 'z' || key == 'Z'){
		if (FOV > 9.5){
			FOV -= 1;
			glfachada.transCamera(0, 0, -0.996666);
			renderFunction();
		}
	}
	else if (key == 'x' || key == 'X'){
		if (FOV < 60){
			FOV += 1;
			glfachada.transCamera(0, 0, 0.16666);
			renderFunction();
		}
	}
	else if (key == 'f' || key == 'F'){
		director = !director;
		renderFunction();
	}
	else if (key == '+' || key == '='){
		glfachada.escalaObjeto(selecObjeto, escala, escala, escala);
		renderFunction();
	}
	else if (key == '-' || key == '_'){
		glfachada.escalaObjeto(selecObjeto, -escala, -escala, -escala);
		renderFunction();
	}
	else if ((key == 'k' || key == 'K') && fogDensity >1){
		fogDensity-=0.1;
		renderFunction();
	}
	else if (key == 'j' || key == 'J'){
		fogDensity+=0.1;
		renderFunction();
	}
}
void mouseClick(GLint btn, GLint state, GLint x, GLint y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseX = x;
		mouseY = y;
		isRotating = true;
	}
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
		isRotating = false;
	}
}
void mouseMove(GLint x, GLint y)
{
	if (isRotating){
		if (x > mouseX + mouseSensiblity){
			glfachada.rotCamera(VelRotCam, 0, 1, 0);
			mouseX = x;
		}
		else if (x < mouseX - mouseSensiblity){
			glfachada.rotCamera(-VelRotCam, 0, 1, 0);
			mouseX = x;
		}
		if (y > mouseY + mouseSensiblity){
			glfachada.rotCamera(VelRotCam, 1, 0, 0);
			mouseY = y;
		}
		else if (y < mouseY - mouseSensiblity){
			glfachada.rotCamera(-VelRotCam, 1, 0, 0);
			mouseY = y;
		}
		renderFunction();
	}
}

void init(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CG Project");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutDisplayFunc(renderFunction);
	glutReshapeFunc(reshapeFunction);
	glutKeyboardFunc(handleKeyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_INDEX, 32);
	glFogf(GL_FOG_START, 1);
	glFogfv(GL_FOG_COLOR, fogColor);
	glEnable(GL_LIGHTING);
	Point4D aux;
	Point4D temp;
	temp.x = 0;
	temp.y = 10;
	temp.z = 0;
	temp.w = 0;
	aux.x = 1;
	aux.y = 1;
	aux.z = 1;
	aux.w = 5;
	glfachada.lightNumber++;
	Light light;
	light.cor = aux;
	light.position = temp;
	glfachada.luz.push_back(light);
	glEnable(GL_LIGHT0);
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	fachadaMatriz::identity(glfachada.userCamera.operations);
	fachadaMatriz::identity(glfachada.directorCamera.operations);
	glfachada.initDirector();
	glfachada.camera.loadObject("Resources/Objects/camera.obj");
	glfachada.loadObject("Resources/Objects/cat.obj");
	glfachada.loadObject("Resources/Objects/dog.obj");
	glfachada.transObjeto(0, -5, 0, 0);
	glfachada.transObjeto(1, 5, 0, 0);
}
int main(){
	init();
	glutMainLoop();	
	return 0;
}