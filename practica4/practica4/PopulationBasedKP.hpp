/*
 * File:   PopulationBasedKP.hpp
 * Author: jesus
 *
 * Created on 27 de abril de 2016, 13:04
 */

#ifndef POPULATIONBASEDKP_HPP
#define	POPULATIONBASEDKP_HPP

#include "PopulationBased.hpp"
#include "InstanceKP.hpp"
#include <vector>
#include <algorithm>
#include "Solucion.hpp"
#include <cmath>

double ALPHA = 0.9999;
double valor = 1.0;
double umbral_Peso = 20000;

using namespace std;

struct candidatosGreedy {
	int pos;
	double ratio;
};

bool compararCandidatos(struct candidatosGreedy cand1,struct candidatosGreedy cand2) { 
	return cand1.ratio > cand2.ratio; 
}

class PopulationBasedKP : public PopulationBased {
	private:
		InstanceKP _instance;
	public:
		PopulationBasedKP(const InstanceKP & instance,const int & numIndividuos) : PopulationBased(numIndividuos){
			_instance=instance;
		}

        void SetInstance(InstanceKP _instance) {
        	this->_instance = _instance;
        }

        const InstanceKP& GetInstance() const {
        	return _instance;
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
		
		void calcPeso(Solucion & generic,const vector<Material> & materiales)
		{
			int tam = GetInstance().getNodos();
			double acumulador = 0.0;
			
			// Calculo de peso total
			for(int i = 0; i < tam; i++)
				if (generic.GetValores()[i] == 1) // Si el material entra en la solucion
					acumulador += materiales[i].peso;
			
			generic.SetPeso(acumulador);			
		}
		
		void calcCoste(Solucion &generic,const std::vector<Material> &materiales) {
			
			int tam = GetInstance().getNodos();
			double coste = 0.0;
			
			for (int i=0;i<tam;++i)
				if ( generic.GetValores()[i] == 1 )
					coste +=  materiales[i].beneficio;
			
			generic.SetCoste(coste);
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
		
		void inicializar()
		{
			Solucion individuo(GetInstance().getNodos()),individuo2(GetInstance().getNodos());
			vector<Solucion> poblacion(GetNumIndividuos());
			int mitad = GetNumIndividuos()/2;
			
			
			for(int i = 0; i < GetNumIndividuos(); i++)
			{
				for(int j = 0; j < GetInstance().getNodos(); j++)
					individuo.GetValores()[j] = rand()%2;
	
				calcPeso(individuo,GetInstance().getMateriales());
				calcCoste(individuo,GetInstance().getMateriales());	
				poblacion[i] = individuo;
			}
			
			SetPoblacion(poblacion);
			GetMejor().SetCoste(0);
		}

		/*
		 * Recorre los individuos de la presente poblacion.
		 * Comprueba si algunos de los individuos mejora a la solucion inicial.
		 * Si es así lo modifica.
		 */
		void evaluar()
		{
			SetMejorPoblacion(GetPoblacion()[0]);
			
			// Recorro los individuos de la poblacion actual
			for (int i = 1 ; i < GetNumIndividuos(); ++i)
				if (GetPoblacion()[i].GetPeso() < _instance.getCapacidad())
					if ( GetPoblacion()[i].GetCoste() > GetMejor().GetCoste() )
						SetMejorPoblacion(GetPoblacion()[i]); // si es mejor sustituyo el mejor del experimento
		
			if (GetMejorPoblacion().GetPeso() < _instance.getCapacidad() and GetMejorPoblacion().GetCoste() > GetMejor().GetCoste())
			{	
				// Si el coste es menor que la que tengo la cambio
				SetMejor(GetMejorPoblacion());
			}
			
		}

		//numContendientes = k
		vector<Solucion> seleccionar(const int & numPadres,const int & numContendientes){

			vector<Solucion> padres(numPadres);
			vector<int> participantes(numContendientes);
			
			int seleccionados=0,pos,contendientes=0,padre=0,maxTam=GetInstance().getCapacidad();
			unsigned int distancia1,distancia2;
			while(seleccionados<numPadres){
				while(contendientes<numContendientes){
					//coger individuo aleatorio
					pos = rand() % GetNumIndividuos();

					//si no esta dentro de la lista de contendientes
					if ( not GetPoblacion()[pos].IsDentro() ){
						GetPoblacion()[pos].SetDentro(true);
											//se guarda como contendiente
						participantes[contendientes] = pos;
						//aumenta el numero de contendientes
						contendientes++;
					}


				}
				int j=0;
				
				while(j<numContendientes and GetPoblacion()[participantes[j]].IsEsPadre()) j++;
				
				if ( j == numContendientes ) {
					contendientes = 0 ;
					continue;
				}
				
				
				padre=j;
				distancia1=maxTam-GetPoblacion()[participantes[padre]].GetPeso();
				
				for(int i=0;i<numContendientes;++i){
					//setear a falso para que entre en la siguiente
					GetPoblacion()[participantes[i]].SetDentro(false);
				if(!GetPoblacion()[participantes[i]].IsEsPadre()){	
					distancia2=maxTam-GetPoblacion()[participantes[i]].GetPeso();
					if(distancia2<distancia1){						
						padre=participantes[i];
						distancia1=distancia2;
					}
				}	
			}
			

				//generar nuevos contendientes
				contendientes = 0 ;
				//guardar mejor padre
				padres[seleccionados] = GetPoblacion()[padre];
				GetPoblacion()[padre].SetEsPadre(true);
				seleccionados++;
			}
			return padres;
		}
		
		vector<Solucion> recombinar(vector< Solucion> & padres,const int &alelos) {
			
			Solucion hijo_aux(GetInstance().getNodos()), aux(GetInstance().getNodos()),padre_aux;
			int padresCruzados = 0, numHijos = 0, nPadres = padres.size(),i,j=1;
			vector<Solucion> hijos(nPadres*nPadres - nPadres);
			
			for (i=0;i<GetInstance().getNodos();++i) {
				
				if ( (rand()%2) == 1 )
					hijo_aux.GetValores()[i] = padres[0].GetValores()[i];
				else
					hijo_aux.GetValores()[i] = padres[j].GetValores()[i];
				
				if ( hijo_aux.GetValores()[i] == 1 ) {
					sumCoste(hijo_aux,i);
					sumPeso(hijo_aux,i);
				}	
					
				if ( hijo_aux.GetPeso() > GetInstance().getCapacidad() ) {
					hijo_aux.GetValores()[i] = 0;
					resCoste(hijo_aux,i);
					resPeso(hijo_aux,i);
				}
				if ( i == GetInstance().getNodos()-1 ) {
					
					i=0;
					//calcCoste(hijo_aux,GetInstance().getMateriales());
					//calcPeso(hijo_aux,GetInstance().getMateriales());
					hijos[numHijos] = hijo_aux;
					numHijos++;
					hijo_aux = aux;
					
					if ( j == nPadres-1 ) {
					
						j=1;
						padresCruzados++;	//se incrementa antes para evitar que se intercambie la primera vez padre[0] con padre[0]
						
						/** si ya han sido todos cruzados con todos salimos **/
						if ( padresCruzados == nPadres )
							break;
						
						padre_aux = padres[0];
						padres[0] = padres[padresCruzados];
						padres[padresCruzados] = padre_aux;
						/** al haber modificado el padre que se va a cruzar con todos tambien se modifica el hijo que va a tener sus
						 mismos primeros cromosomas **/
						padres[0].SetEsPadre(false);
				
					} else
						j++;	//pasar al siguiente padre[j] a cruzar con padre[0]
					
				}
				
			}
			
			return hijos;
			
		}
		
		/** alelos = numero de cortes , operador de cruce para representacion binaria **/
		vector<Solucion> recombinarV2(vector< Solucion> & padres,const int &alelos){
			
			/** tamDivisiones es el tamaño que tiene cada parte cortada en el vector, ej: un vector
			 de 4 elementos con un solo corte cada tamDivision es de 2**/
			int tamDivisiones = ceil(GetInstance().getNodos()/(alelos+1)),nPadres = padres.size();
			/** cruzados es el numero de cromosomas provenientes del otro padre[j] con el que se cruza **/
			int j=1,i,cruzados = 0,padresCruzados=0, numHijos = 0;
			vector<Solucion> hijos(nPadres*nPadres - nPadres);
			Solucion aux, hijo;
			
			padres[0].SetEsPadre(false);
			hijo = padres[0];
			
			/** empezamos en tamDivisiones porque los primeros cromosomas dados por este tamaño son si o si heredados
			 desde el padre[0]*/
			for (i=tamDivisiones;i<GetInstance().getNodos();++i) {
				
				if (cruzados != tamDivisiones) {
					
					hijo.GetValores()[i] = padres[j].GetValores()[i];
					cruzados++;
				}else {
					
					cruzados = 0;
					/** si hay mas de dos divisiones en el vector se deja entre cada division que es cruzada con el 
					 padre[j] una division sin cruzar con los cromosomas del padre[0] **/
					i = i + tamDivisiones;
				
				}
				
				/** al llegar aqui y cumplirse la condicion el padre[0] y el padre[j] han creado un nuevo hijo completo **/
				if ( i >= GetInstance().getNodos()-1 ) {
					
					i=tamDivisiones;
					/** guardamos el hijo **/
					calcCoste(hijo,GetInstance().getMateriales());
					calcPeso(hijo,GetInstance().getMateriales());
					hijos[numHijos] = hijo;
					numHijos++;
					cruzados=0;
					
					/** si se cumple la condicion, quiere decir que el padre[0] ya se ha cruzado con todos los padre[j] y padre[0] 
					 * es el siguiente dado por el indice padresCruzados para cruzar este nuevo padre[0] con todos los demas padre[j] 
					 * inclusive el que estaba antes en padre[0]**/
					if ( j == nPadres-1 ) {
					
						j=1;
						padresCruzados++;	//se incrementa antes para evitar que se intercambie la primera vez padre[0] con padre[0]
						
						/** si ya han sido todos cruzados con todos salimos **/
						if ( padresCruzados == nPadres )
							break;
						
						aux = padres[0];
						padres[0] = padres[padresCruzados];
						padres[padresCruzados] = aux;
						/** al haber modificado el padre que se va a cruzar con todos tambien se modifica el hijo que va a tener sus
						 mismos primeros cromosomas **/
						padres[0].SetEsPadre(false);
						hijo = padres[0];
				
					} else
						j++;	//pasar al siguiente padre[j] a cruzar con padre[0]
					
				}
				
			
			}
			
			return hijos;
			
		}

		void mutacion(vector<Solucion> &hijos){
			
			int pos,j;
			double umbral;
			
			if ( ALPHA < 0.2 )
				ALPHA = 0.9999;
				
			valor *= ALPHA; 
			umbral = 1 - valor;
			
			for(int i=0;i<hijos.size();++i){
				
		//		if( drand48() > umbral ){
					pos = rand() % GetNumIndividuos();
					modificar(hijos[i],pos);
		//		}
			}
		}
		
		void nuevaPoblacion(vector<Solucion> & hijos) {
			
			int tamHijos = hijos.size();
			int tamPoblacion = GetNumIndividuos();
			int pos,i=0,j=0;
			int validos = tamPoblacion/4;
			vector<bool> numeros(tamPoblacion, true);
			vector<Solucion> poblacionAux(tamPoblacion);
			
			// Voy metiendo nuevos individuos
//			while (i < validos)
//			{
//				
//				pos = rand()%tamPoblacion;
//				
//				if ( (GetPoblacion()[pos].GetPeso() < GetInstance().getCapacidad()) and numeros[pos] ) {
//					poblacionAux[i] = GetPoblacion()[pos];
//					numeros[pos] = false;
//					i++;
//				}
//				
//			}
			
			while ( j < tamHijos and i < tamPoblacion ) {
				
				poblacionAux[i] = hijos[j];
				j++;
				i++;
			}
			
			if ( tamPoblacion > (i) ) {
				
				while ( i < tamPoblacion ) {
				
					pos = rand()%tamPoblacion;
					
					if ( numeros[pos] ) {
						poblacionAux[i] = GetPoblacion()[pos];
						numeros[pos] = false;
						i++;
					} 
				}
			} 
			
			//introducir el mejor individuo de la poblacion anterior
			pos = rand() % tamPoblacion;
			poblacionAux[pos] = GetMejorPoblacion();
			
			// Creo la nueva poblacion
			SetPoblacion(poblacionAux);
		}

		void greedy(Solucion &generic,const int &numElementos) {
			
			int pos = 0, i, nNodos=GetInstance().getNodos();
			std::vector<candidatosGreedy> candidatos(numElementos);
			bool mejora = true;
			
			while(mejora) {
				
				mejora = false;
			
				/** obtenemos la lista de candidatos inicial de forma aleatoria, solo nos valen los que 
				 valgan 0 en la solucion actual **/
				for (i=0;i<numElementos;++i) {

					pos = rand() % nNodos;

					while ( generic.GetValores()[pos] == 1 )
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
					modificar(generic,candidatos[i].pos);

				/** si se ha pasado de la capacidad de la mochila lo sacamos restando peso y beneficio **/
					if ( generic.GetPeso() > GetInstance().getCapacidad() )
						modificar(generic,candidatos[i].pos);
					else {/** si no se pasa quiere decir que la solucion a mejorado **/
						mejora = true;
						break;
					}
				}
			}
			
		}
};


#endif	/* POPULATIONBASEDKP_HPP */
