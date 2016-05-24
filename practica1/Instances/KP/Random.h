/***********************************
 * Random.h
 * ---------------------------------
 *
 *  Created on: 02/06/2012
 *      Author: Carlos García-Martínez - at University of Córdoba, Spain
 ***********************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This work is distributed in the hope that it will be useful, but      *
 *   without warranty of any kind, either express or implied.              *
 *                                                                         *
 ***************************************************************************/

/*******************************************************************/
/*  Rand genera un numero real pseudoaleatorio entre 0 y 1,        */
/*  excluyendo el 1.						   */
/*  Randint genera un numero entero entre low y high, ambos 	   */
/*  incluidos.	   						   */
/*  Randfloat genera un numero real entre low y high, incluido low */
/*  y no incluido high                                             */
/*******************************************************************/

// Para compilar g++ main.cpp seeds.cpp -lgsl -lgslcblas


#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <cmath>
//#include <iostream>
#include <ctime>
//#include <cstdlib>
#include <gsl/gsl_rng.h>
using namespace std;
#define PI 3.14159265358979323846

extern unsigned int numSeeds;
extern int seeds[];

class Random {
    const gsl_rng_type *T;
    gsl_rng *r;

public:

    //! Constructor

    //crear constructor por defecto leyendo de time
    Random(){
        gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc(T);
        gsl_rng_set(r,clock());
//        std::cerr << "Error al crear Random vaćio" << std::endl;
//        std::exit(1);
    }

    Random(long int seed) {
        gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc(T);
        gsl_rng_set(r, seed);
    }

    Random(Random &another){
        gsl_rng_env_setup();
        T = another.T;
        r = gsl_rng_clone(another.r);
    }

    void copy(Random &another){
        T = another.T;
        gsl_rng_memcpy(r, another.r);
    }

    ~Random(){
        gsl_rng_free(r);
    }

    void cambiaSemilla(long int seed) {
        gsl_rng_set(r,seed);
    }

    double Rand() {
        return gsl_rng_uniform(r);
    }

    /** Randint genera un numero entero entre low y high, ambos incluidos.
     */
    unsigned long Randint(int low, int high) {
        return (gsl_rng_uniform(r) * (high - low) + low + 0.5); // +0.5 para que al truncar tanto low como high estén incluidos
    }

    unsigned long Randlong(long low, long high) {
        return (gsl_rng_uniform(r) * (high - low) + low + 0.5); // +0.5 para que al truncar tanto low como high estén incluidos
    }

    double Randfloat(double low, double high) {
        return (gsl_rng_uniform(r) * (high - low) + low);
    }

    int* RandPerm(int *perm, int n) {

        int i, top;
        top = n - 1;

        for (i = 0; i < n; i++)
            perm[i] = i;

        for (i = 0; i < n; i++){
            int change = this->Randint(0, top);
            int aux = perm[change];
            perm[change] = perm[i];
            perm[i] = aux;
        }

        return (perm);
    }

    long* RandPerm(long *perm, long n) {

        long i, top;
        top = n - 1;

        for (i = 0; i < n; i++)
            perm[i] = i;

        for (i = 0; i < n; i++){
            long change = this->Randlong(0, top);
            long aux = perm[change];
            perm[change] = perm[i];
            perm[i] = aux;
        }

        return (perm);
    }
    
    double Randn() {
        double u = Rand();
        double v = Rand();
        double x = sqrt(-2 * log(u)) * cos(2 * PI * v);
        return x;
        }
};


#endif
