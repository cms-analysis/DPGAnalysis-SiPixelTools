#! /usr/bin/env python
# Author: Izaak Neutelings (February 2020)
import os, sys, re, glob, json, getpass
from utils import *
from fnmatch import fnmatch
from argparse import ArgumentParser

_basedir      = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
_FPixFEDs = [ 
  ###1296, 1297, 1298, 1299, 1300, 1301, 1302, 1308, 1309, 1310, 1311, 1312, 1313, 1314,
  ###1320, 1321, 1322, 1323, 1324, 1325, 1326, 1332, 1333, 1334, 1335, 1336, 1337, 1338,
]
_BPixFEDs = [
  1201, 1202,
  ###1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1212, 1213, 1214, 1215, 1216, 1217,
  ###1218, 1219, 1220, 1221, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1236, 1237,
  ###1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255,
  ###1256, 1257, 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269, 1272, 1273, 1274, 1275,
  ###1276, 1277, 1278, 1279, 1280, 1281, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293,
]
_PixFEDs = _FPixFEDs + _BPixFEDs

# COMMON
parser     = ArgumentParser(prog='run.py')
parser_cmn = ArgumentParser(add_help=False)
parser_cmn.add_argument('run',               type=int, action='store', #, default=323203
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
parser_cmn.add_argument('-v', '--verbose',   dest='verbosity', type=int, nargs='?', const=1, default=0, action='store',
                                             help="set verbosity" )

# INPUT/OUTPUT
_indir  = "/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_$RUN"
_outdir = "/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/$USER" #/GainCalib_run$RUN"
parser_cmnO  = ArgumentParser(parents=[parser_cmn],add_help=False)
parser_cmnIO = ArgumentParser(parents=[parser_cmn],add_help=False)
parser_cmnIO.add_argument('-i','--indir',    default=_indir, action='store',
                          metavar='DIR',     help="input directory where calibration files are stored" )
parser_cmnIO.add_argument('-o','--outdir',   default=_outdir, action='store',
                          metavar='DIR',     help="output directory where job output is stored" )
parser_cmnO.add_argument( '-o','--outdir',   default=None, action='store',
                          metavar='DIR',     help="output directory where job output is stored" )

# SUBCOMMANDS
subparsers = parser.add_subparsers(title="sub-commands",dest='subcommand', help="sub-command help")
parser_crt = subparsers.add_parser('create',     parents=[parser_cmnIO], help='create submission directory (rundir)')
parser_sub = subparsers.add_parser('submit',     parents=[parser_cmn],   help='submit jobs')
parser_rsb = subparsers.add_parser('resubmit',   parents=[parser_cmn],   help='resubmit failed jobs',conflict_handler='resolve')
#parser_stg = subparsers.add_parser('stage',      parents=[parser_cmn],   help='stage help')
parser_sts = subparsers.add_parser('status',     parents=[parser_cmn],   help='check status of jobs')
parser_hdd = subparsers.add_parser('hadd',       parents=[parser_cmnO],  help='hadd job output')
parser_sum = subparsers.add_parser('summary',    parents=[parser_cmnO],  help='summarize results')
parser_cmp = subparsers.add_parser('compare',    parents=[parser_cmnO],  help='compare files')
parser_pay = subparsers.add_parser('payload',    parents=[parser_cmnO],  help='create payload (DB object)')
#parser_twk = subparsers.add_parser('twiki',      parents=[parser_cmnO],  help='make text for TWiki')
#parser_inf = subparsers.add_parser('info',       parents=[parser_cmnO],  help='get info on run')
parser_sum.add_argument('-p', '--pdf',   dest='pdfonly', default=False, action='store_true',
                                         help="only compile summary pdf, do not create plots" )
parser_cmp.add_argument('file1',         nargs='?', action='store',
                        metavar='FILE1', help="first file" )
parser_cmp.add_argument('run2',          type=int, action='store',
                        metavar='RUN2',  help="second run number" )
parser_cmp.add_argument('file2',         nargs='?', action='store',
                        metavar='FILE2', help="second file" )
parser_pay.add_argument('year',          metavar='YEAR', action='store', help="year" )
parser_pay.add_argument('dbversion',     metavar='DBVERSION', action='store', help="database version" )

# ALLOW ABBREVIATIONS
args = sys.argv[1:]
if args:
  subcmds = [ 
    'create','submit','resubmit','status','stage','hadd','summary',
    'compare','payload','twiki','comp_twiki','info',
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
  global _PixFEDs, _run, batch
  if args.feds!=None and -1 not in args.feds:
    _PixFEDs = args.feds
  elif args.BPixOnly:
    _PixFEDs = _BPixFEDs
  elif args.FPixOnly:
    _PixFEDs = _FPixFEDs
  _run  = args.run
  batch = HTCondor(verb=True)
  
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
  elif args.subcommand=='payload':
    main_payload(args)
  elif args.subcommand=='compare':
    main_compare(args)
  else:
    print ">>> subcommand '%s' not implemented!"%(args.subcommand)
  


##############
#   CREATE   #
##############

def main_create(args):
  #print ">>> main_create", args
  
  # SETTING
  run              = args.run
  cwd              = os.getcwd()
  user             = getpass.getuser()
  ext              = 'dmp'
  fedlist          = ', '.join(str(s) for s in _PixFEDs)
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
  print ">>> %-14s = %r"%('cwd',cwd)
  print ">>> %-14s = %r"%('indir',indir)
  print ">>> %-14s = %r"%('outdir',outdir)
  print ">>> %-14s = %r"%('rundir',rundir)
  print ">>> %-14s = %r"%('config',jsonconfig)
  print ">>> %-14s = %r"%('subtask_script',subtask_script)
  print ">>> %-14s = %r"%('submit_script',submit_script)
  print ">>> %-14s = %r"%('config_script',config_script)
  print '-'*80
  
  # PREPARE
  ensureDirectory(rundir,empty=True)
  removeFile(["filelist.txt","es.log"])
  outstorage = getstorage(outdir)
  outstorage.rm(outdir)
  outstorage.mkdir(outdir)
  outstorage.chmod(outdir)
  instorage = getstorage(indir)
  #set_specifics $indir
  #if [ `is_on_castor $indir` -eq 1 ] ; then wait_for_staging ; fi
  
  # JSON CONFIG
  print ">>> Creating config file..."
  data = { 'run': run, 'indir': indir, 'outdir': outdir, 'feds': _PixFEDs}
  with open(jsonconfig,'w') as file:
    #file.write("run = %s"%run)
    #file.write("indir = %s"%indir)
    #file.write("outdir = %s"%outdir)
    json.dump(data,file,indent=4)
  
  #### CMS PSET CONFIG
  ###template = "gain_calib_cfg.py" #"gain_calib_template_cfg.py"
  ###for fed in _PixFEDs:
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
  run            = args.run
  cfgdict        = getConfig(run)
  cwd            = os.getcwd()
  indir          = cfgdict['indir']
  outdir         = cfgdict['outdir']
  rundir         = cfgdict['rundir']
  logdir         = os.path.join(rundir,"log")
  outstorage     = getstorage(outdir)
  instorage      = getstorage(indir)
  subtask_script = "submit_gain_calib_%s.sub"%run
  print '-'*80
  print ">>> Setting parameters for submission..."
  print ">>> %-14s = %s"%('run',run)
  print ">>> %-14s = %r"%('cwd',cwd)
  print ">>> %-14s = %r"%('indir',indir)
  print ">>> %-14s = %r"%('outdir',outdir)
  print ">>> %-14s = %r"%('rundir',rundir)
  print ">>> %-14s = %r"%('logdir',logdir)
  print ">>> %-14s = %r"%('subtask_script',subtask_script)
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
  run              = args.run
  fedlist          = ','.join(str(s) for s in _PixFEDs)
  cfgdict          = getConfig(run)
  cwd              = os.getcwd()
  indir            = cfgdict['indir']
  outdir           = cfgdict['outdir']
  rundir           = cfgdict['rundir']
  logdir           = os.path.join(rundir,"log")
  outstorage       = getstorage(outdir)
  instorage        = getstorage(indir)
  subtask_template = os.path.join(rundir,"submit_gain_calib_%s.sub"%run)
  subtask_script   = os.path.join(rundir,"submit_gain_calib_%s_retry.sub"%run)
  print '-'*80
  print ">>> Setting parameters for submission..."
  print ">>> %-14s = %s"%('run',run)
  print ">>> %-14s = %s"%('feds',fedlist)
  print ">>> %-14s = %r"%('cwd',cwd)
  print ">>> %-14s = %r"%('indir',indir)
  print ">>> %-14s = %r"%('outdir',outdir)
  print ">>> %-14s = %r"%('rundir',rundir)
  print ">>> %-14s = %r"%('logdir',logdir)
  print ">>> %-14s = %r"%('subtask_script',subtask_script)
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
  run            = args.run
  cfgdict        = getConfig(run)
  outdir         = cfgdict['outdir']
  feds           = _PixFEDs #cfgdict['feds']
  outstorage     = getstorage(outdir)
  print '-'*80
  print ">>> Checking status of job output..."
  print ">>> %-8s = %s"%('run',run)
  print ">>> %-8s = %r"%('outdir',outdir)
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
  run        = args.run
  if args.outdir:
    outdir   = args.outdir
  else:
    cfgdict  = getConfig(run)
    outdir   =  cfgdict['outdir']
  outstorage = getstorage(outdir)
  print '-'*80
  print ">>> Hadd'ing job output..."
  print ">>> %-8s = %s"%('run',run)
  print ">>> %-8s = %r"%('outdir',outdir)
  print '-'*80
  
  # rm -f $dir/GainCalibration.root
  outstorage.hadd("$PATH/*.root","$PATH/GainCalibration.root")
  


###############
#   SUMMARY   #
###############

def main_summary(args):
  print ">>> main_summary", args
  
  # SETTING
  run        = args.run
  if args.outdir:
    outdir   = args.outdir
  else:
    cfgdict  = getConfig(run)
    outdir   = cfgdict['outdir']
    rundir   = cfgdict['rundir']
  outstorage = getstorage(outdir)
  gainfile   = outstorage.file("GainCalibration.root",ensure=True)
  sumdir     = os.path.join(rundir,"Summary_Run%s"%(run)) # TEXToutput
  sumstorage = Local(sumdir,ensure=True)
  texfname   = os.path.join(sumdir,"gain_summary.tex")
  pdffname   = "gain_summary.pdf"
  sumfname   = os.path.join(sumdir,"texSummary_Run%s.tex"%(run))
  print '-'*80
  print ">>> Create summary of gain calibration..."
  print ">>> %-9s = %s"%('run',run)
  print ">>> %-9s = %r"%('outdir',outdir)
  print ">>> %-9s = %r"%('rundir',rundir)
  print ">>> %-9s = %r"%('sumdir',sumdir)
  print ">>> %-9s = %r"%('gainfile',gainfile)
  print '-'*80
  
  # COPY SCRIPTS
  scripts = [
    "make_ComparisonPlots.cc","TMean.*","PixelNameTranslator.*",
    "functions.C","hist_declarations.C","containers.h","header.h"
  ]
  for script in scripts:
    sumstorage.cp("scripts/%s"%(script))
  #sumstorage.cp("scripts/gain_summary.txt","$PATH/script/gain_summary_template.tex")
  
  # CREATE CC TEMPLATE
  if not args.pdfonly:
    sumstorage.cd()
    # TODO: convert CC script to python
    execute('root -l -b -x make_ComparisonPlots.cc+"(\\"%s\\",\\"%s\\")" -q'%(gainfile,run))
    os.chdir(_basedir)
  
  # CREATE TEX TEMPLATE
  tempfname = ensureFile("scripts/gain_summary_template.tex")
  sublist   = [('RUNNUMBER',run),('DIFF_TO_REPLACE',0)]
  sublist   = [(re.compile("#%s(?!\w)"%p),str(v)) for p, v in sublist]
  fregexp   = re.compile("#TOREPLACE(?!\w)")
  #writeFromTemplate(textemp,texfile,sublist)
  with open(tempfname,'r') as tempfile:
    with open(texfname,'w') as texfile:
      for i, line in enumerate(tempfile.readlines()):
        if fregexp.search(line):
          with open(sumfname,'r') as sumfile:
            for sumline in sumfile.readlines():
              texfile.write(sumline)
        else:
          for regexp, value in sublist:
            if regexp.search(line):
              line = regexp.sub(value,line)
            texfile.write(line)
  
  # COMPILE TEX
  print ">>> Compiling %s..."%(texfname)
  sumstorage.cd()
  execute("pdflatex %s"%(pdffname)) #&> latex.log
  execute("pdflatex %s"%(pdffname)) #>> latex.log 2>&1
  if os.path.isfile(pdffname):
    print ">>> Done compiling %s!"%(texfname)
  else:
    print ">>> Compiling of %s failed!"%(texfname)
  #  with open("latex.log",'r') as logfile:
  #    print logfile.read()
  


###############
#   PAYLOAD   #
###############

def main_payload(args):
  print ">>> main_payload", args
  
  # SETTING
  run        = args.run
  if args.outdir:
    outdir   = args.outdir
  else:
    cfgdict  = getConfig(run)
    outdir   = cfgdict['outdir']
    rundir   = cfgdict['rundir']
  year       = args.year
  dbversion  = args.dbversion
  outstorage = getstorage(outdir)
  gainfile   = outstorage.file("GainCalibration.root",ensure=True)
  paydir     = os.path.join(rundir,"Payload_Run%s"%(run))
  paystorage = Local(paydir,ensure=True)
  print '-'*80
  print ">>> Create gain calibration payload..."
  print ">>> %-9s = %s"%('run',run)
  print ">>> %-9s = %s"%('year',year)
  print ">>> %-9s = %s"%('dbversion',dbversion)
  print ">>> %-9s = %r"%('outdir',outdir)
  print ">>> %-9s = %r"%('rundir',rundir)
  print ">>> %-9s = %r"%('paydir',paydir)
  print ">>> %-9s = %r"%('gainfile',gainfile)
  print '-'*80
  
  # OFFLINE PAYLOAD
  cmd = ("cmsRun SiPixelGainCalibrationDBUploader_cfg.py"
         " run=%s year=%s dbversion=%s gain=$GAIN file=%s outdir=%s"%(run,year,dbversion,gainfile,paydir))
  print "Creating payload for offline..."
  execute(cmd.replace('$GAIN','offline'))
  
  ## OFFLINE FULL PAYLOAD
  #print "Creating payload for offline full..."
  #execute(cmd.replace('$GAIN','full'))
  
  ## HLT PAYLOAD
  #print "Creating payload for HLT..."
  #execute(cmd.replace('$GAIN','hlt'))
  


###############
#   COMPARE   #
###############

def main_compare(args):
  print ">>> main_compare", args
  
  # SETTING
  run1       = args.run
  run2       = args.run2
  file1      = args.file1
  file2      = args.file2
  print '-'*80
  print ">>> Create gain calibration payload..."
  print ">>> %-6s = %s"%('run1',run1)
  print ">>> %-6s = %s"%('run2',run2)
  print ">>> %-6s = %r"%('file1',file1)
  print ">>> %-6s = %r"%('file2',file2)
  print '-'*80
  


if __name__ == "__main__":
  main(args)
  print ">>> Done"
  

