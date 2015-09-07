#ifndef COLOR_H
#define COLOR_H

#include <fstream>
#include <iostream>
#include <cmath>

class Color{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		
		Color(){
			r=g=b=0;
		}
		
		Color(unsigned char x,unsigned char y,unsigned char z){
			r=x;
			g=y;
			b=z;
		}

		Color(const Color& o){
			r = o.r;
			g = o.g;
			b = o.b;
		}

		Color operator=(const Color& o){
			r = o.r;
			g = o.g;
			b = o.b;
			return o;
		}

		Color operator+(const Color& o)const {
			int rn = r+o.r;
			int gn = g+o.g;
			int bn = b+o.b;
			if(rn > 255)
				rn=255;
			if(gn > 255)
				gn=255;
			if(bn > 255)
				bn=255;
			return Color(rn, gn, bn);
		}

		Color operator-(const Color& o)const {
			int rn = r-o.r;
			int gn = g-o.g;
			int bn = b-o.b;
			if(rn < 0)
				rn=0;
			if(gn < 0)
				gn=0;
			if(bn < 0)
				bn=0;
			return Color(rn, gn, bn);
		}
		
		inline bool operator==(const Color& o)const {
			return r==o.r and g==o.g and b==o.b;
		}

		inline bool operator!=(const Color& o)const {
			return r!=o.r or g!=o.g or b!=o.b;
		}

		Color operator*(double s)const {
			int rn = s*r;
			int gn = s*g;
			int bn = s*b;
			if(rn < 0)
				rn=0;
			if(gn < 0)
				gn=0;
			if(bn < 0)
				bn=0;
			if(rn > 255)
				rn=255;
			if(gn > 255)
				gn=255;
			if(bn > 255)
				bn=255;
			return Color(rn,gn,bn);
		}

		Color aGris()const{
			unsigned char c;
			c=(unsigned char)(0.30*r + 0.59*g + 0.11*b);
			return Color(c,c,c);
		}


		unsigned char luz()const{
			return (unsigned char)(0.30*r + 0.59*g + 0.11*b);
		}
	
		static Color hsl(int ang){
			int r, g, b;
			ang=(ang+360)%360;
			switch(ang/60){
				case 0:
					r=(int)floor(255);
					g=int(floor(255*double(ang%60)/60.0));
					b=0;
					break;
				case 1:
					r=(int)floor((255-int(floor(255*double(ang%60)/60.0))));
					g=(int)floor(255);
					b=0;
					break;
				case 2:
					r=0;
					g=(int)floor(255);
					b=int(floor(255*double(ang%60)/60.0));
					break;
				case 3:
					r=0;
					g=(int)floor((255-int(floor(255*double(ang%60)/60.0))));
					b=(int)floor(255);
					break;
				case 4:
					r=int(floor(255*double(ang%60)/60.0));
					g=0;
					b=(int)floor(255);
					break;
				case 5:
					r=(int)floor(255);
					g=0;
					b=(int)floor((255-int(floor(255*double(ang%60)/60.0))));
			}
			return Color(r, g, b);
		}

		static Color gris(int n){
			if(n<=0)
				return Color::negro;
			if(n>=255)
				return Color::blanco;
			return Color(n, n, n);
		}

		static const Color azul;
		static const Color rojo;
		static const Color blanco;
		static const Color verde;
		static const Color amarillo;
		static const Color magenta;
		static const Color naranja;
		static const Color negro;
		static const Color cyan;
		static const Color cafe;
		static const Color rosa;
};

const Color Color::azul=Color(0,0,255);
const Color Color::rojo=Color(255,0,0);
const Color Color::blanco=Color(255,255,255);
const Color Color::verde=Color(0,255,0);
const Color Color::amarillo=Color(255,255,0);
const Color Color::magenta=Color(255,0,255);
const Color Color::cyan=Color(0,255,255);
const Color Color::naranja=Color(255,128,0);
const Color Color::negro=Color(0,0,0);
const Color Color::cafe=Color(153,76,0);
const Color Color::rosa=Color(255,153,204);

std::istream& operator>>(std::istream& s, Color& c){
	if(s==std::cin){
		int r,g,b;
		std::cin>>r;
		std::cin>>g;
		std::cin>>b;
		c.r=r;
		c.g=g;
		c.b=b;
		
	}else{
		c.b=s.get();
		c.g=s.get();
		c.r=s.get();
	}
	return s;
}



std::ostream& operator<<(std::ostream& s,const Color& c){
	if(s==std::cout){
		std::cout<<"("<<(int)c.r<<","<<(int)c.g<<","<<(int)c.b<<")";
	}else{
		s.put(c.b);
		s.put(c.g);
		s.put(c.r);
	}
	return s;
}

Color max(const Color& a,const Color& b){
	Color c;
	c.r=(a.r>b.r?a.r:b.r);
	c.g=(a.g>b.g?a.r:b.g);
	c.b=(a.r>b.b?a.b:b.b);
	return c;
}

Color min(const Color& a,const Color& b){
	Color c;
	c.r=(a.r<b.r?a.r:b.r);
	c.g=(a.g<b.g?a.r:b.g);
	c.b=(a.r<b.b?a.b:b.b);
	return c;	
}

Color maxDif(const Color& a,const Color& b){
	return max(a,b)-min(a,b);
}

#endif