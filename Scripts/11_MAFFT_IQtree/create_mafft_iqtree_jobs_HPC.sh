#!/bin/bash

WORK_DIR= #Folder output OSG
IQTreePath= #Path to program IQtree2
SCRIPT_OUTPUT= #Folder where Scripts are moved to
cd $WORK_DIR

FILES=`find . -maxdepth 1 -type f -name '*.fa' -exec basename -a {} + | sed 's/.fa//g'`


for f in $FILES

do
jobname=${f}.sh
(
    echo "#!/bin/bash -l"
    echo "#SBATCH -p medium"
    echo "#SBATCH -t 48:00:00"
    echo "#SBATCH -c 1"
    echo "#SBATCH -C scratch"
	echo "#SBATCH --mem 20G"
    echo ""
	echo "module purge"
    echo "module load mafft"
    echo ""
    echo "mafft --thread 1 --maxiterate 1000 --globalpair ${WORK_DIR}/$f.fa > ${WORK_DIR}/$f.mafft"
	echo ""
    echo "${IQTreePath} -fast -nt 1 -s ${WORK_DIR}/$f.mafft -st AA -m TEST -alrt 1000"
	echo ""
	echo ""
	echo "mv ${WORK_DIR}/${f}.sh ${SCRIPT_OUTPUT}/${f}.sh"
	echo ""
	echo "module purge"
	
) > $jobname
done
