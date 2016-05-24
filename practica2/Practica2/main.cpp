/* 
 * File:   main.cpp
 * Author: jesus
 *
 * Created on 16 de marzo de 2016, 18:11
 */

#include <iostream>
#include <cstdlib>
#include "InstanceTsp.hpp"
#include "LocalSearchTsp.hpp"
#include "InstanceKP.hpp"
#include "LocalSearchKP.hpp"
#include "Miscelanea.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include <stdint.h>				//Para usar uint64_t
#include "Tiempo.hpp"

using namespace std;

int main(int argc, char** argv) {

	Miscelanea misc;		//clase para funciones genericas presentes en todas las practicas
	Reloj tiempo;
	string nombreEntTsp,nombreEntKp;
	string nombreSalTspCostFirst,nombreSalTspFitFirst,nombreSalKpCostFirst,nombreSalKpFitFirst;
	string nombreSalTspCostBest,nombreSalTspFitBest,nombreSalKpCostBest,nombreSalKpFitBest;
	string nombreSalTspVectorFirst, nombreSalTspVectorBest, nombreSalKpVectorFirst, nombreSalKpVectorBest;
	string nombreSalTspTiemposFirst, nombreSalTspTiemposBest, nombreSalKpTiemposFirst, nombreSalKpTiemposBest;
	string comando = "./creaGrafico.sh ";
	double valorOptimoTsp,valorOptimoKp;
	int numExperimentos, tamVecindario, tamPuntosTsp, tamMaterialesKp, i, k;
	vector< vector<double> > costesFirstTsp, fitnessFirstTsp, costesFirstKp, fitnessFirstKp;
	vector< vector<double> > costesBestTsp, fitnessBestTsp, costesBestKp, fitnessBestKp;
	vector< vector<double> > tiempoFirstTsp, tiempoBestTsp, tiempoFirstKp, tiempoBestKp;
	vector< vector<int> > vectoresFirstTsp, vectoresBestTsp, vectoresFirstKp, vectoresBestKp;
	
	/*cout << "Introduce el nombre del fichero TSP (sin ruta): ";
	cin >> nombreEntTsp;  
	cout << "Introduce el nombre del fichero KP (sin ruta): ";
	cin >> nombreEntKp;
         * */
        int opcion;
        cout<<"Introduce opcion(1,2 ó 3)\n";
        cin>>opcion;
        switch(opcion){
            case 1:
                nombreEntTsp="berlin52.tsp";
                nombreEntKp="knapPI_1_200_10000.csv";
            break;
            case 2:
                nombreEntTsp="ch150.tsp";
                nombreEntKp="knapPI_12_500_1000.csv";
            break;
            case 3:
                nombreEntTsp="d2103.tsp";
                nombreEntKp="knapPI_1_10000_1000000.csv";
            break;                
        }
	//cout << "Introduce número de experimentos (ej: 1000): ";
	//cin >> numExperimentos;
	numExperimentos=1000;
	cout << "Introduce tamaño vecindario (ej: 5); ";
	cin >> tamVecindario;
	
	/****** salidas para los ficheros de costes y fitness de los metodos primer mejora y mejor mejora *********/
	nombreSalTspCostFirst = "salidas/costesFirst_" + nombreEntTsp;
	nombreSalTspFitFirst = "salidas/fitnessFirst_" + nombreEntTsp;
	nombreSalTspCostBest = "salidas/costesBest_" + nombreEntTsp;
	nombreSalTspFitBest = "salidas/fitnessBest_" + nombreEntTsp;
	nombreSalTspVectorFirst = "salidas/VectorTspFirst_" + nombreEntTsp;
	nombreSalTspVectorBest = "salidas/VectorTspBest_" + nombreEntTsp;
	nombreSalTspTiemposFirst = "salidas/TiemposTspFirst_" + nombreEntTsp;
	nombreSalTspTiemposBest = "salidas/TiemposTspBest_" + nombreEntTsp;
	nombreSalKpCostFirst = "salidas/costesFirst_" + nombreEntKp;
	nombreSalKpFitFirst = "salidas/fitnessFirst_" + nombreEntKp;
	nombreSalKpCostBest = "salidas/costesBest_" + nombreEntKp;
	nombreSalKpFitBest = "salidas/fitnessBest_" + nombreEntKp;
	nombreSalKpVectorFirst = "salidas/VectorKpFirst_" + nombreEntKp;
	nombreSalKpVectorBest = "salidas/VectorKpBest_" + nombreEntKp;
	nombreSalKpTiemposFirst = "salidas/TiemposKpFirst_" + nombreEntKp;
	nombreSalKpTiemposBest = "salidas/TiemposKpBest_" + nombreEntKp;
	
	ofstream fichSalVectorTspFirst(nombreSalTspVectorFirst.c_str());
	ofstream fichSalVectorTspBest(nombreSalTspVectorBest.c_str());
	ofstream fichSalVectorKpFirst(nombreSalKpVectorFirst.c_str());
	ofstream fichSalVectorKpBest(nombreSalKpVectorBest.c_str());
	
	/******** los constructores abren los ficheros y cargan los datos **********/
	L_S_Tsp localSeTsp(InstanceTSP("ficheros/" + nombreEntTsp));
	L_S_KP localSeKp(InstanceKP("ficheros/" + nombreEntKp));
	
	/******** inicializar tamaño de las soluciones ************/
	tamPuntosTsp = localSeTsp.GetInstance().getPuntos().size();
	tamMaterialesKp = localSeKp.GetInstance().getMateriales().size();
	
	Solucion solTsp(tamPuntosTsp), solTspAux(tamPuntosTsp);
	Solucion solKp(tamMaterialesKp), solKpAux(tamMaterialesKp);
	
	/******** inicializar vectores de coste y fitness *********/
	misc.inicVectores(costesFirstTsp,fitnessFirstTsp,numExperimentos,1);
	misc.inicVectores(costesBestTsp,fitnessBestTsp,numExperimentos,1);
	misc.inicVectores(tiempoFirstTsp,tiempoBestTsp,numExperimentos,1);
	misc.inicVectores(costesFirstKp,fitnessFirstKp,numExperimentos,1);
	misc.inicVectores(costesBestKp,fitnessBestKp,numExperimentos,1);
	misc.inicVectores(tiempoFirstKp,tiempoBestKp,numExperimentos,1);
	misc.inicVectores(vectoresFirstTsp,vectoresBestTsp,numExperimentos,tamPuntosTsp);
	misc.inicVectores(vectoresFirstKp,vectoresBestKp,numExperimentos,tamMaterialesKp);
	
	/******** obtener los costes mas optimos reales antes de aplicar cualquier metodo ************/
	for(i=0; i < tamPuntosTsp; i++)
		solTsp.GetValores()[i] = i;
		
	solTspAux = solTsp;						//copia a solTspAux de la inicializacion siempre necesaria de TSP
		
	localSeTsp.calCost(solTsp,localSeTsp.GetInstance().getPuntos());
	
	valorOptimoTsp = solTsp.GetCoste();
	valorOptimoKp = localSeKp.GetInstance().getValorOptimo();
	
	for(k=0; k < tamMaterialesKp; ++k)
       solKpAux.GetValores()[k] = 0;		//inicializacion KP
	
	/******** realizar experimentos **********/
	for (i=0;i<numExperimentos;++i) {
	
		solTsp = solTspAux;		//con esto nos ahorramos mas inicializaciones
		solKp = solKpAux;
		random_shuffle ( solTsp.GetValores().begin(), solTsp.GetValores().end() );
			
		/***** obtener una primera solucion y generar vecinos a partir de esta ****/
		localSeTsp.getValidSolution(solTsp,localSeTsp.GetInstance().getPuntos());
		localSeKp.getValidSolution(solKp,localSeKp.GetInstance().getMateriales());
		
		/***** fijamos la solucion inicial en localsearch para usar firstImprovement *******/
		
		localSeTsp.SetSol(solTsp);
		localSeKp.SetSol(solKp);
		
		/***** aplicar metodos firstImprovement *****/
		
		tiempo.iniciar();
		localSeTsp.firstImprovement(tamVecindario);
		tiempo.parar();
		tiempoFirstTsp[i][0] = (double) tiempo.transcurrido();
		
		tiempo.iniciar();
		localSeKp.firstImprovement(tamVecindario);
		tiempo.parar();
		tiempoFirstKp[i][0] = (double) tiempo.transcurrido();
		
		/***** guardar valores en vectores firstImprovement ******/
		//cout << localSeTsp.GetSol().GetCoste() << " ===== f" << endl;
		costesFirstTsp[i][0] = localSeTsp.GetSol().GetCoste();
		fitnessFirstTsp[i][0] = misc.Fitness(valorOptimoTsp,localSeTsp.GetSol().GetCoste());
		costesFirstKp[i][0] = localSeKp.GetSol().GetCoste();
		fitnessFirstKp[i][0] = misc.Fitness(valorOptimoKp,localSeKp.GetSol().GetCoste());
		
		/****** guardar los vectores correspondientes a cada experimento de firstImprovement ***/
		
		//vectoresFirstTsp[i] = localSeTsp.GetSol().GetValores();
		//vectoresFirstKp[i] = localSeKp.GetSol().GetValores();
		
		for(k=0;k<tamPuntosTsp;++k)
			fichSalVectorTspFirst << localSeTsp.GetSol().GetValores()[k] << " ";
			
		fichSalVectorTspFirst << endl;
		
		for(k=0;k<tamMaterialesKp;++k)
			fichSalVectorKpFirst << localSeKp.GetSol().GetValores()[k] << " ";
		
		fichSalVectorKpFirst << endl;
			
		/***** fijar de nuevo solucion inicial en localsearch para usar bestImprovement *****/
		
		localSeTsp.SetSol(solTsp);
		localSeKp.SetSol(solKp);
		
		/***** aplicar metodos bestImprovement *****/
		tiempo.iniciar();
		localSeTsp.bestImprovement(tamVecindario);
		tiempo.parar();
		tiempoBestTsp[i][0] = (double) tiempo.transcurrido();
		
		tiempo.iniciar();
		localSeKp.bestImprovement(tamVecindario);
		tiempo.parar();
		tiempoBestKp[i][0] = (double) tiempo.transcurrido();
		
		/***** guardar valores en vectores bestImprovement ******/

		costesBestTsp[i][0] = localSeTsp.GetSol().GetCoste();
		fitnessBestTsp[i][0] = misc.Fitness(valorOptimoTsp,localSeTsp.GetSol().GetCoste());
		costesBestKp[i][0] = localSeKp.GetSol().GetCoste();
		fitnessBestKp[i][0] = misc.Fitness(valorOptimoKp,localSeKp.GetSol().GetCoste());
		
		/****** guardar los vectores correspondientes a cada experimento de bestImprovement ***/
		
		//vectoresBestTsp[i] = localSeTsp.GetSol().GetValores();
		//vectoresBestKp[i] = localSeKp.GetSol().GetValores();
		
		for(k=0;k<tamPuntosTsp;++k)
			fichSalVectorTspBest << localSeTsp.GetSol().GetValores()[k] << " ";
			
		fichSalVectorTspBest << endl;
		
		for(k=0;k<tamMaterialesKp;++k)
			fichSalVectorKpBest << localSeKp.GetSol().GetValores()[k] << " ";
		
		fichSalVectorKpBest << endl;
		
	}
	
	misc.guardarResultados(costesFirstTsp,fitnessFirstTsp,nombreSalTspCostFirst,nombreSalTspFitFirst);
	misc.guardarResultados(costesBestTsp,fitnessBestTsp,nombreSalTspCostBest,nombreSalTspFitBest);
	misc.guardarResultados(tiempoFirstTsp,tiempoBestTsp,nombreSalTspTiemposFirst,nombreSalTspTiemposBest);
	//misc.guardarResultados(vectoresFirstTsp,vectoresBestTsp,nombreSalTspVectorFirst,nombreSalTspVectorBest);
	misc.guardarResultados(costesFirstKp,fitnessFirstKp,nombreSalKpCostFirst,nombreSalKpFitFirst);
	misc.guardarResultados(costesBestKp,fitnessBestKp,nombreSalKpCostBest,nombreSalKpFitBest);
	misc.guardarResultados(tiempoFirstKp,tiempoBestKp,nombreSalKpTiemposFirst,nombreSalKpTiemposBest);
	//misc.guardarResultados(vectoresFirstKp,vectoresBestKp,nombreSalKpVectorFirst,nombreSalKpVectorBest);
	
	fichSalVectorTspFirst.close();
	fichSalVectorTspBest.close();
	fichSalVectorKpFirst.close();
	fichSalVectorKpBest.close();
	
	/** generar graficas costes, fitness y tiempos tsp y kp **/
	
	system((comando + nombreSalTspCostFirst + " " + nombreSalTspCostBest + " salidas/costes_" + 
			nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Iteraciones Costes").c_str());
	system((comando + nombreSalTspFitFirst + " " + nombreSalTspFitBest + " salidas/fitness_" + 
			nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Iteraciones Fitness").c_str());
	system((comando + nombreSalTspTiemposFirst + " " + nombreSalTspTiemposBest + " salidas/tiempos_" + 
			nombreEntTsp.replace(nombreEntTsp.length()-3,3,"png") + " Iteraciones Tiempos_ms").c_str());
	system((comando + nombreSalKpCostFirst + " " + nombreSalKpCostBest + " salidas/costes_" + 
			nombreEntKp.replace(nombreEntKp.length()-3,3,"png") + " Iteraciones Beneficio").c_str());
	system((comando + nombreSalKpFitFirst + " " + nombreSalKpFitBest + " salidas/fitness_" + 
			nombreEntKp.replace(nombreEntKp.length()-3,3,"png") + " Iteraciones Fitness").c_str());
	system((comando + nombreSalKpTiemposFirst + " " + nombreSalKpTiemposBest + " salidas/tiempos_" + 
			nombreEntKp.replace(nombreEntKp.length()-3,3,"png") + " Iteraciones Tiempos_ms").c_str());
	
    return 0;
}

