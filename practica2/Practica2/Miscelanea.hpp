/* 
 * File:   Miscelanea.hpp
 * Author: cristiangj
 *
 * Created on 24 de marzo de 2016, 12:38
 */

#ifndef MISCELANEA_HPP
#define	MISCELANEA_HPP

#include <fstream>
#include <string>

class Miscelanea {
	
public:
	  
	/**
	* @param numExperimentos numero de experimentos a realizar, ejemplo: 1000
	* Nota: tras la inicializacion de los dos vectores correspondientes a cada instancia tan solo hay que hacer vector[i].push_back()
	* siendo dicho push_back el coste o el fitness en cada iteracion o experimento
	*/
	void inicVectores(std::vector< std::vector<double> > &vectorCostes,std::vector< std::vector<double> > &vectorFitness,int numExperimentos,int repeticiones) {

		vectorCostes.resize(numExperimentos);
		vectorFitness.resize(numExperimentos);

		for(int i=0;i<numExperimentos;++i) {
			vectorCostes[i].resize(repeticiones);
			vectorFitness[i].resize(repeticiones);
		}

	}
	
	void inicVectores(std::vector< std::vector<int> > &vectorCostes,std::vector< std::vector<int> > &vectorFitness,int numExperimentos,int repeticiones) {

		vectorCostes.resize(numExperimentos);
		vectorFitness.resize(numExperimentos);

		for(int i=0;i<numExperimentos;++i) {
			vectorCostes[i].resize(repeticiones);
			vectorFitness[i].resize(repeticiones);
		}

	}
  
	void guardarResultados(std::vector< std::vector<double> > &vectorCostes,std::vector< std::vector<double> > &vectorFitness,std::string &fichCostes, std::string &fichFitness) {

		std::ofstream fichSalCostes(fichCostes.c_str());
		std::ofstream fichSalFitness(fichFitness.c_str());
		
		int tam1 = vectorCostes.size(), tam2 = vectorCostes[0].size(), tam3 = vectorFitness[0].size();


		  for (int i=0;i<tam1;++i) {

			  for (int j=0;j<tam2;++j)	
				  fichSalCostes << i << " " << vectorCostes[i][j] << " ";

			  for (int k=0;k<tam3;++k)
				  fichSalFitness << i << " " << vectorFitness[i][k] << " ";

			  fichSalCostes << std::endl;
			  fichSalFitness << std::endl;
		  }

		fichSalCostes.close();
		fichSalFitness.close();

	}
	
	void guardarResultados(std::vector< std::vector<int> > &vectorCostes,std::vector< std::vector<int> > &vectorFitness,std::string &fichCostes, std::string &fichFitness) {

		std::ofstream fichSalCostes(fichCostes.c_str());
		std::ofstream fichSalFitness(fichFitness.c_str());
		
		int tam1 = vectorCostes.size(), tam2 = vectorCostes[0].size(), tam3 = vectorFitness[0].size();


		  for (int i=0;i<tam1;++i) {

			  for (int j=0;j<tam2;++j)	
				  fichSalCostes << i << " " << vectorCostes[i][j] << " ";

			  for (int k=0;k<tam3;++k)
				  fichSalFitness << i << " " << vectorFitness[i][k] << " ";

			  fichSalCostes << std::endl;
			  fichSalFitness << std::endl;
		  }

		fichSalCostes.close();
		fichSalFitness.close();

	}

	double Fitness(double valorOptimo, double valorActual){

		return valorActual - valorOptimo;

	}
};

#endif	/* MISCELANEA_HPP */

