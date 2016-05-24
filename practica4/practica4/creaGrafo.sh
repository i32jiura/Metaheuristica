#!/bin/bash
if [ $# -ne 2 ]
then
	echo "Error de formato:"
	echo "$0 <fichero entrada> <fichero salida>"
	echo $#
	exit
fi

FICHERO_ENTRADA="'$1'"

FICHERO_SALIDA="'$2'"


cat << _end_ | gnuplot
set terminal postscript eps color
set output $FICHERO_SALIDA
set key top right

plot $FICHERO_ENTRADA using 1:2 t 'Camino:' w l, $FICHERO_ENTRADA using 1:2 t 'Nodo:' w p lt 7
_end_
