#!/bin/bash

########################
##
##   Script to run the gain calibration on all 40 FEDs
##
##
###### GAIN CALIBRATION PARAMETERS were passed by a DB before,
## now they are defined in ./CalibTracker/SiPixelGainCalibration/python/SiPixelGainCalibrationAnalysis_cfi.py,
### and passed by the  $runningdir/Run_offline_DQM_${i}_cfg.py to the main macro.To overwrite them just change the parmeters in write_calib_parameters().
########################

#################
# Sourcing castor/T2 utilities
source utils.sh

#################
# Set the extension for input root files

ext=dmp


usage(){

echo 'Usage :
./Run.sh -create     RUNNUMBER INPUTDIR STOREDIR : will create the needed directories, python files to run the calib.
     OR  -create     RUNNUMBER : default INPUTDIR=/castor/cern.ch/user/U/USER/GainCalib_runXXXXX & STOREDIR=/castor/cern.ch/user/U/USER/.
./Run.sh -submit     RUNNUMBER : will launch the 40 calibration jobs
./Run.sh -resubmit   RUNNUMBER iJOB: will resubmit job iJOB , using the submit_iJOB.sh in the working directory.
./Run.sh -stage      RUNNUMBER : will stage all files needed that are stored on castor.
./Run.sh -status     RUNNUMBER : will check the status of the calib jobs
./Run.sh -hadd       RUNNUMBER : will hadd all 40 output files of the calib jobs into one file
./Run.sh -summary    RUNNUMBER : will launch the summary job.
./Run.sh -pdf        RUNNUMBER : will recompile the latex file to recreate the pdf summary.
./Run.sh -compare    RUNNUMBER1 FILE1 RUNNUMBER2 FILE2
     OR  -compare    RUNNUMBER1 RUNNUMBER2 : only if you have run -create/-submit/-hadd for both runs
./Run.sh -payload    RUNNUMBER : will produce the payloads.
./Run.sh -twiki      RUNNUMBER : will produce the text to add to the twiki.
./Run.sh -comp_twiki RUNNUMBER : will produce the text to add to the twiki for all the comparisons with this run.
./Run.sh -info       RUNNUMBER : will output info on the run.
'
exit


}


create(){
  if [ "$indir" == "" ] && [ "$storedir" == "" ];then
    #indir=/castor/cern.ch/user/${USER:0:1}/$USER/GainCalib_run$run
    #storedir=/castor/cern.ch/user/${USER:0:1}/$USER/
    indir=/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Run_$run
    storedir=/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Run_$run
  fi
  
  storedir=$storedir/GainRun_$run
  echo "Setting parameters: "
  echo "run : $run"
  echo "indir : $indir"
  echo "storedir : $storedir"
  if [ "$run" == "" ] || [ "$indir" == "" ] || [ "$storedir" == "" ];then usage ; fi
  runningdir=`pwd`/Run_$run
  
  #making running directory
  echo "Creating working directory, test input/output locations"
  make_dir ${runningdir}
  
  #cleaning
  rm -f filelist.txt es.log

  #cleaning output dir
  set_specifics ${storedir}
  $T2_RM${storedir}
  $T2_MKDIR${storedir}
  $T2_CHMOD${storedir}
  
  #chmod -R 0777 $indir
  set_specifics $indir
  if [ `is_on_castor $indir` -eq 1 ] ; then wait_for_staging ; fi
  
  echo "Creating config files"
  echo "run = $run" 		>  $runningdir/config
  echo "indir = $indir" 	>> $runningdir/config
  echo "storedir = $storedir" 	>> $runningdir/config
  
  for i in "${PIXFEDarray[@]}";do
    file=GainCalibration_${i}_$run.$ext
    if [ `is_file_present $indir/$file` -eq 0 ];then echo "File $file is not present in $indir ...";continue;fi
    sed "s#FILENAME#file:$file#" gain_calib_template_cfg.py > $runningdir/gain_calib_${i}_cfg.py
  done
  echo "Done creating task directory: Run_$run"
}

read_config(){
  config=Run_$run/config
  if [ ! -f $config ];then echo "No config found for run $run. Make sure Run_$run exist in `pwd` ..." ; exit ;fi
  indir=`cat $config|grep -e "indir ="|awk '{printf $3}'`
  storedir=`cat $config|grep -e "storedir ="|awk '{printf $3}'`
  calib_payload=`cat $config|grep -e "calib_payload ="|awk '{printf $3}'`
  if [ "$calib_payload" == "" ];then calib_payload='none';fi
  echo -e "Reading config file $config"
  echo -e "run : $run"
  echo -e "indir : $indir"
  echo -e "storedir : $storedir"
  echo -e "calib payload : $calib_payload\n"
  runningdir=`pwd`/Run_$run
}

make_dir(){
  if [ ! -d $1 ] ; then mkdir $1
  else rm -fr $1/*
  fi
}

lock(){
  if [ -f .lock_gaincalib ];then
    echo "Another instance of Run.sh is already running, so wait for it to finish !"
    echo "In case it is really not the case (like you previously killed it), remove the file \".lock_gaincalib\""
    exit
  else
    touch .lock_gaincalib
  fi
}

wait_for_staging(){
  echo -e "Files are on castor.\nStaging =====>"
  get_done=0
  need_to_wait=1
  while [ $need_to_wait -eq 1 ];do
    need_to_wait=0
    for i in "${PIXFEDarray[@]}";do
      file=GainCalibration_${i}_$run.$ext
      if [ `is_file_present $indir/$file` -eq 0 ];then echo "File $file is not present in $indir ...";continue;fi	 
      stager_qry -M $indir/$file
      if [ `is_staged $indir/$file` -eq 0 ];then
        need_to_wait=1
	if [ $get_done -eq 1 ] ; then break ; fi
      fi
    done
    get_done=1
    if [ $need_to_wait -eq 1 ];then
      echo "At least one file is not staged. Will sleep for 5min before trying again ..."
      sleep 300
    fi
  done
  echo -e "Staging is finished !"
}

submit_calib(){
  set_specifics $indir
  sed "s;INDIR;$indir;;s;RUN;$run;;s;.EXT;.$ext;;s;STOREDIR;${storedir};" submit_template.sh |\
      sed "s;T2_CP;${T2_CP};;s;T2_TMP_DIR;${T2_TMP_DIR};;s;CFGDIR;${runningdir};;s;T2_PREFIX;${T2_PREFIX};" > ${runningdir}/submit_template.sh
  set_specifics $storedir
  sed -i "s#T2_OUT_CP#${T2_CP}#" ${runningdir}/submit_template.sh
  cd $runningdir
  echo "pwd  "`pwd`
  echo "Run "${run}
  for i in "${PIXFEDarray[@]}";
  do
      if ls ${runningdir}/gain_calib_${i}_cfg.py > /dev/null 2>&1;
	  then
	  make_dir ${runningdir}/JOB_${i}
	  rm -f submit_${i}.sh
	  sed "s/NUM/${i}/" submit_template.sh > submit_${i}.sh
	  submit_to_queue ${run}_${i} ${runningdir}/JOB_${i}/stdout submit_${i}.sh
      else
	  echo "Could not find: gain_calib_${i}_cfg.py"
      fi
  done
  
  cd -
}

resubmit_job(){
  set_specifics $storedir 
  
  if [ `is_file_present $storedir/$ijob.root` -eq 1 ];then
    echo "Output of job $ijob is already in $storedir."
    exit
  fi
  
  echo "Re-submitting job $ijob:"
  submit_to_queue ${run}_${ijob} ${runningdir}/JOB_${ijob}/stdout submit_${ijob}.sh
}

resubmit_all(){
  set_specifics $storedir 
  cd $runningdir
  nothing_to_resubmit=true
  for ijob in "${PIXFEDarray[@]}";do
    if [ `is_file_present $storedir/$ijob.root` -eq 0 ];then
	nothing_to_resubmit=false
	resubmit_job
    fi  
  done

  if [ "$nothing_to_submit" = true ];
  then
      echo "Nothing to resubmit."
  fi

  cd -
}

stage_all_files(){
  set_specifics $indir
  f_to_stage=''
  if [ `is_on_castor $indir` -eq 1 ];then
    for file in `nsls $indir`;do
      f_to_stage="$f_to_stage $indir/$file"
    done
  fi
  if [ `is_on_castor $storedir` -eq 1 ];then
    for file in `nsls $storedir`;do
      f_to_stage="$f_to_stage $storedir/$file"
    done
  fi
  
  if [ `is_on_castor $indir` -eq 1 ] || [ `is_on_castor $storedir` -eq 1 ];then
    stage_list_of_files $f_to_stage
  else
    echo "Nothing to stage ..."
  fi
}

status(){
  cmsLs $storedir | grep ".root" | sed 's;/; ;g;s;.root;;' | awk '{ print $NF }' | sort -n > done.txt
  bjobs | grep $run | sed 's;_; ;' | awk '{ print $(NF-3) }' | sort -n > running.txt
  touch missing.txt
  for n in `seq 0 39`; do
    if [ `grep '^'$n'$' done.txt | wc -l` == 0 ] && [ `grep '^'$n'$' running.txt | wc -l` == 0 ]; then
      echo $n >> missing.txt
    fi
  done
  echo "jobs with output in: $storedir"
  cat done.txt | tr '\n' ' '; echo
  echo "jobs running on lxbatch:"
  cat running.txt | tr '\n' ' ' ; echo
  echo "missing:"
  cat missing.txt | tr '\n' ' ' ; echo
  echo "resubmit:"
  cat missing.txt | awk '{ print "./Run.sh -resubmit '$run' "$NF }'
  rm done.txt running.txt missing.txt
}

hadd_files(){
  /afs/cern.ch/project/eos/installation/cms/bin/eos.select -b fuse mount /tmp/`whoami`/eos
  dir="/tmp/`whoami`/eos/cms$storedir"
  rm -f $dir/GainCalibration.root
  hadd -f $dir/GainCalibration.root $dir/*.root
  /afs/cern.ch/project/eos/installation/cms/bin/eos.select -b fuse umount /tmp/`whoami`/eos
}

submit_summary_new(){

  set_specifics ${storedir}
  if [ `$T2_LS  $storedir/GainCalibration.root 2>&1|grep "No such"|wc -l` -eq 1 ]; then
    echo "File $storedir/GainCalibration.root is not present ..."; exit ; fi ;
  stage_list_of_files $storedir/GainCalibration.root

  #making directories
  make_dir ${runningdir}/TEXToutput
  make_dir $runningdir/Summary_Run$run
  
  cp -fr scripts/make_ComparisonPlots.cc ${runningdir}/Summary_Run$run/make_ComparisonPlots.cc
  cp -fr scripts/gain_summary.txt  ${runningdir}/Summary_Run$run/gain_summary_template.tex
  cp -fr scripts/TMean.* ${runningdir}/Summary_Run$run/.
  cp -fr scripts/PixelNameTranslator.* ${runningdir}/Summary_Run$run/.
  cp -fr scripts/header.h scripts/functions.C scripts/containers.h scripts/hist_declarations.C ${runningdir}/Summary_Run$run/.
  
  cd $runningdir/Summary_Run$run

  rm -fr gain_summary.tex
  #sed "s#RUNNUMBER#$run#" < make_SummaryPlots_template.cc > make_SummaryPlots.cc
  cat gain_summary_template.tex | sed "s#RUNNUMBER#$run#" | sed "s#DIFF_TO_REPLACE#0#" > gain_summary.tex
  #rm -fr gain_summary_template.tex make_SummaryPlots_template.cc

  #rm -fr $T2_TMP_DIR/*
  #$T2_CP `file_loc $storedir/GainCalibration.root` $T2_TMP_DIR/GainCalibration.root
  echo "(root -l -b -x make_ComparisonPlots.cc+\"(\"`file_loc $storedir/GainCalibration.root`\",\"$run\")\" -q)"
  root -l -b -x make_ComparisonPlots.cc+"(\"`file_loc $storedir/GainCalibration.root`\",\"$run\")" -q
  echo -e "\n************************* SUMMARY"
  cat *.txt
  echo -e "\nlog files: \n"`ls *.log|sed "s:^:   --> Run_$run/Summary_Run$run/:"`"\n"

  rm -f gain_summary_final_run_$run.tex
  sed '/TOREPLACE/,$ d' < gain_summary.tex > gain_summary_final_run_$run.tex
  cat texSummary_Run${run}.tex >> gain_summary_final_run_$run.tex
  sed '1,/TOREPLACE/d'< gain_summary.tex >> gain_summary_final_run_$run.tex

  echo "Making pdf ..."
  pdflatex gain_summary_final_run_$run.tex &>  latex.log
  pdflatex gain_summary_final_run_$run.tex >> latex.log 2>&1
  if [ ! -f gain_summary_final_run_$run.pdf ];then cat latex.log;fi
  
  echo -e "\nPDF file:\n `pwd`/gain_summary_final_run_$run.pdf"
}


compile_pdf(){

  cp -fr scripts/gain_summary.txt  ${runningdir}/Summary_Run$run/gain_summary_template.tex
  cd $runningdir/Summary_Run$run
  
  rm -fr gain_summary.tex
  cat gain_summary_template.tex | sed "s#RUNNUMBER#$run#" | sed "s#DIFF_TO_REPLACE#0#" > gain_summary.tex

  rm -f gain_summary_final_run_$run.tex
  sed '/TOREPLACE/,$ d' < gain_summary.tex > gain_summary_final_run_$run.tex
  cat texSummary_Run${run}.tex >> gain_summary_final_run_$run.tex
  sed '1,/TOREPLACE/d'< gain_summary.tex >> gain_summary_final_run_$run.tex

  echo "Making pdf ..."
  pdflatex gain_summary_final_run_$run.tex &>  latex.log
  pdflatex gain_summary_final_run_$run.tex >> latex.log 2>&1
  if [ ! -f gain_summary_final_run_$run.pdf ];then cat latex.log;fi
  echo -e "\nPDF file:\n `pwd`/gain_summary_final_run_$run.pdf"
}

set_files_for_comparison(){
  if [ "$run2" == "" ] && [ "$file2" == "" ];then
    run2=$file1
  
    run=$run1
    read_config
    file1=$storedir/GainCalibration.root
  
    run=$run2
    read_config
    file2=$storedir/GainCalibration.root  
  fi
  
  echo -e "\n-----------------------------------------------------------"
  echo "Comparing run $run1 & run $run2"
  echo "File for run $run1: $file1"
  echo "File for run $run2: $file2"
  echo -e "-----------------------------------------------------------\n"
  
  
}


compare_runs(){

  #echo t $run1 t $file1 t $run2 t $file2

  if [ "$run1" == "0" ] || [ "$run2" == "0" ] || [ "$file1" == "" ] || [ "$file2" == "" ];then usage ; fi

  if [ $run1 -gt $run2 ];then echo "Warning !! Inverted runnumbers: you should put the newest run at the end ..."; fi

  stage_list_of_files $file1 $file2
  
  dir=Comp_${run1}-${run2}
  make_dir $dir

  set_specifics $file1
  if [ `$T2_LS $file1 2>&1|grep "No such"|wc -l` -eq 1 ]; then
    echo "File $file1 is not present ..."; exit ; fi ;
  file1=${T2_FSYS}${file1}

  set_specifics $file2
  if [ `$T2_LS $file2 2>&1|grep "No such"|wc -l` -eq 1 ]; then
    echo "File $file2 is not present ..."; exit ; fi ;
  file2=${T2_FSYS}${file2}

  #cat scripts/make_ComparisonPlots.cc |\
  #  sed "s#RUNNUMBER1#${run1}#" |\
  #  sed "s#RUNNUMBER2#${run2}#" > $dir/make_ComparisonPlots.cc
  
  cp -fr scripts/make_ComparisonPlots.cc $dir/.
  cp -fr scripts/TMean.* $dir/.
  cp -fr scripts/PixelNameTranslator.* $dir/.
  cp -fr scripts/header.h scripts/functions.C scripts/containers.h scripts/hist_declarations.C $dir/.
  
  cd $dir
  
  echo "( root -l -b -q make_ComparisonPlots.cc+\"(\"$file1\",\"$run1\",\"$file2\",\"$run2\")\" )"
  root -l -b -q make_ComparisonPlots.cc+"(\"$file1\",\"$run1\",\"$file2\",\"$run2\")"
  echo -e "\n************************* SUMMARY"
  cat *.txt
  echo -e "\nlog files: \n"`ls *.log|sed "s:^:   --> $dir/:"`"\n"
  
  cp -fr ../scripts/gain_summary.txt  gain_summary_template.tex
  rm -fr gain_summary.tex
  cat gain_summary_template.tex | sed "s#RUNNUMBER#$run1-$run2#" | sed "s#DIFF_TO_REPLACE#1#" > gain_summary.tex

  rm -f gain_summary_final_$run1-$run2.tex
  sed '/TOREPLACE/,$ d' < gain_summary.tex > gain_summary_final_$run1-$run2.tex
  cat texSummary_Run${run1}-$run2.tex >> gain_summary_final_$run1-$run2.tex
  sed '1,/TOREPLACE/d'< gain_summary.tex >> gain_summary_final_$run1-$run2.tex

  pdflatex gain_summary_final_$run1-$run2.tex &>  latex.log
  pdflatex gain_summary_final_$run1-$run2.tex >> latex.log 2>&1
  if [ ! -f gain_summary_final_$run1-$run2.pdf ];then cat latex.log;fi
  echo -e "\nPDF file:\n `pwd`/gain_summary_final_$run1-$run2.pdf"
}




make_payload(){

  if [ -z $db_version ] || [ -z $year ];
  then echo -e "\e[31mYou must provide a year AND a version!\n  e.g.: ./Run.sh -payload RUNNUMBER YEAR VERSION --> SiPixelGainCalibration_YEAR_vVERSION_offline\n e.g.: ./Run.sh -payload 265850 2016 1\e[0m";exit
  fi

  set_specifics $storedir
  stage_list_of_files $storedir/GainCalibration.root
  
  file=$T2_TMP_DIR/GainCalibration_$run.root
  echo "Copying $storedir/GainCalibration.root to $file"
  $T2_CP $T2_PREFIX$storedir/GainCalibration.root $file
  
  ###########################################   OFFLINE PAYLOAD
  
  payload=SiPixelGainCalibration_${year}_v${db_version}_offline.db
  payload_root=Summary_payload_Run${run}_${year}_v${db_version}.root
  
  echo -e "RM: $T2_RM$storedir/$payload"
  echo -e "RM: $T2_RM$storedir/$payload_root"
  
  #Changing some parameters in the python file:
  cat SiPixelGainCalibrationDBUploader_cfg.py |\
    sed "s#file:///tmp/rougny/test.root#`file_loc $file`#"  |\
    sed "s#sqlite_file:dummy.db#sqlite_file:$T2_TMP_DIR/${payload}#" |\
    sed "s#/tmp/rougny/histos.root#$T2_TMP_DIR/$payload_root#" |\
    sed "s#GainCalib_TEST_offline#SiPixelGainCalibration_${year}_v${db_version}_offline#" > SiPixelGainCalibrationDBUploader_Offline_cfg.py
    
    
  echo -e "\n--------------------------------------"
  echo "Making the payload for offline:"
  echo "  $storedir/$payload"
  echo "  ==> Summary root file: $payload_root"
  echo -e "--------------------------------------\n"
  
  echo "  (\" cmsRun SiPixelGainCalibrationDBUploader_Offline_cfg.py \" )"
  cmsRun SiPixelGainCalibrationDBUploader_Offline_cfg.py
  
  echo " finish SiPixelGainCalibrationDBUploader_Offline_cfg.py "

  $T2_CP $T2_TMP_DIR/$payload $T2_PREFIX$storedir/$payload
  $T2_CP $T2_TMP_DIR/$payload_root $T2_PREFIX$storedir/$payload_root
  
  echo "Copying   $T2_CP $T2_TMP_DIR/$payload $T2_PREFIX$storedir/$payload "
  echo "Copying   $T2_CP $T2_TMP_DIR/$payload_root $T2_PREFIX$storedir/$payload_root "
 
  rm -f $T2_TMP_DIR/${payload}
  rm -f $T2_TMP_DIR/${payload_root}
  
  echo "removing  $T2_TMP_DIR/${payload} "
  echo "removing  $T2_TMP_DIR/${payload_root} "
  ###########################################   HLT PAYLOAD
  
  payload=SiPixelGainCalibration_${year}_v${db_version}_HLT.db
  payload_root=Summary_payload_Run${run}_${year}_v${db_version}_HLT.root
  
  echo -e "RM: `$T2_RM$storedir/$payload`"
  echo -e "RM: `$T2_RM$storedir/$payload_root`"
  
  #Changing some parameters in the python file:
  cat SiPixelGainCalibrationDBUploader_cfg.py |\
    sed "s#file:///tmp/rougny/test.root#`file_loc $file`#"  |\
    sed "s#sqlite_file:dummy.db#sqlite_file:$T2_TMP_DIR/${payload}#" |\
    sed "s#/tmp/rougny/histos.root#$T2_TMP_DIR/$payload_root#" |\
    sed "s#cms.Path(process.gainDBOffline)#cms.Path(process.gainDBHLT)#"|\
    sed "s#record = cms.string('SiPixelGainCalibrationOfflineRcd')#record = cms.string('SiPixelGainCalibrationForHLTRcd')#"|\
    sed "s#GainCalib_TEST#SiPixelGainCalibration_${year}_v${db_version}_HLT#" > SiPixelGainCalibrationDBUploader_HLT_cfg.py
  
  echo -e "\n--------------------------------------"
  echo "Making the payload for HLT:"
  echo "  $storedir/$payload"
  echo "  ==> Summary root file: $payload_root"
  echo -e "--------------------------------------\n"
  
  echo "  (\" cmsRun SiPixelGainCalibrationDBUploader_HLT_cfg.py \" )"
  cmsRun SiPixelGainCalibrationDBUploader_HLT_cfg.py
  
  ls $T2_TMP_DIR
  $T2_CP $T2_TMP_DIR/$payload $T2_PREFIX$storedir/$payload
  $T2_CP $T2_TMP_DIR/$payload_root $T2_PREFIX$storedir/$payload_root
  
  rm -f $T2_TMP_DIR/${payload}
  rm -f $T2_TMP_DIR/${payload_root}
  
  ####################################
   
  rm -f $file  
}


print_twiki_text(){

echo '---++++ Run '$run'
   * DMP Files : '$indir'
   * Merged Analyzed File : '$storedir'/GainCalibration.root
   * PDF summary : [[%ATTACHURL%/gain_summary_final_run_'$run'.pdf][gain_summary_final_run_'$run'.pdf]]
   * TAR file containing PDF + root + figs PNGs + logs : [[%ATTACHURL%/run'$run'.gz.tar][run'$run'.gz.tar]]
   * Payloads 31X & summaries: '$storedir

  cd $runningdir
  mkdir -p ZIP/figs
  cp Summary_Run$run/*.png ZIP/figs/.
  cp Summary_Run$run/gain_summary_final_run_$run.pdf ZIP/.
  cp Summary_Run$run/*Run$run.log ZIP/.
  cp Summary_Run$run/Comp_Run$run.root ZIP/Summary_Run$run.root
  
  mv ZIP summary_run$run
  zip_file=run$run.gz.tar
  echo -e "\nMaking Run_$run/$zip_file ...\n"
  tar czf $zip_file summary_run$run
  rm -fr summary_run$run
  
  echo -e "  To get it, please issue:\nscp $USER@lxplus.cern.ch:`pwd`/$zip_file .\ntar xzf $zip_file\n"
}

print_comp_twiki_text(){

  for dir in `ls -d Comp*$run*`;do
    run1=`echo $dir|sed 's:Comp_::'|awk -F"-" '{print $1}'`
    run2=`echo $dir|sed 's:Comp_::'|awk -F"-" '{print $2}'`

echo '---++++++ Run '$run1' - '$run2'
   * PDF summary : [[%ATTACHURL%/gain_summary_final_'$run1-$run2'.pdf][gain_summary_final_'$run1-$run2'.pdf]]
   * TAR file containing PDF + root + figs PNGs + logs : [[%ATTACHURL%/run'$run1-$run2'.gz.tar][run'$run1-$run2'.gz.tar]]'


    cd Comp_$run1-$run2
    mkdir -p ZIP/figs
    cp *.png ZIP/figs/.
    cp gain_summary_final_*.pdf ZIP/.
    cp *Run*.log ZIP/.
    cp Comp_Run*.root ZIP/Summary_Run$run1-$run2.root
    
    mv ZIP summary_run$run1-$run2
    zip_file=run$run1-$run2.gz.tar
    echo -e "\nMaking Comp_$run1-$run2/$zip_file ...\n"
    tar czf $zip_file summary_run$run1-$run2
    rm -fr summary_run$run1-$run2
    
    echo -e "  To get it, please issue:\nscp $USER@lxplus.cern.ch:`pwd`/$zip_file .\ntar xzf $zip_file\n"
    cd .. 
done
}




create=0
submit=0
resubmit=0
stage=0
hadd=0
status=0
summary=0
pdf=0
compare=0
verbose=0
ijob=-1
createPayload=0
twiki=0
comp_twiki=0
info=0

run1=0
file1=""
run2=0
file2=""
dat_file=''

FPIXFEDarray=(1296 1297 1298 1299 1300 1301 1302 1308 1309 1310 1311 1312 1313 1314 1320 1321 1322 1323 1324 1325 1326 1332 1333 1334 1335 1336 1337 1338)
BPIXFEDarray=(1200 1201 1202 1203 1204 1205 1206 1207 1208 1209 1212 1213 1214 1215 1216 1217 1218 1219 1220 1221 1224 1225 1226 1227 1228 1229 1230 1231 1232 1233 1236 1237 1238 1239 1240 1241 1242 1243 1244 1245 1248 1249 1250 1251 1252 1253 1254 1255 1256 1257 1260 1261 1262 1263 1264 1265 1266 1267 1268 1269 1272 1273 1274 1275 1276 1277 1278 1279 1280 1281 1284 1285 1286 1287 1288 1289 1290 1291 1292 1293)
PIXFEDarray=("${BPIXFEDarray[@]}" "${FPIXFEDarray[@]}")


#lock

if [ $# -eq 0 ] ; then usage ; fi
for arg in $* ; do
  case $arg in
    -create)       create=1     ; run=$2 ; indir=$3 ; storedir=$4 ; shift ; shift ; shift ; shift ;;
    -submit)       submit=1     ; run=$2 ; shift ;;
    -resubmit)     resubmit=1   ; run=$2 ; ijob=$3  ; shift ;;
    -stage)        stage=1      ; run=$2 ; shift ;;
    -status)       status=1     ; run=$2 ; shift ;;
    -hadd)         hadd=1       ; run=$2 ; shift ;;
    -summary)      summary=1    ; run=$2 ; shift ;;
    -pdf)          pdf=1        ; run=$2 ; shift ;;
    -compare)      compare=1    ; run=0  ; run1=$2 ; file1=$3 ; run2=$4 ; file2=$5 ; shift ; shift ; shift ; shift ; shift ;;
    -payload)      createPayload=1      ; run=$2 ; year=$3 ; db_version=$4 ; shift ; shift ; shift ;;
    -twiki)        twiki=1      ; run=$2 ; shift ;;
    -comp_twiki)   comp_twiki=1 ; run=$2 ; shift ;;
    -info)         info=1       ; run=$2 ; shift ;;
    -v)            verbose=1    ; shift ;;
    -help)         usage ;;
    *)             ;;
  esac
done

if [ "$run" == "" ];then 
  usage
fi

if [ $create -eq 1 ];then
  create
fi

if [ $submit -eq 1 ];then
  read_config
  submit_calib
fi

if [ $resubmit -eq 1 ];then
  read_config
  if [ "$ijob" == "" ];then resubmit_all;
  else resubmit_job; fi
fi

if [ $stage -eq 1 ];then
  read_config
  stage_all_files
fi

if [ $status -eq 1 ];then
  read_config
  status
fi

if [ $hadd -eq 1 ];then
  read_config
  hadd_files
fi

if [ $summary -eq 1 ];then
  read_config
  submit_summary_new
fi


if [ $pdf -eq 1 ];then
  read_config
  compile_pdf
fi


if [ $compare -eq 1 ];then
  set_files_for_comparison
  compare_runs
fi


if [ $createPayload -eq 1 ];then
  read_config
  make_payload
fi


if [ $twiki -eq 1 ];then
  read_config
  print_twiki_text
fi

if [ $comp_twiki -eq 1 ];then
  #read_config
  print_comp_twiki_text
fi

if [ $info -eq 1 ];then
  read_config
fi


rm -f .lock_gaincalib

