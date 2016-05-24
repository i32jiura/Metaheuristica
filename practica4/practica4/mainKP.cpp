/* 
 * File:   mainKP.cpp
 * Author: cristiangj
 *
 * Created on 11 de abril de 2016, 21:03
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include "InstanceKP.hpp"
#include "PopulationBasedKP.hpp"
#include "Miscelanea.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <stdint.h>				//Para usar uint64_t
#include <sstream>

#define NUM_EXPERIMENTOS 1

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	Miscelanea misc;									//clase para funciones genericas presentes en todas las practicas
	string nombreEntKp;
	string nombreSalKpFit1;								//ficheros de salida de fitness
	string comando = "./creaGrafico.sh ";				//comando para crear las graficas
	int tamMaterialesKp, i, k, opcion, numIndividuos,numPadres,numEpocas,numContendientes,numCortes;
	vector<vector<Solucion> > mejores;

	cout<<"Main KP\n===================\n";
	cout<<"Introduce opcion(1,2 ó 3)\n";
	cin>>opcion;
	
	switch(opcion){
		case 1:
			nombreEntKp="knapPI_1_200_10000.csv";
		break;
		case 2:
			nombreEntKp="knapPI_12_500_1000.csv";
		break;
		case 3:
			nombreEntKp="knapPI_1_10000_1000000.csv";
		break;                
	}
		
   /***********Recoger número de evaluaciones***************************************/
	cout<<"introduce número de individuos: ";cin>>numIndividuos;
    cout<<"introduce número de padres: ";cin>>numPadres;
	cout<<"introduce número de epocas: ";cin>>numEpocas;
	cout<<"introduce número de contendientes: ";cin>>numContendientes;
	cout<<"introduce número de cortes: ";cin>>numCortes;
    /********************************************************************************/
		 
	/******** el constructor abre el fichero y carga los datos ***************/
	PopulationBasedKP popBasKP(InstanceKP("ficheros/" + nombreEntKp),numIndividuos);
	mejores.resize(NUM_EXPERIMENTOS);
	tamMaterialesKp = popBasKP.GetInstance().getNodos();
	/********************************************************************************/
	
	
	/****** nombres de los ficheros de salida ***************************************/
	nombreSalKpFit1 = "fitnessGenetico_" + nombreEntKp;
	/********************************************************************************/

	   
	/******** realizar experimentos **********/
	for (i=0;i<NUM_EXPERIMENTOS;++i) {
             
		cout<<"Iteración Nº: "<<i+1<<endl;
		
		mejores[i] = popBasKP.Genetico(numPadres,numEpocas,numContendientes,numCortes);
		
	}
	   
	
	/***** guardar vectores de fitness en ficheros *****************************/
	   
	for(i=0;i<NUM_EXPERIMENTOS;++i){

		std::stringstream s;
		s<<i;
		misc.guardarResultados(mejores[i],("salidas/salidas_kp/" + s.str() + "_" + nombreSalKpFit1));
    }
	/********************************************************************************/
	
	        
	/******* generar graficas fitness  *****************************/
	for(i=0;i<NUM_EXPERIMENTOS;++i){

		std::stringstream s;
		s<<i;
		system((comando +"salidas/salidas_kp/"+s.str()+"_"+ nombreSalKpFit1 + " salidas/graficas_kp/fitness_" + 
						s.str()+"_"+nombreEntKp.replace(nombreEntKp.length()-3,3,"png") + " Evaluacion Fitness").c_str());
	}
    /********************************************************************************/
        
	
    return 0;
}

