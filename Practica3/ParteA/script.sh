#!/bin/bash
if [ "$1" = "prod" ]
then
	echo "En 3 segundos será lanzado el productor"
	sleep 3
	echo "Productor lanzado!"
	for i in {1..150}
		do
			echo add $i > /proc/modlist
			echo "Se ha introducido $i en /proc/modlist"
			sleep 0.5
		done
elif [ "$1" = "cons" ]
then
	for i in {1..150}
		do
			echo remove $i > /proc/modlist
			cat /proc/modlist
			sleep 0.5
		done
fi
