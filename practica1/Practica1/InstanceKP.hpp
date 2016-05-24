/* 
 * File:   InstanceKP.hpp
 * Author: cristiangj
 *
 * Created on 7 de marzo de 2016, 13:02
 */

#ifndef INSTANCEKP_HPP
#define	INSTANCEKP_HPP

#include<vector>
#include<string>
#include<fstream>
#include "Solucion.hpp"

struct Material{
	int peso,beneficio;
	bool esSolucion; 
	
};

class InstanceKP {
	
	private:
		int _nodos,_linea,_capacidad,_valorOptimo;
		std::vector<Material> materiales;
		
	public:
		
		InstanceKP(std::string fichero,int linea=0) { 
			_linea = linea; 
			std::ifstream fich(fichero.c_str());
			fich >> this;
			fich.close();
		}

        void setValorOptimo(int _valorOptimo)
        {
        	this->_valorOptimo = _valorOptimo;
        }

        const int getValorOptimo() const
        {
        	return _valorOptimo;
        }
		InstanceKP(const InstanceKP& orig);

		// MODIFICADORES

		inline void setNodos(int &nodos) { _nodos = nodos; }

		inline void setLinea(int &linea) { _linea = linea; }		
		
		inline void setCapacidad(int &capacidad) { _capacidad = capacidad; }

		// OBSERVADORES

		inline const int& getNodos() const { return _nodos; }

		inline const int& getLinea() const { return _linea; }

		inline const std::vector<Material>& getMateriales() const { return materiales; }
		
		inline std::vector<Material>& getMateriales() { return materiales; }
		
        inline const int getCapacidad() const { return _capacidad; }
		
		const Solucion& Aptitud(const Solucion &s1,const Solucion &s2)
		{
			if (s1.GetCoste() >= s2.GetCoste())
				return s1;
			else
				return s2;
		}

		// SOBRECARGA OPERADORES

		friend std::ifstream& operator>>(std::ifstream &in, InstanceKP &instance) {

			std::string basura;
			int aux,capacidad;

				for(int i = 0; i < instance.getLinea(); i++)
					getline(in,basura);	

				in>>basura;
				in>>basura>>aux;

				instance.setNodos(aux);

				in>>basura>>capacidad;
				
				instance.setCapacidad(capacidad);
				
				in>>basura>>capacidad;
				instance.setValorOptimo(capacidad);
				in>>basura>>basura;

				instance.getMateriales().resize(aux);

				for(int i = 0; i < aux; i++) {
					getline(in,basura,',');
					getline(in,basura,',');
					instance.getMateriales()[i].beneficio = atoi(basura.c_str());

					getline(in,basura,',');
					instance.getMateriales()[i].peso = atoi(basura.c_str());				
				} 

				return in;
		}

};

#endif	/* INSTANCEKP_HPP */

