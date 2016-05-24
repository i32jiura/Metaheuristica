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
  std::vector<int> dentro;
  double coste,peso;
  bool dentro,esPadre;
  
public:
	
  Solucion (){
	  coste = 0;
	  peso=0;
	  dentro=false;
	  esPadre=false;
  };
  
  //NUEVO CONSTRUCTOR
  Solucion (int tamVector,int tamDentro) {
	  coste = 0;
	  peso = 0;
	  dentro.resize(tamDentro);
	  valores.resize(tamVector);
	  dentro=false;
	  esPadre=false;
  }

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

  void SetPeso(const double &peso)
  {
  	this->peso = peso;
  }

  const double & GetPeso() const
  {
  	return peso;
  }

  void SetDentro(bool dentro)
  {
  	this->dentro = dentro;
  }

  const bool& IsDentro() const
  {
  	return dentro;
  }

  void SetEsPadre(bool esPadre)
  {
  	this->esPadre = esPadre;
  }

  bool IsEsPadre() const
  {
  	return esPadre;
  }

  void SetDentro(std::vector<int> & dentro) {
  	this->dentro = dentro;
  }

  std::vector<int> & GetDentro() {
  	return dentro;
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

