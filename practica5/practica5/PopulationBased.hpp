/* 
 * File:   PopulationBased.hpp
 * Author: jesus
 *
 * Created on 27 de abril de 2016, 12:07
 */

#ifndef POPULATIONBASED_HPP
#define	POPULATIONBASED_HPP

#include "Solucion.hpp"
#include <vector>
#include <ctime>
#include "Timer.h"

using namespace std;

class PopulationBased{


	private:
		
		vector< Solucion > _poblacion;
		int _numIndividuos;
		Solucion _mejor, _mejorPoblacion;
		
	public:
		PopulationBased(const int & numIndividuos){
			
			_poblacion.resize(numIndividuos),_numIndividuos=numIndividuos;
			srand(time(NULL));
			srand48(time(NULL));
			
		}
		
		void SetMejor(Solucion _mejor) {
			this->_mejor = _mejor;
		}

		Solucion & GetMejor() {
			return _mejor;
		}

		void SetNumIndividuos(int _numIndividuos) {
			this->_numIndividuos = _numIndividuos;
		}

		const int & GetNumIndividuos() const {
			return _numIndividuos;
		}

		void SetPoblacion(vector<Solucion> _poblacion) {
			this->_poblacion = _poblacion;
		}

		vector<Solucion> & GetPoblacion() {
			return _poblacion;
		}

        void SetMejorPoblacion(Solucion _mejorPoblacion)
        {
        	this->_mejorPoblacion = _mejorPoblacion;
        }

        Solucion& GetMejorPoblacion() {
			
        	return _mejorPoblacion;
        }
		
		std::vector<Solucion> Genetico(const int & numPadres,const int & epocas,const int & numContendientes,const int & alelos){
			Timer timer;
			int i;
			vector<Solucion> padres,hijos;
			vector<Solucion> mejores(epocas);
			
			inicializar();
			evaluar();
			for( i=0;i<epocas && timer.elapsed_time(timer.REAL) < 600;++i ){
				padres=seleccionar(numPadres,numContendientes);
				hijos=recombinar(padres,alelos);
				mutacion(hijos);
				nuevaPoblacion(hijos);
				evaluar();	
				mejores[i] = GetMejor();
			}
			mejores.resize(i);
			return mejores;
		}
	protected:
		virtual void inicializar() = 0;// Crea una poblacion
		virtual void evaluar() = 0;// eliges la mejor solución y guardarla
		virtual vector<Solucion> seleccionar(const int & numPadres,const int & numContendientes) = 0;// Torneo
		virtual vector<Solucion> recombinar(vector< Solucion> & padres,const int & alelos) = 0;// Crossover
		virtual void mutacion(vector< Solucion> & hijos) = 0;
		virtual void nuevaPoblacion(vector<Solucion> & hijos) = 0;
		
		
};

#endif	/* POPULATIONBASED_HPP */

