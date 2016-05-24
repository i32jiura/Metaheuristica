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
set key top right
set xlabel "repeticiones"
set ylabel "Beneficio"
plot $FICHERO_ENTRADA using 1:2 t "Prueba1" w l , $FICHERO_ENTRADA using 1:3 t "Prueba2" w l , $FICHERO_ENTRADA using 1:4 t "Prueba3" w l , $FICHERO_ENTRADA using 1:5 t "Prueba4" w l , $FICHERO_ENTRADA using 1:6 t "Prueba5"  w l
_end_
