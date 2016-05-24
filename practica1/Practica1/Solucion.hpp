/* 
 * File:   Solucion.hpp
 * Author: jesus
 *
 * Created on 3 de marzo de 2016, 14:46
 */

#ifndef SOLUCION_HPP
#define	SOLUCION_HPP
#include <vector>

class Solucion
{
private:
  std::vector<int> valores;
  double coste;
  
public:
  Solucion (){
	  coste = 0;
  };
  Solucion (const Solucion& orig);

  void SetCoste (const double &coste)
  {
    this->coste = coste;
  }

  const double & GetCoste () const
  {
    return coste;
  }

  void SetValores (const std::vector<int> &valores)
  {
    this->valores = valores;
  }

  const std::vector<int> &GetValores () const
  {
    return valores;
  }
  
  std::vector<int> &GetValores ()
  {
    return valores;
  }
  
	friend bool operator==(std::vector<int> &valores1,std::vector<int> &valores2) {
		unsigned int tam= valores1.size();
		for (unsigned int i=0;i<tam;i++)
			if (valores1[i] != valores2[i])
				return false;

		return true;
	}
	
	
  
};

#endif	/* SOLUCION_HPP */

