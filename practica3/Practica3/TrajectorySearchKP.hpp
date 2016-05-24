/* 
 * File:   SolGeneratorKP.hpp
 * Author: cristiangj
 *
 * Created on 7 de marzo de 2016, 13:02
 */

#ifndef SOLGENERATORKP_HPP
#define	SOLGENERATORKP_HPP

#include "Solucion.hpp"
#include "InstanceKP.hpp"
#include "TrajectorySearch.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>    // std::sort

struct candidatosGreedy {
	int pos;
	double ratio;
};

bool compararCandidatos(struct candidatosGreedy cand1,struct candidatosGreedy cand2) { 
	return cand1.ratio > cand2.ratio; 
}

class TrajectorySearchKP : public TrajectorySearch {

	private:
		/*CAMBIO 1 añadir una instancia como atributo*/
		InstanceKP _instance;
		int numUnos;
	public:
		
		// Constructores
		
		TrajectorySearchKP(const InstanceKP& orig,int numEval) : TrajectorySearch(numEval) {_instance=orig;}
		/*CAMBIO 2 constructor*/
		TrajectorySearchKP(const InstanceKP orig){_instance=orig;}

        void SetNumUnos(int numUnos) {
			
        	this->numUnos = numUnos;
        }

        int& GetNumUnos() {
			
        	return numUnos;
        }

        void SetInstance(InstanceKP _instance)
        {
        	this->_instance = _instance;
        }

		//observador referencia no constante
		InstanceKP& GetInstance()
        {
        	return _instance;
        }
		
		//observador referencia constante
        const InstanceKP& GetInstance() const
        {
        	return _instance;
        }
		
		Solucion Neighoperator() {
			
			Solucion generic(GetSol());
			int tam=generic.GetValores().size();
			int pos = rand() % tam;
				
			modificar(generic,pos);
			
			return generic;
			
		}
	
		void firstImprovement(int kNeigh){
			bool seguir=true;
			Solucion s;
		

			while(seguir){
				seguir=false;
				for (int i= 0;i<kNeigh;++i){
					s=Neighoperator();
					
					if(s.GetCoste()>GetSol().GetCoste() && s.GetPeso()<=GetInstance().getCapacidad()){
						seguir=true;
						SetSol(s);
						break;
					}
				}
			}
		}
		
		void bestImprovement(int kNeigh) {
		
			bool seguir = true;
			int i;
			Solucion sB, sPrima;

			while (seguir) {
				
				seguir = false;
				sB = GetSol();
				
				/** buscar vecinos desde la posicion */
				for (i=0;i<kNeigh;++i) {
					
					sPrima = Neighoperator();

					if ( (sPrima.GetCoste() > sB.GetCoste()) && (sPrima.GetPeso()<= GetInstance().getCapacidad()) ) {
						
						sB = sPrima;
						
					}
					
				}
				
				if ( sB.GetCoste() > GetSol().GetCoste() ) {
					
					SetSol(sB);
					seguir = true;
				}
				
			}
		
		}
		
		std::vector<double> IteratedGreedy(int num) {
			
			std::vector<int> destruidos;
			std::vector<double> fitness(GetNumEval());
			int iteracion = 0,i,unos = 0,nodos = GetInstance().getNodos();
			Solucion sPrima,auxiliar;
			
			greedy(10);
			firstImprovement(GetInstance().getNodos());
			
			fitness[iteracion] = comprobarSolucion(GetSol());
			
			while(iteracion < GetNumEval()) {
				
				for(i=0;i<nodos;++i)
					if ( GetSol().GetValores()[i] == 1 ) {
						destruidos.push_back(i);
						unos++;
					}
				
				SetNumUnos(unos);
				
				sPrima = Destruir(num,destruidos);
				//reconstruir(sPrima,destruidos);
				
				auxiliar = GetSol();
				SetSol(sPrima);
			
				greedy(10);
				firstImprovement(GetInstance().getNodos());
				
				sPrima = GetSol();
				SetSol(auxiliar);
			
				SetSol(seleccionar(GetSol(),sPrima));
				
				iteracion++;
				
				fitness[iteracion] = GetSol().GetCoste();
				destruidos.clear();
				unos = 0;
			}
			
			return fitness;
			
		}
		
		/*
		* Aplica enfriamiento Simulado a una instancia
		* Devuelve un vector con los costes de las diferentes soluciones generadas en el algoritmo
		*/
		std::vector<double> EnfriamientoSimulado()
		{
			std::vector<double> fitness(GetNumEval()); // Aqui guardo los fitness
			double T = TemperaturaInicial(); // Calculo la Temperatura inicial

			Solucion sActual(GetInstance().getNodos()); // Solucion Actual
			Solucion sBest(GetInstance().getNodos()); // Mejor Solucion

			getValidSolution(sActual, GetInstance().getMateriales()); // Doy valores a la solucion actual

			SetSol(sActual); // La solucion del problema inicialmente es la actual
			sBest = GetSol(); // La mejor solucion es la actual

			unsigned int numIteraciones = 0;
			fitness[numIteraciones] = GetSol().GetCoste(); // El primer valor del fitness es el de la solucion actual

			while(numIteraciones < GetNumEval()) // 1000000 iteraciones por experimento
			{
				// Por cada iteracion, mejor solucion es la solucion actual del problema
				sBest = GetSol();

				for(int i = 0; i < GetInstance().getNodos() ; i++)
				{
					// La solucion actual es la vecina a la que tenia
					sActual = GetSol();
					getValidSolution(sActual, GetInstance().getMateriales());

					// Si es mejor que la que tenia la guardo como mejor
					sBest = seleccionar(sBest, sActual);

					// Compruebo si acepto la solucion actual como la solucion para la siguiente iteracion
					if(Aceptar(sActual, GetSol(), T))
						SetSol(sActual);
				}

				// Enfrio la temperatura
				T = Enfriar(T,numIteraciones);
				
				numIteraciones++;
				fitness[numIteraciones] = GetSol().GetCoste();
			}
			
			SetSol(sBest);

			return fitness;
		}

		
		void getValidSolution(Solucion &generic, std::vector<Material> &materiales) {
			
			// Declaracion, Inicializacion
            int indice = rand()%materiales.size();
			int intentos = 100;
		
			// Modificacion
			modificar(generic,indice);
			
			while( generic.GetPeso() > _instance.getCapacidad() and intentos!=0) {
			
				// Restaurar
				modificar(generic,indice);
				
				indice = rand()%materiales.size();
				
				// Modificar
				modificar(generic,indice);
				
				intentos--;
				
			}
			
			if ( generic.GetPeso() > _instance.getCapacidad() )
				modificar(generic,indice);
			
		}
		
	protected:
		
		inline void sumPeso(Solucion &generic,int &indice) {
			
			generic.SetPeso(generic.GetPeso() + GetInstance().getMateriales()[indice].peso);
			
		}
		
		inline void resPeso(Solucion &generic,int &indice) {
			
			generic.SetPeso(generic.GetPeso() - GetInstance().getMateriales()[indice].peso);
			
		}
		
		inline void sumCoste(Solucion &generic, int &indice) {
			
			generic.SetCoste(generic.GetCoste() + GetInstance().getMateriales()[indice].beneficio);

		}
		
		inline void resCoste(Solucion &generic, int &indice) {
			
			generic.SetCoste(generic.GetCoste() - GetInstance().getMateriales()[indice].beneficio);

		}
		
		double comprobarSolucion(Solucion &generic) {
			
			/** si el peso de la solucion no se pasa o es igual a la capacidad de la mochila, el fitness
			 es la suma de los beneficios de los materiales que han entrado, si se pasa el fitness es
			 negativo al penalizar la solucion invalida **/
			if ( generic.GetPeso() <= GetInstance().getCapacidad() )
				return generic.GetCoste();
			else
				return GetInstance().getCapacidad() - generic.GetPeso();
			
		}
		
		Solucion seleccionar(Solucion &sol1,Solucion &sol2) {
		   
		  /** la solucion guardada en la clase es valida, comprobaremos la obtenida recientemente 
		   para saber cual es mejor **/ 
		   if ( comprobarSolucion(sol1) > comprobarSolucion(sol2) )
			   return sol1;
		   else
			   return sol2;
		   
		}   
		
		void modificar(Solucion &generic,int &indice) {
			
			/** entra un material, sumamos su peso y beneficio **/
			if (generic.GetValores()[indice] == 0) {
				
				generic.GetValores()[indice] = 1;
				sumCoste(generic,indice);
				sumPeso(generic,indice);
				
			/** quitamos un material, restamos su peso y beneficio **/
			} else {
			
				generic.GetValores()[indice] = 0;
				resCoste(generic,indice);
				resPeso(generic,indice);
			}
		}
		
		void greedy(const int &numElementos) {
			
			int pos = 0, i, nNodos=GetInstance().getNodos();
			std::vector<candidatosGreedy> candidatos(numElementos);
			bool mejora = true;
			
			while(mejora) {
				
				mejora = false;
			
				/** obtenemos la lista de candidatos inicial de forma aleatoria, solo nos valen los que 
				 valgan 0 en la solucion actual **/
				for (i=0;i<numElementos;++i) {

					pos = rand() % nNodos;

					while ( GetSol().GetValores()[pos] == 1 )
						pos = rand() % nNodos;

				/** asignar un posible candidato **/
					candidatos[i].ratio = (double) GetInstance().getMateriales()[pos].beneficio/GetInstance().getMateriales()[pos].peso;
					candidatos[i].pos = pos;
				}

				/** ordenamos de mayor a menor ratio **/
				std::sort(candidatos.begin(), candidatos.end(),compararCandidatos);

				/** recorremos los candidatos de mayor a menor hasta que entre uno o ninguno **/
				for (i=0;i<numElementos;++i) {

				/** ponemos a 1 el candidato en la solucion y sumamos peso y beneficio **/
					modificar(GetSol(),candidatos[i].pos);

				/** si se ha pasado de la capacidad de la mochila lo sacamos restando peso y beneficio **/
					if ( GetSol().GetPeso() > GetInstance().getCapacidad() )
						modificar(GetSol(),candidatos[i].pos);
					else {/** si no se pasa quiere decir que la solucion a mejorado **/
						mejora = true;
						break;
					}
				}
			}
			
		}

	   Solucion Destruir(const int &num,std::vector< int > &eliminados) {
		   
		   /** si num=2 destruimos la mitad de los unos existentes **/
		   int maxDestruccion = GetNumUnos()/num;
		   if ( maxDestruccion == 0 )
			   maxDestruccion = 2;
		   int pos=0;
		   Solucion aux=GetSol();
		   
		   /** barajamos el vector de posiciones donde se encuentran los unos, en la mochila al
			no entrar todos solo podemos destruir los que han entrado **/
		   std::random_shuffle(eliminados.begin(),eliminados.end());
		   
		   while(pos<maxDestruccion) {
			   
				resCoste(aux,eliminados[pos]);
				resPeso(aux,eliminados[pos]);
				
				aux.GetValores()[eliminados[pos]] = 0;

				++pos;
        
			}
		   
		   /** el numero de unos aqui no es la cantidad existente en la solucion, sino la 
			cantidad de destruidos, se usara al reconstruir **/
		   SetNumUnos(maxDestruccion);
		   
		   return aux;
	   }
	   
	   void reconstruir(Solucion &parcial,const std::vector<int> &eliminados){
		   
			int i=0, pos, nNodos = GetInstance().getNodos(), nReconstruir = (rand() % GetNumUnos()) + 1;

			/** intentaremos reconstruir tantos 1 como salga aleatorio en nReconstruir, al ser aleatoria 
			 tambien la eleccion si entra o no, puede ser que no se reconstruya el mismo numero **/
			while ( i < nReconstruir ) {
				
			/** buscamos una posicion aleatoria en la solucion que no valga 1 **/
				pos = rand() % nNodos;

				while( parcial.GetValores()[pos] == 1 )
					pos = rand() % nNodos;

			/** puede entrar o no entrar el material de dicha posicion **/
				parcial.GetValores()[pos]= rand() % 2;
				
			/** si ha entrado sumamos su peso y beneficio, al ser aleatorio no comprobamos la
			 validez de la solucion en este punto **/
				if ( parcial.GetValores()[pos] == 1 ) {
					sumCoste(parcial,pos);
					sumPeso(parcial,pos);
				}

				++i;
			}	
		
		}
	   
		double TemperaturaInicial()
		{
		/* Preparo los datos de la funcion*/

		// Declaro una solucion inicial
		Solucion s0(GetInstance().getNodos());
		Solucion s1 = s0;

		// Hago que sea valida (No se salga de la capacidad)
		getValidSolution(s1, GetInstance().getMateriales());
		SetSol(s1);

		// Hago una solucion vecina a la actual
		Solucion s2 = Neighoperator();

		/* Caluclar Incremento E */
		double IE = 0;
		// Tengo que coger 10 soluciones aletarorias y 10 vecinas a esas
		for (int i = 0; i < 10; i++)
		{
			// Calculo IE sucesivamente
			IE += (s1.GetCoste() - s2.GetCoste());

			// Reinicio solucion
			s1 = s0;

			// Vuelvo a calcular soluciones aleatorias.
			getValidSolution(s1, GetInstance().getMateriales());
			SetSol(s1);
			s2 = Neighoperator();
		}

		IE = IE / 10;

		if(IE < 0)
			IE = abs(IE);

		return (-IE)/(log10(0.99));
	}

	bool Aceptar(Solucion &s1, Solucion &s2, double &T)
	{
		double IE = s1.GetCoste() - s2.GetCoste();

		// Si acepta si mejora
		if (IE > 0)
			return true;

		long double PIE = exp(IE/T);

		if(PIE > 0.5)
			return true;
		else
			return false;
	}

};

#endif	/* SOLGENERATORKP_HPP */

