/* 
 * File:   LocalSearch.hpp
 * Author: jesus
 *
 * Created on 16 de marzo de 2016, 18:17
 */

#ifndef LOCALSEARCH_HPP
#define	LOCALSEARCH_HPP

#include "Solucion.hpp"
#include <ctime>
#include <vector>

class LocalSearch{
	private:
		Solucion sol;
		
	public:
		LocalSearch(){
			srand(time(NULL));
		}
		
		/*std::vector<int> Neighoperator(int kNeigh){
			int tam=sol.GetValores().size(),inicio=rand()%tam;
			
			if(2*kNeigh>tam || kNeigh>tam)
				return sol.GetValores();
			
			if(inicio+kNeigh>=tam)
				inicio=inicio-((inicio+kNeigh)-tam);
			if(inicio-kNeigh<0)
				inicio=inicio+(kNeigh-inicio);
			
			std::vector<int> aux(sol.GetValores().begin()+(inicio-kNeigh),sol.GetValores().begin()+(inicio+kNeigh));
			return aux;
				
		}*/
		
		virtual Solucion Neighoperator(const int & pos=0) = 0;
		
		virtual void firstImprovement(int kNeigh)=0;
		virtual void bestImprovement(int kNeigh)=0;

        void SetSol(Solucion sol)
        {
        	this->sol = sol;
        }
		
		Solucion& GetSol() {
			return sol;
		}

        const Solucion& GetSol() const
        {
        	return sol;
        }
		
};




#endif	/* LOCALSEARCH_HPP */

