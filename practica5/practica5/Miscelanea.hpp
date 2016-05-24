/* 
 * File:   Miscelanea.hpp
 * Author: cristiangj
 *
 * Created on 24 de marzo de 2016, 12:38
 */

#ifndef MISCELANEA_HPP
#define	MISCELANEA_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "InstanceMD.hpp"

class Miscelanea {
	
public:
	
	void guardarResultados(std::vector<Solucion> &vector,const std::string &fichero) {

		std::ofstream fichSal(fichero.c_str());
		
		int tam = vector.size();

		  for (int i=0;i<tam;++i) {

			  fichSal << i << " " << vector[i].GetCoste() << std::endl;
		  }
		
		fichSal.close();

	}
	
	void guardarResultados(std::vector<std::vector<double> > &vector,std::string &fichero) {

		std::ofstream fichSal(("salidas/0_"+fichero).c_str());
		int j=0,i;
		int tam = vector[0].size(),tam2=vector.size();
		
		  for (i=0;i<tam && j<tam2;++i) {
   
			  if ( not fichSal.is_open() ){
                               
                                std::stringstream s;
                                s<<j;
                                fichSal.open(("salidas/"+s.str()+"_"+fichero).c_str()); 
			  }
			  fichSal << i << " " << vector[j][i] << std::endl;

			  if( i == (tam-1) ){
				  ++j;
				  i=0;
				  fichSal.close();
			  }
			  
		  }
		
		fichSal.close();

	}

	inline double Fitness(double valor1, double valor2){

		return valor1 - valor2;

	}
        
        void GuardarGrafo(std::vector<Solucion >  grafo,InstanceMD instancia,const int &it){
            
 	std::ofstream fichSal;
        std::string cadena;
        
   
            cadena="GeneticoTSP";
        
        
        for(int i=0;i<grafo.size();i=i+grafo.size()/4){
            //Se crea un flujo para guardar el entero como una cadena 
         std::stringstream s;
         s<<it;
         s<<"_";
         s<<i;
         //concateno el flujo para crear anterior
         fichSal.open(("salidas/grafo"+cadena+"/"+s.str()+"_PuntosGrafo.csv").c_str());
                
            for(int j=0;j<grafo[0].GetValores().size();++j){
                
               fichSal<<instancia.getPuntos()[grafo[i].GetValores()[j]].x<<" "<<instancia.getPuntos()[grafo[i].GetValores()[j]].y<<std::endl;                        
        }
         fichSal<<instancia.getPuntos()[grafo[i].GetValores()[0]].x<<" "<<instancia.getPuntos()[grafo[i].GetValores()[0]].y<<std::endl;                
         fichSal.close();
         
       }     
      }  
   void creaGrafo(std::vector<Solucion >  grafo,InstanceMD instancia,const int &it){
    std::ifstream fichEntrada;        
 	std::ofstream fichSal;
        std::string cadena;
        
   
            cadena="GeneticoTSP";
        
        
        for(int i=0;i<grafo.size();i=i+grafo.size()/4){
            //Se crea un flujo para guardar el entero como una cadena 
         std::stringstream s;
         s<<it;
         s<<"_";
         s<<i;
         //concateno el flujo para crear anterior
         system(("./creaGrafo.sh salidas/grafo"+cadena+"/"+s.str()+"_PuntosGrafo.csv salidas/grafo"+cadena+"/"+s.str()+"_Grafo.png").c_str()); 
       }     
      }        
};

#endif	/* MISCELANEA_HPP */

