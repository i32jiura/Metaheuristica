#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "Solucion.hpp"

struct coordenadas {
		double x;
		double y;
};

class InstanceTSP {

	private:

			int _nodos,_nLinea;
			std::vector<coordenadas> _puntos;

	public:

			//CONSTRUCTOR
			InstanceTSP(std::string fichero,int linea=0) {
				_nLinea = linea;
				std::ifstream fich(fichero.c_str());
				fich >> this;
				fich.close();
			}

			//MODIFICADORES
			void setNodos(int &nodos) {
				_nodos = nodos;
			}

			void setLinea(int &linea) {
				_nLinea = linea;
			}

			void setPuntos(std::vector<coordenadas> &vectorAux) {
				_puntos = vectorAux;
			}

			//OBSERVADORES
			int& getNodos(){
				return _nodos;
			}

			int& getLinea(){
				return _nLinea;
			}

			std::vector<coordenadas>& getPuntos(){
				return _puntos;
			}
			
			const Solucion& Aptitud(const Solucion &s1,const Solucion &s2)
			{
				if (s1.GetCoste() <= s2.GetCoste())
					return s1;
				else
					return s2;
			}

			//SOBRECARGA OPERADORES
			friend std::ifstream& operator>>(std::ifstream &in,InstanceTSP &instance) {

				std::string basura;
				int aux0,aux3;
				double aux1,aux2;
				
				in>> basura;
				while(basura!="DIMENSION" and basura!="DIMENSION:")in>>basura;
				if(basura=="DIMENSION")
					in>> basura;
				in>>aux0;
				while(basura!="NODE_COORD_SECTION")in>> basura;
				
				instance.getPuntos().resize(aux0);

				for (int i=0;i<aux0;i++) {
						in >> aux3 >> aux1 >> aux2;
						instance.getPuntos()[i].x = aux1;
						instance.getPuntos()[i].y = aux2;
				}

				return in;

			}

};

