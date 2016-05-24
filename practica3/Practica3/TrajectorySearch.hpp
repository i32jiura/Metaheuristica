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
#include <cmath>
#include "Miscelanea.hpp"

class TrajectorySearch{
	private:
		Solucion sol;
                int _numEval;
		
	public:
		TrajectorySearch(const int &numeval=1000){
			srand(time(NULL)),_numEval=numeval;
		}
	
		virtual Solucion Neighoperator() = 0;
		
		virtual void firstImprovement(int kNeigh)=0;
		virtual void bestImprovement(int kNeigh)=0;
		virtual std::vector<double> IteratedGreedy(int num) = 0;

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

                void SetNumEval(int _numEval) {
                    this->_numEval = _numEval;
                }

                int GetNumEval() const {
                    return _numEval;
                }
        protected:
            //num: Porcion del vector a detruir(1/num); eliminados: vector con los valores sacados de sol,debe venir desde fuera sin inicializar. 
            virtual Solucion Destruir(const int &num,std::vector< int > &eliminados) = 0;
            virtual void reconstruir(Solucion &parcial,const std::vector<int> &eliminados ) = 0;
            virtual bool Aceptar(Solucion &Sp,Solucion &S,double &T)=0;
            virtual void greedy(const int & numElementos)=0;
            double Enfriar(const double &T,const int &it) {
                    return 0.99*T;
            }   
			

		
};




#endif	/* LOCALSEARCH_HPP */

