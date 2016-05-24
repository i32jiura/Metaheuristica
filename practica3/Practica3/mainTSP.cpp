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
#include "TrajectorySearchTSP.hpp"
#include "Miscelanea.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <stdint.h>//Para usar uint64_t
#include <sstream>
#include "Tiempo.hpp"

#define NUM_EXPERIMENTOS 50
#define PARTICION_GREEDY 4

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	Miscelanea misc;//clase para funciones genericas presentes en todas las practicas
	Reloj tiempo;
	string nombreEntTsp;
	string nombreSalTspFit1, nombreSalTspFit2;			//ficheros de salida de fitness
	string nombreSalTspTiempos1, nombreSalTspTiempos2;	//ficheros de salida de tiempos
	string comando = "./creaGrafico.sh ";				//comando para crear las graficas
	double valorOptimoTsp;
	int tamPuntosTsp, i, k, opcion,evaluaciones;
	vector< vector<double > > fitnessTsp1, fitnessTsp2;//vectores con el fitness obtenido por experimento y algoritmo
	vector< double >tiempoTsp1, tiempoTsp2;	//vectores con el tiempo obtenido por experimento y algoritmo
        //vector con el número de evaluaciones que guarda un vector con las distintas evaluaciones dentro del método
        vector< vector< vector<int > > > grafoGreedy, grafoSA;
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
         cout<<"introduce número de evaluaciones: ";cin>>evaluaciones;
        /********************************************************************************/
		
	/******** el constructor abre el fichero y carga los datos ***************/
	TrajectorySearchTSP trajecSeTsp(InstanceTSP("ficheros/" + nombreEntTsp),evaluaciones);
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

	nombreSalTspFit1 = "fitnessGreedy_" + nombreEntTsp;
	nombreSalTspFit2 = "fitnessEnfriamiento_" + nombreEntTsp;
	//nombreSalTspTiempos1 = "salidas/TiemposTspGreedy_" + nombreEntTsp;
	//nombreSalTspTiempos2 = "salidas/TiemposTspEnfriamiento_" + nombreEntTsp;
	
	/********************************************************************************/
        
	/******** inicializar tamaño de la solucion inicial *****************************/
	tamPuntosTsp = trajecSeTsp.GetInstance().getNodos();
	Solucion solTsp(tamPuntosTsp), solTspAux(tamPuntosTsp);
	/********************************************************************************/
	
	
	/******** reservar tamaño vectores de coste,fitness y tiempo ********************/
	fitnessTsp1.resize(NUM_EXPERIMENTOS);
	fitnessTsp2.resize(NUM_EXPERIMENTOS);
	tiempoTsp1.resize(NUM_EXPERIMENTOS);
	tiempoTsp2.resize(NUM_EXPERIMENTOS);
        grafoGreedy.resize(NUM_EXPERIMENTOS); 
        grafoSA.resize(NUM_EXPERIMENTOS);
	/********************************************************************************/
	
	
	/***** obtener el coste optimo de TSP a partir del orden en el fichero **********/
	for(i=0; i < tamPuntosTsp; i++)
		solTsp.GetValores()[i] = i;
		
	solTspAux = solTsp;							//copia a solTspAux de la inicializacion siempre necesaria de TSP
		
	trajecSeTsp.calCost(solTsp);				//calculamos el coste optimo de TSP

	valorOptimoTsp = solTsp.GetCoste();		
	/********************************************************************************/
	
	
	/******** inicializacion TSP , ningun material entra *****************************/
	for(k=0; k < tamPuntosTsp; ++k)
       solTsp.GetValores()[k] = 0;
	/********************************************************************************/
	   
	
	/******** realizar experimentos **********/
	for (i=0;i<NUM_EXPERIMENTOS;++i) {
            //Número de iteración
            cout<<"Iteración Nº: "<<i+1<<endl;
		solTsp = solTspAux;
		random_shuffle ( solTsp.GetValores().begin(), solTsp.GetValores().end() );
		
		/***** fijamos el vector de ceros en trajectorySearchTSP para usar Greedy ********/
		trajecSeTsp.SetSol(solTsp);
		/********************************************************************************/
		
		
		/***** ejecutar greedy y guardar tiempo transcurrido ****************************/
		tiempo.iniciar();
		fitnessTsp1[i]=trajecSeTsp.IteratedGreedy(PARTICION_GREEDY);
		tiempo.parar();
		tiempoTsp1[i] = (double) tiempo.transcurrido();
                grafoGreedy[i]=trajecSeTsp.GetSoluciones();
		/********************************************************************************/
	
		/***** obtener un primer vecino para Enfriamiento *******************************/
		trajecSeTsp.getValidSolution(solTsp,trajecSeTsp.GetInstance().getPuntos());
		/********************************************************************************/
		
		
		/***** solucion inicial en trajectorySearchTSP para usar Enfriamiento ************/
		trajecSeTsp.SetSol(solTsp);
		/********************************************************************************/
		
		
		/***** aplicar Enfriamiento y guardar tiempo transcurrido ***********************/
		tiempo.iniciar();
		fitnessTsp2[i]=trajecSeTsp.EnfriamientoSimulado();
		tiempo.parar();
		tiempoTsp2[i] = (double) tiempo.transcurrido();
                grafoSA[i]=trajecSeTsp.GetSoluciones();
		/********************************************************************************/
		
		
	}
	   
	
	/***** guardar vectores de costes y fitness en ficheros *****************************/
        misc.guardarResultados(fitnessTsp1,nombreSalTspFit1);
        misc.guardarResultados(fitnessTsp2,nombreSalTspFit2);
			
	//misc.guardarResultados(tiempoTsp1,nombreSalTspTiempos1);
	//misc.guardarResultados(tiempoTsp2,nombreSalTspTiempos2);
	/********************************************************************************/
        
        /*vector de soluciones a imprimir en fichero(hay que hacer un bucle fuera con el número de experimentos)*******/
	for(i=0;i<NUM_EXPERIMENTOS;++i){
                misc.GuardarGrafo(grafoGreedy[i],trajecSeTsp.GetInstance(),true,i);
                misc.GuardarGrafo(grafoSA[i],trajecSeTsp.GetInstance(),false,i);
        }
        /*********************************************************************************************/
        
	/******* generar graficas costes, fitness y tiempos *****************************/
        for(i=0;i<NUM_EXPERIMENTOS;++i){

            std::stringstream s;
            s<<i;

            system((comando +"salidas/salidas_tsp/"+s.str()+"_"+ nombreSalTspFit1 + " " + "salidas/salidas_tsp/"+s.str()+"_"+nombreSalTspFit2 + " salidas/graficas_tsp/fitness_" + 
                            s.str()+"_"+nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Evaluacion Fitness").c_str());
            //system((comando +"salidas/"+ nombreSalTspTiempos1 + " " + nombreSalTspTiempos2 + " salidas/tiempos_" + 
                            //nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Iteraciones Tiempos_ms").c_str());
	}
        /********************************************************************************/
        
        
	
    return 0;
}



