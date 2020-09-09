verbose=0

#################S
#Queues
q_lxplus=cmscaf1nw
q_T2=localgrid@cream01


#order: name stdout submit.sh
submit_to_queue(){
 if [ `working_on_lxplus` -eq 1 ]; then
      # echo "bsub -q $q_lxplus -J $1 -eo $2 < $3"
   bsub -q $q_lxplus -J $1 -eo $2 < $3
 else
     echo "bla"
   #qsub -q $q_T2 -j oe -N $1 -o $2 $3
 fi
}


##############################################
# Your T2 Specifics (if not on castor/lxplus)
set_T2_specifics(){
  T2_LS='ls'
  T2_RM='rm -r '
  T2_CP='dccp -d 2'
  T2_MKDIR='mkdir -p '
  T2_CHMOD='chmod 0777 '
  T2_FSYS='dcap://'
  T2_CHECK='/pnfs/iihe/'
  T2_TMP_DIR='/scratch/'
  if [ $verbose -eq 1 ] ; then echo "Setting T2 specifics ..." ; fi
}

set_castor_specifics(){
  T2_LS='nsls'
  T2_RM='nsrm -r '
  T2_CP='rfcp'
  T2_MKDIR='rfmkdir -p '
  T2_CHMOD='rfchmod 0777 '
  T2_FSYS='rfio://'
  T2_TMP_DIR="/tmp/$USER/"
  if [ $verbose -eq 1 ] ; then echo "Setting castor specifics ..." ; fi
}

# set_eos_specifics(){
#   # alias eos='/afs/cern.ch/project/eos/installation/cms/bin/eos.select' # This doesn't work (eos cmd missing in bash?)
#   T2_LS='/afs/cern.ch/project/eos/installation/cms/bin/eos.select ls'
#   T2_RM='/afs/cern.ch/project/eos/installation/cms/bin/eos.select rm -r /eos/cms' # Do not add space before next argument when using this cmd
#   #T2_CP='cmsStage'
#   #T2_CP='/afs/cern.ch/project/eos/installation/cms/bin/eos.select cp'
#   T2_CP='xrdcp'
#   T2_PREFIX='root://eoscms.cern.ch//eos/cms/'
#   T2_MKDIR='/afs/cern.ch/project/eos/installation/cms/bin/eos.select mkdir -p /eos/cms' # Do not add space before next argument when using this cmd
#   T2_CHMOD='/afs/cern.ch/project/eos/installation/cms/bin/eos.select chmod 2777 /eos/cms' # Do not add space before next argument when using this cmd
#   T2_FSYS='root://eoscms//eos/cms' # Can check what needs to be added with cmsPfn /store/...
#   T2_TMP_DIR="/tmp/$USER"
#   if [ $verbose -eq 1 ] ; then echo "Setting eos specifics ..." ; fi
# }

set_eos_specifics(){
  # alias eos='/afs/cern.ch/project/eos/installation/cms/bin/eos.select' # This doesn't work (eos cmd missing in bash?)
  T2_LS='ls /eos/cms'
  T2_RM='rm -r /eos/cms' # Do not add space before next argument when using this cmd
  #T2_CP='cmsStage'
  #T2_CP='/afs/cern.ch/project/eos/installation/cms/bin/eos.select cp'
  T2_CP='xrdcp -f'
  T2_PREFIX='root://eoscms.cern.ch//eos/cms/'
  T2_MKDIR='mkdir -p /eos/cms' # Do not add space before next argument when using this cmd
  T2_CHMOD=' chmod 2777 /eos/cms' # Do not add space before next argument when using this cmd
  T2_FSYS='root://eoscms//eos/cms' # Can check what needs to be added with cmsPfn /store/...
  T2_TMP_DIR="/tmp/$USER"
  if [ $verbose -eq 1 ] ; then echo "Setting eos specifics ..." ; fi
}

set_local_specifics(){
  T2_LS='ls'
  T2_RM='rm -r '
  T2_CP='cp'
  T2_MKDIR='mkdir -p '
  T2_CHMOD='chmod 777 '
  T2_FSYS=''
  T2_TMP_DIR='/scratch/'
  if [ $verbose -eq 1 ] ; then echo "Setting local specifics ..." ; fi
}

set_specifics(){
  if [ $verbose -eq 1 ] ; then echo "Specifics for $1" ; fi
  if      [ `is_on_castor $1` -eq 1 ]; then set_castor_specifics ;
  else if [ `is_on_T2 $1` -eq 1 ]; then set_T2_specifics;
  else if [ `is_on_eos $1` -eq 1 ]; then set_eos_specifics;
  else  set_local_specifics ;
  fi ; fi ; fi
}


is_on_castor(){
  if [ `echo $1 | grep -e "/castor/cern.ch/" |wc -l` -eq 1 ];then echo 1
  else echo 0
  fi
}

is_on_T2(){
  if [ `echo $1 | grep -e "/pnfs/iihe/" |wc -l` -eq 1 ];then echo 1
  else echo 0
  fi
}

is_on_eos(){

    if [ `echo $1 | grep -e "/store/" | wc -l` -eq 1 ];then echo 1
    else echo 0
    fi
    # echo "bla"
    # if [[ `echo $1` == store/* ]] || [[ `echo $1` == /store/* ]]; 
    # then echo 1
    # else echo 0
    # fi
}

file_loc(){
  file=$1
  if [ `is_on_castor $file` -eq 1 ];then
    echo "rfio://$file"
  else
    if [ `is_on_T2 $file` -eq 1 ];then
      echo "${T2_FSYS}$file"
    else if [ `is_on_eos $file` -eq 1 ];then
      echo "${T2_FSYS}$file"
    else
      echo $file
    fi ; fi
  fi
}

is_file_present(){
    #echo "command: $T2_LS"
    if eval $T2_LS$1 >/dev/null 2>&1; then echo 1;else echo 0;fi
  # if [ `$T2_LS $1 2>&1|grep "No such"|wc -l` -eq 1 ];then echo 0;
  # else echo 1;
  # fi
}

working_on_lxplus(){
  if [ `uname -a | grep lxplus | wc -l` -gt 0 ];then echo 1
  else echo 0
  fi
}

is_staged(){
  status=`stager_qry -M $1`
  if [ `echo $status|grep -e "STAGED"|wc -l` -eq 1 ] || [ `echo $status|grep -e "CANBEMIGR"|wc -l` -eq 1 ]; then
    echo 1
  else
    if [ `echo $status|grep -e "STAGEIN"|wc -l` -eq 1 ];then
      echo 0
    else
      stager_get -M $1 &> /dev/null
      echo 0
    fi
  fi
}

stage_list_of_files(){

  need_to_wait=0
  for file in $@;do
    if [ `is_on_castor $file` -eq 1 ];then
      if [ `nsls $file 2>&1|grep "No such"|wc -l` -eq 1 ]; then
        echo "$file is not present on /castor !!" ; continue ;
      fi 
    
      if [ `is_staged $file` -eq 0 ];then
        echo "File $file is being staged."
        need_to_wait=1
      fi
    fi
  done

  if [ $need_to_wait -eq 1 ];then
    echo "Waiting 5min before continuing !"
    sleep 300
    stage_list_of_files $@
  fi
  
  echo "All files are finally staged ! Moving on ..."

}

stage_dir(){
  if [ $# -ne 1 ];then echo "Takes as argument only 1 castor dir." ; exit ; fi
  list=""
  for file in `nsls $1`;do
    list="$list $1/$file"  
  done
  stage_list_of_files $list
}

