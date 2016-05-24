/* 
 * File:   mainTSP.cpp
 * Author: cristiangj
 *
 * Created on 12 de abril de 2016, 0:24
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include "InstanceTsp.hpp"
#include "PopulationBasedTSP.hpp"
#include "Miscelanea.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <stdint.h>//Para usar uint64_t
#include <sstream>

#define NUM_EXPERIMENTOS 2

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	Miscelanea misc;//clase para funciones genericas presentes en todas las practicas
	string nombreEntTsp;
	string nombreSalTspFit1;			//ficheros de salida de fitness
	string comando = "./creaGrafico.sh ";				//comando para crear las graficas
	int tamPuntosTsp, i, k, opcion, numIndividuos,numPadres,numEpocas,numContendientes,alelos;
	vector< vector<double > > fitnessTsp1;//vectores con el fitness obtenido por experimento y algoritmo
	vector< vector<Solucion> > mejores;
    //vector con el número de evaluaciones que guarda un vector con las distintas evaluaciones dentro del método
    //vector< vector< vector<Solucion > > > grafoGenetico;
	cout<<"Main TSP\n===================\n";
	cout<<"Introduce opcion(1,2 ó 3)\n";
	cin>>opcion;
	
	switch(opcion){	//ficheros sin extension para leer o crear los .mat de TSP
		case 1:
			nombreEntTsp="berlin52";
                break;
		case 2:
			nombreEntTsp="ch150";
                break;
		case 3:
			nombreEntTsp="d2103";
		break;                
	}
        
    /***********Recoger número de evaluaciones***************************************/
	cout<<"introduce número de individuos: ";cin>>numIndividuos;
    cout<<"introduce número de padres: ";cin>>numPadres;
	cout<<"introduce número de epocas: ";cin>>numEpocas;
	cout<<"introduce número de contendientes: ";cin>>numContendientes;
	cout<<"introduce número de alelos recombinados: ";cin>>alelos;
    /********************************************************************************/
		
	/******** el constructor abre el fichero y carga los datos ***************/
	PopulationBasedTSP popBasTSP(InstanceTSP("ficheros/" + nombreEntTsp),numIndividuos);
	mejores.resize(NUM_EXPERIMENTOS);
	tamPuntosTsp = popBasTSP.GetInstance().getNodos();
	/********************************************************************************/
	
	switch(opcion){
       case 1:
        nombreEntTsp="berlin52.tsp";
	break;
       case 2:
        nombreEntTsp="ch150.tsp";
        break;
       case 3:
         nombreEntTsp="d2103.tsp";
         break;                
    }
	

	/****** nombres de los ficheros de salida ***************************************/

	nombreSalTspFit1 = "fitnessGenetico_" + nombreEntTsp;

	/********************************************************************************/
 
	   
	/******** realizar experimentos **********/
	for (i=0;i<NUM_EXPERIMENTOS;++i) {
            //Número de iteración
        cout<<"Iteración Nº: "<<i+1<<endl;
	
		mejores[i] = popBasTSP.Genetico(numPadres,numEpocas,numContendientes,alelos);
	
	}
	   
	/***** guardar vectores de fitness en ficheros *****************************/
	   
	for(i=0;i<NUM_EXPERIMENTOS;++i){

		std::stringstream s;
		s<<i;
		misc.guardarResultados(mejores[i],("salidas/salidas_tsp/" + s.str() + "_" + nombreSalTspFit1));
    }
	/********************************************************************************/
        
    /*vector de soluciones a imprimir en fichero(hay que hacer un bucle fuera con el número de experimentos)*******/
	for(i=0;i<NUM_EXPERIMENTOS;++i){
		misc.GuardarGrafo(mejores[i],popBasTSP.GetInstance(),i);
    }
	
	for(i=0;i<NUM_EXPERIMENTOS;++i){
		misc.creaGrafo(mejores[i],popBasTSP.GetInstance(),i);
    }
    /*********************************************************************************************/
        
	/******* generar graficas fitness ************************************************************/
	for(i=0;i<NUM_EXPERIMENTOS;++i){

		std::stringstream s;
		s<<i;

		system((comando +"salidas/salidas_tsp/"+s.str()+"_"+ nombreSalTspFit1 + " salidas/graficas_tsp/fitness_" + 
						s.str()+"_"+nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Evaluacion Fitness").c_str());
	}
    /********************************************************************************/
        
        
	
    return 0;
}