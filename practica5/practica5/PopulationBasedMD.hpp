/*
 * File:   PopulationBasedTSP.hpp
 * Author: jesus
 *
 * Created on 27 de abril de 2016, 13:04
 */

#ifndef POPULATIONBASEDTSP_HPP
#define	POPULATIONBASEDTSP_HPP

#include "PopulationBased.hpp"
#include "InstanceTsp.hpp"
#include "Solucion.hpp"
#include <vector>
#include <algorithm>
#include <limits>	//Poner infinito al principio en mejor solucion, evitar coste 0 en TSP

double ALPHA = 0.99999;
double valor = 1.0;

using namespace std;

class PopulationBasedTSP : public PopulationBased {
	private:
		InstanceMD _instance;
	public:
		PopulationBasedTSP(const InstanceMD & instance,const int & numIndividuos) : PopulationBased(numIndividuos){
			_instance=instance;
			//GetMejor().SetCoste(std::numeric_limits<double>::infinity());
		}

        void SetInstance(InstanceMD _instance) {
        	this->_instance = _instance;
        }

        InstanceMD GetInstance() const {
        	return _instance;
        }

		void calCost(Solucion &generic){

			double coste = 0.0;

			for(int i = 0;i < generic.GetValores().size()-1; i++){
				coste += euclidean(generic.GetValores()[i],generic.GetValores()[i+1]);
			}

			generic.SetCoste(coste);

		}

	protected:

		double euclidean(const int & p1,const int & p2){
			return _instance.getMatDist()[p1][p2];
		}

		void inicializar(){

			Solucion s(GetInstance().getNodos());
			Solucion sAux = s;
			int i;

			//Inicializar los genes de un individuo
			for(i = 0; i < GetInstance().getNodos();i++)
			{
				sAux.GetValores()[i] = i;
			}

			for(i = 0; i < GetNumIndividuos(); i++)
			{

				s = sAux;

				// Permutamos los genes del individuo
				std::random_shuffle(s.GetValores().begin(),s.GetValores().end());

				// Calculamos l fenotipo del individuo
				calCost(s);

				// Insertamos el individuo dentro de la población
				GetPoblacion()[i] = s;
			}

			GetMejor().SetCoste(std::numeric_limits<double>::infinity());
			
		}

		/*
		 * Recorre los individuos de la presente poblacion.
		 * Comprueba si algunos de los individuos mejora a la solucion inicial.
		 * Si es así lo modifica.
		 */
		void evaluar(){
			
			SetMejorPoblacion(GetPoblacion()[0]);
			// Recorro los individuos de la poblacion actual
			for (int i = 1 ; i < GetNumIndividuos(); ++i)
				if (GetPoblacion()[i].GetCoste() < GetMejorPoblacion().GetCoste())
					SetMejorPoblacion(GetPoblacion()[i]);
		
			// tengo que comprobar si alguno mejora la mejor solucion global
			if (GetMejorPoblacion().GetCoste() < GetMejor().GetCoste())
			{	
				// Si el coste es menor que la que tengo la cambio
				SetMejor(GetMejorPoblacion());
			}
		}

		//numContendientes = k
		vector<Solucion> seleccionar(const int & numPadres,const int & numContendientes){

			//vector de posiciones que corresponden a posiciones del vector de poblacion
			vector<int> contendientes(numContendientes);
			vector<Solucion> padres(numPadres);
			int padresAct = 0, pos,i, j, contendientesAct = 0, mejorPadre = 0;

			//mientras no se generen el numero total de padres
			while (padresAct < numPadres) {

				//crear torneo
				while (contendientesAct < numContendientes) {

					//coger individuo aleatorio
					pos = rand() % GetNumIndividuos();

					//si no esta dentro de la lista de contendientes
					if ( not GetPoblacion()[pos].IsDentro() ) {
						
						GetPoblacion()[pos].SetDentro(true);
					
						//se guarda como contendiente
						contendientes[contendientesAct] = pos;
						//aumenta el numero de contendientes
						contendientesAct++;
					}
				}

				j=0;
				
				while(j<numContendientes and GetPoblacion()[contendientes[j]].IsEsPadre()) j++;
				
				if ( j == numContendientes ) {
					contendientesAct = 0 ;
					continue;
				}
				
				mejorPadre = contendientes[j];
				
				//jugar el torneo, buscar el de menor coste
				for(i=0;i<numContendientes;++i) {

					//deja de estar dentro para que pueda entrar en la siguiente iteracion
					GetPoblacion()[contendientes[i]].SetDentro(false);

					if ( not GetPoblacion()[contendientes[i]].IsEsPadre() )
						if (GetPoblacion()[contendientes[i]].GetCoste() < GetPoblacion()[mejorPadre].GetCoste() )
							mejorPadre = contendientes[i];

				}
				
				//generar nuevos contendientes
				contendientesAct = 0 ;

				//guardar mejor padre
				GetPoblacion()[mejorPadre].SetEsPadre(true);
				padres[padresAct] = GetPoblacion()[mejorPadre];
				padresAct++;
			}

			return padres;

		}
		vector<Solucion> recombinar(vector< Solucion> & padres,const int &alelos){
			int numPadres=padres.size(),dominante=1,asignados=0,pos;
			Solucion aux;
			vector<Solucion> hijos((numPadres*numPadres)-numPadres);
			vector<int> mascara(GetInstance().getNodos(),0);

			while(asignados<alelos){//generacion de la mascara
				pos=rand()%GetInstance().getNodos();
				if(mascara[pos]==0){
					mascara[pos]=1;
					asignados++;
				}
			}

			for(int i=0;i<((numPadres*numPadres)-numPadres);){//numero y posicion de los hijos
				for(int j=1;j<padres.size();j++){
					hijos[i]=mezcla(padres[0],padres[j],mascara,alelos);//Guardar todos los hijos. mezclar genera un hijo de dos padres, siendo el primero que se le pasa el dominante.
					calCost(hijos[i]);
					i++;
				}
				if(dominante<padres.size()){//mover padre siguiente a padre dominante(el padre del que se generaran los hijos siguientes).
					aux=padres[0];
					padres[0]=padres[dominante];
					padres[dominante]=aux;
					dominante++;
				}
			}
			return hijos;
		}

		Solucion mezcla(Solucion & s1,Solucion &s2,const vector<int> & mascara,const int & alelos){
			s1.SetEsPadre(false);
			s2.SetEsPadre(false);
			Solucion aux=s1;
			vector<int> aux2(alelos),restantes(s1.GetValores().size()-alelos),aux3=s2.GetValores();
			bool esta=false;
			int posicion=0,n=0,k=0,num=0;
			
			for(int i=0;i<mascara.size();++i)//Aqui se guardan los valores a conservar del padre
				if(mascara[i]==1){
					aux2[n]=s1.GetValores()[i];
					n++;
				}
				
			for(int i=0;i<aux2.size();i++){//Aqui se marcan esos valores(los obtenidos arriba) con -1 en un vector auxiliar
				for(int j=0;j<s2.GetValores().size();j++){
					if(aux2[i]==s2.GetValores()[j]){
						aux3[j]=-1;
					}
				}
			}
			n=0;
			for(int i=0;i<mascara.size();++i)//Guardamos los valores no marcados con -1 en el vector restantes
				if(aux3[i]!=-1){
					restantes[n]=s2.GetValores()[i];
					n++;
				}
			for(int i=0;i<aux.GetValores().size();i++){//introducimos en la copia del padre los valores que no se encuentran en la mascara
				if(mascara[i]==0){
					aux.GetValores()[i]=restantes[k];
					k++;
				}
			}//magia chicas, magia
			return aux;	
		}		
		
		void mutacion(vector<Solucion> &hijos){
			int pos1,pos2,aux;
			double umbral;
			
			if ( ALPHA < 0.2 )
				ALPHA = 0.99;
				
			valor *= ALPHA; 
			umbral = 1 - valor;
			
			for(int i=0;i<hijos.size();++i){
				
				if( drand48() > umbral ){
					pos1=rand()%(GetInstance().getNodos()-2) + 1;
					pos2=rand()%(GetInstance().getNodos()-2) + 1;
					while(pos2==pos1)pos2=rand()%(GetInstance().getNodos()-2) + 1;
				
					
					/** restar aristas **/
					hijos[i].SetCoste(hijos[i].GetCoste() - 
					(GetInstance().getMatDist()[hijos[i].GetValores()[pos1]][hijos[i].GetValores()[pos1-1]] + 
					GetInstance().getMatDist()[hijos[i].GetValores()[pos1]][hijos[i].GetValores()[pos1+1]] +
                    GetInstance().getMatDist()[hijos[i].GetValores()[pos2]][hijos[i].GetValores()[pos2-1]]+
					GetInstance().getMatDist()[hijos[i].GetValores()[pos2]][hijos[i].GetValores()[pos2+1]]));
					
					/** swap **/
					aux=hijos[i].GetValores()[pos1];
					hijos[i].GetValores()[pos1]=hijos[i].GetValores()[pos2];
					hijos[i].GetValores()[pos2]=aux;
				
					/** sumar nuevas aristas **/
					hijos[i].SetCoste(hijos[i].GetCoste() + 
					(GetInstance().getMatDist()[hijos[i].GetValores()[pos1]][hijos[i].GetValores()[pos1-1]] +
					GetInstance().getMatDist()[hijos[i].GetValores()[pos1]][hijos[i].GetValores()[pos1+1]] +
                    GetInstance().getMatDist()[hijos[i].GetValores()[pos2]][hijos[i].GetValores()[pos2-1]] +
					GetInstance().getMatDist()[hijos[i].GetValores()[pos2]][hijos[i].GetValores()[pos2+1]]));
				}
			}

		}
		
		void nuevaPoblacion(vector<Solucion> & hijos)
		{
			int tamVector = hijos.size();
			int tamPoblacion = GetNumIndividuos();
			
			int individuo = rand()%tamPoblacion;
			vector<bool> numeros(tamPoblacion, true);
			
			// Calculo el numero de elementos que tengo que mtamVectoreter
			int i = tamPoblacion - tamVector;
			
			// Redimensiono el tamaño del vector para que pueda meter más individuos
			hijos.resize(tamPoblacion);
			
			// Voy metiendo nuevos individuos
			while (i > 0)
			{
				// Compruebo que el individuo que vaya a meter no sea repetido
				while (numeros[individuo] == false)
					individuo = rand()%tamPoblacion;
				
				// Cuando lo tenga lo meto
				hijos[tamVector] = GetPoblacion()[individuo];
				tamVector++;
				
				// Elimino el individuo de los posibles
				numeros[individuo] = false;
				
				// Cambio individuo siguiente y resto iteración
				individuo = rand()%GetNumIndividuos();
				i--;
			}
			int pos=rand()%GetNumIndividuos();
			hijos[pos]=GetMejorPoblacion();
			// Creo la nueva poblacion
			SetPoblacion(hijos);
		}

};


#endif	/* POPULATIONBASEDTSP_HPP */
