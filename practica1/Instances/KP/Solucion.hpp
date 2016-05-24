#include <InstanceKp>
#include <vector>

class Solucion{


	private:
		vector<Material> materiales;
	public:
		inline void SetMateriales (const vector<Material> &materiales){ this->materiales = materiales;}
		inline vector<Material> GetMateriales () const { return materiales; }
		
		bool operator==(std::vector<Material> &materiales1, std::vector<Material> &materiales2) {

			for (unsigned int i=0;i<Materiales1.size();i++)
				if ( !(materiales1[i].esSolucion and Materiales2[i].esSolucion))
					return false;

			return true;

		}
}