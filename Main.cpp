#include <iostream>
#include "neurona.h"
using namespace std;

const int CANTIDAD_ENTRADAS = 1;
const int CANTIDAD_SALIDAS = 1;
const int CANTIDAD_CAPAS = 2;
const int CAPAS[] = {3,2};

const int PUNTOS_ENTRENAMIENTO = 20;
const int PUNTOS_PRUEBA = 50;
const double X_i = 0.0;
const double X_f = 2.0 * M_PI;

const int EPOCAS = 1000;
const double TASA_APRENDIZAJE = 0.8;
const double ERROR_MAXIMO = 0.001;

double func(double x){
	return sin(x);
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
		cout<<"Epoca: "<<k+1<<"/"<<EPOCAS<<endl;
		for(int i = 0; i < PUNTOS_ENTRENAMIENTO ; i++){
		//	cout<<"\tDato:  "<<i+1<<"/"<<PUNTOS_ENTRENAMIENTO<<endl;
			rna.entrena(entradas[i], salidas[i]);
		}
		// if(rna.errorMedio() < ERROR_MAXIMO){
		// 	break;
		// }

		rna.muestraTopologia();
	}

	for(int i = 0; i < PUNTOS_PRUEBA; i++){
		pruebas[i] = double(i)*(X_f - X_i)/(PUNTOS_PRUEBA - 1);
		resultados[i] = rna.evalua(pruebas[i]);
		cout<<"["<<pruebas[i]<<":"<<resultados[i]<<"]"<<" error: "<<func(pruebas[i])-resultados[i]<<endl;
	}

	return 0;
}