#!/bin/bash

DIR=$1_`date '+%d%m20%y_%H%M'`
storedir=`pwd`/${DIR}
useCRAB=1
tracktag=ctfPixelLess

if [ ! -d ${DIR} ]; then 
  mkdir ${DIR}
else
  rm -fr ${DIR}/*
fi

cp -f ../../plugins/PixelEfficiency.cc ${DIR}/PixelEfficiency-Sauv
cp -f merge.C ${DIR}/merge.C

#./pythonTemplateGenerator.csh 1 filelistCRAFT09_rePro_SP_RV4_CosmicSeq.txt
#./pythonTemplateGenerator.csh 10 filelistCRAFT09_rePro_TP_RV4_CosmicSeq.txt
./pythonTemplateGenerator.csh 100 filelist123151.txt

let i=0

for file in template*; do
  let i=$i+1
  echo Creating job $i
  sed "s#NUMOFEVENTS#-1#" < $file > temp1
  sed "s#TRACKINPUTTAG0T#${tracktag}#" < temp1 > temp2
  #SuperPointing tracks, ReReco TrackerPointing tracks
  #sed "s#TRACKINPUTTAG#ctfWithMaterialTracksP5#" < temp2 > temp3
  sed "s#TRACKINPUTTAG#${tracktag}#" < temp2 > temp3
  #sed "s#TRACKINPUTTAG#ALCARECOTkAlCosmicsCTF#" < temp2 > temp3
  sed "s#ROOTFILE0T#pixelEfficiency0T$i.root#" < temp3 > temp4
  sed "s#ROOTFILE#pixelEfficiency$i.root#" < temp4 > ${DIR}/pixelefficiency_cfg$i.py

  rm -f temp1 temp2 temp3 temp4

  sed "s#CFGFILE#pixelefficiency_cfg$i.py#" < submit_template.csh > temp
  sed "s#STOREDIR#${storedir}#" < temp > ${DIR}/submit_$i.csh
  rm -fr temp

  sed "s#DIRECTORY#${storedir}#" < crab_template.cfg > ${DIR}/crab.cfg

  cd $DIR

  if [  ${useCRAB} -ne "1" ]; then
    bsub -q cmscaf1nd -J job_$i < submit_$i.csh
  fi

  cd ..
done

rm -f template*

cd ${DIR}

if [  ${useCRAB} -eq "1" ]; then
  echo "Submitting the crab job. Please wait ..."
  crab -create -submit
fi

cd ..

