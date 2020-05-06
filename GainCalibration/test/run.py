#! /usr/bin/env python
# Author: Izaak Neutelings (February 2020)
import os, sys, re, glob, json, getpass
from utils import *
from fnmatch import fnmatch
from argparse import ArgumentParser

FPixFEDs = [ 
  ###1296, 1297, 1298, 1299, 1300, 1301, 1302, 1308, 1309, 1310, 1311, 1312, 1313, 1314,
  ###1320, 1321, 1322, 1323, 1324, 1325, 1326, 1332, 1333, 1334, 1335, 1336, 1337, 1338,
]
BPixFEDs = [
  1201, 1202,
  ###1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1212, 1213, 1214, 1215, 1216, 1217,
  ###1218, 1219, 1220, 1221, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1236, 1237,
  ###1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255,
  ###1256, 1257, 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269, 1272, 1273, 1274, 1275,
  ###1276, 1277, 1278, 1279, 1280, 1281, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293,
]
PixFEDs = FPixFEDs + BPixFEDs

"""
Usage :
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
"""

parser = ArgumentParser(prog='run.py')

# COMMON
parser_cmn   = ArgumentParser(add_help=False)
parser_cmn.add_argument('run',               type=int, default=323203, action='store',
                        metavar='RUN',       help="run number" )
parser_cmn.add_argument('-d','--dry',        dest='dryrun', action='store_true',
                                             help='dry run for debugging purposes')
parser_cmn.add_argument('-f','--force',      dest='force', action='store_true',
                                             help='force (re)submission or overwriting of existing files')
parser_cmn.add_argument('-B', '--BPix-only', dest='BPixOnly', default=False, action='store_true',
                                             help="only run over BPix" )
parser_cmn.add_argument('-F', '--FPix-only', dest='FPixOnly', default=False, action='store_true',
                                             help="only run over FPix" )
parser_cmn.add_argument('-n', '--fed',       dest='feds', type=int, nargs='+', default=None, action='store',
                        metavar='FED',       help="only run over these feds" )
parser_cmn.add_argument('-v', '--verbose',   dest='verbose', default=False, action='store_true',
                                             help="set verbose" )

# INPUT/OUTPUT
defindir  = "/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_$RUN"
defoutdir = "/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/$USER" #/GainCalib_run$RUN"
parser_cmnO  = ArgumentParser(parents=[parser_cmn],add_help=False)
parser_cmnIO = ArgumentParser(parents=[parser_cmn],add_help=False)
parser_cmnIO.add_argument('-i','--indir',    default=defindir, action='store',
                          metavar='DIR',     help="input directory where calibration files are stored" )
parser_cmnIO.add_argument('-o','--outdir',   default=defoutdir, action='store',
                          metavar='DIR',     help="output directory where job output is stored" )
parser_cmnO.add_argument( '-o','--outdir',   default=defoutdir, action='store',
                          metavar='DIR',     help="output directory where job output is stored" )

# SUBCOMMANDS
subparsers = parser.add_subparsers(title="sub-commands",dest='subcommand', help="sub-command help")
parser_crt = subparsers.add_parser('create',     parents=[parser_cmnIO], help='create help')
parser_sub = subparsers.add_parser('submit',     parents=[parser_cmn],   help='submit help')
parser_rsb = subparsers.add_parser('resubmit',   parents=[parser_cmn],   help='resubmit help',conflict_handler='resolve')
#parser_stg = subparsers.add_parser('stage',      parents=[parser_cmn],   help='stage help')
parser_sts = subparsers.add_parser('status',     parents=[parser_cmn],   help='status help')
parser_hdd = subparsers.add_parser('hadd',       parents=[parser_cmnO],  help='hadd help')
parser_sum = subparsers.add_parser('summary',    parents=[parser_cmnO],  help='summary help')
parser_pdf = subparsers.add_parser('pdf',        parents=[parser_cmnO],  help='pdf help')
parser_cmp = subparsers.add_parser('compare',    parents=[parser_cmnO],  help='compare help')
parser_pyl = subparsers.add_parser('payload',    parents=[parser_cmnO],  help='payload help')
parser_twk = subparsers.add_parser('twiki',      parents=[parser_cmnO],  help='twiki help')
parser_ctw = subparsers.add_parser('comp_twiki', parents=[parser_cmnO],  help='comp_twiki help')
parser_inf = subparsers.add_parser('info',       parents=[parser_cmnO],  help='info help')

# ALLOW ABBREVIATIONS
args = sys.argv[1:]
if args:
  subcmds = [ 
    'create','submit','resubmit','status','stage','hadd','summary',
    'pdf','compare','payload','twiki','comp_twiki','info',
  ]
  for subcmd in subcmds:
    if args[0] in subcmd[:len(args[0])]: # match abbreviation
      args[0] = subcmd
      break
args = parser.parse_args(args)



############
#   MAIN   #
############

def main(args):
  print ">>> main", args
  
  # SETTING
  global PixFEDs, run, batch
  if args.feds!=None and -1 not in args.feds:
    PixFEDs = args.feds
  elif args.BPixOnly:
    PixFEDs = BPixFEDs
  elif args.FPixOnly:
    PixFEDs = FPixFEDs
  run   = args.run
  batch = HTCondor(verbose=True)
  
  # SUBCOMMANDS
  if args.subcommand=='create':
    main_create(args)
  elif args.subcommand=='submit':
    main_submit(args)
  elif args.subcommand=='resubmit':
    main_resubmit(args)
  elif args.subcommand=='status':
    main_status(args)
  elif args.subcommand=='hadd':
    main_hadd(args)
  elif args.subcommand=='summary':
    main_summary(args)
  else:
    print ">>> subcommand '%s' not implemented!"%(args.subcommand)
  


##############
#   CREATE   #
##############

def main_create(args):
  #print ">>> main_create", args
  
  # SETTING
  cwd              = os.getcwd()
  user             = getpass.getuser()
  ext              = 'dmp'
  fedlist          = ', '.join(str(s) for s in PixFEDs)
  indir            = args.indir.replace('$USER',user).replace('$RUN',str(run))
  outdir           = args.outdir.replace('$USER',user).replace('$RUN',str(run))
  outdir           = os.path.join(outdir,"GainRun_%d"%run)
  rundir           = os.path.join(cwd,"Run_%d"%run)
  subtask_template = "submit_gain_calib_template.sub"
  submit_template  = "submit_gain_calib_template.sh"
  config_template  = "gain_calib_cfg.py"
  jsonconfig       = os.path.join(rundir,"config.json")
  submit_script    = os.path.join(rundir,"submit_gain_calib_%s.sh"%run)
  subtask_script   = os.path.join(rundir,"submit_gain_calib_%s.sub"%run)
  config_script    = os.path.join(rundir,config_template)
  print '-'*80
  print ">>> Setting parameters for creating config files..."
  print ">>> %-14s = %s"%('run',run)
  print ">>> %-14s = '%s'"%('cwd',cwd)
  print ">>> %-14s = '%s'"%('indir',indir)
  print ">>> %-14s = '%s'"%('outdir',outdir)
  print ">>> %-14s = '%s'"%('rundir',rundir)
  print ">>> %-14s = '%s'"%('config',jsonconfig)
  print ">>> %-14s = '%s'"%('subtask_script',subtask_script)
  print ">>> %-14s = '%s'"%('submit_script',submit_script)
  print ">>> %-14s = '%s'"%('config_script',config_script)
  print '-'*80
  
  # PREPARE
  ensureDirectory(rundir,empty=True)
  removeFile(["filelist.txt","es.log"])
  outstorage = getStorage(outdir)
  outstorage.rm(outdir)
  outstorage.mkdir(outdir)
  outstorage.chmod(outdir)
  instorage = getStorage(indir)
  #set_specifics $indir
  #if [ `is_on_castor $indir` -eq 1 ] ; then wait_for_staging ; fi
  
  # JSON CONFIG
  print ">>> Creating config file..."
  data = { 'run': run, 'indir': indir, 'outdir': outdir, 'feds': PixFEDs}
  with open(jsonconfig,'w') as file:
    #file.write("run = %s"%run)
    #file.write("indir = %s"%indir)
    #file.write("outdir = %s"%outdir)
    json.dump(data,file,indent=4)
  
  #### CMS PSET CONFIG
  ###template = "gain_calib_cfg.py" #"gain_calib_template_cfg.py"
  ###for fed in PixFEDs:
  ###  fedconfig  = os.path.join(rundir,"gain_calib_%s_cfg.py"%fed)
  ###  fedoutfile = "GainCalibration_%s_%s.dmp"%(fed,run)
  ###  if not instorage.exists(indir,fedoutfile):
  ###    print ">>> File %s is not present in %s..."%(fedoutfile,indir)
  ###  sublist = [('FILENAME',"file:%s"%fedoutfile)]
  ###  writeFromTemplate(template,fedconfig,sublist)
  
  # PREPARES SCRIPTS
  print ">>> Creating submission files..."
  subtask_sublist = [
    ('RUN',run),('FEDS',fedlist),
  ]
  submit_sublist = [
    ('RUN',run),('RUNDIR',rundir),('INDIR',indir),('OUTDIR',outdir),
    #('RUN',str(run)),('T2_CP',...),('T2_OUT_CP',...),('T2_TMP_DIR',...),('T2_PREFIX',...),('EXT','dmp')
  ]
  writeFromTemplate(subtask_template,subtask_script,subtask_sublist)
  writeFromTemplate(submit_template,submit_script,submit_sublist)
  writeFromTemplate(config_template,config_script,[ ])
  
  print ">>> Created run directory '%s'"%rundir
  


##############
#   SUBMIT   #
##############

def main_submit(args):
  #print ">>> main_submit", args
  
  # SETTING
  cfgdict        = getConfig(run)
  cwd            = os.getcwd()
  indir          = cfgdict['indir']
  outdir         = cfgdict['outdir']
  rundir         = cfgdict['rundir']
  logdir         = os.path.join(rundir,"log")
  outstorage     = getStorage(outdir)
  instorage      = getStorage(indir)
  subtask_script = "submit_gain_calib_%s.sub"%run
  print '-'*80
  print ">>> Setting parameters for submission..."
  print ">>> %-14s = %s"%('run',run)
  print ">>> %-14s = '%s'"%('cwd',cwd)
  print ">>> %-14s = '%s'"%('indir',indir)
  print ">>> %-14s = '%s'"%('outdir',outdir)
  print ">>> %-14s = '%s'"%('rundir',rundir)
  print ">>> %-14s = '%s'"%('logdir',logdir)
  print ">>> %-14s = '%s'"%('subtask_script',subtask_script)
  print '-'*80
  
  # PREPARE
  ensureDirectory(logdir,empty=False)
  
  # SUBMIT
  if not os.path.exists(rundir):
    raise IOError("Run directory '%s' does not exist! Please try './run.py create %s'. cwd=%s"%(rundir,run,os.getcwd()))
  os.chdir(rundir)
  batch.submit(subtask_script,dry=args.dryrun)
  os.chdir('..')
  


################
#   RESUBMIT   #
################

def main_resubmit(args):
  print ">>> main_resubmit", args
  if args.feds==None and not args.BPixOnly and not args.FPixOnly:
    raise IOError("Please specify which FED(s) to resubmit with '-n' or '--fed'. Use '-n -1' to resubmit all.")
  
  # SETTING
  fedlist          = ','.join(str(s) for s in PixFEDs)
  cfgdict          = getConfig(run)
  cwd              = os.getcwd()
  indir            = cfgdict['indir']
  outdir           = cfgdict['outdir']
  rundir           = cfgdict['rundir']
  logdir           = os.path.join(rundir,"log")
  outstorage       = getStorage(outdir)
  instorage        = getStorage(indir)
  subtask_template = os.path.join(rundir,"submit_gain_calib_%s.sub"%run)
  subtask_script   = os.path.join(rundir,"submit_gain_calib_%s_retry.sub"%run)
  print '-'*80
  print ">>> Setting parameters for submission..."
  print ">>> %-14s = %s"%('run',run)
  print ">>> %-14s = %s"%('feds',fedlist)
  print ">>> %-14s = '%s'"%('cwd',cwd)
  print ">>> %-14s = '%s'"%('indir',indir)
  print ">>> %-14s = '%s'"%('outdir',outdir)
  print ">>> %-14s = '%s'"%('rundir',rundir)
  print ">>> %-14s = '%s'"%('logdir',logdir)
  print ">>> %-14s = '%s'"%('subtask_script',subtask_script)
  print '-'*80
  
  # PREPARE
  ensureDirectory(logdir,empty=False)
  
  # TODO: check existence
  
  # PREPARES SCRIPTS
  print ">>> Creating submission files..."
  if args.feds!=None and -1 in args.feds:
    writeFromTemplate(subtask_template,subtask_script,[ ])
  else:
    writeFromTemplate(subtask_template,subtask_script,rmlist=["queue arg"])
  
  # RESUBMIT
  subtask_script = subtask_script.split('/')[-1]
  if not os.path.exists(rundir):
    raise IOError("Run directory '%s' does not exist! Please try './run.py create %s'. cwd=%s"%(rundir,run,os.getcwd()))
  os.chdir(rundir)
  taskargs = None if args.feds!=None and -1 in args.feds else fedlist
  batch.submit(subtask_script,dry=args.dryrun,taskargs=taskargs)
  os.chdir('..')
  


##############
#   STATUS   #
##############

def main_status(args):
  print ">>> main_status", args
  
  # SETTING
  cfgdict        = getConfig(run)
  outdir         = cfgdict['outdir']
  feds           = PixFEDs #cfgdict['feds']
  outstorage     = getStorage(outdir)
  print '-'*80
  print ">>> Checking status of job output..."
  print ">>> %-8s = %s"%('run',run)
  print ">>> %-8s = '%s'"%('outdir',outdir)
  print '-'*80
  
  # CHECK OUTPUT
  rootexp  = re.compile(r"(\d+).root")
  filelist = [f for f in outstorage.ls()] #if rootexp.match(f)]
  fedsdone = [ ]
  fedspend = [ ] # pending, running
  fedsfail = [ ]
  for file in filelist:
    match = rootexp.match(file)
    if not match: continue
    fed = match.group(1)
    fedsdone.append(fed)
  
  # CHECK PENDING
  jobs = batch.jobs()
  for job in jobs:
    print job, job.jobid, job.taskid, job.args, job.getStatus()
    if job.getStatus() in ['q','r']:
      fed = str(job.args)
      fedspend.append(fed)
  
  # CHECK FAILED
  for fed in feds:
    fed = str(fed)
    if fed not in fedsdone and fed not in fedspend:
      fedsfail.append(fed)
  
  # PRINT
  done = color('DONE','green',True) if fedsdone else 'DONE'
  pend = color('RUNNING','white',True) if fedspend else 'RUNNING'
  fail = color('FAILED','red',True) if fedsfail else 'FAILED'
  def printJobs(fedden,label,text,col):
    if fedden:
      ratio = color("%2d/%d"%(len(fedden),len(feds)),col,False)
      label = color(label,col,True)
      print ">>> %s %s - %s:"%(ratio,label,text)
      print ">>>    " + ', '.join(fedden)
    else:
      print ">>> %2d/%d %s - %s"%(len(fedden),len(feds),label,text)
  printJobs(fedsdone,'DONE',   "FEDs with output in outdir",'green')
  printJobs(fedspend,'RUNNING',"FEDs with pending or running jobs",'white')
  printJobs(fedsfail,'FAILED', "FEDs with no output in outdir",'red')
  


############
#   HADD   #
############

def main_hadd(args):
  print ">>> main_hadd", args
  
  # SETTING
  if args.outdir:
    outdir  = args.outdir
  else:
    cfgdict = getConfig(run)
    outdir  =  cfgdict['outdir']
  print '-'*80
  print ">>> Checking status of job output..."
  print ">>> %-8s = %s"%('run',run)
  print ">>> %-8s = '%s'"%('outdir',outdir)
  print '-'*80
  
  # rm -f $dir/GainCalibration.root
  outstorage.hadd("$PATH/*.root","$PATH/GainCalibration.root")
  


###############
#   SUMMARY   #
###############

def main_summary(args):
  print ">>> main_summary", args
  
  # SETTING
  if args.outdir:
    outdir  = args.outdir
  else:
    cfgdict = getConfig(run)
    outdir  =  cfgdict['outdir']
  outstorage = getStorage(outdir)
  print '-'*80
  print ">>> Checking status of job output..."
  print ">>> %-8s = %s"%('run',run)
  print ">>> %-8s = '%s'"%('outdir',outdir)
  print '-'*80
  
  # 1. "GainCalibration.root"
  # 
  
  # 2. make dirs
  # ${runningdir}/TEXToutput
  # $runningdir/Summary_Run$run
  
  # 3. copy scripts
  # cp -fr scripts/make_ComparisonPlots.cc ${runningdir}/Summary_Run$run/make_ComparisonPlots.cc
  # cp -fr scripts/gain_summary.txt  ${runningdir}/Summary_Run$run/gain_summary_template.tex
  # cp -fr scripts/TMean.* ${runningdir}/Summary_Run$run/.
  # cp -fr scripts/PixelNameTranslator.* ${runningdir}/Summary_Run$run/.
  # cp -fr scripts/header.h scripts/functions.C scripts/containers.h scripts/hist_declarations.C ${runningdir}/Summary_Run$run/.
  
  # cd $runningdir/Summary_Run$run
  
  # 4. create tex template
  # rm -fr gain_summary.tex
  # cat gain_summary_template.tex | sed "s#RUNNUMBER#$run#" | sed "s#DIFF_TO_REPLACE#0#" > gain_summary.tex
  # sed '/TOREPLACE/,$ d' < gain_summary.tex > gain_summary_final_run_$run.tex
  # cat texSummary_Run${run}.tex >> gain_summary_final_run_$run.tex
  # sed '1,/TOREPLACE/d'< gain_summary.tex >> gain_summary_final_run_$run.tex
  
  # 4. create cc template
  # echo "(root -l -b -x make_ComparisonPlots.cc+\"(\"`file_loc $storedir/GainCalibration.root`\",\"$run\")\" -q)"
  # root -l -b -x make_ComparisonPlots.cc+"(\"`file_loc $storedir/GainCalibration.root`\",\"$run\")" -q
  # echo -e "\n************************* SUMMARY"
  # cat *.txt
  # echo -e "\nlog files: \n"`ls *.log|sed "s:^:   --> Run_$run/Summary_Run$run/:"`"\n"
  
  # 5. compile tex
  # echo "Making pdf ..."
  # pdflatex gain_summary_final_run_$run.tex &>  latex.log
  # pdflatex gain_summary_final_run_$run.tex >> latex.log 2>&1
  # if [ ! -f gain_summary_final_run_$run.pdf ];then cat latex.log;fi
  
  # echo -e "\nPDF file:\n `pwd`/gain_summary_final_run_$run.pdf"
  


if __name__ == "__main__":
  main(args)
  print ">>> Done"
  

