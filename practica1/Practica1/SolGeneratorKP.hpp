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
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class SolGeneratorKP {

	private:

	public:
		// Constructores
		SolGeneratorKP()
		{
            srand(time(NULL));
        }
		
		SolGeneratorKP(const SolGeneratorKP& orig);
		
		// Destructor
		virtual ~SolGeneratorKP();
		
		// Modificadores
		void getValidSolution(Solucion & generic, vector<Material> & materiales, InstanceKP & instance)
		{
			// Declaracion, Inicializacion
            int indice;
			
            indice = rand()%materiales.size();
            
			// Modificacion
			if (generic.GetValores()[indice] == 0)
			{
				generic.GetValores()[indice] = 1;
			}

			else
			{
				generic.GetValores()[indice] = 0;
			}
			
			// Si pasa el peso
			while (calcPeso(generic, materiales) > instance.getCapacidad())
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
		}
	
	protected:
		int calcPeso(Solucion & generic, vector<Material> & materiales)
		{
			int tam = materiales.size();
			int acumulador = 0;
			
			// Calculo de peso total
			for(int i = 0; i < tam; i++)
			{
				if (generic.GetValores()[i] == 1) // Si el material entra en la solucion...
				{
					acumulador += materiales[i].peso;
				}
			}
			
			// Devuelvo peso
			return acumulador;			
		}
};

#endif	/* SOLGENERATORKP_HPP */

