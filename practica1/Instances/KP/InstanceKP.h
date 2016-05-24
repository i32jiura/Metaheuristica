#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

struct Material{
	
	int peso,beneficio;
	bool esSolucion; 
};

class InstanceKP{

	private:
		int _nodos,_linea;
		vector<Material> materiales;
		
	public:
	
		InstanceKP(int linea=0)
		{
			_linea = linea;
		}
	
		// MODIFICADORES

		void setNodos(int &nodos)
		{
			_nodos = nodos;
		}
		
		void setLinea(int &linea)
		{
			_linea = linea;
		}		

		// OBSERVADORES
		
		int& getNodos()
		{
			return _nodos;
		}
		
		int& getLinea()
		{
			return _linea;
		}
		
		vector<Material>& getMateriales()
		{
			return materiales;
		}
		
		//FUNCIONES

		friend ifstream& operator>>(ifstream &in, InstanceKP &instance)
		{
		string basura;
		int aux;
		
			for(int i = 0; i < instance.getLinea(); i++)
				getline(in,basura);	
			
			in>>basura;
			in>>basura>>aux;
			
			instance.setNodos(aux);
			
			in>>basura>>basura;
			in>>basura>>basura;
			in>>basura>>basura;
			
			instance.getMateriales().resize(aux);
			
			for(int i = 0; i < aux; i++)
			{
				getline(in,basura,',');
				getline(in,basura,',');
				instance.getMateriales()[i].beneficio = atoi(basura.c_str());
				
				getline(in,basura,',');
				instance.getMateriales()[i].peso = atoi(basura.c_str());				
			} 
			
			return in;
		}

};
