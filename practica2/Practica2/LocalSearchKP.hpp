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


class L_S_KP : public LocalSearch{

	private:
		/*CAMBIO 1 añadir una instancia como atributo*/
		InstanceKP _instance;
	public:
		
		// Constructores
		
		L_S_KP(const L_S_KP& orig);
		/*CAMBIO 2 constructor*/
		L_S_KP(const InstanceKP orig){_instance=orig;}

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
		
		Solucion Neighoperator(const int & pos=0) {
			
			Solucion generic(GetSol());
			int tam=generic.GetValores().size();
			int pos2 = rand() % tam;
				
			generic.GetValores()[pos2]=1;
			generic.SetCoste(generic.GetCoste() + GetInstance().getMateriales()[pos2].beneficio);
				
			generic.SetPeso(generic.GetPeso()+GetInstance().getMateriales()[pos2].peso);
			return generic;
			
		}
		/*AHORA PODRIAMOS ELIMINAR EL <CLASS T> Y USAR DIRECTAMENTE ESTA INSTANCIA*/
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
		
		
		// Modificadores, CAMBIO 3 Quitar el instanceKP de aqui.
		void getValidSolution(Solucion & generic, vector<Material> & materiales/*, InstanceKP & instance*/)
		{
			// Declaracion, Inicializacion
			
            int indice;
			indice = rand()%materiales.size();
		
			// Modificacion
			modificar(generic,indice);
			GetSol().SetPeso(generic.GetPeso()+GetInstance().getMateriales()[indice].peso);
			while(  GetSol().GetPeso()> _instance.getCapacidad()) 
			{
			
				// Restaurar
				modificar(generic,indice);
				
				indice = rand()%materiales.size();
				
				// Modificar
				modificar(generic,indice);
				GetSol().SetPeso(generic.GetPeso()+GetInstance().getMateriales()[indice].peso);
				
			}

			calcCoste(generic,materiales);
			
			
		}
		
	//protected:
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

