#!/bin/bash
if [ $# -ne 4 ]
then
	echo "Error de formato:"
	echo "$0 <fichero entrada_1> <fichero salida> <etiqueta_X> <etiqueta_Y>"
	echo $#
	exit
fi

FICHERO_ENTRADA_1="'$1'"
#FICHERO_ENTRADA_2="'$2'"
FICHERO_SALIDA="'$2'"
ETIQUETA_X="'$3'"
ETIQUETA_Y="'$4'"

cat << _end_ | gnuplot
set terminal postscript eps color
set output $FICHERO_SALIDA
set key top right
set xlabel $ETIQUETA_X
set ylabel $ETIQUETA_Y
plot $FICHERO_ENTRADA_1 using 1:2 t "TSP genetico" w l #,$FICHERO_ENTRADA_1 using 1:2 t "Greedy Iterativo" w l
_end_
