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
double escalon(double x){
	return tanh(x)*0.5+0.5;
}

double escalonp(double x){
	double ex = exp(x);
	double ex2 = 2.0*ex/(1.0+ex*ex);
	return ex2 * ex2;
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
};

int Neurona::cont_id = 0;

class NeuronaEntrada: public Neurona{
public:
	double dato;
	NeuronaEntrada(){
		this->id = Neurona::cont_id ++;
	}
	virtual double salida(){
		return this->dato;
	}
	virtual bool calculado(){
		return true;
	}
	virtual void reiniciar(){

	}
	virtual void propagarError(){

	}
	virtual void recibeGradiente(double g){
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

class NeuronaOculta :public NeuronaEntrada{
public:
	double bias;
	std::vector<Dendrita> entradas;
	double gradiente;
	//double (*escalon)(double);
	double calculo;
	double activacion;
	bool calculado_;
	void agregarEntrada(Neurona* n, double peso=0){
		entradas.push_back(Dendrita(n, peso));
	}
	virtual double salida(){
		// std::cout<<"Calculando salida de neurona en "<<this<<std::endl;
		if(not calculado_){
			calculo = bias;
			for(Dendrita& d: entradas){
				// std::cout<<":vvvv"<<std::endl;
				calculo+=d.peso*d.neurona->salida();
			}
			calculado_ = true;
			return activacion = escalon(calculo);
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
		// std::cout<<this<<":: recibeGradiente "<<g<<std::endl;
		this->gradiente += g;
	}	
	virtual void propagarError(){
		for(unsigned i = 0; i < entradas.size(); i++){
			// std::cout<<this<<" propagarError::"<<entradas[i].peso<<"::"<<entradas.size()<<std::endl;
			entradas[i].neurona->recibeGradiente(this -> gradiente * entradas[i].peso);
		}
	}
	virtual void ajustaPesos(double etha){
		this->gradiente *= escalonp(this->calculo);
		double dw = etha * this -> gradiente * std::abs(this -> activacion);
		bias += dw; 
		std::cout<<"Ajusta pesos NO "<<etha<<","<<this->gradiente<<","<<this->activacion<<","<<dw<<"::"<<std::endl;
		for(Dendrita& d: entradas){
			d.peso += dw;
		}
	}

	virtual void muestraTopologia(){
		std::cout<<"<Oculta id="<<this<<" etradas={ ";
		for(unsigned i=0;i<entradas.size();i++){
			// std::cout<<"("<<entradas[i].neurona<<","<<entradas[i].peso<<") ";
		}
		std::cout<<"} bias="<<this->bias;
		std::cout<<">"<<std::endl;
	}

};


class NeuronaSalida: public NeuronaOculta{
public:
	virtual void ajustaError(double y){
		this -> gradiente = (y - this->activacion);
	}

	virtual void ajustaPesos(double etha){
		double dw = etha * this -> gradiente * std::abs(this -> activacion);
		bias += dw; 
		std::cout<<"Ajusta pesos "<<etha<<","<<this->gradiente<<","<<this->activacion<<","<<dw<<"::"<<std::endl;
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

	virtual double salida(){
		if(not calculado_){
			calculo = bias;
			for(Dendrita& d: entradas){
				// std::cout<<":vvvv"<<std::endl;
				calculo+=d.peso*d.neurona->salida();
			}
			this -> calculado_ = true;
			return activacion = calculo;
		}
		return activacion;
	}
	virtual void propagarError(){
		for(unsigned i = 0; i < entradas.size(); i++){
			// std::cout<<this<<" propagarErrorSalida::"<<entradas[i].peso<<"::"<<entradas.size()<<std::endl;
			entradas[i].neurona->recibeGradiente(this -> gradiente * entradas[i].peso);
		}
	}

};

class RedNeuronal{
public:
	std::vector<NeuronaEntrada*> neuronasEntrada;
	std::vector<NeuronaOculta*> neuronasIntermedias;
	std::vector<NeuronaSalida*> neuronasSalida;

	double etha;
	double aprendizaje;
	double error;
	double errorPrevio;

	RedNeuronal(int entradas, int salidas, int capasIntermedias,const int cantidades[], double e){
		etha = e;
		aprendizaje = e;
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
				for(int k = 0; k<cantidades[i-1]; k++){
					n->agregarEntrada(neuronasIntermedias[inicioCapa + k],urandom());
				}
				neuronasIntermedias.push_back(n);
			}
			inicioCapa += cantidades[i-1];
		}
		for(int i=0;i<salidas;i++){
			NeuronaSalida* n = new NeuronaSalida();
			n->bias = urandom();
			for(unsigned j = inicioCapa; j<neuronasIntermedias.size(); j++){
				n->agregarEntrada(neuronasIntermedias[j], urandom());
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
		errorPrevio = error;
		error = 0;
	}

	double evalua(double input){
		neuronasEntrada[0]->dato = input;
		reiniciarNeuronas();
		return neuronasSalida[0]->salida();
	}

	void entrena(double input, double output){
		double y = evalua(input);
		std::cout<<(output - y)<<std::endl;
		error += 0.5 * (output - y) * (output - y);
		neuronasSalida[0]->ajustaError(output);
		neuronasSalida[0]->ajustaPesos(aprendizaje);
		neuronasSalida[0]->propagarError();
		for(int i = neuronasIntermedias.size() - 1; i >= 0 ; --i){
			neuronasIntermedias[i]->ajustaPesos(aprendizaje);
			neuronasIntermedias[i]->propagarError();
		}
	}
	void ajustaAprendizaje(){
		if(error > errorPrevio){	
			aprendizaje *= etha;
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


