#!/bin/bash

WORK_DIR= #Output of Filter PPP Result
IQTreePath= #iqtree2 file path iqtree2
SCRIPT_OUTPUT= #Output folder for the scripts
cd $WORK_DIR

FILES=`find . -maxdepth 1 -type f -name '*.fa' -exec basename -a {} + | sed 's/.fa//g'`


for f in $FILES

do
jobname=${f}.sh
(
    echo "#! /bin/bash -l"
    echo "#SBATCH -p medium"
    echo "#SBATCH -t 4:00:00"
    echo "#SBATCH -o Job-Prequal_%J.out"
    echo "#SBATCH -c 1"
    echo "#SBATCH -C scratch"
	echo "#SBATCH --mem 15G"
    echo ""
	echo "module purge"
    echo "module load anaconda3/2021.05"
    echo "source activate Prequal"
    echo ""
    echo "prequal ${WORK_DIR}${f}.fa"
	echo "ginsi --allowshift --unalignlevel 0.8 ${WORK_DIR}${f}.fa.filtered > ${WORK_DIR}${f}.fa.filtered.ginsi"
    echo "${IQTreePath} -fast -nt 1 -s ${WORK_DIR}${f}.fa.filtered.ginsi -st AA -m TEST -msub nuclear -alrt 1000"
	echo "clipkit -m gappy -g 0.75 -o ${WORK_DIR}$f.fa.filtered.ginsi.g075 ${WORK_DIR}${f}.fa.filtered.ginsi"
	echo ""
	echo "mv ${WORK_DIR}${f}.sh ${SCRIPT_OUTPUT}${f}.sh"
	echo ""
	echo "module purge"
	
) > $jobname
done
