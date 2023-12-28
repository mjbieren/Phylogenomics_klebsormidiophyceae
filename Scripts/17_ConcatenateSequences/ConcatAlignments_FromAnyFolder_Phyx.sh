#!/bin/bash

INPUT_FOLDER= #InputFolder with all the files #Change This
OUTPUT_File= #Output file path+name
FILE_EXTENSION=fas #can also be .fa, it's up to you

#Folder
cd $INPUT_FOLDER
pxcat -s *.${FILE_EXTENSION} -p ${OUTPUT_File}.output_partition_file -o ${OUTPUT_File}.${FILE_EXTENSION}
