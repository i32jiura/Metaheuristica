#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "Solucion.hpp"

struct coordenadas {
		double x;
		double y;
};

class InstanceMD {

	private:

			int _nodos,_nLinea;
			std::vector<coordenadas> _puntos;
			std::vector< std::vector<double> > _matDist;
			
	public:

			//CONSTRUCTOR
		
			InstanceMD() { }
		
			InstanceMD(std::string fichero,int linea=0) {
				_nLinea = linea;
				
				std::ifstream fichMat((fichero + ".mat").c_str());
				
				if ( fichMat.is_open() ) {
					cargaDist(fichMat);
					fichMat.close();
        			std::ifstream fich((fichero+".tsp").c_str());
				if ( !fich.is_open() ) {
				std::cout << "Error en la carga de fichero TSP" << std::endl;
				} else {
					fich >> (*this);
					fich.close();
                                        }
				} else {
				
					std::ifstream fich((fichero+".tsp").c_str());
					if ( !fich.is_open() ) {
						std::cout << "Error en la carga de fichero TSP" << std::endl;
					} else {
						fich >> (*this);
						fich.close();
						guardaDist(fichero + ".mat");
						std::ifstream fichMat((fichero + ".mat").c_str());
						cargaDist(fichMat);
						fichMat.close();
					}
				}
				
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

            void setMatDist(std::vector<std::vector<double> > _matDist)
            {
            	this->_matDist = _matDist;
            }
			
			std::vector<std::vector<double> >& getMatDist() {
				
				return _matDist;
			}

            const std::vector<std::vector<double> >& getMatDist() const
            {
            	return _matDist;
            }

			//SOBRECARGA OPERADORES
			friend std::ifstream& operator>>(std::ifstream &in,InstanceMD &instance) {

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
				instance.setNodos(aux0);

				for (int i=0;i<aux0;i++) {
						in >> aux3 >> aux1 >> aux2;
						instance.getPuntos()[i].x = aux1;
						instance.getPuntos()[i].y = aux2;
				}

				return in;

			}

			//FUNCIONES
			void cargaDist(std::ifstream &fichero) {
				
				int numPuntos, i=0,j=0;
				double dato;
				
				fichero >> numPuntos;
				
				_matDist.resize(numPuntos);
				
				for (int i=0;i<numPuntos;++i)
					_matDist[i].resize(numPuntos);
				
				while ( fichero >> dato ) {
					//std::cout << dato << " ";
					_matDist[i][j] = dato;
					j++;
					if ( j == numPuntos ) {
						j=0;
						i++;
					}
					
				}
				
			}
			
			void guardaDist(std::string nombreFich) {
				
				std::ofstream fichero(nombreFich.c_str());
				
				fichero << _nodos << std::endl;
				
				for(int i=0;i<_nodos;++i) {
					for(int j=0;j<_nodos;++j)
						fichero << euclideanDist(_puntos[i],_puntos[j]) << " ";
					fichero << std::endl;
				}
				
				fichero.close();
				
			}
			
			double euclideanDist(coordenadas & p1,coordenadas & p2){
				return std::sqrt((pow(((double) p1.x-p2.x),2))+(pow(((double) p1.y-p2.y),2)));
			}
			
};

