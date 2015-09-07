#ifndef HEAP_H
#define HEAP_H 

template<class T>
void heap_insert(T* cola, int& size, const T& c){
	cola--;
	cola[++size]=c;
	int idx = size;
	while(idx>1 and cola[idx]<cola[idx/2]){
		swap(cola[idx],cola[idx/2]);
		idx/=2;
	}
}


template<class T>
T heap_pop(T* cola,int& size){//,bool (*cpm)(const T&, const T& ) = nullptr){
	cola--;
	T retorno = cola[1];
	cola[1]=cola[size--];
	int idx = 1;
	while(idx<size){
		int izq=idx*2;
		int der=izq+1;
		if(izq<=size and (cola[izq]<cola[der] or der>size) and (cola[izq]<cola[idx])){
			swap(cola[izq],cola[idx]);
			idx=izq;
		}else if(cola[der] < cola[idx] and der<=size)
		{
			swap(cola[der],cola[idx]);
			idx=der;
		}
		else break;
	}


	return retorno;
}


template<class T, typename F>
void heap_insert(T* cola, int& size, const T& c ,F cmp){
	cola--;
	cola[++size]=c;
	int idx = size;
	while(idx>1 and cmp(cola[idx],cola[idx/2])){
		std::swap(cola[idx],cola[idx/2]);
		idx/=2;
	}
}


template<class T, typename F>
T heap_pop(T* cola,int& size,F cmp){
	cola--;
	T retorno = cola[1];
	cola[1]=cola[size--];
	int idx = 1;
	while(idx<size){
		int izq=idx*2;
		int der=izq+1;
		if(izq<=size and ( der>size or cmp(cola[izq],cola[der]) ) and cmp(cola[izq],cola[idx])){
			std::swap(cola[izq],cola[idx]);
			idx=izq;
		}else if(der<=size and cmp(cola[der], cola[idx])){
			std::swap(cola[der],cola[idx]);
			idx=der;
		}
		else break;
	}


	return retorno;
}

#endif