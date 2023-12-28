#!/bin/bash

INPUT_FOLDER= #Change This

cd $INPUT_FOLDER || exit -1


FILES=`ls *.sh`

for f in $FILES

do
(
	sbatch ${f}
)
done

