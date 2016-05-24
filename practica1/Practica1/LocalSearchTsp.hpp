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


class L_S_Tsp : public LocalSearch<InstanceTSP> {
private:
	
public:
	
	L_S_Tsp(const L_S_Tsp& orig);
	
		void firstImprovement(T instance,int kNeigh){/*Implementar codigo */}
		void bestImprovement(T instance,int kNeigh){/*Implementar codigo */}

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

