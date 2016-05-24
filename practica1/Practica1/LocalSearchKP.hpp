/* 
 * File:   SolGeneratorKP.hpp
 * Author: cristiangj
 *
 * Created on 7 de marzo de 2016, 13:02
 */

#ifndef SOLGENERATORKP_HPP
#define	SOLGENERATORKP_HPP

using namespace std;

#include "Solucion.hpp"
#include "InstanceKP.hpp"
#include "LocalSearch.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class L_S_KP : public LocalSearch<InstanceKP>{

	private:

	public:
		
		// Constructores
		
		L_S_KP(const L_S_KP& orig);
		

		void firstImprovement(T instance,int kNeigh){/*Implementar codigo */}
		void bestImprovement(T instance,int kNeigh){/*Implementar codigo */}
		
		
		// Modificadores
		void getValidSolution(Solucion & generic, vector<Material> & materiales, InstanceKP & instance)
		{
			// Declaracion, Inicializacion
			
            int indice;
			indice = rand()%materiales.size();
		
			// Modificacion
			modificar(generic,indice);
			
			while( calcPeso(generic, materiales) > instance.getCapacidad()) 
			{
			
				// Restaurar
				modificar(generic,indice);
				
				indice = rand()%materiales.size();
				
				// Modificar
				modificar(generic,indice);
				
			}

			calcCoste(generic,materiales);
			
		}
	
	protected:
		int calcPeso(Solucion & generic, vector<Material> & materiales)
		{
			int tam = materiales.size();
			int acumulador = 0;
			
			// Calculo de peso total
			for(int i = 0; i < tam; i++)
				if (generic.GetValores()[i] == 1) // Si el material entra en la solucion
					acumulador += materiales[i].peso;
			
			// Devolver peso
			return acumulador;			
		}
		
		void modificar(Solucion &generic,int &indice)
		{
			
			if (generic.GetValores()[indice] == 0)
			{
				generic.GetValores()[indice] = 1;
			}
			else
			{
				generic.GetValores()[indice] = 0;
			}
		}
		
		void calcCoste(Solucion &generic, std::vector<Material> &materiales) {
			
			int tam = materiales.size();
			double coste = 0.0;
			
			for (int i=0;i<tam;++i)
				if ( generic.GetValores()[i] == 1 )
					coste +=  materiales[i].beneficio;
			
			generic.SetCoste(coste);
		}
};

#endif	/* SOLGENERATORKP_HPP */

