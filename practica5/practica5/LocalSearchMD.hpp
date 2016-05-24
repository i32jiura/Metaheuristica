/* 
 * File:   SolGenerator.hpp
 * Author: jesus
 *
 * Created on 4 de marzo de 2016, 19:14
 */

#ifndef LOCALSEARCHMD_HPP
#define	LOCALSEARCHMD_HPP

#include "Solucion.hpp"
#include "InstanceMD.hpp"
#include "LocalSearch.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>


class L_S_Tsp : public LocalSearch {
private:
	InstanceMD _instance;
public:
	
	
	L_S_Tsp(const InstanceMD orig){_instance=orig;}
	
	 void SetInstance(InstanceMD _instance)
	{
		this->_instance = _instance;
	}

	//observador referencia no constante
	InstanceMD& GetInstance()
	{
		return _instance;
	}

	//observador referencia constante
	const InstanceMD& GetInstance() const
	{
		return _instance;
	}

	Solucion Neighoperator(const int tamPuntos) { 
			
		Solucion generic(GetSol(),_instance.getNodos());
            int indice1,indice2,aux;
            
			
            indice1 = rand()%tamPuntos;
            indice2 = rand()%generic.GetValores().size();
            
            while(generic.GetDentro()[indice1])indice1 = rand()%tamPuntos;
            
			
            generic.GetDentro()[indice1]=1;
			generic.GetDentro()[indice2]=0;
			
			aux=generic.GetValores()[indice2];
            generic.GetValores()[indice2]=indice1;
			
			
			
			reCalCost(generic,indice2,aux);
	
		return generic;
	}
	
	void randomSearch(const int & busquedas,const int & numPuntos){
		
		Solucion generic1(numPuntos,_instance.getNodos());
		SetSol(generic1);
		getValidSolution(GetSol(),_instance.getNodos());
		
		for(int i = 0;i<busquedas;++i){
			
			Solucion generic(numPuntos,_instance.getNodos());
			getValidSolution(generic,_instance.getNodos());
			
			if(GetSol().GetCoste()>generic.GetCoste())
				SetSol(generic);
			
		}
		
		
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

	void getValidSolution(Solucion & generic, const int & tamPuntos, std::vector<int> & dentro){
		
            int indice1,indice2,aux,contador=0;
            	
			
            
			while(contador<generic.GetValores().size()){
				
				indice1 = rand()%tamPuntos;
				
				if(!dentro[indice1]){
					dentro[indice1]=1;
					generic.GetValores()[contador]=indice1;
					contador++;
				}	
			
			}
			
			calCost(generic);
		
	}
         
       void calCost(Solucion &generic){
			double coste = 0.0;
			
			for(int i = 0;i < generic.GetValores().size()-1; i++)
				for(int j= i+1;j<generic.GetValores().size();j++)
					coste += euclidean(generic.GetValores()[i],generic.GetValores()[j]);
				
			generic.SetCoste(coste);
			
		}
	   
       void reCalCost(Solucion &generic,const int & indice2,const int & aux){
			double coste = generic.GetCoste();
			
			for(int i = 0;i < generic.GetValores().size()-1; i++)
				coste -= euclidean(generic.GetValores()[aux],generic.GetValores()[i]);
			
			
			
			for(int i = 0;i < generic.GetValores().size()-1; i++)
					coste += euclidean(generic.GetValores()[indice2],generic.GetValores()[i]);
			
			
			generic.SetCoste(coste);
			
		}	   
        
    protected: 

        double euclidean(const int & p1,const int & p2){
			return _instance.getMatDist()[p1][p2];
		}
};




#endif	/* SOLGENERATOR_HPP */

