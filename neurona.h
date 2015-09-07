#include <cmath>
#include <vector>
#include <cstdlib>

const int RANDOM_MAX = 1000000;

double urandom(){
	return (double)(rand()%RANDOM_MAX+1)/RANDOM_MAX;
}

double sigmoidal(double x){
	return 1.0/(1+exp(-x));
}
double sigmoidalp(double x){
	double s =  sigmoidal(x);
	return s*(1-s);
}


class Neurona{
public:
	int id;
	static int cont_id;
	virtual double salida() = 0;
	virtual bool calculado() = 0;
	virtual void reiniciar() = 0;
	virtual void propagarError() = 0;
	virtual void recibeGradiente(double) = 0;
	virtual void muestraTopologia() = 0;
//	virtual double entrena(double, double, double);
};

int Neurona::cont_id = 0;

class NeuronaEntrada: public Neurona{
public:
	double dato;
	NeuronaEntrada(){
		this->id = Neurona::cont_id ++;
	}
	virtual double salida(){
		//std::cout<<":v si entro "<<this->dato<<std::endl;
		return this->dato;
	}
	virtual bool calculado(){
		return true;
	}
	virtual void reiniciar(){

	}
	virtual void propagarError(){

	}
	virtual void recibeGradiente(double x){

	}
	virtual void muestraTopologia(){
		std::cout<<"<Entrada id="<<this<<">"<<std::endl;
	}
};

class Dendrita{
public:
	Neurona* neurona;
	double peso;
	Dendrita(Neurona*& n, double p){
		this->neurona = n;
		this->peso = p;
	}
};

class NeuronaOculta{
public:
	double bias;
	std::vector<Dendrita> entradas;
	//double (*sigmoidal)(double);
	double calculo;
	double activacion;
	bool calculado_;
	double gradiente;
	void agregarEntrada(Neurona* n, double peso=0){
		entradas.push_back(Dendrita(n, peso));
	}
	virtual double salida(){
		//std::cout<<"Calculando salida de neurona en "<<this<<std::endl;
		if(not calculado_){
			calculo = bias;
			for(Dendrita& d: entradas){
				// std::cout<<":vvvv"<<std::endl;
				calculo+=d.peso*d.neurona->salida();
			}
			calculado_ = true;
			return activacion = sigmoidal(calculo);
		}
		return activacion;
	}
	virtual bool calculado(){
		return calculado_;
	}
	virtual void reiniciar(){
		activacion = 0.0;
		calculado_ = false;
		gradiente = 0.0;
	}
	virtual void recibeGradiente(double g){
		this->gradiente += g;
	}	
	virtual void propagarError(){
		for(Dendrita& d: entradas){
			d.neurona->recibeGradiente(gradiente * d.peso);
		}
	}
	virtual void ajustaPesos(double etha){
		this->gradiente  *= sigmoidalp(this->calculo);
		double dw = etha * this -> gradiente * this -> activacion;
		// std::cout<<"Ajuste: "<<dw<<std::endl;
		bias += dw; 
		for(Dendrita& d: entradas){
			d.peso += dw;
		}
	}

	virtual void muestraTopologia(){
		std::cout<<"<Oculta id="<<this<<" etradas={ ";
		for(unsigned i=0;i<entradas.size();i++){
			std::cout<<"("<<entradas[i].neurona<<","<<entradas[i].peso<<") ";
		}
		std::cout<<"} bias="<<this->bias;
		std::cout<<">"<<std::endl;
	}

};


class NeuronaSalida: public NeuronaOculta{
public:
	void ajustaError(double y){
		this -> gradiente = sigmoidalp(this -> calculo) * (activacion - y);
	}

	void ajustaPesos(double etha){
		double dw = etha * this -> gradiente * this -> activacion;
		bias += dw; 
		for(Dendrita& d: entradas){
			d.peso += dw;
		}
	}

	virtual void muestraTopologia(){
		std::cout<<"<Salida id="<<this<<" etradas={ ";
		for(unsigned i=0;i<entradas.size();i++){
			std::cout<<"("<<entradas[i].neurona<<","<<entradas[i].peso<<") ";
		}
		std::cout<<"} bias="<<this->bias;
		std::cout<<">"<<std::endl;
	}

};

class RedNeuronal{
public:
	std::vector<NeuronaEntrada*> neuronasEntrada;
	std::vector<NeuronaOculta*> neuronasIntermedias;
	std::vector<NeuronaSalida*> neuronasSalida;

	double etha;
	double error;

	RedNeuronal(int entradas, int salidas, int capasIntermedias,const int cantidades[], double e){
		etha = e;
		for(int i=0;i<entradas;i++){
			neuronasEntrada.push_back(new NeuronaEntrada());
		}
		for(int i=0;i<cantidades[0];i++){
			NeuronaOculta* n = new NeuronaOculta();
			n->bias = urandom();
			for(int j=0;j<entradas;j++){
				n->agregarEntrada(neuronasEntrada[j],urandom());
			}
			neuronasIntermedias.push_back(n);
		}
		int inicioCapa = 0;
		for(int i=1;i<capasIntermedias;i++){
			for(int j=0;j<cantidades[i];j++){
				NeuronaOculta* n = new NeuronaOculta();
				n->bias = urandom();
				for(int k = inicioCapa; k<cantidades[i-1]; k++){
					n->agregarEntrada((Neurona*)neuronasIntermedias[k],urandom());
				}
				neuronasIntermedias.push_back(n);
			}
			inicioCapa = neuronasIntermedias.size()-1;
		}
		for(int i=0;i<salidas;i++){
			NeuronaSalida* n = new NeuronaSalida();
			n->bias = urandom();
			for(int j = 0; j<cantidades[capasIntermedias-1]; j++){
				n->agregarEntrada((Neurona*)neuronasIntermedias[inicioCapa+j-1], urandom());
			}
			neuronasSalida.push_back(n);
		}
	}

	void reiniciarNeuronas(){
		for(NeuronaSalida*& n: neuronasSalida){
			n->reiniciar();		
		}
		for(NeuronaOculta*& n: neuronasIntermedias){
			n->reiniciar();
		}
	}

	void inicializaError(){
		error = 10000;
	}

	double evalua(double input){
		neuronasEntrada[0]->dato = input;
		reiniciarNeuronas();
		return neuronasSalida[0]->salida();
	}

	void entrena(double input, double output){
		evalua(input);
		// std::cout<<":v"<<std::endl;
		neuronasSalida[0]->ajustaError(output);
		neuronasSalida[0]->ajustaPesos(etha);
		neuronasSalida[0]->propagarError();
		for(int i = neuronasIntermedias.size() - 1; i >= 0 ; --i){
			neuronasIntermedias[i]->ajustaPesos(etha);
			neuronasIntermedias[i]->propagarError();
		}

	}


	double errorMedio(){
		return error;
	}

	std::vector<double> evalua(const std::vector<double>& input){
		/*neuronasEntrada[0]->dato = input;
		reiniciarNeuronas();
		return neuronasSalida[0]->salida();*/
	}

	

	void muestraTopologia(){
		for(NeuronaSalida* n: neuronasSalida){
			n->muestraTopologia();		
		}
		for(NeuronaOculta* n: neuronasIntermedias){
			n->muestraTopologia();
		}
		for(Neurona* n: neuronasEntrada){
			n->muestraTopologia();
		}
	}
};


