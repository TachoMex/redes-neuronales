#ifndef IMAGEN_H
#define IMAGEN_H

#include <cstring>
#include "color.h"
#include <algorithm>
#include "jpgutil.h"
#include <queue>


class Imagen{
	private:
		Color*pixels;
		int x, y;

	public:
		Imagen(int a, int b){
			x=a;
			y=b;
			pixels= new Color[x*y];
		}
		
		Imagen(){
			pixels=NULL;
			x=y=0;
		}
		
		Imagen(int a, int b,const Color& c){
			x=a;
			y=b;
			pixels = new Color[x*y];
			for(int i=0;i<x*y;i++)
				pixels[i]=c;
		}

		Imagen(const Imagen &I){
			x=I.x;
			y=I.y;
			pixels=new Color[x*y];
			for(int i=0;i<x*y;i++)
				pixels[i]=I.pixels[i];
		}


		
		/*
		Imagen(Imagen&& I){
			x=I.x;
			y=I.y;
			pixels = I.pixels;
		}
		Imagen operator=(Imagen&& I){
			x=I.x;
			y=I.y;
			pixels = I.pixels;
			return *this;
		}
		*/
		int filas()const{
			return y;
		}
		
		int columnas()const{
			return x;
		}
		
		Color& en(int i, int j){
			if(i < 0 or i >= y or j < 0 or j >= x )
				throw 0;
			//std::cout<<i*x+j<<std::endl;
			return pixels[i*x+j];
		}

		Color en(int i, int j)const{
			if(i < 0 or i >= y or j < 0 or j >= x )
				throw 0;
			//std::cout<<i*x+j<<std::endl;
			return pixels[i*x+j];
		}
		
		Imagen escalaDeGrises()const{
			Imagen ret(x,y);
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++)
					ret.en(i,j) = en(i,j).aGris();
			return ret;
		}

		Imagen derivadax()const {
			Imagen ret(x,y);
			for(int i=0;i<y;i++){
				ret.en(i,0)=en(i,0);
				for(int j=1;j<x;j++)
					ret.en(i,j) =maxDif(en(i,j),en(i,j-1));
			}
			return ret;
		}

		Imagen derivaday()const{
			Imagen ret(x,y);
			for(int i=0;i<x;i++){
				ret.en(0,i)=en(0,i);
				for(int j=1;j<y;j++)
					ret.en(j,i) = maxDif(en(j,i),en(j-1,i));
			}
			return ret;
		}

		Imagen derivada()const{
			Imagen dx=derivadax();
			Imagen dy=derivaday();
			Imagen ret(x,y);

			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++)
					ret.en(i,j) =max(dx.en(i,j),dy.en(i,j));
			}
			return ret;
		}


		Imagen umbral(unsigned char min=128, Color down=Color())const{
			Imagen ret(x,y);
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++)
					ret.en(i,j) = (en(i,j).luz()>min?en(i,j):down);
			return ret;
		}

		Imagen umbral(unsigned char min, const Color& down,const Color& up)const{
			Imagen ret(x,y);
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++)
					ret.en(i,j) = (en(i,j).luz()>min?up:down);
			return ret;
		}

		void copiar(int h, int k, Imagen &o)const{
			for(int i=0;i<o.y;i++)
				for(int j=0;j<o.x;j++){
					int p,q;
					p=h+i;
					q=k+j;
					try{
						en(p,q) = o.en(i,j);
					}catch(int e){
					
					}
				}
		}
	
		~Imagen(){
			delete[] pixels;
		}
		
		Imagen operator=(const Imagen& I){
			delete pixels;
			x=I.x;
			y=I.y;
			pixels = new Color[x*y];
			for(int i=0;i<x*y;i++)
				pixels[i]=I.pixels[i];
			return *this;
		}

		
		Imagen filtroMatriz(double coeficientes[3][3])const {
			Imagen ret(x,y);
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++){
					double sr=0;
					double sg=0;
					double sb=0;
					for(int k=-1;k<2;k++)
						for(int l=-1;l<2;l++){
							try{
								Color aux=en(i+k,j+l);
								sr+=(double)aux.r*coeficientes[k+1][l+1];
								sg+=(double)aux.g*coeficientes[k+1][l+1];
								sb+=(double)aux.b*coeficientes[k+1][l+1];
							}catch(int e){
							}
						}

					if(sr>255)
						sr=255;
					if(sr<0)
						sr=0;
					if(sg>255)
						sg=255;
					if(sg<0)
						sg=0;
					if(sb>255)
						sb=255;
					if(sb<0)
						sb=0;
					ret.en(i,j) = Color(sr,sg,sb);
				}
				//ret.guardaBMP("filtro.bmp");
			}
			return ret;
		}

		Imagen filtroGauss()const{
			double coeficientes[3][3]={
				{1.0/21,1.0/7,1.0/21},
				{1.0/7,5.0/21,1.0/7},
				{1.0/21,1.0/7,1.0/21}
			};
			return filtroMatriz(coeficientes);	
		}

		Imagen filtroAfilador()const{
			double coeficientes[3][3]={
			 {  0, -1,  0},
    		 {-1,  5, -1},
     		 {	0, -1,  0 }
     		};
     		return filtroMatriz(coeficientes);
		}

		Imagen filtroMediana()const{
			double coeficientes[3][3]={
				{1.0/9.0,1.0/9.0,1.0/9.0},
				{1.0/9.0,1.0/9.0,1.0/9.0},
				{1.0/9.0,1.0/9.0,1.0/9.0}
			};
			return filtroMatriz(coeficientes);
		}

		Imagen erosionar()const{
			Imagen ret(x,y);
			int dx[] = {1,-1,0,0};
			int dy[] = {0,0,1,-1};
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++){
					Color c=en(i,j);
					for(int k=0;k<4;k++){
						try{
							Color z=en(i+dy[k],j+dx[k]);
							c=min(c,z); 
						}catch(int e){

						}
					}
					ret.en(i,j) = c;
				}
			return ret;
		}

		Imagen dilatar()const{
			Imagen ret(x,y);
			int dx[] = {1,-1,0,0};
			int dy[] = {0,0,1,-1};
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++){
					Color c=en(i,j);
					for(int k=0;k<4;k++){
						try{
							Color z=en(i+dy[k],j+dx[k]);
							c=max(c,z); 
						}catch(int e){

						}
					}
					ret.en(i,j) = c;
				}
			return ret;
		}

		Imagen laplace()const{
			double coeficientes[3][3]={
			 {  -1, -1,  -1},
    		 {-1,  8, -1},
     		 {	-1, -1,  -1 }
     		};
     		return filtroMatriz(coeficientes);
		}

	//private:
		static int max_bit(int n){
			if(n==0)
				return 0;
			int i=1;
			int k=0;
			while(n){
				if(n&i)
					n^=i;
				i<<=1;
				k++;
			}
			return k;
		}

		static void rcl(int*zigzag,std::vector<int>& cadena, std::vector<int>& coeficientes, const Matrix8x8& M){
			int i=1;
			int cont_z = 0;
			while(i<64){
				if(zigzag[i]!=0){
					if(cont_z>15){
						cadena.push_back(0xf0);
						cont_z-=16;
						continue;
					}else{
						if(cont_z*16+max_bit(abs(zigzag[i]))>255){
							std::cout<<M<<std::endl;
							std::cout<<Matrix8x8::DCTMatrix*M*(Matrix8x8::DCTMatrix.traspuesta())<<std::endl;
							std::cout<<cuantificar(Matrix8x8::DCTMatrix*M*(Matrix8x8::DCTMatrix.traspuesta()),Matrix8x8::HQMatrix)<<std::endl;

						}
						cadena.push_back(cont_z*16+max_bit(abs(zigzag[i])));
						coeficientes.push_back(zigzag[i]);
						cont_z=0;
					}
				}else{
					cont_z++;
				}
				i++;
			}
			if(cont_z){
				cadena.push_back(0);
			}
		}

		void escribe_tabla(std::ofstream& f,std::vector<int> tabla[], int identificador){
			if(tabla[0].size()){
				tabla[1].push_back(tabla[0][0]);	
			}
			int longitud=18;
			for(int i=1;i<=16;i++){
				longitud+=tabla[i].size();
			}
			f.put(0xff); f.put(0xc4);
			f.put(longitud/256); f.put(longitud%256);
			f.put(identificador);
			for(int i=1;i<=16;i++){
				f.put(tabla[i].size());
			}
			for(int i=1;i<=16;i++){
				for(unsigned j=0;j<tabla[i].size();j++){
					f.put(tabla[i][j]);
				}
			}
		}

		NodoHuffman<int>* construye_arbol(std::vector<int> tabla[]){
			if(tabla[0].size()){
				NodoHuffman<int>* arbol = new NodoHuffman<int>();
				arbol->izq=new NodoHuffman<int>();
				arbol->der=new NodoHuffman<int>();
				arbol->izq->letra = tabla[0][0];
				arbol->der->letra = tabla[0][0]+1;
				return arbol;
			}
			std::queue<NodoHuffman<int>*> cola; 
			NodoHuffman<int>* arbol = new NodoHuffman<int>();
			arbol->letra=123123;
			arbol->repeticiones=0;
			arbol->izq = new NodoHuffman<int>();
			arbol->der = new NodoHuffman<int>();
			arbol->izq->repeticiones = 1;
			arbol->izq->letra = 4200;
			arbol->der->repeticiones = 1;
			arbol->der->letra = 4200;
			cola.push(arbol->izq);
			cola.push(arbol->der);
			for(int i=1;i<=16;i++){
				for(int codigo:tabla[i]){
					NodoHuffman<int>* aux = cola.front();
					cola.pop();
					aux->letra = codigo;
				}
				if(i!=16)
				while(not cola.empty() and cola.front()->repeticiones==i){
					NodoHuffman<int>* aux = cola.front();
					cola.pop();
					aux->izq = new NodoHuffman<int>();
					aux->der = new NodoHuffman<int>();
					aux->izq->repeticiones = aux->repeticiones+1;
					aux->der->repeticiones = aux->repeticiones+1;
					cola.push(aux->izq);
					cola.push(aux->der);
				}
			}

			return arbol;
		}

		void escribe_minbyte(int categoria, int dato,std:: vector<bool>& flujobytes){
			if(dato<0)
				dato=~dato;
			for(int k=categoria-1;k>=0;k--){
				flujobytes.push_back(dato>>k&1);
			}
		}

		void escribe_ac(std::vector<int>& cadena,std::vector<bool>& flujo,std::map<int,std::vector<bool>>& diccionario,std:: vector<int>& coeficientes){
			bool EOB = false;
			for(int i=0;i<63 and not EOB;i++){
				flujo.insert(flujo.end(),diccionario[cadena[0]].begin(), diccionario[cadena[0]].end());
				int categoria = cadena[0]%16; 
				EOB = cadena[0]==0;
				cadena.erase(cadena.begin());
				if(categoria!=0){	
					int dato = coeficientes[0];
					coeficientes.erase(coeficientes.begin());
					escribe_minbyte(categoria,dato, flujo);
				}
			}
		}

		void escribe_dc(std::vector<int>& cadena,std::vector<bool>& flujo,std::map<int,std::vector<bool>>& diccionario,std:: vector<int>& coeficientes){
			flujo.insert(flujo.end(),diccionario[cadena[0]].begin(), diccionario[cadena[0]].end());
			int categoria = cadena[0]%16; 
			cadena.erase(cadena.begin());
			if(categoria!=0){	
				int dato = coeficientes[0];
				escribe_minbyte(categoria,dato, flujo);
			}
			coeficientes.erase(coeficientes.begin());
		}

	public:



		void guardaJPG(const char*nombre){
			std::ofstream f(nombre);
			
			//cabeceras
			f.put(0xff); f.put(0xd8); f.put(0xff); f.put(0xe0);
			f.put(0x00); f.put(0x10); f.put(0x4a); f.put(0x46);
			f.put(0x49); f.put(0x46); f.put(0x00); f.put(0x01);
			f.put(0x01); f.put(0x00); f.put(0x00); f.put(0x01);
			f.put(0x00); f.put(0x01); f.put(0x00); f.put(0x00);
			//Inicia escritura de las tablas de cuantificacion
			f.put(0xff); f.put(0xdb);
			f.put(0x00); f.put(0x84); //longitud
			f.put(0x00); 			  //Luminancia
			int* matriz = Matrix8x8::HQMatrix.zigzag();
			for(int i=0;i<64;i++){
				f.put(matriz[i]);
			}
			f.put(0x01); 			  //Crominancia
			for(int i=0;i<64;i++){
				f.put(matriz[i]);
			}
			delete[] matriz;		

			f.put(0xff); f.put(0xc0); 
			f.put(0x00); f.put(0x11); 
			f.put(0x08); f.put(y/256);
			f.put(y%256); f.put(x/256); 
			f.put(x%256); f.put(0x03); 
			f.put(0x01); f.put(0x11); 
			f.put(0x00); f.put(0x02); 
			f.put(0x11); f.put(0x01); 
			f.put(0x03); f.put(0x11); 
			f.put(0x01);

			//Calcular la cantidad de chunks
			int chunks_y = columnas()/8;
			int chunks_x = filas()/8;
			//int cantidad_chunks = chunks_x*chunks_y;

			//Se crean varios arreglos para las cadenas que seran reducidas con huffman
			std::vector<int> cadena_YAC;
			std::vector<int> coeficientes_YDC;
			std::vector<int> cadena_YDC;
			std::vector<int> coeficientesY;
			std::vector<int> cadena_CAC;
			std::vector<int> cadena_CDC;
			std::vector<int> coeficientesC;
			std::vector<int> coeficientesDCr;
			std::vector<int> coeficientesDCb;

			for(int i=0;i<chunks_x;i++){
				for(int j=0;j<chunks_y;j++){

					//Se calculan 3 matrices para cada uno de los canales de color
					Matrix8x8 Y, Cb, Cr;
					for(int x=0;x<8;x++){
						for(int y=0;y<8;y++){
							//Se obtiene el pixel actual de la matriz y se convierte de RGB a YCbCr
							YcbcrColor pix = en(i*8+x,j*8+y);
							Y[x][y] = pix.y;
							Cb[x][y] = pix.cb;
							Cr[x][y] = pix.cr;
						}
					}

					Matrix8x8 Y2=Y, Cb2=Cb, Cr2=Cr;

				//	cout<<Cr2<<endl;

					//Se calcula la DCT de cada una de las matrices y se cuantifica, por ahora con la matriz de alta calidad
					Y = cuantificar(Matrix8x8::DCTMatrix*Y*(Matrix8x8::DCTMatrix.traspuesta()),Matrix8x8::HQMatrix);
					Cb = cuantificar(Matrix8x8::DCTMatrix*Cb*(Matrix8x8::DCTMatrix.traspuesta()),Matrix8x8::HQMatrix);
					Cr = cuantificar(Matrix8x8::DCTMatrix*Cr*(Matrix8x8::DCTMatrix.traspuesta()),Matrix8x8::HQMatrix);

					//Se obtienen los coeficientes de las matrices en zigzag
					int *zigzagY = Y.zigzag();
					int *zigzagCb = Cb.zigzag();
					int *zigzagCr = Cr.zigzag();

/*
	//Despues se corre un RLC en cada uno de los coeficientes. para cada valor, se guarda como un par (z,d)
	//donde z es la cantidad de ceros que anteceden al dato y d es la cantidad minima de bits que se requieren para codificar al dato 
	//Se usa el siguiente sistema:
	//Ambos datos se guardan en un byte, por lo que no puede haber coeficientes mayores a 15
	//(15,0) representa un bloque de 16 ceros consecutivos. 
	*				Values             Category        Bits for the value
	*					0            	        0                   -
	*			      -1,1                  	1                  0,1
	*			   -3,-2,2,3   		            2              00,01,10,11
	*		  	   -7,-6,-5,-4,4,5,6,7          3    000,001,010,011,100,101,110,111
	*		       -15,..,-8,8,..,15            4       0000,..,0111,1000,..,1111
	*		      -31,..,-16,16,..,31           5     00000,..,01111,10000,..,11111
	*		      -63,..,-32,32,..,63           6                   .
	*		     -127,..,-64,64,..,127          7                   .
	*		    -255,..,-128,128,..,255         8                   .
	*		    -511,..,-256,256,..,511         9                   .
	*		   -1023,..,-512,512,..,1023       10                   .
	*		  -2047,..,-1024,1024,..,2047      11                   .
	*		  -4095,..,-2048,2048,..,4095      12                   .
	*		  -8191,..,-4096,4096,..,8191      13                   .
	*		 -16383,..,-8192,8192,..,16383     14                   .
	*		-32767,..,-16384,16384,..,32767    15                   .
*/
					rcl(zigzagY, cadena_YAC, coeficientesY, Y2);
					rcl(zigzagCb, cadena_CAC, coeficientesC, Cb2);
					rcl(zigzagCr, cadena_CAC, coeficientesC, Cr2);
					coeficientes_YDC.push_back(zigzagY[0]);
					coeficientesDCb.push_back(zigzagCb[0]);
					coeficientesDCr.push_back(zigzagCr[0]);
					//se liberan los datos
					delete[] zigzagY;
					delete[] zigzagCr;
					delete[] zigzagCb;
				}
			}	

			for(unsigned i=coeficientes_YDC.size()-1;i>0;i--){
				coeficientes_YDC[i]-=coeficientes_YDC[i-1];
				coeficientesDCr[i]-=coeficientesDCr[i-1];
				coeficientesDCb[i]-=coeficientesDCb[i-1];
			}


			for(unsigned i=0;i<coeficientes_YDC.size();i++){
				std::cout<<coeficientes_YDC[i]<<" ";
			}
			std::cout<<std::endl;
			/*
			for(int i=0;i<coeficientesDCb.size();i++){
				cout<<coeficientesDCb[i]<<" ";
			}
			cout<<endl;

			for(int i=0;i<coeficientesDCr.size();i++){
				cout<<coeficientesDCr[i]<<" ";
			}
			cout<<endl;*/
			//Los coeficientes de DC se calculan con diferencias
			for(unsigned i=0;i<coeficientes_YDC.size();i++){
				cadena_YDC.push_back(max_bit(abs(coeficientes_YDC[i])));
				cadena_CDC.push_back(max_bit(abs(coeficientesDCb[i])));
				cadena_CDC.push_back(max_bit(abs(coeficientesDCr[i])));
			}

			std::cout<<"YDC:"<<std::endl;
			for(int i: cadena_YDC){
				std::cout<<i<<" ";
			}
			std::cout<<std::endl;

			std::cout<<"CDC:"<<std::endl;
			for(int i: cadena_CDC){
				std::cout<<i<<" ";
			}
			std::cout<<std::endl;


			NodoHuffman<int> * arbolYAC = codifica<int,std::vector<int>::iterator>(cadena_YAC.begin(), cadena_YAC.end());
			NodoHuffman<int> * arbolYDC = codifica<int,std::vector<int>::iterator>(cadena_YDC.begin(), cadena_YDC.end());
			NodoHuffman<int> * arbolCAC = codifica<int,std::vector<int>::iterator>(cadena_CAC.begin(), cadena_CAC.end());
			NodoHuffman<int> * arbolCDC = codifica<int,std::vector<int>::iterator>(cadena_CDC.begin(), cadena_CDC.end());
			std::vector<int> tabla_YAC[30];
			std::vector<int> tabla_YDC[30];
			std::vector<int> tabla_CAC[30];
			std::vector<int> tabla_CDC[30];
			construye_tabla(0,tabla_YAC,arbolYAC);
			construye_tabla(0,tabla_CAC,arbolCAC);
			construye_tabla(0,tabla_CDC,arbolCDC);
			construye_tabla(0,tabla_YDC,arbolYDC);
			escribe_tabla(f,tabla_YAC,0x10);
			escribe_tabla(f,tabla_YDC,0x00);
			escribe_tabla(f,tabla_CAC,0x11);
			escribe_tabla(f,tabla_CDC,0x01);
			borrar(arbolYAC); arbolYAC = ArbolVacio;
			borrar(arbolYDC); arbolYDC = ArbolVacio;
			borrar(arbolCDC); arbolCDC = ArbolVacio;
			borrar(arbolCAC); arbolCAC = ArbolVacio;

			std::map<int,std::vector<bool>> diccionarioYAC;
			std::map<int,std::vector<bool>> diccionarioYDC;
			std::map<int,std::vector<bool>> diccionarioCAC;
			std::map<int,std::vector<bool>> diccionarioCDC;
			

			std::vector<bool> aux;//vector auxiliar para la construccion de las tablas;

			//Se reconstruyen los arboles de modo que sea decodificado igual
			arbolYAC = construye_arbol(tabla_YAC);
			arbolYDC = construye_arbol(tabla_YDC);
			arbolCAC = construye_arbol(tabla_CAC);
			arbolCDC = construye_arbol(tabla_CDC);


			construye_diccionario(aux,diccionarioYAC, arbolYAC);
			construye_diccionario(aux,diccionarioYDC, arbolYDC);
			construye_diccionario(aux,diccionarioCAC, arbolCAC);
			construye_diccionario(aux,diccionarioCDC, arbolCDC);
			//muestraHuffman(arbolCDC);
			/*cout<<endl;
			for(std::map<int, vector<bool>>::iterator i=diccionarioCDC.begin();i!=diccionarioCDC.end();i++){
				cout<<std::hex<<i->first<<":\t";
				for(bool j:i->second){
					cout<<j;
				}
				cout<<endl;
			}*/

			

			f.put(0xff); f.put(0xda); 
			f.put(0x00); f.put(0x0c);
			f.put(0x03); f.put(0x01);
			f.put(0x00); f.put(0x02);
			f.put(0x11); f.put(0x03);
			f.put(0x11); f.put(0x00);
			f.put(0x3f); f.put(0x00);

			std::vector<bool> flujobytes;

			/*
				vector<int> cadena_YAC;
				vector<int> coeficientes_YDC;
				vector<int> cadena_YDC;
				vector<int> coeficientesY;
				vector<int> cadena_CAC;
				vector<int> cadena_CDC;
				vector<int> coeficientesC;
				vector<int> coeficientesDCr;
				vector<int> coeficientesDCb;
			*/
			// /cout<<chunks_y<<" "<<chunks_x<<endl;
			/*		cout<<"coeficientes_YDC: "<<coeficientes_YDC.size()<<endl;
					cout<<"cadena_YDC: "<<cadena_YDC.size()<<endl;
					cout<<"cadena_CDC: "<<cadena_CDC.size()<<endl;
					cout<<"coeficientesDCr: "<<coeficientesDCr.size()<<endl;
					cout<<"coeficientesDCb: "<<coeficientesDCb.size()<<endl;
			*/for(int i=0;i<chunks_x;i++){
				for(int j=0;j<chunks_y;j++){
					//Escribir DC Y
					escribe_dc(cadena_YDC,flujobytes,diccionarioYDC, coeficientes_YDC);
					//Escribir AC Y
					escribe_ac(cadena_YAC,flujobytes, diccionarioYAC, coeficientesY);
					//Escribir DC Cb
					escribe_dc(cadena_CDC, flujobytes,diccionarioYDC, coeficientesDCb);
					//Escribir AC Cb
					escribe_ac(cadena_CAC,flujobytes, diccionarioCAC, coeficientesC);
					//Escribir DC Cr
					escribe_dc(cadena_CDC, flujobytes,diccionarioYDC, coeficientesDCr);
					//Escribir AC Cr
					escribe_ac(cadena_CAC,flujobytes, diccionarioCAC, coeficientesC);

					//cout<<i<<" "<<j<<endl;
					while(flujobytes.size()>=8){
						int byte = 0;
						int acum = 128;
						for(int k=0;k<8;k++){
							byte+=(int)flujobytes[k]*acum;
							acum>>=1;
						}
						flujobytes.erase(flujobytes.begin(), flujobytes.begin()+8);
						f.put(byte);
						std::cout<<std::hex<<byte<<" ";
						if(byte==255){
							std::cout<<"00 ";
							f.put(0);
						}
					}
					f.flush();
				}
			}
/*			cout<<"cadena_YAC: "<<cadena_YAC.size()<<endl;
					cout<<"coeficientes_YDC: "<<coeficientes_YDC.size()<<endl;
					cout<<"cadena_YDC: "<<cadena_YDC.size()<<endl;
					cout<<"coeficientesY: "<<coeficientesY.size()<<endl;
					cout<<"cadena_CAC: "<<cadena_CAC.size()<<endl;
					cout<<"cadena_CDC: "<<cadena_CDC.size()<<endl;
					cout<<"coeficientesC: "<<coeficientesC.size()<<endl;
					cout<<"coeficientesDCr: "<<coeficientesDCr.size()<<endl;
					cout<<"coeficientesDCb: "<<coeficientesDCb.size()<<endl;
*/			while(flujobytes.size()%8){
				flujobytes.push_back(0);
			}
			while(flujobytes.size()>=8){
						int byte = 0;
						int acum = 128;
						for(int k=0;k<8;k++){
							byte+=(int)flujobytes[k]*acum;
							acum>>=1;
						}
						flujobytes.erase(flujobytes.begin(), flujobytes.begin()+8);
						f.put(byte);
						std::cout<<std::hex<<byte<<" ";
						if(byte==255){
							std::cout<<"00 ";
							f.put(0);
						}
					}
			f.put(0xff); f.put(0xd9); 
			f.close();
			borrar(arbolYAC); arbolYAC = ArbolVacio;
			borrar(arbolYDC); arbolYDC = ArbolVacio;
			borrar(arbolCDC); arbolCDC = ArbolVacio;
			borrar(arbolCAC); arbolCAC = ArbolVacio;

		}

		void guardaBMP(const char *nombre){
			std::ofstream f(nombre);
			f.put('B'); f.put('M'); //Tipo
			int tam=54+x*y;
			f.write((char*)&tam,sizeof(int)); 
			int reservado=0;
			f.write((char*)&reservado,sizeof(int));  
			int offset=0x36;
			f.write((char*)&offset,sizeof(int));  
			int tamCabeceras=40;
			f.write((char*)&tamCabeceras,sizeof(int));
			f.write((char*)&(x),sizeof(int)); //ancho
			f.write((char*)&(y),sizeof(int)); //alto
			short planos=1;
			f.write((char*)&planos,sizeof(short));
			short bits=24;
			f.write((char*)&bits,sizeof(short));
			int compresion=0;
			f.write((char*)&compresion,sizeof(int));
			f.put(0); f.put(0); f.put(0); f.put(0); //Tamaño Paleta
			f.put(0); f.put(0); f.put(0); f.put(0); //BitsPorMetroX
			f.put(0); f.put(0); f.put(0); f.put(0); //BitsPorMetroY
			f.put(0); f.put(0); f.put(0); f.put(0); //Colores Usados
			f.put(0); f.put(0); f.put(0); f.put(0); //Colores Importantes
			int ajuste=(4-(x*3)%4)%4;
			//std::cout<<f.tellp()<<std::endl;
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++)
					try{f<<en(i,j);}catch(int t){f<<Color();}
				for(int j=0;j<ajuste;j++)
					f<<'\0';
			}
			f.close();
		}
		
		void leeBMP(const char *nombre){
			delete pixels;
			std::ifstream f(nombre);
			if(f.get()!='B' or f.get()!='M'){
				std::cout<<"No es BMP"<<std::endl;
			}
			int tam;
			f.read((char*)&tam,sizeof(int));
			f.get(); f.get(); f.get(); f.get(); //Reservado
			f.get(); f.get(); f.get(); f.get(); //Offset
			f.get(); f.get(); f.get(); f.get(); //Tamaño cabecera
			f.read((char*)&x,sizeof(int)); //x++;
			f.read((char*)&y,sizeof(int)); //y++;
			f.get(); f.get(); //Plastd::nos
			short bits;
			f.read((char*)&bits,sizeof(short));
			f.get(); f.get(); f.get(); f.get(); //Compresion
			f.get(); f.get(); f.get(); f.get(); //Tamaño Paleta
			f.get(); f.get(); f.get(); f.get(); //BitsPorMetroX
			f.get(); f.get(); f.get(); f.get(); //BitsPorMetroY
			f.get(); f.get(); f.get(); f.get(); //Colores Usados
			f.get(); f.get(); f.get(); f.get(); //Colores Importantes
			pixels=new Color[x*y];
			int ajuste=((x*3)%4)%4;
			f.get();
			f.get();
			f.seekg(-(long(x+ajuste)*y*3),f.end);
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++){
					//std::cout<<x-j<<std::endl;
					f>>en(i,j);
				}
				for(int j=0;j<3*ajuste;j++)
					f.get();
			}
			f.close();
		}
		
		Imagen* region(int p, int q, int h, int k, Imagen *r)const{
			delete r->pixels;
			r->x=h;
			r->y=k;
			r->pixels = new Color[h*k];
			
			for(int i=0;i<k;i++)
				for(int j=0;j<h;j++)
					try{
						r->en(i,j) = en(q+i,p+j);
					}catch(int e){
					}
			
			
			return r;
		}

		
		Imagen region(int p, int q, int h, int k)const{
			Imagen r;
			r.x=h;
			r.y=k;
			r.pixels = new Color[h*k];
			
			for(int i=0;i<k;i++)
				for(int j=0;j<h;j++)
					try{
						r.en(i,j) = en(q+i,p+j);
					}catch(int e){
					}
			
			
			return r;
		}



		Color& operator()(int x, int y){
			return this->en(x,y);
		}

		Imagen operator+(const Imagen& b)const{
			if(x!=b.x or y!=b.y){
				Imagen I(std::max(x,b.x),std::max(y,b.y));
				for(int i=0;i<I.filas();i++){
					for(int j=0;j<I.columnas();j++){
						if(i>=filas()){
							I.en(i,j) = b.en(i,j);
						}else if(i>=b.filas()){
							I.en(i,j) = en(i,j);
						}else{
							I.en(i,j) = en(i,j)+b.en(i,j);
						}
					}
				}
				return I;
			}else{
				int tam = x*y;
				Color *res = new Color[tam];
				for(int i=0;i<tam;i++){
					res[i] = pixels[i]+b.pixels[i];
				}
				Imagen I;
				I.x = x;
				I.y = y;
				I.pixels = res;
				return I;
			}
		}

		Imagen operator-(const Imagen& b)const{
			if(x!=b.x or y!=b.y){
				Imagen I(std::max(x,b.x),std::max(y,b.y));
				for(int i=0;i<I.filas();i++){
					for(int j=0;j<I.columnas();j++){
						if(i>=filas()){
							I.en(i,j) = b.en(i,j);
						}else if(i>=b.filas()){
							I.en(i,j) = en(i,j);
						}else{
							I.en(i,j) = en(i,j)+b.en(i,j);
						}
					}
				}
				return I;
			}else{
				int tam = x*y;
				Color *res = new Color[tam];
				for(int i=0;i<tam;i++){
					res[i] = pixels[i]+b.pixels[i];
				}
				Imagen I;
				I.x = x;
				I.y = y;
				I.pixels = res;
				return I;
			}
		}


	friend void drawAt(const Imagen& M, int h, int k);
	friend inline void glReadPixels(Imagen &M);
	friend inline void glReadPixels(Imagen &M ,int x2, int y2);	
	friend inline void glDrawPixels(const Imagen &M);
	friend inline void glDrawPixels(const Imagen *M);

};




#endif