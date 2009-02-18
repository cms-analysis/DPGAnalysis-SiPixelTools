#!/bin/bash

DIR=$1
storedir=`pwd`/${DIR}

if [ ! -d ${DIR} ]; then 
  mkdir ${DIR}
else
  rm -fr ${DIR}/*
fi

#./pythonTemplateGenerator.csh 50 SMALLfilelistTrackerPointingReReco.txt
#./pythonTemplateGenerator.csh 30 filelistTrackerPointingReReco.txt
#./pythonTemplateGenerator.csh 10 filelistSuperPointingMagON.txt
#./pythonTemplateGenerator.csh 100 filelistAlcaReco.txt
#./pythonTemplateGenerator.csh 100 filelistAlCaSkimmed.txt
#./pythonTemplateGenerator.csh 200 filelistSuperPointing.txt
#./pythonTemplateGenerator.csh 3 filelistSuperPointingV9.txt
#./pythonTemplateGenerator.csh 15 filelistMC.txt
#./pythonTemplateGenerator.csh 20 filelistTrackerPointingV9.txt
./pythonTemplateGenerator.csh  4 filelistMCSuperPointing.txt

let i=0

for file in template*; do
let i=$i+1
echo Creating job $i
sed "s#NUMOFEVENTS#-1#" < $file > temp1
sed "s#TRACKINPUTTAG0T#ctfWithMaterialTracksP5#" < temp1 > temp2
#SuperPointing tracks, ReReco TrackerPointing tracks
sed "s#TRACKINPUTTAG#ctfWithMaterialTracksP5#" < temp2 > temp3
#sed "s#TRACKINPUTTAG#ALCARECOTkAlCosmicsCTF#" < temp2 > temp3
sed "s#ROOTFILE0T#pixelEfficiency0T$i.root#" < temp3 > temp4
sed "s#ROOTFILE#pixelEfficiency$i.root#" < temp4 > ${DIR}/pixelefficiency_cfg$i.py

rm -f temp1 temp2 temp3 temp4

sed "s#CFGFILE#pixelefficiency_cfg$i.py#" < submit_template.csh > temp
sed "s#STOREDIR#${storedir}#" < temp > ${DIR}/submit_$i.csh
rm -fr temp

cd $DIR

bsub -q cmscaf -J job_$i < submit_$i.csh

cd ..
done

rm -f template*


