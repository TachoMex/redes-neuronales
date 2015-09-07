#include <iostream>
#include "neurona.h"
#include "imagen/imagen.h"
#include "imagen/primitivas.h"

using namespace std;

const int CANTIDAD_ENTRADAS = 1;
const int CANTIDAD_SALIDAS = 1;
const int CANTIDAD_CAPAS = 1;
const int CAPAS[] = {4};

const int PUNTOS_ENTRENAMIENTO = 8;
const int PUNTOS_PRUEBA = 50;
const double X_i = 0.0;
const double X_f = 2.0 * M_PI;

const int EPOCAS = 100;
const double TASA_APRENDIZAJE = 0.1;
const double ERROR_MAXIMO = 0.001;

double func(double x){
	return sin(x);
}

void graficaPuntos(double xi, double yi, double xf, double yf, double x[], double y[], int n,Color c, Imagen& I){
	double dx = (xf - xi);
	double dy = (yf - yi);
	for(int i=1;i<n;i++){
		int x1 = (int)floor((x[i-1]-xi)*I.columnas()/dx);
		int y1 = (int)floor((y[i-1]-yi)*I.filas()/dy);
		int x2 = (int)floor((x[i]-xi)*I.columnas()/dx);
		int y2 = (int)floor((y[i]-yi)*I.filas()/dy);
		primitivas::linea(I, x1,y1,x2,y2,c);
	}
}

int main(){
	srand((unsigned)time(NULL));

	RedNeuronal rna(CANTIDAD_ENTRADAS, CANTIDAD_SALIDAS, CANTIDAD_CAPAS, CAPAS, TASA_APRENDIZAJE);
	double entradas[PUNTOS_ENTRENAMIENTO];
	double salidas[PUNTOS_ENTRENAMIENTO];

	double pruebas[PUNTOS_PRUEBA];
	double resultados[PUNTOS_PRUEBA];

	for(int i=0;i<PUNTOS_ENTRENAMIENTO;i++){
		entradas[i] = double(i)*(X_f - X_i)/(PUNTOS_ENTRENAMIENTO - 1);
		salidas[i] = func(entradas[i]);
	}

	//rna.muestraTopologia();
	//return 0;
	cout<<"Calculados los datos de entrenamiento: "<<PUNTOS_ENTRENAMIENTO<<endl;
	cout<<"Inicia el entrenamiento"<<endl;
	for(int k = 0; k < EPOCAS; k++){
		rna.inicializaError();
		for(int i = 0; i < PUNTOS_ENTRENAMIENTO ; i++){
		//	cout<<"\tDato:  "<<i+1<<"/"<<PUNTOS_ENTRENAMIENTO<<endl;
			rna.entrena(entradas[i], salidas[i]);
		}
		cout<<"Epoca: "<<k+1<<"/"<<EPOCAS<<". Error: "<<rna.errorMedio()<<endl;
		// if(rna.errorMedio() < ERROR_MAXIMO){
		// 	break;
		// }
		rna.ajustaAprendizaje();
		//rna.muestraTopologia();
	}
	rna.muestraTopologia();
	for(int i = 0; i < PUNTOS_PRUEBA; i++){
		pruebas[i] = double(i)*(X_f - X_i)/(PUNTOS_PRUEBA - 1);
		resultados[i] = rna.evalua(pruebas[i]);
		cout<<"["<<pruebas[i]<<":"<<resultados[i]<<"]"<<" error: "<<func(pruebas[i])-resultados[i]<<endl;
	}
	Imagen I(800,600);
	graficaPuntos(X_i, -1, X_f, 1, entradas, salidas, PUNTOS_ENTRENAMIENTO, Color::verde,I);
	graficaPuntos(X_i, -1, X_f, 1, pruebas,resultados , PUNTOS_PRUEBA, Color::rojo,I);
	I.guardaBMP("grafica.bmp");

	return 0;
}