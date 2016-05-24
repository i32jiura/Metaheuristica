#!/bin/bash

rm Medias$1.csv

for fichero in *_$1.csv
do
	echo $(cat $fichero | tail -1) >> Medias$1.csv
done
