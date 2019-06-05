#pragma once
// classes example
#include <GL/glut.h>
#include "Pontos.h"
#include "vector"
#include <iostream>

using namespace std;

class Poligono {
	int numeroLados;
	vector<Pontos> pontos;



public:

	Poligono() {}
	void setNumeroLados(int numeroLados) {
		this->numeroLados = numeroLados;
	}

	int getNumeroLados() {
		return this->numeroLados;
	}

	void setPontos(vector<Pontos> pontos) {
		this->pontos = pontos;
	}

	void adicionarPonto(Pontos pontos) {
		this->pontos.push_back(pontos);

	}

	vector<Pontos> getPontos() {
		return this->pontos;
	}

	void desenhar() {


	}






};
#pragma once
#pragma once
