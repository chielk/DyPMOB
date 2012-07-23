#!/bin/bash

for i in `seq .0 .1 1`;
do
	for j in `seq .0 .1 1`;
	do
		echo $i $j
		./size -qr3 -n2 $i $j;
	done;
	echo "";
done
