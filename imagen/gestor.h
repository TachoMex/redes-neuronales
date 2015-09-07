#ifndef GESTOR_H
#define GESTOR_H 

#include "imagen.h"
#include <map>
#include <string>
#include <fstream>

class GestorImagenes{
	std::map<std::string, Imagen*> imagens;
public:
	Imagen* abrir(const std::string &s);
	Imagen* operator[](const std::string &s){
		return abrir(s);
	}
};

Imagen* GestorImagenes::abrir(const std::string &s){
	if(imagens.count(s))
		return imagens[s];
	std::ifstream f(s);
	if(f.good()){
		f.close();
		Imagen * i = new Imagen();
		i->leeBMP(s.c_str());
		return imagens[s]=i;
	}
	return nullptr;
}

#endif