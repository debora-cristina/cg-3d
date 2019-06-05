#include <GL/glut.h>
#include <soil/SOIL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Pontos.h"
#include "Poligono.h"

#define MAX_TEX 40    /*numero de texturas que serao usadas no vetor de texturas*/



#define AREIA 0       /*posições do vetor de texturas que armazenam determinadas texturas*/
#define CEU 1
#define CEU2 2
#define CEU3 3
#define MENU1 4
#define MENU2 5
#define MENU3 6
#define OPCAO 7
#define MOINHO 8
#define GUARDASOL 9
#define PLACA 10
#define PAQUITO1 11
#define PAQUITO2 12
#define ASA 13
#define PENA 14
#define BICO 15
#define CARA 16
#define OLHO 17
#define PONTE 18
#define ARVORE1 19
#define COCO 20
#define TRONCO 21
#define AGUA 22
#define TIJOLO 23
#define CHAO 24
#define MADEIRA 25
#define TELHA 26
#define MADEIRA2 27
#define FAIXA 28
#define MADEIRA3 29
#define BRANCO 30
#define VINHO 31
#define CINZA 32
#define BRANCO2 33
#define FORRO 34
#define TELHA2 35
#define COIN 36
#define ESTRADA 37
#define CAVERNA 38
#define TIJOLO 39
#define HELICE 40


float eixoX = 0.0f;
float eixoY = 0.0f;
int contVolta = 0;
float DEG2RAD = 3.14159 / 180.0;
float luzAmbiente = 1.0f;
bool fontePressionada = false;
float rota_paquito = 0.0f;
bool luzPressionada = false;
int count1 = 0;
#define DELAY_ABERTURA 10000
float FPS = 200 / 45; //fps
float delay_abertura = 0;
bool   gp;                      // G Pressed? ( New )
GLuint filter;                      // Which Filter To Use
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLuint fogfilter = 0;                    // Which Fog To Use
float densidadeFog = 0.0f;
GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };      // Fog Color

float angulo = 0.0f, rota_moinho = 0, controle = 1, controle2 = 1;
float xcamera = 0.0f, ycamera = 5.0f, zcamera = 30.0f;     /*posicao da camera*/
float dxcamera = 0.0f, dycamera = 0.0f, dzcamera = -1.0f;  /*direção ao deslocar a camera*/

int mostramenu = MENU1;     /*variaveis para o menu*/
float quant = 0.0f, cmenu = 0;  /*quant é o valor das coordenas rgb da luz difusa da camera*/
GLfloat luzdifusa[4] = { quant, quant, quant, 1.0 };

GLuint texturas[MAX_TEX]; /*vetor de texturas*/

						  /*função que configura o menu*/

void menu() {
	glBindTexture(GL_TEXTURE_2D, texturas[mostramenu]);
	glPushMatrix();
	glTranslatef(xcamera + 17.0f, ycamera - 5, zcamera - 24.7f);
	glRotatef(-0.6f * 57, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, -5.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, -5.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 15.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 15.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

/*função que configura a direção da camera*/
void OrientaCamera(float ang) {
	dxcamera = sin(ang);
	dzcamera = -cos(ang);
	glLoadIdentity();
	gluLookAt(xcamera, ycamera, zcamera, xcamera + dxcamera, ycamera + dycamera, zcamera + dzcamera, 0.0f, 1.0f, 0.0f);
}

/*função que configura a posição da camera*/
void MoveCamera(int direcao) {

	if (xcamera>990.0f) {
		if (direcao*(dxcamera)*0.7 < 0)
			xcamera = xcamera + direcao*(dxcamera)*0.9f;
	}
	if (xcamera<-990.0f) {
		if (direcao*(dxcamera)*0.7 > 0)
			xcamera = xcamera + direcao*(dxcamera)*0.9f;
	}
	if (xcamera>-990.0f && xcamera<990.0f)
		xcamera = xcamera + direcao*(dxcamera)*0.9f;

	if (zcamera>990.0f) {
		if (direcao*(dzcamera)*0.7f < 0)
			zcamera = zcamera + direcao*(dzcamera)*0.9f;
	}
	if (zcamera<-990.0f) {
		if (direcao*(dzcamera)*0.7f > 0)
			zcamera = zcamera + direcao*(dzcamera)*0.9f;
	}
	if (zcamera<990.0f && zcamera>-990.0f)
		zcamera = zcamera + direcao*(dzcamera)*0.9f;

	glLoadIdentity();
	gluLookAt(xcamera, ycamera, zcamera, xcamera + dxcamera, ycamera + dycamera, zcamera + dzcamera, 0.0f, 1.0f, 0.0f);
	GLfloat posicaocamera[4] = { xcamera,ycamera,zcamera,1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, posicaocamera);
}

/*função que trata eventos do teclado normal*/
void TeclasNormais(unsigned char key, int x, int y) {

	key = tolower(key);


	if (cmenu != 0) {
		switch (key) {
		case 27:
			exit(0);
			break;
		case 'j':
			angulo -= 0.1f;
			break;
		case 's':
			MoveCamera(-1);
			break;
		case 'w':
			MoveCamera(1);
			break;
		case 'a':
			angulo -= 0.1f;
			OrientaCamera(angulo);
			break;
		case 'd':
			angulo += 0.1f;
			OrientaCamera(angulo);
			break;
		case 'r':
			angulo = 0.0f;
			rota_moinho = 0.0f;
			xcamera = 0.0f;
			ycamera = 5.0f;
			zcamera = 30.0f;
			dxcamera = 0.0f;
			dycamera = 0.0f;
			dzcamera = -20.0f;
			quant = 0.4f;
			MoveCamera(1);
			luzdifusa[0] = quant;
			luzdifusa[1] = quant;
			luzdifusa[2] = quant;
			glLightfv(GL_LIGHT1, GL_DIFFUSE, luzdifusa);
			break;
		case 'l':

			if (!fontePressionada) {
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				luzAmbiente = 1.0f;
				GLfloat ambientLight[] = { 0.0f, 0.0f, 0.0f, luzAmbiente };
				glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
				glEnable(GL_LIGHT0);
				glEnable(GL_LIGHT1);
				fontePressionada = true;
			}
			else {
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				luzAmbiente = 0.0f;
				fontePressionada = false;
			}
			break;
		case 'o':
			glEnable(GL_LIGHT1);
			if (!luzPressionada) {
				luzPressionada = true;
			}
			else {
				luzPressionada = false;
				glDisable(GL_LIGHT1);
			}
			break;
		case 'p':
			//glDisable(GL_LIGHT1);
			break;
		case 'f':
			if (densidadeFog > 0.0f) {
				densidadeFog = densidadeFog - 0.0015f;
			}
			break;
		case 'g':
			densidadeFog = densidadeFog + 0.0015f;
			cout << densidadeFog;

			break;
		case 43:  /*+*/
			if (luzPressionada) {
				quant += 0.1f;
				if (quant > 1.0) { quant = 1.0f; }
				luzdifusa[0] = quant;
				luzdifusa[1] = quant;
				luzdifusa[2] = quant;
				glLightfv(GL_LIGHT1, GL_DIFFUSE, luzdifusa);
			}
			break;

		case 45:  /*-*/
			if (luzPressionada) {
				quant -= 0.1f;
				if (quant < 0) {
					quant = 0.0f;
				}
				luzdifusa[0] = quant;
				luzdifusa[1] = quant;
				luzdifusa[2] = quant;
				glLightfv(GL_LIGHT1, GL_DIFFUSE, luzdifusa);
			}

			break;
		}
	}
	else {
		switch (key) {
		case 27:
			exit(0);
			break;
		case 13:
			switch (mostramenu) {
			case MENU1: {
				angulo = 0.0f;
				rota_moinho = 0.0f;
				xcamera = 0.0f;
				ycamera = 5.0f;
				zcamera = 30.0f;
				dxcamera = 0.0f;
				dycamera = 0.0f;
				dzcamera = -20.0f;
				quant = 0.4f;
				cmenu = 1;
				MoveCamera(1);
				GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 0.1f };
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
				luzdifusa[0] = quant;
				luzdifusa[1] = quant;
				luzdifusa[2] = quant;
				glLightfv(GL_LIGHT1, GL_DIFFUSE, luzdifusa);
			}
						break;
			case MENU2:
				mostramenu = OPCAO;
				menu();
				glutSwapBuffers();
				break;
			case MENU3:
				exit(0);
				break;
			case OPCAO:
				mostramenu = MENU1;
				menu();
				glutSwapBuffers();
				break;
			}
			break;
		case 'q':
			if (mostramenu == OPCAO) {
				mostramenu = MENU1;
				menu();
				glutSwapBuffers();
			}
		}
	}
}


/*função que trata os eventos de teclas especiais*/
void TeclasEspeciais(int key, int x, int y) {
	if (cmenu != 0) {
		switch (key) {
		case GLUT_KEY_UP:
			ycamera += 1.0f;
			if (ycamera == 41) ycamera = 40;
			MoveCamera(0);
			break;
		case GLUT_KEY_DOWN:
			ycamera -= 1.0f;
			if (ycamera == 0) ycamera = 1.0f;
			MoveCamera(0);
			break;
		}
	}
	else {
		switch (key) {
		case GLUT_KEY_UP:
			if (mostramenu == MENU2)
				mostramenu = MENU1;
			if (mostramenu == MENU3)
				mostramenu = MENU2;
			menu();
			glutSwapBuffers();
			break;
		case GLUT_KEY_DOWN:
			if (mostramenu == MENU2)
				mostramenu = MENU3;
			if (mostramenu == MENU1)
				mostramenu = MENU2;
			menu();
			glutSwapBuffers();
			break;
		}
	}
}

/*função que configura o redimensionamento de janela*/
void AlteraTamanho(int largura, int altura) {
	if (altura == 0) altura = 1;
	float taxa = 1.0f * largura / altura;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, largura, altura);
	gluPerspective(45.0, taxa, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (cmenu == 0) {
		gluLookAt(xcamera, ycamera, zcamera, xcamera + dxcamera, ycamera + dycamera, zcamera + dzcamera, 0.0f, 1.0f, 0.0f);
		angulo = 0.6f;
		OrientaCamera(angulo);
		int i;
		cmenu = 1;
		for (i = 0; i<1000; i++) {
			MoveCamera(-1);
			TeclasEspeciais(GLUT_KEY_UP, 0, 0);
		}
		cmenu = 0;
	}
	else {
		gluLookAt(xcamera, ycamera, zcamera, xcamera + dxcamera, ycamera + dycamera, zcamera + dzcamera, 0.0f, 1.0f, 0.0f);
	}
}

/*função que inicializa o programa, nela são configuradas as luzes do programa e as texturas que serão utilizadas*/
void Inicializa() {


	// Somewhere in the initialization part of your program…

	glEnable(GL_LIGHTING);
	//GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHT0);
	GLfloat ambientLight[] = { 0.0f, 0.0f, 0.0f, luzAmbiente };
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);

	// Create light components

	GLfloat diffuseLight[] = { quant,quant,quant, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { xcamera,ycamera,zcamera,1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzdifusa);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT3, GL_POSITION, position);

	glDisable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);                                               /*habilita teste de profundidade*/

	glEnable(GL_TEXTURE_2D);                                            /*habilita texturas 2d e elas são configuradas*/
	glGenTextures(1, texturas);

	texturas[AREIA] = SOIL_load_OGL_texture("textura/areia.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[CEU] = SOIL_load_OGL_texture("textura/ceu.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[CEU2] = SOIL_load_OGL_texture("textura/ceu2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[CEU3] = SOIL_load_OGL_texture("textura/ceu3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MENU1] = SOIL_load_OGL_texture("textura/menu1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MENU2] = SOIL_load_OGL_texture("textura/menu2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MENU3] = SOIL_load_OGL_texture("textura/menu3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[OPCAO] = SOIL_load_OGL_texture("textura/opcao.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MOINHO] = SOIL_load_OGL_texture("textura/cadeira.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[GUARDASOL] = SOIL_load_OGL_texture("textura/guardasol.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[PLACA] = SOIL_load_OGL_texture("textura/placa.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[PAQUITO1] = SOIL_load_OGL_texture("textura/paquito1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[PAQUITO2] = SOIL_load_OGL_texture("textura/paquito2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[ASA] = SOIL_load_OGL_texture("textura/asa.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[PENA] = SOIL_load_OGL_texture("textura/pena.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[BICO] = SOIL_load_OGL_texture("textura/bico.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[CARA] = SOIL_load_OGL_texture("textura/RED.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[OLHO] = SOIL_load_OGL_texture("textura/olho.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[PONTE] = SOIL_load_OGL_texture("textura/wood.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[ARVORE1] = SOIL_load_OGL_texture("textura/tree.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[COCO] = SOIL_load_OGL_texture("textura/coco.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[TRONCO] = SOIL_load_OGL_texture("textura/trunk.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[AGUA] = SOIL_load_OGL_texture("textura/agua.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MADEIRA] = SOIL_load_OGL_texture("textura/madeira.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	//texturas[OLHO] = SOIL_load_OGL_texture("textura/olho.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[TELHA] = SOIL_load_OGL_texture("textura/telha.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MADEIRA2] = SOIL_load_OGL_texture("textura/madeira2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[FAIXA] = SOIL_load_OGL_texture("textura/faixa.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MADEIRA3] = SOIL_load_OGL_texture("textura/madeira3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[MADEIRA3] = SOIL_load_OGL_texture("textura/branco.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[VINHO] = SOIL_load_OGL_texture("textura/vinho.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[CINZA] = SOIL_load_OGL_texture("textura/cinza.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[BRANCO2] = SOIL_load_OGL_texture("textura/branco2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[FORRO] = SOIL_load_OGL_texture("textura/forro.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[TELHA2] = SOIL_load_OGL_texture("textura/telha2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[COIN] = SOIL_load_OGL_texture("textura/coin.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[ESTRADA] = SOIL_load_OGL_texture("textura/ground.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[CAVERNA] = SOIL_load_OGL_texture("textura/ice2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[TIJOLO] = SOIL_load_OGL_texture("textura/tijolo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	texturas[HELICE] = SOIL_load_OGL_texture("textura/helice.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

}

int countlines(char *filename)
{

	FILE *file = fopen(filename, "r");
	//int integers[100];

	int i = 0;
	int ch = 0;
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch == '\n')
		{
			i++;
		}
	}
	fclose(file);

	return i;
}


void lerArquivo(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[AGUA]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f(((float)(count1 + poligono.getPontos()[value[i][j]].getX()) / 30), (float)(count1 + poligono.getPontos()[value[i][j]].getZ() / 30));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}

void lerCasa2_telha(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[TELHA]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 1, (poligono.getPontos()[value[i][j]].getZ() / 1));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa2_madeira(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[MADEIRA2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 1, (poligono.getPontos()[value[i][j]].getZ() / 1));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa2_faixa(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[FAIXA]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 90, (poligono.getPontos()[value[i][j]].getZ() / 90));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa2_madeira2(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[MADEIRA2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 1, (poligono.getPontos()[value[i][j]].getZ() / 1));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa2_madeira3(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[MADEIRA2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 1, (poligono.getPontos()[value[i][j]].getZ() / 1));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}


void lerCasa1_telha(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[TIJOLO]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 30, (poligono.getPontos()[value[i][j]].getZ() / 30));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa1_madeira(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[MADEIRA]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 90, (poligono.getPontos()[value[i][j]].getZ() / 90));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa1_madeira2(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[MADEIRA]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 90, (poligono.getPontos()[value[i][j]].getZ() / 90));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}
void lerCasa1_madeira3(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[MADEIRA]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 90, (poligono.getPontos()[value[i][j]].getZ() / 90));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);
	count1++;
	if (count1 == 3000)
	{
		count1 = 0;

	}

}


void cave() {


	glBindTexture(GL_TEXTURE_2D, texturas[CAVERNA]);	    /*cilindro que é o corpo do passaro*/
	glPushMatrix();
	glTranslatef(-267.0f, 1.0f, 140.0f);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(80, 0.0, 0.0, 1.0);

	const float PI = 3.141592f;
	GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles        
	GLfloat radius = 60.0f;
	int gradation = 10;

	for (alpha = 0.0; alpha < PI; alpha += PI / gradation)
	{
		glBegin(GL_TRIANGLE_STRIP);

		for (beta = 0.0; beta < 1.5*PI; beta += PI / gradation)
		{
			x = radius*cos(beta)*sin(alpha);
			y = radius*sin(beta)*sin(alpha);
			z = radius*cos(alpha);
			glVertex3f(x / 2, y / 2, z);
			x = radius*cos(beta)*sin(alpha + PI / gradation);
			y = radius*sin(beta)*sin(alpha + PI / gradation);
			z = radius*cos(alpha + PI / gradation);
			glVertex3f(x / 2, y / 2, z);
		}
		glEnd();
	}
	glPopMatrix();



}




void estrada() {
	glPushMatrix();
	glTranslatef(-79.0f, -1.5f, 155.0f);
	glBindTexture(GL_TEXTURE_2D, texturas[ESTRADA]);
	//glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);


	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 2.0f, -330.0f);
	glTexCoord2f(0.0f, 80.0f); glVertex3f(-25.0f, 2.0f, -330.0f);
	glTexCoord2f(80.0f, 80.0f); glVertex3f(-25.0f, 2.0f, 0.0f);
	glTexCoord2f(80.0f, 0.0f); glVertex3f(-50.0f, 2.0f, 0.0f);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(150.0f, -1.5f, 60.0f);
	glBindTexture(GL_TEXTURE_2D, texturas[ESTRADA]);
	//glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);


	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 2.0f, -280.0f);
	glTexCoord2f(0.0f, 80.0f); glVertex3f(-25.0f, 2.0f, -280.0f);
	glTexCoord2f(80.0f, 80.0f); glVertex3f(-25.0f, 2.0f, 0.0f);
	glTexCoord2f(80.0f, 0.0f); glVertex3f(-50.0f, 2.0f, 0.0f);
	glEnd();

	glPopMatrix();



	glPushMatrix();
	glTranslatef(-129.0f, -1.5f, 106.0f);
	glBindTexture(GL_TEXTURE_2D, texturas[ESTRADA]);
	glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);


	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 2.0f, -280.0f);
	glTexCoord2f(0.0f, 80.0f); glVertex3f(-25.0f, 2.0f, -280.0f);
	glTexCoord2f(80.0f, 80.0f); glVertex3f(-25.0f, 2.0f, 0.0f);
	glTexCoord2f(80.0f, 0.0f); glVertex3f(-50.0f, 2.0f, 0.0f);
	glEnd();

	glPopMatrix();





	glPushMatrix();
	glTranslatef(-129.0f, -1.5f, 106.0f);
	glBindTexture(GL_TEXTURE_2D, texturas[ESTRADA]);
	glRotatef(90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);


	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 2.0f, -280.0f);
	glTexCoord2f(0.0f, 80.0f); glVertex3f(-25.0f, 2.0f, -280.0f);
	glTexCoord2f(80.0f, 80.0f); glVertex3f(-25.0f, 2.0f, 0.0f);
	glTexCoord2f(80.0f, 0.0f); glVertex3f(-50.0f, 2.0f, 0.0f);
	glEnd();

	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-129.0f, -1.5f, -145.0f);
	glBindTexture(GL_TEXTURE_2D, texturas[ESTRADA]);
	glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);


	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, 2.0f, -150.0f);
	glTexCoord2f(0.0f, 80.0f); glVertex3f(-25.0f, 2.0f, -150.0f);
	glTexCoord2f(80.0f, 80.0f); glVertex3f(-25.0f, 2.0f, 0.0f);
	glTexCoord2f(80.0f, 0.0f); glVertex3f(-40.0f, 2.0f, 0.0f);
	glEnd();

	glPopMatrix();




}


void lerArquivoBridge(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glTranslatef(-15.3f, 1.0f, 10.0f);
			glBindTexture(GL_TEXTURE_2D, texturas[PONTE]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX() / 4), (poligono.getPontos()[value[i][j]].getZ() / 4));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}

void lerArquivoBoat(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glTranslatef(14.0f, 3.0f, 12.0f);

			glBindTexture(GL_TEXTURE_2D, texturas[PONTE]);
			GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);

			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX() / 4), (poligono.getPontos()[value[i][j]].getZ() / 4));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());
			}
			glEnd();
			glPopMatrix();
		}
		if (i == linhas)
		{
			glPushMatrix();
			glTranslatef(21.0f, 18.0f, 6.5f);
			glRotatef(90, 0.0, 1.0, 0.0);
			glRotatef(-90, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[CARA]);
			GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX() / 4), (poligono.getPontos()[value[i][j]].getZ() / 4));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());
			}
			glEnd();
			glPopMatrix();
		}





	}







	fclose(archivo);


}



void lerArquivoCilindroBridge(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	//float n, m;
	//Poligono poligono;




	for (i = 0; i < linhas + 1; i++) {


		for (float j = 1; j <= 2; j++)
		{
			for (float k = 0; k <= 2; k++)
			{
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, texturas[PONTE]);
				glTranslatef(-j * 5 - 5, 0.0f, k * 5);
				glRotatef(90, 0.0, 1.0, 0.0);
				glRotatef(-90, 1.0, 0.0, 0.0);
				GLUquadricObj *quad2;
				quad2 = gluNewQuadric();
				gluQuadricNormals(quad2, GLU_SMOOTH);
				gluQuadricTexture(quad2, GL_TRUE);
				gluCylinder(quad2, (GLint)value[i][0], (GLint)value[i][1], (GLint)value[i][2], (GLint)value[i][3], (GLint)value[i][4]);
				glPopMatrix();
			}

		}
		//DesenhaFunc();

		//	glTexCoord2f((count1 + poligono.getPontos()[value[i][j]].getX()) / 30, (count1 + poligono.getPontos()[value[i][j]].getZ() / 30));
		//	glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());







	}


	fclose(archivo);


}

void lerArquivoCilindroBoat(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	//float n, m;
	//Poligono poligono;




	for (i = 0; i < linhas + 1; i++) {

		if (i == 0)
		{
			for (float k = 0; k <= 2; k++)
			{
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, texturas[PLACA]);
				glTranslatef(13.5f, 6.0f, 5 * k + 2.0f);
				glRotatef(90, 0.0, 1.0, 0.0);
				GLUquadricObj *quad2;
				quad2 = gluNewQuadric();
				gluQuadricNormals(quad2, GLU_SMOOTH);
				gluQuadricTexture(quad2, GL_TRUE);
				gluCylinder(quad2, value[i][0], value[i][1], value[i][2], value[i][3], value[i][4]);
				glPopMatrix();

			}
		}

		else {
			glBindTexture(GL_TEXTURE_2D, texturas[PONTE]);	    /*cilindro que é o corpo do passaro*/
			glPushMatrix();
			glTranslatef(17.0f, 7.0f, 7.0f);
			glRotatef(90, 0.0, 1.0, 0.0);
			glRotatef(-90, 1.0, 0.0, 0.0);
			GLUquadricObj *quad3;
			quad3 = gluNewQuadric();
			gluQuadricNormals(quad3, GLU_SMOOTH);
			gluQuadricTexture(quad3, GL_TRUE);
			gluCylinder(quad3, (GLint)value[i][0], (GLint)value[i][1], (GLint)value[i][2], (GLint)value[i][3], (GLint)value[i][4]);
			glPopMatrix();
		}

		//DesenhaFunc();

		//	glTexCoord2f((count1 + poligono.getPontos()[value[i][j]].getX()) / 30, (count1 + poligono.getPontos()[value[i][j]].getZ() / 30));
		//	glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());







	}


	fclose(archivo);


}

void lerArquivoCilindroB(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	//float n, m;
	//Poligono poligono;




	for (i = 0; i < linhas + 1; i++) {

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texturas[COIN]);
		glTranslatef(3.5f, 15.0f, 2.0f);
		glRotatef(90, 0.0, 1.0, 0.0);
		//glNewList(1, GL_COMPILE);
		GLUquadricObj *quad2;
		quad2 = gluNewQuadric();
		gluQuadricDrawStyle(quad2, GLU_FILL);
		gluQuadricNormals(quad2, GLU_FLAT);
		gluQuadricTexture(quad2, GL_TRUE);
		//gluDisk(quad2, value[i][0], value[i][1], value[i][2], value[i][3]);
		gluCylinder(quad2, (GLint)value[i][0], (GLint)value[i][1], (GLint)value[i][2], (GLint)value[i][3], (GLint)value[i][4]);
		glPopMatrix();

	}


	fclose(archivo);


}

void lerArquivoConeCoqueiro(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	//float n, m;
	//Poligono poligono;




	for (i = 0; i < linhas + 1; i++) {


		glBindTexture(GL_TEXTURE_2D, texturas[ARVORE1]);
		glPushMatrix();
		glTranslatef(9.7f, 10.0f, 1.0f);
		glRotated(90, 0.0f, 0.0f, 1.0f);
		glRotated(90, 0.0f, 1.0f, 0.0f);
		glutSolidCone(value[i][0], value[i][1], value[i][2], value[i][3]);
		//gluCylinder(quad, 3.0, 3.0, 1.0, 10.0, 10.0);
		glPopMatrix();
	}
	for (i = 0; i < linhas + 1; i++) {


		glBindTexture(GL_TEXTURE_2D, texturas[ARVORE1]);
		glPushMatrix();

		glTranslatef(6.0f, 10.0f, 1.5f);
		glRotated(60, 0.0f, 1.0f, 0.0f);
		glRotated(90, 1.0f, 0.0f, 0.0f);
		glutSolidCone(value[i][0], value[i][1], value[i][2], value[i][3]);
		//gluCylinder(quad, 3.0, 3.0, 1.0, 10.0, 10.0);
		glPopMatrix();
	}

	for (i = 0; i < linhas + 1; i++) {


		glBindTexture(GL_TEXTURE_2D, texturas[ARVORE1]);
		glPushMatrix();

		glTranslatef(9.0f, 10.0f, -1.5f);
		glRotated(60, 0.0f, 1.0f, 0.0f);
		glRotated(90, 1.0f, 0.0f, 0.0f);
		glutSolidCone(value[i][0], value[i][1], value[i][2], value[i][3]);
		//gluCylinder(quad, 3.0, 3.0, 1.0, 10.0, 10.0);
		glPopMatrix();
	}



	fclose(archivo);


}

void lerArquivoCilindroCoqueiro(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	//float n, m;
	//Poligono poligono;




	for (i = 0; i < linhas + 1; i++) {



		glBindTexture(GL_TEXTURE_2D, texturas[TRONCO]);	    /*cilindro que é o corpo do passaro*/
		glPushMatrix();
		glTranslatef(8.0f, 0.0f, 0.0f);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		GLUquadricObj *quad;
		quad = gluNewQuadric();
		gluQuadricNormals(quad, GLU_SMOOTH);
		gluQuadricTexture(quad, GL_TRUE);
		gluCylinder(quad, value[i][0], value[i][1], value[i][2], value[i][3], value[i][4]);
		glPopMatrix();





	}


	fclose(archivo);


}

void lerArquivoEsferaCoqueiro(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	//float n, m;
	//Poligono poligono;


	for (i = 0; i < linhas + 1; i++) {


		glBindTexture(GL_TEXTURE_2D, texturas[COCO]);
		glPushMatrix();

		glTranslatef(6.0f, 10.0f, 1.5f);
		glutSolidSphere(value[i][0], value[i][1], value[i][2]);
		//gluCylinder(quad, 3.0, 3.0, 1.0, 10.0, 10.0);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(8.0f, 9.4f, 0.8f);
		glutSolidSphere(value[i][0], value[i][1], value[i][2]);


		//gluCylinder(quad, 3.0, 3.0, 1.0, 10.0, 10.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(7.2f, 9.2f, 0.4f);
		glutSolidSphere(value[i][0], value[i][1], value[i][2]);


		//gluCylinder(quad, 3.0, 3.0, 1.0, 10.0, 10.0);
		glPopMatrix();


	}

	for (i = 0; i < linhas + 1; i++) {


		glBindTexture(GL_TEXTURE_2D, texturas[COCO]);
		glPushMatrix();
		glTranslatef(8.0f, 9.4f, 0.8f);
		glutSolidSphere(value[i][0], value[i][1], value[i][2]);

		glPopMatrix();
	}



	fclose(archivo);


}


/*função que desenha o cenario, ceu e mar*/
void cenario() {

	GLfloat mat_specular[] = { 1.0f, 0.8f, 0.7f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	GLfloat luzEmissiva[4] = { 0.2f, 0.2f, 0.2f, 0.1f };
	GLfloat luzAmbiente[4] = { 1.0f, 1.0f, 1.0f, 0.2f };                         /*vetores que armazenam dados a serem utilizados na configuracao de luzes*/


																				 //desenha as nuvens em uma esfera
	glBindTexture(GL_TEXTURE_2D, texturas[CEU]);
	glPushMatrix();
	glNormal3f(0, 1, 0);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);                         /*configura as diversas luzes do programa*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, luzEmissiva);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	//glTranslatef(0.0f, 10.0f, 0.0f);
	gluSphere(quad, 350.0f, 10.0f, 10.0f);
	glPopMatrix();


	glBindTexture(GL_TEXTURE_2D, texturas[CEU]);             /*planos que definem os "horizontes" da cena*/
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-400.0f, 0.0f, -400.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-400.0f, 1000.0f, -400.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0f, 1000.0f, -400.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0f, 0.0f, -400.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texturas[CEU3]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-400.0f, 0.0f, 400.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-400.0f, 1000.0f, 400.0f);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(400.0f, 1000.0f, 400.0f);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(400.0f, 0.0f, 400.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texturas[CEU2]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-400.0f, 0.0f, 400.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-400.0f, 1000.0f, 400.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-400.0f, 1000.0f, -400.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-400.0f, 0.0f, -400.0f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0f, 0.0f, 400.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0f, 1000.0f, 400.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0f, 1000.0f, -400.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0f, 0.0f, -400.0f);
	glEnd();
}


/*função que desenha o moinho, modelada atraves de quadrica*/
void moinho() {

	glBindTexture(GL_TEXTURE_2D, texturas[MOINHO]);	    /*cilindro que é o eixo do moinho*/
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	gluCylinder(quad, 2, 1, 9.0f, 10.0f, 10.0f);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[GUARDASOL]);	    /*varios triangulos que modelam o guarda sol*/
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.6f, 8.0f, 1.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.6f, 8.0f, -1.5f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.6f, 8.0f, 1.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 8.0f, 3.0f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.6f, 8.0f, 1.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 8.0f, 3.0f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.6f, 8.0f, 1.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.6f, 8.0f, -1.5f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 8.0f, -3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.6f, 8.0f, -1.5f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 8.0f, -3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.6f, 8.0f, -1.5f);
	glEnd();
}

void paquito() {
	glBindTexture(GL_TEXTURE_2D, texturas[PLACA]);     /*cilindro que é a base do outdoor */
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	gluCylinder(quad, 0.25f, 0.25f, 20.0f, 10.0f, 10.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 15.0f, 0.0f);
	glRotatef(rota_paquito, 0.0f, 0.0f, 1.0f);
	rota_paquito += 0.05f;
	if (rota_paquito > 360) rota_paquito = 0;
	glBindTexture(GL_TEXTURE_2D, texturas[FORRO]);          /*planos que contem as duas fotos de paquito*/
	for (int i = 0; i < 4; i++) {
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 11.0f, 3.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, 11.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 3.0f);
		glEnd();
		glRotatef(90.0, 0.0, 0.0, 1.0);
	}
	glPopMatrix();
}
void passaro() {

	if (contVolta > 359)
		contVolta = 0;
	else
		contVolta++;

	eixoX = cos(contVolta*DEG2RAD)*5.0f;
	eixoY = sin(contVolta*DEG2RAD)*5.3f;


	glBindTexture(GL_TEXTURE_2D, texturas[PENA]);	    /*cilindro que é o corpo do passaro*/
	glPushMatrix();
	glTranslatef(0.0f, -2.5f, 0.0f);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(eixoX, eixoY, 0.0f);

	rota_paquito += 0.05f;
	if (rota_paquito > 360) rota_paquito = 0;

	glTranslatef(15.7f, 0.0f, 16.5f); //posicao do corpo do passaro
	glRotatef(-90, 1.0, 0.0, 0.0); //rotaciona o cilindro para deita-lo

	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	gluCylinder(quad, 0.5, 0.2, 2.5, 10.0, 10.0);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[ASA]);
	glPushMatrix();
	glTranslatef(0.0f, 15.0f, 0.0f);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(eixoX, eixoY, 0.0f);
	rota_paquito += 0.05;
	if (rota_paquito > 360) rota_paquito = 0;

	glTranslatef(16.4f, -1.5f, -1.0f);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 3.0f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 3.0f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.9f, 3.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.9f, 3.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 3.1f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 3.1f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.9f, 3.1f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.9f, 3.1f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 3.1f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 3.0f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.9f, 3.0f, 0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.9f, 3.1f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 3.1f, -0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 3.0f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.9f, 3.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.9f, 3.1f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 3.1f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 3.0f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 3.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 3.1f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 3.1f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 3.0f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 3.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 3.1f, -0.5f);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[CARA]);
	glPushMatrix();
	glTranslatef(-0.1f, 16.5f, -1.0f);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(eixoX, eixoY, 0.0f);

	rota_paquito += 0.05;
	if (rota_paquito > 360) rota_paquito = 0;

	glTranslatef(15.7f, 1.5f, -2.5f);
	glutSolidSphere(0.5, 10, 10);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[BICO]);
	glPushMatrix();
	glTranslatef(-0.1f, 16.5f, -1.0f);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(eixoX, eixoY, 0.0f);

	rota_paquito += 0.05;
	if (rota_paquito > 360) rota_paquito = 0;

	glTranslated(15.7f, 1.5f, -3.0f);  //posicao do bico
	glutSolidCone(0.1, -0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, 16.5f, -1.0f);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(eixoX, eixoY, 0.0f);

	rota_paquito += 0.05;
	if (rota_paquito > 360) rota_paquito = 0;

	glTranslated(15.4f, 1.8f, -2.8f);
	glBindTexture(GL_TEXTURE_2D, texturas[OLHO]);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, 16.5f, -1.0f);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(eixoX, eixoY, 0.0f);

	rota_paquito += 0.05;
	if (rota_paquito > 360) rota_paquito = 0;

	glTranslated(16.0f, 1.8f, -2.8f);
	glBindTexture(GL_TEXTURE_2D, texturas[OLHO]);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
}


void lerArquivoDiamante(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];



	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[CEU2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 30, (poligono.getPontos()[value[i][j]].getZ() / 30));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}

void lerCasa3_telha(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[TELHA2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 30, (poligono.getPontos()[value[i][j]].getZ() / 30));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}
void lerCasa3_telha2(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[TELHA2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 15, (poligono.getPontos()[value[i][j]].getZ() / 15));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}
void lerCasa3_branco(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[BRANCO2]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 1);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 15, (poligono.getPontos()[value[i][j]].getZ() / 15));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}

void lerCasa3_branco2(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[BRANCO2]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 1);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 90, (poligono.getPontos()[value[i][j]].getZ() / 90));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);



}
void lerCasa3_vinho(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturas[VINHO]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 1);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 15, (poligono.getPontos()[value[i][j]].getZ() / 15));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}
void lerCasa3_vinho2(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[VINHO]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 1);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 15, (poligono.getPontos()[value[i][j]].getZ() / 15));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);

}
void lerCasa3_vinho3(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[VINHO]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 1);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 15, (poligono.getPontos()[value[i][j]].getZ() / 15));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}
void lerCasa3_forroPrivi(char* nomeArquivo)
{
	int i;
	int linhas = countlines(nomeArquivo);
	float value[10000][12];


	FILE *archivo;
	archivo = fopen(nomeArquivo, "r");
	if (archivo == NULL)
		exit(1);
	i = 0;
	while (feof(archivo) == 0)
	{
		fscanf(archivo, "%f %f %f %f %f %f %f %f %f %f %f %f", &value[i][0], &value[i][1], &value[i][2], &value[i][3], &value[i][4], &value[i][5], &value[i][6], &value[i][7], &value[i][8], &value[i][9], &value[i][10], &value[i][11]);
		i++;
	}
	float n, m;
	Poligono poligono;

	m = value[0][0];
	n = value[0][1];



	for (i = 1; i < linhas + 1; i++) {

		if (i>0 && i <= m) {
			Pontos p;
			p.setX(value[i][0]);
			p.setY(value[i][1]);
			p.setZ(value[i][2]);
			poligono.adicionarPonto(p);
		}

		if (i > m)
		{
			//DesenhaFunc();
			glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, texturas[FORRO]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			for (int j = 1; j <= value[i][0]; j++)
			{
				glTexCoord2f((poligono.getPontos()[value[i][j]].getX()) / 15, (poligono.getPontos()[value[i][j]].getZ() / 15));
				glVertex3f(poligono.getPontos()[value[i][j]].getX(), poligono.getPontos()[value[i][j]].getY(), poligono.getPontos()[value[i][j]].getZ());


			}
			glEnd();
			glPopMatrix();



		}


	}


	fclose(archivo);


}





/*função que configura os materiais dos objetos que serao chamados para serem desenhados*/
void Desenha() {

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);          // We'll Clear To The Color Of The Fog ( Modified )

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);        // Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
	glFogf(GL_FOG_DENSITY, densidadeFog);
	//cout << densidadeFog;// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
	glFogf(GL_FOG_START, 0.0f);             // Fog Start Depth
	glFogf(GL_FOG_END, 0.0f);               // Fog End Depth
	glEnable(GL_FOG);                   // Enables GL_FOG

	if (densidadeFog < 0.000001)
	{
		glDisable(GL_FOG);
	}


	GLfloat espec_grama[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLint especMaterial = 100;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//glMaterialfv(GL_FRONT, GL_AMBIENT, espec_grama);
	//glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
	glBindTexture(GL_TEXTURE_2D, texturas[AREIA]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-400.0f, 0.0f, -400.0f);
	glTexCoord2f(0.0f, 80.0f); glVertex3f(400.0f, 0.0f, -400.0f);
	glTexCoord2f(80.0f, 80.0f); glVertex3f(400.0f, 0.0f, 400.0f);
	glTexCoord2f(80.0f, 0.0f); glVertex3f(-400.0f, 0.0f, 400.0f);
	glEnd();

	cenario();
									//Moinho
	glPushMatrix();
	glTranslatef(115.0f, 0.0f, -230.0f);
	glScalef(5.0, 4.0, 5.0);
	moinho();
	paquito();
	glPopMatrix();

	passaro();
	estrada();
	cave();

	






	if (cmenu == 0) {
		//glBindTexture(GL_TEXTURE_2D, texturas[P]);
		menu();
	}
	glPushMatrix();
	glTranslatef(7.0f, 0.0f, -60.0f);
	// glScalef(0.7, 0.7, 0.7);
	//	glRotatef(180, 0.0, 1.0, 0.0);

	char* nome = "itens.txt";
	lerArquivo(nome);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 0.0f, -60.0f);
	// glScalef(0.7, 0.7, 0.7);
	//	glRotatef(180, 0.0, 1.0, 0.0);
	//casa();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-75.0f, -6.0f, -45.0f);
	glRotatef(113, 0.0, 1.0, 0.0);
	glScalef(2.2, 2.2, 2.2);
	nome = "bridge.txt";
	lerArquivoBridge(nome);
	nome = "cilindroBridge.txt";
	lerArquivoCilindroBridge(nome);
	glPopMatrix();


	glPushMatrix();

	glTranslatef(0.0f, -11.0f, 0.0f);
	//glRotatef(113, 0.0, 1.0, 0.0);
	glScalef(2.0, 2.0, 2.0);

	//glScalef(1.7, 1.7, 1.7);
	nome = "boat.txt";
	lerArquivoBoat(nome);
	nome = "cilindroBoat.txt";
	lerArquivoCilindroBoat(nome);
	glPopMatrix();

	glPushMatrix();
	nome = "cilindroB.txt";
	lerArquivoCilindroB(nome);
	glPopMatrix();



	glPushMatrix();

	nome = "coneCoqueiro.txt";
	lerArquivoConeCoqueiro(nome);
	nome = "cilindroCoqueiro.txt";
	lerArquivoCilindroCoqueiro(nome);
	nome = "esferaCoqueiro.txt";
	lerArquivoEsferaCoqueiro(nome);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 20.0f, 0.0f);
	glScalef(0.5, 0.5, 0.5);

	nome = "Diamante.txt";
	lerArquivoDiamante(nome);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -128.0f);


	glTranslatef(7.0f, 11.0f, -78.0f);
	char* nome3 = "casa3_telha2.txt";
	lerCasa3_telha2(nome3);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -128.0f);


	char* nome2 = "casa3_telha.txt";
	lerCasa3_telha(nome2);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -128.0f);
	char* nome4 = "casa3_branco.txt";
	lerCasa3_branco(nome4);
	glPopMatrix();


	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -128.0f);
	char* nome5 = "casa3_branco2.txt";
	lerCasa3_branco2(nome5);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -128.0f);
	char* nome6 = "casa3_vinho.txt";
	lerCasa3_vinho(nome6);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -128.0f);
	char* nome7 = "casa3_vinho2.txt";
	lerCasa3_vinho2(nome7);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -128.0f);
	char* nome8 = "casa3_vinho3.txt";
	lerCasa3_vinho3(nome8);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -128.0f);
	char* nome9 = "casa3_forroPrivi.txt";
	lerCasa3_forroPrivi(nome9);
	glPopMatrix();

														//CASA 1///

	glPushMatrix();
	glTranslatef(110.0f, 5.0f, 25.0f);
	glScalef(0.7, 0.4, 0.7);
	glRotatef(90, 0.0, 1.0, 0.0);
	char* nome10 = "casa1_telha.txt";
	lerCasa1_telha(nome10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(110.0f, 5.0f, 25.0f);
	glScalef(0.7, 0.4, 0.7);
	glRotatef(90, 0.0, 1.0, 0.0);
	char* nome12 = "casa1_madeira.txt";
	lerCasa1_madeira(nome12);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(110.0f, 5.0f, 25.0f);
	glScalef(0.7, 0.4, 0.7);
	glRotatef(90, 0.0, 1.0, 0.0);
	char* nome13 = "casa1_madeira2.txt";
	lerCasa1_madeira2(nome13);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(110.0f, 5.0f, 25.0f);
	glScalef(0.7, 0.4, 0.7);
	glRotatef(90, 0.0, 1.0, 0.0);
	char* nome14 = "casa1_madeira3.txt";
	lerCasa1_madeira3(nome14);
	glPopMatrix();

														//CASA2///
	glPushMatrix();
	//glTranslatef(7.0f, 11.0f, -78.0f);
	char* nome15 = "casa2_telha.txt";
	lerCasa2_telha(nome15);
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(7.0f, 11.0f, -78.0f);
	char* nome16 = "casa2_madeira.txt";
	lerCasa2_madeira(nome16);
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(7.0f, 11.0f, -78.0f);
	char* nome17 = "casa2_faixa.txt";
	lerCasa2_faixa(nome17);
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(7.0f, 11.0f, -78.0f);
	char* nome18 = "casa2_madeira2.txt";
	lerCasa2_madeira2(nome18);
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(7.0f, 11.0f, -78.0f);
	char* nome19 = "casa2_madeira3.txt";
	lerCasa2_madeira3(nome19);
	glPopMatrix();

	// Array to specify position
	GLfloat  lightPos[] = { 1.0f, 1.0f, 360.0f, 1.0f };
	GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat  spotDir[] = { -1.0f, -1.0f, -1.0f };

	GLfloat lt_ambient[] = { 0.0, 0.0, 0.0, 1.0 };

	// Enable this light in particular


	// Specific spot effects
	// Cut off angle is 60 degrees


	glBindTexture(GL_TEXTURE_2D, texturas[BICO]);
	glPushMatrix();
	// Front material ambient and diffuse colors track glColor

	// Specify new position and direction in rotated coords.
	if (fontePressionada) {

		glLightfv(GL_LIGHT0, GL_AMBIENT, lt_ambient);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	}
	else {
		glEnable(GL_LIGHT1);
		GLfloat mat_specular[] = { 1.0f, 0.8f, 0.7f, 1.0f };
		GLfloat mat_shininess[] = { 100.0f };
		GLfloat luzEmissiva[4] = { 0.2f, 0.2f, 0.2f, 0.1f };
		GLfloat luz[4] = { 0.0f, 0.0f, 0.0f, 1.0f };                         /*vetores que armazenam dados a serem utilizados na configuracao de luzes*/



		glLightfv(GL_LIGHT0, GL_AMBIENT, luz);                         /*configura as diversas luzes do programa*/
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, luzEmissiva);
		luzAmbiente = 1.0f;
	}

	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
	glNormal3f(0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(10, 15, 50, 50);
	glEnd();

	glPopMatrix();






	glutSwapBuffers();


}

void Timer(int value)
{

	//FPS = MapaNos.size()/50;

	FPS = 200 / 45;
	//if (delayItem < DELAY_SPAWN_ITEM) {
	//	delayItem += FPS;
	//}
	//value++;
	if (delay_abertura < DELAY_ABERTURA) {
		delay_abertura += FPS;
		glutTimerFunc(FPS, Timer, 0);
	}

	glutTimerFunc(FPS, Timer, 0);
}


/*função principal do programa*/
int main(int argc, char **argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Donkey Conga");

	Inicializa();
	glutKeyboardFunc(TeclasNormais);
	glutSpecialFunc(TeclasEspeciais);

	glutDisplayFunc(Desenha);
	glutIdleFunc(Desenha);
	//glutFullScreen();
	glutReshapeFunc(AlteraTamanho);
	glutTimerFunc(30, Timer, 30);
	glutMainLoop();


	return(0);



}

