#include<vector>
#include<fstream>
#include<string>

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
			InstanceTSP(int linea=0) {
				_nLinea = linea;
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

			//SOBRECARGA OPERADORES
			friend std::ifstream operator>>(std::ifstream &in,InstanceTSP &instance) {

				std::string basura;
				int aux0,aux1,aux2;

				in >> basura;
				in >> basura;
				in >> basura;
				getline(in,basura," ");
				in >> aux0;
				instance.setNodos(aux0);
				in >> basura;
				in >> basura;

				instance.getPuntos().resize(aux0);

				for (int i=0;i<aux1;i++) {
						in >> aux0 >> aux1 >> aux2;
						instance.getPuntos[i].x = aux1;
						instance.getPuntos[i].y = aux2;
				}

				return in;

			}

}
