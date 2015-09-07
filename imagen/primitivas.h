#ifndef PRIMITIVASI_H
#define PRIMITIVASI_H

#include <algorithm>
#include <cmath>
#include "imagen.h"
#include <stack>

namespace primitivas{
	void linea(Imagen &M, int y1,int  x1,int y2, int x2,const Color& c){
		//std::cout<<y1<<" "<<x1<<" "<<y2<<" "<<x2<<std::endl;
		int dx=abs(x2-x1);
		int dy=abs(y2-y1);
		if(dy==0){
			if(x2<x1)
			std::swap(x1,x2);
			for(;x1<=x2;x1++)
			try{M(x1,y1)=c;}catch(int n){}
		}else if(dx==0){
			if(y1>y2)
			std::swap(y1,y2);
			for(;y1<=y2;y1++)
			try{M(x1,y1)=c;}catch(int n){}
			
		}else if(dx==dy){
			if(x1>x2){
				std::swap(x1,x2);
				std::swap(y1, y2);
			}
			int incremento=(y1<y2?1:-1);
			for(;x1<=x2;x1++,y1+=incremento){
				try{M(x1,y1)=c;}catch(int n){}
			}
		}
		else if(dy<dx){
			if(x1>x2){
				std::swap(x1,x2);
				std::swap(y1, y2);
			}
			int x=x1, y=y1;
			int incremento=(y1<y2?1:-1);
			int p=0;
			p=2*dy-dx;
			while(x!=x2 or y!=y2){
				try{M(x,y)=c;}catch(int n){}
				if(p>=0){
					x++;
					y+=incremento;
					p=p+2*dy-2*dx;
				}else{
					x++;
					p=p+2*dy;
				}
			}
		}else{	
			if(y1>y2){
				std::swap(x1,x2);
				std::swap(y1, y2);
			}
			int y=y1, x=x1;
			int incremento=(x1<x2?1:-1);
			int p=0;
			p=2*dx-dy;
			while(y!=y2 or x!=x2){
				try{M(x,y)=c;}catch(int n){}
				if(p>0){
					y++;
					x+=incremento;
					p=p+2*dx-2*dy;
				}else{
					y++;
					p=p+2*dx;
				}
			}
		}
		try{M(x2,y2)=c;}catch(int n){}
	}


	void elipse(Imagen &M, int h, int k, double a, double b, const Color& c){
		int x=0;
		int y=b;
		int p;
		p=int(b*b-a*a*b+0.25*a*a);
		try{
			M.en(h+x,k+y)=c;
		}catch(...){}
		try{
			M.en(h-x,k+y)=c;
		}catch(...){}
		try{
			M.en(h+x,k-y)=c;
		}catch(...){}
		try{
			M.en(h-x,k-y)=c;
		}catch(...){}
		
		while(x*b*b < y*a*a){
			if(p<0){
				p=p+2*x*b*b+b*b;
			}else{
				y--;
				p=p+2*x*b*b+b*b-2*y*a*a;
			}
			x++;
			try{
				M.en(h+x,k+y)=c;
			}catch(...){}
			try{
				M.en(h-x,k+y)=c;
			}catch(...){}
			try{
				M.en(h+x,k-y)=c;
			}catch(...){}
			try{
				M.en(h-x,k-y)=c;
			}catch(...){}
		}
		
		p=int(b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b);
		
		while(y>0){
			if(p>0){
				y--;
				p=p-2*a*a*y+a*a;
			}else{
				x++;
				y--;
				p=p+2*b*b*x-2*a*a*y+a*a;
			}
			try{
				M.en(h+x,k+y)=c;
			}catch(...){}
			try{
				M.en(h-x,k+y)=c;
			}catch(...){}
			try{
				M.en(h+x,k-y)=c;
			}catch(...){}
			try{
				M.en(h-x,k-y)=c;
			}catch(...){}
		}
	}

	void poligono(Imagen &M, int x, int y, int r, double ang, int l, const Color& c){
		int h, k;
		int p, q;
		h=(int)floor(r*cos(ang)); 
		k=(int)floor(r*sin(ang)); 
		for(int i=1;i<=l;i++){
			p=(int)floor(r*cos(i*(2.0*M_PI/l)+ang));
			q=(int)floor(r*sin(i*(2.0*M_PI/l)+ang));
			linea(M,h+x, k+y, p+x, q+y,c);
			h=p;
			k=q;
		}
	}

	void puntosCirc(Imagen &M,int h,int k, int x, int y, const Color&c){
		try{
			M.en(h+x, k+y)=c;
		}catch(...){

		}
		try{
			M.en(h-x, k+y)=c;
		}catch(...){

		}
		try{
			M.en(h+x, k-y)=c;
		}catch(...){

		}
		try{
			M.en(h-x, k-y)=c;
		}catch(...){

		}
		try{
			M.en(h+y, k+x)=c;
		}catch(...){

		}
		try{
			M.en(h-y, k+x)=c;
		}catch(...){

		}
		try{
			M.en(h+y, k-x)=c;
		}catch(...){

		}
		try{
			M.en(h-y, k-x)=c;
		}catch(...){

		}
	}

	void circulo(Imagen &M, int h, int k, int r, const Color& c){
		int x, y;
		x=0;
		y=r;
		double p;
		p = 1-r;

		puntosCirc(M,h, k, x, y,c);

		while(x<y){
			x++;
			if(p<0){
				p+=2*x+1;
			}else{
				y--;
				p+=2*(x-y)+1;
			}
			puntosCirc(M,h, k, x, y,c);
		}
	}

	void estrella(Imagen& M, int x, int y, int r, double ang, int l, const Color& c){
		//int h, k;
		int p, q;
		int s, t;
		s=(int)floor(double(r)*cos(M_PI/l+ang)/2);
		t=(int)floor(double(r)*sin(M_PI/l+ang)/2);
		for(int i=1;i<=l;i++){
			p=(int)floor(r*cos(i*(2.0*M_PI/l)+ang));
			q=(int)floor(r*sin(i*(2.0*M_PI/l)+ang));
			linea(M,s+x, t+y, p+x, q+y,c);
			s=(int)floor(r*cos((2.0*i+1.0)*M_PI/l+ang)/2);
			t=(int)floor(r*sin((2.0*i+1.0)*M_PI/l+ang)/2);
			linea(M,s+x, t+y, p+x, q+y,c);
			//h=p;
			//k=q;
		}
	}
	
	void corazon(Imagen& M,int x, int y, int a, int b, const Color& c){
		int h, k;
		int p, q;
		int IT=100;
		double t=0;
		h=round(a*pow(sin(t),3));
		k=round(((13*cos(t)-5*cos(2*t)-2*cos(3*t)-cos(4*t)+2.5)/29+0.5)*b);
		for(int i=0;i<=IT;i++){
			p=round(a*pow(sin(t),3));
			q=round(((13*cos(t)-5*cos(2*t)-2*cos(3*t)-cos(4*t)+2.5)/29+0.5)*b);
			linea(M,x+h, y+k, x+p, y+q,c);
			h=p;
			k=q;
			t+=2*M_PI/IT;
		}
		
	}
	
	void curvaSpline(Imagen& M,double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, const Color& c){
		int p, q;
		p=round(x1);
		q=round(y1);
		for(double t=0;t<=1;t+=1.0/45){
			int x=round(pow(1-t, 3)*x1+3*t*pow(1-t, 2)*x3+3*t*t*(1-t)*x4+t*t*t*x2);
			int y=round(pow(1-t, 3)*y1+3*t*pow(1-t, 2)*y3+3*t*t*(1-t)*y4+t*t*t*y2);
			linea(M,p,q, x, y,c);
			p=x;
			q=y;
		}
	}
	
	void ilumina(Imagen& M, int x, int y, const Color& colorUso){
		std::stack<int> cola;
		cola.push(y);
		cola.push(x);
		Color c;
		try{
			 c=M.en(y,x);
		}catch(int e){
			return;
		}
		
		if(c==colorUso)
			return;
		int dx[]={0,-1,1,0};
		int dy[]={-1,0,0,1};
		while(!cola.empty()){
			int h, k;
			h=cola.top();
			cola.pop();
			k=cola.top();
			cola.pop();
			for(int i=0;i<4;i++){
				x=h+dx[i];
				y=k+dy[i];
				try{
					Color pix = M.en(y,x);
					if(pix==c){
						M.en(y,x) = colorUso;
						cola.push(y);
						cola.push(x);
					}
				}
				catch(int e){
				}
			}
		}		
	}

}
#endif
