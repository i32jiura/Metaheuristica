/**
* @file Tiempo.hpp
* @brief clase Tiempo para poder medir tiempo transcurrido
* @note: compilar con g++ -Wall main.cpp -lrt
*/
#ifndef TIEMPO_HPP
#define TIEMPO_HPP

#include <cassert>
#include <ctime>
#include <cstdio>
#include <cstring> 				//Para usar memset
#include <iostream>
#include <stdint.h>				//Para usar uint64_t

 /** @brief Clase Reloj.*/
  class Reloj {

		private:

			timespec _iniciar;	//!< variable para guardar hora de inicio
   			timespec _parar;	//!< variable para guardar hora de parada
   			bool _estaIniciado;	//!< variable que almacena el estado actual del reloj

  		public:

		/** @CONSTRUCTORES */
		/** @{ */

		/**
		* @fn Reloj()
		* @brief constructor de la clase Reloj
		*/
		Reloj () {

			 memset(&_iniciar,0,sizeof(timespec));
    		 memset(&_parar,0,sizeof(timespec));
     		_estaIniciado = false;

		}
		/** @} */

		/** @FUNCIONES */
		/** @{ */

   		/**
		* @fn iniciar()
		* @brief inicia el reloj
     	* @pre not estaIniciado()
     	* @post estaIniciado()
     	*/
    	void iniciar () {

			assert (!estaIniciado());
      		clock_gettime (CLOCK_REALTIME, &_iniciar);
      		_estaIniciado = true;
    	}

		/**
		* @fn reiniciar()
   		* @brief reinicia el reloj
    	* @post estaIniciado()
     	*/
   		 void reiniciar () {

    		clock_gettime (CLOCK_REALTIME, &_iniciar);
     		_estaIniciado = true;

   		 }

		/**
		* @fn parar()
   		* @brief para el reloj
     	* @pre estaIniciado()
     	* @post not estaIniciado()
     	*/
    	void parar () {

			assert (_estaIniciado);
      		clock_gettime (CLOCK_REALTIME, &_parar);
      		_estaIniciado = false;

    	}

		/**
		* @fn estaIniciado()
	    * @brief comprueba si el reloj esta iniciado
     	* @return true si el reloj se encuentra iniciado actualmente
     	*/
    	inline bool estaIniciado() const { return _estaIniciado; }

		/**
		* @fn transcurrido()
   		* @brief devuelve el tiempo transcurrido en mcs.
		* @pre not _estaIniciado
		* @return fin-inicio
		*/
   		uint64_t transcurrido() const {

			assert (!_estaIniciado);
		  	uint64_t inicio = (uint64_t)_iniciar.tv_sec * 1000000LL + (uint64_t)_iniciar.tv_nsec / 1000LL;
		  	uint64_t fin = (uint64_t)_parar.tv_sec * 1000000LL + (uint64_t)_parar.tv_nsec / 1000LL;

			return fin-inicio;
   		 }
		/** @} */
  };

#endif
