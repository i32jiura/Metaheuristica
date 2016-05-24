#include<iostream>
#include<vector>
#include"InstanceTSP.hpp"

class SolutionTSP {

	private:
		std::vector<coordenadas> optimaPuntos;	//solucion optima de TSP

	public:

		//SOBRECARGA OPERADORES
		bool operator==(std::vector<coordenadas> &puntos1, std::vector<coordenadas> &puntos2) {

			for (unsigned int i=0;i<puntos1.size();i++)
				if ( (puntos1[i].x != puntos2[i].x) or (puntos1[i].y != puntos2[i].y) )
					return false;

			return true;

		}
}
