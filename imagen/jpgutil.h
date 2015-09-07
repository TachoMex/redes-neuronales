#ifndef JPGUTIL_H
#define JPGUTIL_H 
#include "color.h"
#include "huffman.h"

class YcbcrColor{
	public:
		float y;
		float cb;
		float cr;
	YcbcrColor(){
		y=0;
		cb=0;
		cr=0;
	}
	YcbcrColor(const Color& u){
		float r,g,b;
		r=u.r;
		g=u.g;
		b=u.b;
		y = 0.299*r+0.587*g+0.114*b-128;
		cb = -0.1687*r-0.3313*g+0.5*b;
		cr = 0.5*r-0.4187*g-0.0813*b;
	}
};

class Matrix8x8{
private:
	float *datos;
public:
	Matrix8x8(){
		datos = new float[64];
		memset(datos,0,sizeof(float)*64);
	}

	const float* operator[](int i) const{
		return datos+8*i;
	}

	float* operator[](int i){
		return datos+8*i;
	}

	Matrix8x8(const float matriz[8][8]){
		datos = new float[64];
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				datos[i*8+j]=matriz[i][j];
			}
		}
	}

	Matrix8x8 operator*(const Matrix8x8& b)const{
		Matrix8x8 c;
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				c[i][j]=0.0;
				for(int k=0;k<8;k++){
					c[i][j]+=datos[i*8+k]*b[k][j];
				}
			}
		}
		return c;
	}


	Matrix8x8 traspuesta()const{
		Matrix8x8 res;
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				res[i][j]=datos[i+j*8];
			}
		}
		return res;
	}

	~Matrix8x8(){
		delete[] datos;
	}

	Matrix8x8(const Matrix8x8& b){
		datos=new float[64];
		for(int i=0;i<64;i++){
			datos[i]=b.datos[i];
		}
	}

	Matrix8x8 operator=(const Matrix8x8& b){
		delete[] datos;
		datos=new float[64];
		for(int i=0;i<64;i++){
			datos[i]=b.datos[i];
		}
		return b;
	}

	int* zigzag() const{
		int * res = new int[64];
		
		int pos[]={0, 1, 5, 6,14,15,27,28,
		  2, 4, 7,13,16,26,29,42,
		  3, 8,12,17,25,30,41,43,
		  9,11,18,24,31,40,44,53,
		 10,19,23,32,39,45,52,54,
		 20,22,33,38,46,51,55,60,
		 21,34,37,47,50,56,59,61,
		 35,36,48,49,57,58,62,63};
		for(int i=0;i<64;i++){
			res[pos[i]]=(int)floor(0.5+datos[i]);
		}

		return res;
	}

	static Matrix8x8 zigzag(int* entrada){
		Matrix8x8 res;
		
		int pos[]={0, 1, 5, 6,14,15,27,28,
		  2, 4, 7,13,16,26,29,42,
		  3, 8,12,17,25,30,41,43,
		  9,11,18,24,31,40,44,53,
		 10,19,23,32,39,45,52,54,
		 20,22,33,38,46,51,55,60,
		 21,34,37,47,50,56,59,61,
		 35,36,48,49,57,58,62,63};
		for(int i=0;i<64;i++){
			res.datos[i]=entrada[pos[i]];
		}
		return res;	
	}

	const static Matrix8x8 Losheller; 
	const static Matrix8x8 HQMatrix; 
	const static Matrix8x8 DCTMatrix; 
	friend std::ostream& operator<<(std::ostream&,const Matrix8x8&);
};
const float LoshellerFloat[8][8] = {
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99}
};
const Matrix8x8 Matrix8x8::Losheller(LoshellerFloat);

/*const float HQM_FLOAT[8][8]={
{3,2,2,3,4,6,8,10 },
{2,2,2,3,4,9,10,9 },
{2,2,3,4,6,9,11,9 },
{2,3,4,5,8,14,13,10 },
{3,4,6,9,11,17,16,12 },
{4,6,9,10,13,17,18,15 },
{8,10,12,14,16,19,19,16 },
{12,15,15,16,18,16,16,16 },
};*/
const float HQM_FLOAT[8][8]={
	{2,   2,   2,   2,   3,   4,   5,   6}, 
    {2,   2,   2,   2,   3,   4,   5,   6}, 
    {2,   2,   2,   2,   4,   5,   7,   9}, 
    {2,   2,   2,   4,   5,   7,   9,  12}, 
    {3,   3,   4,   5,   8,  10,  12,  12}, 
    {4,   4,   5,   7,  10,  12,  12,  12}, 
    {5,   5,   7,   9,  12,  12,  12,  12}, 
    {6,   6,   9,  12,  12,  12,  12,  12}, 
};
const Matrix8x8 Matrix8x8::HQMatrix(HQM_FLOAT);

std::ostream& operator<<(std::ostream& s, const Matrix8x8& m){
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			s<<m[i][j]<<'\t';
		}
		s<<std::endl;
	}
	return s;
}


Matrix8x8 DCTM(){
	Matrix8x8 res;
	for(int i=0;i<8;i++){
		res[0][i]=sqrt(2.0)/4.0;
	}
	for(int i=1;i<8;i++){
		for(int j=0;j<8;j++){
			res[i][j]=cos(i*(2.0*j+1.0)/16.0*M_PI)/2.0;
		}
	}
	return res;
}

const Matrix8x8 Matrix8x8::DCTMatrix = DCTM();
Matrix8x8 cuantificar(const Matrix8x8& m, const Matrix8x8& q){
	Matrix8x8 res;
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			res[i][j]=floor(0.5+m[i][j]/q[i][j]);
		}
	}
	return res;
}
#endif