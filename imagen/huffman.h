#ifndef HUFFMAN_H
#define HUFFMAN_H 
#include <iostream>
#include <map>
#include "heap.h"

template<class T>
class NodoHuffman;

#define ArbolVacio nullptr

template<class T>
class NodoHuffman{
	public:
		T letra;
		NodoHuffman* izq;
		NodoHuffman* der;
		int repeticiones;
	
	NodoHuffman():letra(){
		izq = ArbolVacio;
		der = ArbolVacio;
		repeticiones=0;
	}

	NodoHuffman(const T& l, int x){
		izq = ArbolVacio;
		der = ArbolVacio;
		letra=l;
		repeticiones=x;
	}
};

template<class T>
bool es_hoja(NodoHuffman<T>*nodo){
	if(nodo==ArbolVacio)
		return false;
	return nodo->izq==ArbolVacio and nodo->der==ArbolVacio;
}

template<class T>
void muestraHuffman(NodoHuffman<T>* ptr){
	if(ptr == ArbolVacio){
		std::cout<<"ArbolVacio";
	}else{
		std::cout<<"("<<ptr->letra<<":"<<ptr->repeticiones<<" ";
		muestraHuffman(ptr->izq);
		std::cout<<" ";
		muestraHuffman(ptr->der);
		std::cout<<")";
	}
}


template<typename T,typename Pointer>
NodoHuffman<T>* codifica(Pointer begin,Pointer end){
	std::map<T,int> repeticiones;
	for(Pointer i=begin;i!=end;i++){
		repeticiones[*i]++;
	}
	NodoHuffman<T>** cola = new NodoHuffman<T>*[repeticiones.size()];
	int size = 0;
	auto cmp = [](NodoHuffman<T>* a, NodoHuffman<T>* b)-> bool {
		return a->repeticiones < b->repeticiones;
	};
	for(typename std::map<T,int>::iterator i = repeticiones.begin();i!=repeticiones.end();i++){
		heap_insert(cola ,size, new NodoHuffman<T>(i->first,i->second),cmp);
	}
	while(size>1){
		NodoHuffman<T>* a = heap_pop(cola,size,cmp);
		NodoHuffman<T>* b = heap_pop(cola,size,cmp);
		NodoHuffman<T>* c = new NodoHuffman<T>(T(),a->repeticiones+b->repeticiones);
		c->izq = b;
		c->der = a;
		heap_insert(cola,size,c,cmp);
	}
	NodoHuffman<T>* tree = heap_pop(cola,size,cmp);
	delete[] cola;
	return tree;
}

template<class T>
void construye_tabla(int profundidad, std::vector<T> tabla[],NodoHuffman<T>*nodo){
	if(nodo!=ArbolVacio){
		if(es_hoja(nodo)){
			tabla[profundidad].push_back(nodo->letra);
		}else{
			construye_tabla(profundidad+1,tabla, nodo->izq);
			construye_tabla(profundidad+1,tabla, nodo->der);
		}
	}
}

template<class T>
void construye_diccionario(std::vector<bool>&profundidad,std::map<T,std::vector<bool>>& tabla, NodoHuffman<T>*nodo){
	if(nodo!=ArbolVacio){
		if(es_hoja(nodo)){
			if(not tabla.count(nodo->letra))
			tabla[nodo->letra]=profundidad;
		}else{
			profundidad.push_back(0);
			construye_diccionario(profundidad,tabla, nodo->izq);
			profundidad.pop_back();
			profundidad.push_back(1);
			construye_diccionario(profundidad,tabla, nodo->der);
			profundidad.pop_back();
		}
	}
}

template<class T>
void borrar(NodoHuffman<T>*nodo){
	if(nodo!=ArbolVacio){
		borrar(nodo->izq);
		borrar(nodo->der);
		delete nodo;
	}
}
#endif