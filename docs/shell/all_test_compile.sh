#!/bin/bash

for nation in tld brz mex rus ger frc pld spn ita uk usa
do
	make cleanall && make ${nation} || exit 1;
done

