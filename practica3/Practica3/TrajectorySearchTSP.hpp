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
#include "TrajectorySearch.hpp"
#include "Miscelanea.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>

class TrajectorySearchTSP : public TrajectorySearch {
private:
	InstanceTSP _instance;
        std::vector<std::vector<int> > _soluciones;
public:
	
	TrajectorySearchTSP(const InstanceTSP orig,int numEval): TrajectorySearch(numEval) {_instance=orig,_soluciones.resize(GetNumEval()+1);}
	
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

        void SetSoluciones(std::vector<std::vector<int> > _soluciones) {
            this->_soluciones = _soluciones;
        }

        std::vector<std::vector<int> > & GetSoluciones() {
            return _soluciones;
        }

	Solucion Neighoperator() { 
			
		Solucion generic(GetSol());
		int tam=generic.GetValores().size();
		int pos1=(rand()%(tam-2))+1;
		int pos2=(rand()%(tam-2))+1;
               
			generic.SetCoste(generic.GetCoste() - (_instance.getMatDist()[generic.GetValores()[pos1]][generic.GetValores()[pos1-1]]+_instance.getMatDist()[generic.GetValores()[pos1]][generic.GetValores()[pos1+1]]+
                                _instance.getMatDist()[generic.GetValores()[pos2]][generic.GetValores()[pos2-1]]+_instance.getMatDist()[generic.GetValores()[pos2]][generic.GetValores()[pos2+1]]));
			//HASTA AQUI LA RESTA
			int aux=generic.GetValores()[pos1];
			generic.GetValores()[pos1]=generic.GetValores()[pos2];
			generic.GetValores()[pos2]=aux;
			//AQUI LA SUMA
			generic.SetCoste(generic.GetCoste() + (_instance.getMatDist()[generic.GetValores()[pos1]][generic.GetValores()[pos1-1]]+_instance.getMatDist()[generic.GetValores()[pos1]][generic.GetValores()[pos1+1]]+
                                _instance.getMatDist()[generic.GetValores()[pos2]][generic.GetValores()[pos2-1]]+_instance.getMatDist()[generic.GetValores()[pos2]][generic.GetValores()[pos2+1]]));
	
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
        
        std::vector<double> IteratedGreedy(int num){
            
            std::vector< int > eliminados;
            std::vector<double> fitness(GetNumEval());
            eliminados.resize(_instance.getNodos()/num);
            int it=0;
            Solucion s2,aux;
            //Empieza la magia chicas
            greedy(5);
            firstImprovement(_instance.getNodos());
            fitness[it]=GetSol().GetCoste();
            //solucion a mejorar
            GetSoluciones()[it]= GetSol().GetValores();
            while(it<GetNumEval()){
                s2=Destruir(num,eliminados); 
                reconstruir(s2,eliminados);
                calCost(s2);
                
                aux=GetSol();
                GetSol()=s2;   
                firstImprovement(_instance.getNodos());
                s2=GetSol();
                GetSol()=aux;
                
                GetSol()=seleccionar(s2);
                ++it;
                fitness[it]=GetSol().GetCoste();
                //soluciones parciales
                GetSoluciones()[it]= GetSol().GetValores();
            }
            return fitness;
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
			
			calCost(generic);
		
	}
	std::vector<double> EnfriamientoSimulado(){
		
			std::vector<double> fitness(GetNumEval());
			int fallos = 0,contador = 0;
			double T = TemperaturaInicial();
			Solucion Sbest(GetInstance().getNodos()),Sp(GetInstance().getNodos());

			Sbest = GetSol();
			fitness[contador] = GetSol().GetCoste();
                        //Primera solucion(La que se va a mejorar)
			GetSoluciones()[contador]= GetSol().GetValores();
			while(contador < GetNumEval()){
				for(int i=0;i < GetInstance().getNodos(); i++){
					Sp = Neighoperator();
					
					if(Sp.GetCoste() < Sbest.GetCoste())
						Sbest = Sp;
					
					if(Aceptar(Sp,GetSol(),T)){
						SetSol(Sp);
						fallos = 0;
					}
					else
						fallos = fallos + 1;
				}
				//std::cout<<GetSol().GetCoste()<<"///"<<Sp.GetCoste()<<std::endl;
				//std::cout<<T<<std::endl;
				T = Enfriar(T,contador);
				contador++;
				fitness[contador] = GetSol().GetCoste();
                                //Soluciones intermedias
                                GetSoluciones()[contador]= GetSol().GetValores();
			}

			SetSol(Sbest);
			
			return fitness;
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
            		
            bool Aceptar(Solucion &Sp,Solucion &S,double &T){
                    double PiE,n = 0;

                    if ((Sp.GetCoste()-S.GetCoste())< 0)
                            return true;

                            PiE = exp((-(Sp.GetCoste()-S.GetCoste()))/(T));

                            if(PiE > 0.5)
                                    return true;
                            else
                                    return false;


            }            
            void greedy(const int & numElementos){
                //La solucion se inicializa desde fuera
                int pos1=rand()%(_instance.getNodos()-1),j=0,pos2;
                int tam=_instance.getNodos();
                std::vector<int> aux=GetSol().GetValores();
                GetSol().GetValores()[0]=aux[pos1];
                pos1=aux[pos1];

                aux.erase(aux.begin()+pos1);

                int distAct=100000000;

                for(int i=0;i<_instance.getNodos() && tam>2;++i){//se evaluan N soluciones vecinas aleatorias y se elige la mejor
                    //sol aleatoria
                    pos2=rand()%(tam-1);
                    if(_instance.getMatDist()[pos1][aux[pos2]]<distAct && _instance.getMatDist()[pos1][aux[pos2]]>0){
                        GetSol().GetValores()[j+1]=aux[pos2];
                        distAct=_instance.getMatDist()[pos1][aux[pos2]];
                    }
                   // std::cout<<i<<",";
                    if(i==numElementos){

                        aux.erase(aux.begin()+pos2);//Se elimina la solucion evaluada para no volver a seleccionarla
                        i=0;
                        --tam;
                        ++j;
                        pos1=GetSol().GetValores()[j];
                    }
                }

                if(_instance.getMatDist()[pos1][aux[0]]<_instance.getMatDist()[pos1][aux[1]]){
                    GetSol().GetValores()[j]=aux[0];
                    ++j;
                    GetSol().GetValores()[j]=aux[1];
                }else{
                    GetSol().GetValores()[j]=aux[1];
                    ++j;
                    GetSol().GetValores()[j]=aux[0];                        
                }
                calCost(GetSol());
            }

                Solucion seleccionar(const Solucion & s2){
                    if(s2.GetCoste()<GetSol().GetCoste())
                        return s2;
                    else
                        return GetSol();
                }
		double TemperaturaInicial(){
		
			Solucion S(GetInstance().getNodos()),Sp(GetInstance().getNodos());
			double IE = 0;
			
			for(int i = 0;i<10;i++){
				//Inicialización Solucion incial
				for(int j = 0; j < GetInstance().getNodos(); j++){
					S.GetValores()[j] = j;
				}
				
				std::random_shuffle(S.GetValores().begin(), S.GetValores().end() );
				calCost(S);
				
				SetSol(S);
				Sp = Neighoperator();
				
				IE = IE + S.GetCoste()-Sp.GetCoste();
			}
			IE = IE / 10;
			
			if(IE < 0)
				IE = abs(IE);

			return (-IE)/(log10(0.99));
                }
        
       Solucion Destruir(const int &num,std::vector< int > &eliminados){
        
            Solucion aux=GetSol();
            int elementos=aux.GetValores().size()/num,i=0,pos;
            int tam=aux.GetValores().size();
            eliminados.resize(elementos);

            while(i<elementos){
                pos=(rand()%(tam-2))+1;
                if(aux.GetValores()[pos]!=-1){
                    aux.SetCoste(aux.GetCoste()-(_instance.getMatDist()[aux.GetValores()[pos]][aux.GetValores()[pos+1]]+
                    _instance.getMatDist()[aux.GetValores()[pos]][aux.GetValores()[pos-1]]));
                    eliminados[i]=aux.GetValores()[pos];
                    aux.GetValores()[pos]=-1;
                    ++i;        
                }        
             }
             return aux;
         }
         void reconstruir(Solucion &parcial,const std::vector<int> &eliminados ){
            int j=0;
            for(int i=parcial.GetValores().size();i>-1;--i)
                if(parcial.GetValores()[i]==-1){
                    parcial.GetValores()[i]=eliminados[j];
                    parcial.SetCoste(parcial.GetCoste()+(_instance.getMatDist()[parcial.GetValores()[i]][parcial.GetValores()[i+1]]+
                    _instance.getMatDist()[parcial.GetValores()[i]][parcial.GetValores()[i-1]]));
                    ++j;
                }
         }
};

#endif	/* SOLGENERATOR_HPP */

