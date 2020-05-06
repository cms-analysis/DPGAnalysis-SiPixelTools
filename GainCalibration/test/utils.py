# Author: Izaak Neutelings (February 2020)
import os, re, shutil
from utils import *
from abc import ABCMeta, abstractmethod
from subprocess import Popen, PIPE, STDOUT


def getConfig(run,verbose=False):
  """Get configuration from JSON file."""
  cwd        = os.getcwd()
  rundir     = os.path.join(cwd,"Run_%s"%run)
  jsonconfig = os.path.join(rundir,"config.json")
  ###cfgdict = { 'run': "", 'indir': "", 'outdir': "", 'calib_payload': "", 'rundir': rundir }
  if not os.path.exists(rundir):
    raise IOError("Run directory '%s' does not exist! Please try './run.py create %s'. cwd=%s"%(rundir,run,os.getcwd()))
  if not os.path.isfile(jsonconfig):
    raise IOError("Config file '%s' does not exist! cwd=%s"%(jsonconfig,os.getcwd()))
  
  with open(jsonconfig,'r') as file:
    cfgdict = json.load(file)
  cfgdict.setdefault('rundir',rundir)
  cfgdict.setdefault('calib_payload','none')
  if verbose>=1:
    print '-'*80
    print ">>> Reading config JSON file '%s'"%jsonconfig
    for key in ['run', 'indir', 'outdir', 'calib_payload']:
      print ">>> %-10s = '%s'"%(key,cfgdict[key])
    print '-'*80
  return cfgdict
  

def writeFromTemplate(templatename,outfilename,sublist=[],rmlist=[],**kwargs):
  """Write file from template."""
  sublist = [(re.compile("\$%s(?!\w)"%p),str(v)) for p, v in sublist]
  with open(templatename,'r') as template:
    with open(outfilename,'w') as file:
      for i, line in enumerate(template.readlines()):
        #linenum = "L%d:"%i
        if any(r in line for r in rmlist):
          continue # skip line
        for regexp, value in sublist:
          #pattern = '$'+pattern
          if regexp.search(line):
            #line = line.replace(pattern,str(value))
            line = regexp.sub(value,line)
        #if '$' in line:
        #  for key in parampattern.findall(line):
        #    if key in params:
        #      value   = makeParamValue(key,params[key])
        #      pattern = '$'+key
        #      line    = line.replace(pattern,value)
        #      print ">>>   %-4s replacing '%s' -> '%s'"%(linenum,pattern,value)
        #    else:
        #      print ">>>   %-4s Found no given value for '$%s'"%(linenum,key)
        #  for pattern, key, value in defaultpattern.findall(line):
        #      value = makeParamValue(key,params.get(key,value))
        #      line  = defaultpattern.sub(value,line)
        #      print ">>>   %-4s replacing '%s' -> '%s'"%(linenum,pattern,value)+("" if value in params else " (default)")
        #  lines.append(line)
        file.write(line)
  

def ensureDirectory(dirname,empty=False):
  """Make directory if it does not exist."""
  if not os.path.exists(dirname):
    os.makedirs(dirname)
    print '>>> Made directory "%s"'%(dirname)
    if not os.path.exists(dirname):
      print '>>> Failed to make directory "%s"'%(dirname)
  elif empty:
    for filename in os.listdir(dirname):
      filepath = os.path.join(dirname,filename)
      if os.path.isfile(filepath) or os.path.islink(filepath):
        os.unlink(filepath)
      elif os.path.isdir(filepath):
        shutil.rmtree(filepath)
  return dirname
  

def removeFile(filepaths):
  """Remove (list of) files."""
  if isinstance(filepaths,str):
    filepaths = [filepaths]
  for filepath in filepaths:
    if os.path.isfile(filepath):
      os.unlink(filepath)
  

def execute(command,dry=False,verbose=1):
  """Execute shell command."""
  out    = ""
  cmdstr = repr(str(command))
  if verbose>=1:
    print ">>> Executing: %s"%(cmdstr)
  if dry:
    print ">>> Dry run: %s"%(cmdstr)
  else:
    try:
      #process = Popen(command.split(),stdout=PIPE,stderr=STDOUT) #,shell=True)
      process = Popen(command,stdout=PIPE,stderr=STDOUT,shell=True)
      for line in iter(process.stdout.readline,""):
        print line[:-1] if line[-1:]=='\n' else line
        out += line
      process.stdout.close()
      #print 0, process.communicate()
      #out     = process.stdout.read()
      #err     = process.stderr.read()
      retcode = process.wait()
      #print out
    except Exception as e:
      print ">>> Failed: %s"%(cmdstr)
      raise e
    if verbose>=2:
      print ">>> Output: %s"%(out)
    if retcode:
      raise Exception("Command '%s' ended with return code %s"%(command,retcode)) #,err)
  return out



def getStorage(path):
  if onCastor(path): return Castor(path)
  if onIIHE_T2(path): return IIHE_T2(path)
  if '/store/' in path or onEOS(path): return EOS(path)
  return StorageSystem(path)
  
def onEOS(path): return '/eos/' in path
def onCastor(path): return '/castor/cern.ch/' in path
def onIIHE_T2(path): return '/pnfs/iihe/' in path
def onPSI_T3(path): return '/pnfs/psi.ch/' in path


class StorageSystem(object):
  
  def __init__(self,path,verbose=1):
    self.path    = path
    self.lscmd   = 'ls'
    self.lsurl   = ''
    self.cpcmd   = 'cp'
    self.cpurl   = ''
    self.rmcmd   = 'rm -rf'
    self.rmurl   = ''
    self.mkdrcmd = 'mkdir -p'
    self.mkdrurl = ''
    self.chmdprm = '777'
    self.chmdcmd = 'chmod'
    self.chmdurl = ''
    self.haddcmd = 'hadd -f'
    self.haddurl = ''
    self.tmpurl  = '/tmp/$USER/' # $TMPDIR
    self.prefix  = ""
    self.fsys    = ""
    self.verbose = verbose
    
  def execute(self,cmd,dry=False):
    return execute(cmd,dry=dry,verbose=self.verbose)
    
  def expandPath(self,*paths):
    """Help function to replace variables in given path, or set default to own path."""
    if paths:
      paths = [p.replace('$PATH',self.path) for p in paths]
      path  = os.path.join(*paths)
    else:
      path  = self.path
    return path.replace('$PATH',self.path)
    
  def ls(self,*paths):
    path    = self.expandPath(*paths)
    retlist = self.execute("%s %s%s"%(self.lscmd,self.lsurl,path)).split('\n')
    return retlist
    
  def cp(self,source,target=None):
    target = self.expandPath(target)
    return self.execute("%s %s%s %s"%(self.cpcmd,self.cpurl,source,target))
    
  def hadd(self,sources,target=None):
    target = self.expandPath(target)
    if isinstance(sources,str):
      sources = [ sources ]
    source = ""
    for i, file in enumerate(sources,1):
      source = self.haddurl+self.expandPath(file)
      if i<len(sources): source += " "
    return self.execute("%s %s %s"%(self.haddcmd,target,source))
    
  def rm(self,dirname):
    return self.execute("%s %s%s"%(self.rmcmd,self.rmurl,dirname))
    
  def mkdir(self,dirname='$PATH'):
    dirname = self.expandPath(dirname)
    return self.execute("%s %s%s"%(self.mkdrcmd,self.mkdrurl,dirname))
    
  def chmod(self,file,perm=None):
    if not perm: perm = self.chmdprm
    return self.execute("%s %s %s%s"%(self.chmdcmd,perm,self.chmdurl,file))
    
  def exists(self,*paths):
    path = self.expandPath(*paths)
    cmd  = "if `%s %s%s >/dev/null 2>&1`; then echo 1; else echo 0; fi"%(self.lscmd,self.lsurl,path)
    out  = self.execute(cmd)
    return out=='1'
  

class EOS(StorageSystem):
  
  def __init__(self,path,verbose=1):
    # EOS is mounted on lxplus now
    super(EOS,self).__init__(path,verbose=verbose)
    self.cpcmd  = 'cp' #xrdcp
    self.chmdprm = '2777'
    self.tmpurl = '/tmp/$USER/'
    self.prefix = "root://eoscms.cern.ch/"
    self.fsys   = "root://eoscms/"
  

class Castor(StorageSystem):
  
  def __init__(self,path,verbose=False):
    super(Castor,self).__init__(path,verbose=verbose)
    self.lscmd  = 'nsls'
    self.cpcmd  = 'rfcp'
    self.rmcmd  = 'nsrm -rf'
    self.mkdrcmd = 'rfmkdir -p'
    self.chmdcmd = 'rfchmod'
    self.chmdprm = '0777'
    self.chmdurl = ''
    self.tmpurl = '/tmp/$USER/'
    self.prefix = ""
    self.fsys   = "rfio://"
  

class IIHE_T2(StorageSystem):
  
  def __init__(self,path,verbose=False):
    super(Castor,self).__init__(path,verbose=verbose)
    self.cpcmd  = 'dccp -d 2'
    self.chmdcmd = 'chmod'
    self.chmdprm = '0777'
    self.chmdurl = ''
    self.tmpurl = '/scratch/$USER/'
    self.prefix = ""
    self.fsys   = "dcap://"
    # check /pnfs/iihe/'
    


class BatchSystem(object):
  __metaclass__ = ABCMeta
  
  def __init__(self,verbose=1):
    self.verbose = verbose
    
  def execute(self,cmd,dry=False):
    return execute(cmd,dry=dry,verbose=self.verbose)
    
  @abstractmethod
  def submit(self,script,**kwargs):
    """Submit a script with some optional parameters."""
    raise NotImplementedError("BatchSystem.submit is an abstract method.")
    
  @abstractmethod
  def status(self,**kwargs):
    """Check status of queued or running jobs."""
    raise NotImplementedError("BatchSystem.status is an abstract method.")
    

class HTCondor(BatchSystem):
  # https://research.cs.wisc.edu/htcondor/manual/quickstart.html
  # https://twiki.cern.ch/twiki/bin/view/ABPComputing/LxbatchHTCondor
  
  def __init__(self,verbose=False):
    super(HTCondor,self).__init__(verbose=verbose)
    # http://pages.cs.wisc.edu/~adesmet/status.html
    self.statusdict = { 1: 'q', 2: 'r', 3: 'f', 4: 'c', 5: 'f', 6: 'f' }
  
  def submit(self,script,**kwargs):
    #jobname   = kwargs.get('name',   'job'          )
    #queue     = kwargs.get('queue',  'microcentury' )
    taskargs  = kwargs.get('taskargs', None        )
    dry       = kwargs.get('dry',      False       )
    subcmd    = "condor_submit %s"%(script)
    if taskargs:
      subcmd += " -queue arg in %s"%(taskargs)
    return self.execute(subcmd,dry=dry)
  
  def queue(self,job,**kwargs):
    qcmd  = "condor_q"
    return self.execute(qcmd)
  
  def status(self,job,**kwargs):
    jobid   = str(job.jobid)
    if job.taskid>=0:
      jobid+= '.%s'%job.taskid
    subcmd  = "condor_q -wide %s"%(jobid)
    return self.execute(subcmd)
  
  def jobs(self,**kwargs):
    subcmd = "condor_q -format '%s ' Owner -format '%s ' ClusterId -format '%s ' ProcId -format '%s ' JobStatus -format '%s\n' Args"
    rows   = self.execute(subcmd)
    jobs   = JobList()
    if rows and self.verbose>=1:
      print ">>> %10s %10s %8s %8s   %s"%('user','jobid','taskid','status','args')
    for row in rows.split('\n'):
      values = row.split()
      if len(values)<5: continue
      user   = values[0]
      jobid  = values[1]
      taskid = values[2]
      status = self.statusdict.get(int(values[3]),'?')
      args   = ' '.join(values[4:])
      if self.verbose>=1:
        print ">>> %10s %10s %8s %8s   %s"%(user,jobid,taskid,status,args)
      job    = Job(self,jobid,args=args,status=status)
      jobs.append(job)
    return jobs
  

class JobList(object):
  """Job list container class."""
  
  def __init__(self,jobs=[ ]):
    self.jobs = jobs
  
  def __iter__(self):
    for job in self.jobs: yield job
  
  def __len__(self):
    return len(self.jobs)
  
  def append(self,job):
    self.jobs.append(job)
  
  def running(self):
    return [j for j in jobs if j.getStatus()=='r']
  
  def failed(self):
    return [j for j in jobs if j.getStatus()=='f']


class Job(object):
  """Job container class. Status:
  q=queued/pending/idle, r=running, f=failed, ?=unknown/missing"""
  
  def __init__(self,batch,jobid,**kwargs):
    self.batch  = batch
    self.jobid  = int(jobid) # or 'clusterid'
    self.taskid = int(kwargs.get('taskid', -1 )) # or 'procid'
    self.name   = kwargs.get('name',   "none" )
    self.args   = kwargs.get('args',   ""     )
    self.status = kwargs.get('status', None   )
    if self.status==None:
      self.getStatus()
    
  def getStatus(self):
    #status = self.batch.status(job)
    return self.status


tcol_dict = { 'black':  30,  'red':     31, 'green': 32,
              'yellow': 33,  'orange':  33, 'blue':  34,
              'purple': 35,  'magenta': 36, 'white': 37,
              'grey':   90,  'none':     0 }
bcol_dict = {k: (10+v if v else v) for k,v in tcol_dict.iteritems()}
def color(string,c='green',b=False,**kwargs):
  tcol_key   = kwargs.get('color',     c     )
  bcol_key   = kwargs.get('background','none')
  bold_code  = "\033[1m" if kwargs.get('bold',b) else ""
  tcol_code  = "\033[%dm"%tcol_dict[tcol_key] if tcol_key!='none' else ""
  bcol_code  = "\033[%dm"%bcol_dict[bcol_key] if bcol_key!='none' else ""
  stop_code  = "\033[0m"
  reset_code = stop_code if kwargs.get('reset',False) else ""
  return kwargs.get('pre',"") + reset_code + bcol_code + bold_code + tcol_code + string + stop_code

def warning(string,**kwargs):
  return color(kwargs.get('exclam',"Warning! ")+string, color="yellow", bold=True, pre=kwargs.get('pre',">>> "))

def error(string,**kwargs):
  return color(kwargs.get('exclam',"ERROR! ")+string, color="red", bold=True, pre=kwargs.get('pre',">>> "))

def green(string,**kwargs): return "\033[32m%s\033[0m"%string

def error(string,**kwargs): print ">>> \033[1m\033[91m%sERROR! %s\033[0m"%(kwargs.get('pre',""),string)

def warning(string,**kwargs): print ">>> \033[1m\033[93m%sWarning!\033[0m\033[93m %s\033[0m"%(kwargs.get('pre',""),string)

def bold(string): return "\033[1m%s\033[0m"%(string)




# verbose=0
# 
# #################S
# #Queues
# q_lxplus=cmscaf1nw
# q_T2=localgrid@cream01
# 
# #order: name stdout submit.sh
# submit_to_queue(){
#  if [ `working_on_lxplus` -eq 1 ]; then
#       # echo "bsub -q $q_lxplus -J $1 -eo $2 < $3"
#    bsub -q $q_lxplus -J $1 -eo $2 < $3
#  else
#      echo "bla"
#    #qsub -q $q_T2 -j oe -N $1 -o $2 $3
#  fi
# }
# 
# file_loc(){
#   file=$1
#   if [ `is_on_castor $file` -eq 1 ];then
#     echo "rfio://$file"
#   else
#     if [ `is_on_T2 $file` -eq 1 ];then
#       echo "${T2_FSYS}$file"
#     else if [ `is_on_eos $file` -eq 1 ];then
#       echo "${T2_FSYS}$file"
#     else
#       echo $file
#     fi ; fi
#   fi
# }
# 
# working_on_lxplus(){
#   if [ `uname -a | grep lxplus | wc -l` -gt 0 ];then echo 1
#   else echo 0
#   fi
# }
# 
# is_staged(){
#   status=`stager_qry -M $1`
#   if [ `echo $status|grep -e "STAGED"|wc -l` -eq 1 ] || [ `echo $status|grep -e "CANBEMIGR"|wc -l` -eq 1 ];then
#     echo 1
#   else
#     if [ `echo $status|grep -e "STAGEIN"|wc -l` -eq 1 ];then
#       echo 0
#     else
#       stager_get -M $1 &> /dev/null
#       echo 0
#     fi
#   fi
# }
# 
# stage_list_of_files(){
# 
#   need_to_wait=0
#   for file in $@;do
#     if [ `is_on_castor $file` -eq 1 ];then
#       if [ `nsls $file 2>&1|grep "No such"|wc -l` -eq 1 ]; then
#         echo "$file is not present on /castor !!" ; continue ;
#       fi 
#     
#       if [ `is_staged $file` -eq 0 ];then
#         echo "File $file is being staged."
#         need_to_wait=1
#       fi
#     fi
#   done
# 
#   if [ $need_to_wait -eq 1 ];then
#     echo "Waiting 5min before continuing !"
#     sleep 300
#     stage_list_of_files $@
#   fi
#   
#   echo "All files are finally staged ! Moving on ..."
# 
# }
# 
# stage_dir(){
#   if [ $# -ne 1 ];then echo "Takes as argument only 1 castor dir." ; exit ; fi
#   list=""
#   for file in `nsls $1`;do
#     list="$list $1/$file"  
#   done
#   stage_list_of_files $list
# }
# 