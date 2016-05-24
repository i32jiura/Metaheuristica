#!/bin/bash
if [ $# -ne 2 ]
then
	echo "Error de formato:"
	echo "$0 <fichero entrada> <fichero salida>"
	exit
fi

FICHERO_ENTRADA="'$1'"
FICHERO_SALIDA="'$2'"

cat << _end_ | gnuplot
set terminal postscript eps color
set output $FICHERO_SALIDA
set key bottom right
set xlabel "repeticiones"
set ylabel "fitness"
plot $FICHERO_ENTRADA using 1:2 t "Fitness prueba1" w l , $FICHERO_ENTRADA using 1:3 t "Fitness prueba2" w l , $FICHERO_ENTRADA using 1:4 t "Fitness prueba3" w l , $FICHERO_ENTRADA using 1:5 t "Fitness prueba4" w l , $FICHERO_ENTRADA using 1:6 t "Fitness prueba5"  w l
_end_
