#!/bin/bash

for i in `seq 0 .1 1`;
do
	for j in `seq 0 .1 1`;
	do
		./size -qr3 -n2 $i $j;
	done;
	echo "";
done
