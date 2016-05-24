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
#include "TrajectorySearchKP.hpp"
#include "Miscelanea.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <stdint.h>				//Para usar uint64_t
#include <sstream>
#include "Tiempo.hpp"

#define NUM_EXPERIMENTOS 50
#define PARTICION_GREEDY 16

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	Miscelanea misc;									//clase para funciones genericas presentes en todas las practicas
	Reloj tiempo;
	string nombreEntKp;
	string nombreSalKpFit1, nombreSalKpFit2;			//ficheros de salida de fitness
	string nombreSalKpTiempos1, nombreSalKpTiempos2;	//ficheros de salida de tiempos
	string comando = "./creaGrafico.sh ";				//comando para crear las graficas
	double valorOptimoKp;
	int tamMaterialesKp, i, k, opcion,evaluaciones;
	vector<vector<double> > fitnessKp1, fitnessKp2;				//vectores con el fitness obtenido por experimento y algoritmo
	vector<double> tiempoKp1, tiempoKp2;				//vectores con el tiempo obtenido por experimento y algoritmo

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
    cout<<"introduce número de evaluaciones: ";cin>>evaluaciones;
    /********************************************************************************/
		 
	/******** el constructor abre el fichero y carga los datos ***************/
	TrajectorySearchKP trajecSeKp(InstanceKP("ficheros/" + nombreEntKp),evaluaciones);
	/********************************************************************************/
	
	
	/****** nombres de los ficheros de salida ***************************************/
	nombreSalKpFit1 = "fitnessGreedy_" + nombreEntKp;
	nombreSalKpFit2 = "fitnessEnfriamiento_" + nombreEntKp;
	//nombreSalKpTiempos1 = "TiemposKpGreedy_" + nombreEntKp;
	//nombreSalKpTiempos2 = "TiemposKpEnfriamiento_" + nombreEntKp;
	/********************************************************************************/
	
	
	/******** inicializar tamaño de la solucion inicial *****************************/
	tamMaterialesKp = trajecSeKp.GetInstance().getNodos();
	Solucion solKp(tamMaterialesKp), solKpAux(tamMaterialesKp);
	/********************************************************************************/
	
	
	/******** reservar tamaño vectores de coste,fitness y tiempo ********************/
	fitnessKp1.resize(NUM_EXPERIMENTOS);
	fitnessKp2.resize(NUM_EXPERIMENTOS);
	tiempoKp1.resize(NUM_EXPERIMENTOS);
	tiempoKp2.resize(NUM_EXPERIMENTOS);
	/********************************************************************************/
	
	
	valorOptimoKp = trajecSeKp.GetInstance().getValorOptimo();
	
	
	/******** inicializacion KP , ningun material entra *****************************/
	for(k=0; k < tamMaterialesKp; ++k)
       solKp.GetValores()[k] = 0;
	/********************************************************************************/
	   
	/******** realizar experimentos **********/
	for (i=0;i<NUM_EXPERIMENTOS;++i) {
             
		cout<<"Iteración Nº: "<<i+1<<endl;
		solKp = solKpAux;
		
		/***** fijamos el vector de ceros en trajectorySearchKP para usar Greedy ********/
		trajecSeKp.SetSol(solKp);
		/********************************************************************************/
		
		
		/***** ejecutar greedy y guardar tiempo transcurrido ****************************/
		tiempo.iniciar();
		fitnessKp1[i] = trajecSeKp.IteratedGreedy(PARTICION_GREEDY);
		tiempo.parar();
		tiempoKp1[i] = (double) tiempo.transcurrido();
		/********************************************************************************/

			
		/***** obtener un primer vecino para Enfriamiento *******************************/
		trajecSeKp.getValidSolution(solKp,trajecSeKp.GetInstance().getMateriales());
		/********************************************************************************/
		
		/***** solucion inicial en trajectorySearchKP para usar Enfriamiento ************/
		trajecSeKp.SetSol(solKp);
		/********************************************************************************/
		
		
		/***** aplicar Enfriamiento y guardar tiempo transcurrido ***********************/
		tiempo.iniciar();
		fitnessKp2[i] = trajecSeKp.EnfriamientoSimulado();
		tiempo.parar();
		tiempoKp2[i] = (double) tiempo.transcurrido();
		/********************************************************************************/
		
	}
	   
	
	/***** guardar vectores de costes y fitness en ficheros *****************************/
		misc.guardarResultados(fitnessKp1,nombreSalKpFit1);
		misc.guardarResultados(fitnessKp2,nombreSalKpFit2);
		//misc.guardarResultados(tiempoKp1,nombreSalKpTiempos1);
		//misc.guardarResultados(tiempoKp2,nombreSalKpTiempos2);
	/********************************************************************************/

	
	        
	/******* generar graficas costes, fitness y tiempos *****************************/
        for(i=0;i<NUM_EXPERIMENTOS;++i){

            std::stringstream s;
            s<<i;

            system((comando +"salidas/salidas_kp/"+s.str()+"_"+ nombreSalKpFit1 + " " + "salidas/salidas_kp/"+s.str()+"_"+nombreSalKpFit2 + " salidas/graficas_kp/fitness_" + 
                            s.str()+"_"+nombreEntKp.replace(nombreEntKp.length()-3,3,"png") + " Evaluacion Fitness").c_str());
            //system((comando +"salidas/"+ nombreSalTspTiempos1 + " " + nombreSalTspTiempos2 + " salidas/tiempos_" + 
                            //nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Iteraciones Tiempos_ms").c_str());
		}
    /********************************************************************************/
        
	
    return 0;
}

