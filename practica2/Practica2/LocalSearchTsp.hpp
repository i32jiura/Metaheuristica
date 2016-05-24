/* 
 * File:   SolGenerator.hpp
 * Author: jesus
 *
 * Created on 4 de marzo de 2016, 19:14
 */

#ifndef SOLGENERATOR_HPP
#define	SOLGENERATOR_HPP

#include "Solucion.hpp"
#include "InstanceTsp.hpp"
#include "LocalSearch.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>


class L_S_Tsp : public LocalSearch {
private:
	InstanceTSP _instance;
public:
	
	L_S_Tsp(const InstanceTSP orig){_instance=orig;}
	
	 void SetInstance(InstanceTSP _instance)
	{
		this->_instance = _instance;
	}

	//observador referencia no constante
	InstanceTSP& GetInstance()
	{
		return _instance;
	}

	//observador referencia constante
	const InstanceTSP& GetInstance() const
	{
		return _instance;
	}

	Solucion Neighoperator(const int & pos=0) { 
			
		Solucion generic(GetSol());
		int tam=generic.GetValores().size();
		int pos1=(rand()%(tam-1))+1;
		int pos2=(rand()%(tam-1))+1;
		
			generic.SetCoste(generic.GetCoste() - (euclidean(_instance.getPuntos()[generic.GetValores()[pos1-1]],
				_instance.getPuntos()[generic.GetValores()[pos1]])+ euclidean(_instance.getPuntos()[generic.GetValores()[pos1]],
				_instance.getPuntos()[generic.GetValores()[pos1+1]]) + 
			euclidean(_instance.getPuntos()[generic.GetValores()[pos2]],_instance.getPuntos()[generic.GetValores()[pos2+1]])+euclidean(_instance.getPuntos()[generic.GetValores()[pos2-1]],
				_instance.getPuntos()[generic.GetValores()[pos2]])));
			//HASTA AQUI LA RESTA
			int aux=generic.GetValores()[pos1];
			generic.GetValores()[pos1]=generic.GetValores()[pos2];
			generic.GetValores()[pos2]=aux;
			//AQUI LA SUMA
			generic.SetCoste(generic.GetCoste() + (euclidean(_instance.getPuntos()[generic.GetValores()[pos1-1]],
				_instance.getPuntos()[generic.GetValores()[pos1]])+ euclidean(_instance.getPuntos()[generic.GetValores()[pos1]],
				_instance.getPuntos()[generic.GetValores()[pos1+1]]) + 
			euclidean(_instance.getPuntos()[generic.GetValores()[pos2]],_instance.getPuntos()[generic.GetValores()[pos2+1]])+euclidean(_instance.getPuntos()[generic.GetValores()[pos2-1]],
				_instance.getPuntos()[generic.GetValores()[pos2]])));
	
		return generic;
	}
	
	void firstImprovement(int kNeigh){
		
		//KNeigh es la amplitud del intervalo de busqueda
		bool seguir = true;
		Solucion s1, s2;

			// Recorre todos los vecinos hasta la solución final
			while(seguir)
			{
				s2 = GetSol();
				// Sale de aqui cuando no se hacen mas mejoras por vecindario
				seguir = false;
				
				// Recorro el vecindario y salgo cuando encuentro una mejor
				for(unsigned int i = 0; i<kNeigh; i++)
				{
					// genero una nueva solucion y compruebo su coste
					s1 = Neighoperator();

					if(s1.GetCoste() < s2.GetCoste())
					{
						// Aqui cambio si es mejor y salgo
						SetSol(s1);
						seguir=true;

						break;
					}
				}

			}
		}
		
	void bestImprovement(int kNeigh){
		
		Solucion sB,sP;
		bool seguir = true;

		
			while(seguir) 
			{			
				seguir = false;
				sB = GetSol();
				
				for(int i=0;i<kNeigh;i++) 
				{	
					sP = this->Neighoperator();	
					
					if(sP.GetCoste() < sB.GetCoste())
					{
						sB = sP;
					}
					
				}
				//calCost(sB,_instance.getPuntos());
				if(sB.GetCoste() < GetSol().GetCoste())
				{
					
					SetSol(sB);
					seguir = true;
				}
				
			}
		}

	void getValidSolution(Solucion & generic, std::vector<coordenadas> &puntos){
            int indice1,indice2,aux;
            
			
            indice1 = rand()%puntos.size();
            indice2 = rand()%puntos.size();
            
            while(indice1 == indice2){
                indice2 = rand()%puntos.size();
            }
            
            aux = generic.GetValores()[indice1];
            generic.GetValores()[indice1] = generic.GetValores()[indice2];
            generic.GetValores()[indice2] = aux;
			
			calCost(generic,puntos);
		
	}
        
    //protected: 
        void calCost(Solucion &generic, std::vector<coordenadas> &puntos){
			double coste = 0.0;
			
			for(int i = 0;i < generic.GetValores().size()-1; i++){
				coste += euclidean(puntos[generic.GetValores()[i]],puntos[generic.GetValores()[i+1]]);
			}
			
			generic.SetCoste(coste);
			
		}
		double euclidean(coordenadas & p1,coordenadas & p2){
			return std::sqrt((pow(((double) p1.x-p2.x),2))+(pow(((double) p1.y-p2.y),2)));
		}
};

#endif	/* SOLGENERATOR_HPP */

