# Author: Izaak Neutelings (February 2020)
import os, re, shutil, json
from fnmatch import fnmatch # for glob pattern
from utils import *
from abc import ABCMeta, abstractmethod
from subprocess import Popen, PIPE, STDOUT, CalledProcessError


def getconfig(run,verb=False):
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
  if verb>=1:
    print '-'*80
    print ">>> Reading config JSON file '%s'"%jsonconfig
    for key in ['run', 'indir', 'outdir', 'calib_payload']:
      print ">>> %-10s = '%s'"%(key,cfgdict[key])
    print '-'*80
  return cfgdict
  

def writetemplate(templatename,outfilename,sublist=[],rmlist=[],applist=[],key='$',**kwargs):
  """Write file from template."""
  sublist = [(re.compile("%s%s(?!\w)"%(re.escape(key),p)),str(v)) for p, v in sublist]
  with open(templatename,'r') as template:
    with open(outfilename,'w') as file:
      for line in template.readlines():
        #linenum = "L%d:"%i
        if any(r in line for r in rmlist):
          continue # skip line
        for regexp, value in sublist:
          #pattern = '$'+pattern
          if regexp.search(line):
            #line = line.replace(pattern,str(value))
            line = regexp.sub(value,line)
        file.write(line)
      for line in applist:
        file.write(line+'\n') # append
  

def ensuredir(dirname,empty=False,verb=0):
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
  
def ensurefile(*paths,**kwargs):
  """Ensure file exists."""
  fatal = kwargs.get('fatal',True)
  path = os.path.join(*paths)
  if not os.path.isfile(path):
    if fatal:
      raise IOError("Did not find file %s."%(path))
    else:
      print ">>> Warning! Did not find file %s."%(path)
  return path
  
def rmfile(filepaths):
  """Remove (list of) files."""
  if isinstance(filepaths,str):
    filepaths = [filepaths]
  for filepath in filepaths:
    if os.path.isfile(filepath):
      os.unlink(filepath)
  
def isglob(string):
  """Return if string is likely a glob pattern."""
  return '*' in string or '?' in string or ('[' in string and ']' in string)
  

def execute(command,dry=False,fatal=True,verb=0):
  """Execute shell command."""
  command = str(command)
  out = ""
  if dry:
    print ">>> Dry run: %r"%(command)
  else:
    if verb>=1:
      print ">>> Executing: %r"%(command)
    try:
      #process = Popen(command.split(),stdout=PIPE,stderr=STDOUT) #,shell=True)
      process = Popen(command,stdout=PIPE,stderr=STDOUT,bufsize=1,shell=True) #,universal_newlines=True
      for line in iter(process.stdout.readline,""):
        if verb>=1: # real time print out (does not work for python scripts without flush)
          print line.rstrip()
        out += line
      process.stdout.close()
      retcode = process.wait()
      ##print 0, process.communicate()
      ##out     = process.stdout.read()
      ##err     = process.stderr.read()
      ##print out
      out = out.strip()
    except Exception as e:
      if verb<1:
        print out #">>> Output: %s"%(out)
      print ">>> Failed: %r"%(command)
      raise e
    if retcode and fatal:
      if verb<1:
        print out
      raise CalledProcessError(retcode,command)
      #raise Exception("Command '%s' ended with return code %s"%(command,retcode)) #,err)
  return out
  

def getstorage(path):
  if '/castor/cern.ch/' in path: return Castor(path)
  if '/pnfs/iihe/' in path: return T2_IIHE(path)
  if '/pnfs/psi.ch/' in path: return T3_PSI(path)
  if '/store/' in path or '/eos/' in path: return EOS(path)
  return StorageSystem(path)
  

class StorageSystem(object):
  
  def __init__(self,path,verb=0,ensure=False):
    self.path    = path
    self.lscmd   = 'ls'
    self.lsurl   = ''
    self.cdcmd   = 'cd'
    self.cdurl   = ''
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
    self.tmpdir  = '/tmp/$USER/' # $TMPDIR # mounted temporary directory
    self.fileurl = ""
    self.verbosity = verb
    if path.startswith('/'):
      self.parent = '/'.join(path.split('/')[:3])
    else:
      self.parent = '/'+'/'.join(path.split('/')[:2])
    self.mounted  = os.path.exists(self.parent)
  
  def __str__(self):
    return self.path
  
  def __repr__(self):
    return '<%s("%s") at %s>'%(self.__class__.__name__,self.path,hex(id(self)))
  
  def execute(self,cmd,**kwargs):
    kwargs.setdefault('verb',self.verbosity)
    return execute(cmd,**kwargs)
  
  def expandpath(self,*paths,**kwargs):
    """Help function to replace variables in given path, or set default to own path."""
    #verb = kwargs.get('verb',self.verbosity)
    here  = kwargs.get('here',False)
    url   = kwargs.get('url', "")
    paths = [p for p in paths if p]
    if paths:
      path = os.path.join(*paths)
    else:
      path = self.path
    if here and path[0] not in ['/','$']:
      path = os.path.join(self.path,path)
    if url and ('$PATH' in path or path.startswith(self.parent)):
      path = url+path
    path = path.replace('$PATH',self.path)
    return path
  
  def file(self,*paths,**kwargs):
    """Ensure that a given file exists, and append a file URL if needed."""
    ensure = kwargs.get('ensure',False)
    path   = self.expandpath(*paths,here=True)
    if path.startswith(self.parent):
      path = self.fileurl + path
    if ensure:
      if not self.exists(path):
        LOG.throw(IOError,"Did not find %s."%(path))
    return path
  
  def exists(self,*paths,**kwargs):
    """Ensure that a given path exists."""
    verb = kwargs.get('verb',self.verbosity)
    path = self.expandpath(*paths,here=True)
    cmd  = "if `%s %s%s >/dev/null 2>&1`; then echo 1; else echo 0; fi"%(self.lscmd,self.lsurl,path)
    out  = self.execute(cmd,verb=verb).strip()
    return out=='1'
  
  def ensuredir(self,*paths,**kwargs):
    """Ensure path exists."""
    verb = kwargs.get('verb',self.verbosity)
    path = self.expandpath(*paths)
    if not self.exists(path,verb=verb):
      self.mkdir(path,verb=verb)
    return True
  
  def cd(self,*paths,**kwargs):
    """Change directory if mounted."""
    if self.mounted:
      #verb = kwargs.get('verb',self.verbosity)
      path = self.expandpath(*paths)
      ret  = os.chdir(path)
      #ret  = self.execute("%s %s%s"%(self.cdcmd,self.cdurl,path)).split('\n')
    return ret
  
  def ls(self,*paths,**kwargs):
    """List contents of given directory."""
    verb    = kwargs.get('verb',self.verbosity)
    dryrun  = kwargs.get('dry', False)
    filter  = kwargs.get('filter',None) # filter with glob pattern, like '*' or '[0-9]' wildcards
    path    = self.expandpath(*paths)
    retlist = self.execute("%s %s%s"%(self.lscmd,self.lsurl,path),fatal=False,dry=dryrun,verb=verb).split('\n')
    if retlist and 'No such file or directory' in retlist[0]:
      LOG.warning(retlist[0])
      retlist = [ ]
    elif filter:
      for file in retlist[:]:
        if not fnmatch(file,filter):
          retlist.remove(file)
    return retlist
  
  def getfiles(self,*paths,**kwargs):
    """Get list of files in a given path.
    Return list of files with full path name, and if needed, a file URL.
    Use the 'filter' option to filter the list of file names with some pattern."""
    verb     = kwargs.get('verb',self.verbosity)
    fileurl  = kwargs.get('url', self.fileurl)
    filter   = kwargs.get('filter',None) # filter with glob pattern, like '*' or '[0-9]' wildcards
    path     = self.expandpath(*paths)
    filelist = self.ls(path,**kwargs)
    if fileurl and path.startswith(self.parent):
      if not isinstance(fileurl,basestring):
        fileurl = self.fileurl
    else:
      fileurl = ""
    for i, file in enumerate(filelist):
      if filter and not fnmatch(file,filter): continue
      filelist[i] = fileurl+os.path.join(path,file)
    return filelist
  
  def cp(self,source,target=None,**kwargs):
    """Copy files."""
    dryrun = kwargs.get('dry', False)
    verb   = kwargs.get('verb',self.verbosity)
    source = self.expandpath(source,url=self.cpurl)
    target = self.expandpath(target,url=self.cpurl)
    return self.execute("%s %s %s"%(self.cpcmd,source,target),dry=dryrun,verb=verb)
  
  def hadd(self,sources,target,**kwargs):
    """Hadd files. Create intermediate target file if needed."""
    target  = self.expandpath(target,here=True)
    dryrun  = kwargs.get('dry',    False)
    verb    = kwargs.get('verb',   self.verbosity)
    fileurl = kwargs.get('url',    self.fileurl)
    tmpdir  = kwargs.get('tmpdir', target.startswith(self.parent) and self.cpurl!='')
    htarget = target
    if tmpdir:
      if not isinstance(tmpdir,str):
        tmpdir = self.tmpdir
      tmpdir  = ensuredir(tmpdir,verb=verb)
      htarget = os.path.join(tmpdir,os.path.basename(target))
    if isinstance(sources,basestring):
      sources = [ sources ]
    source = ""
    for i, file in enumerate(sources,1):
      fname = os.path.basename(file)
      if '$PATH' in file and fileurl and isglob(fname): # expand glob pattern
        parent = os.path.dirname(file)
        files  = self.getfiles(parent,filter=fname,url=fileurl)
        source += ' '.join(files)+' '
      else:
        source += self.expandpath(file,url=fileurl)+' '
    source = source.strip()
    if verb>=2:
      print ">>> %-10s = %r"%('sources',sources)
      print ">>> %-10s = %r"%('source',source)
      print ">>> %-10s = %r"%('target',target)
      print ">>> %-10s = %r"%('htarget',htarget)
    out = self.execute("%s %s %s"%(self.haddcmd,htarget,source),dry=dryrun,verb=verb)
    if tmpdir:
      cpout = self.cp(htarget,target,dry=dryrun,verb=verb)
      if not dryrun:
        rmfile(htarget)
    return out
  
  def rm(self,*paths,**kwargs):
    """Remove given file or director."""
    path = self.expandpath(*paths,here=True)
    verb = kwargs.get('verb',self.verbosity)
    return self.execute("%s %s%s"%(self.rmcmd,self.rmurl,path),verb=verb)
  
  def mkdir(self,dirname='$PATH',**kwargs):
    verb    = kwargs.get('verb',self.verbosity)
    dirname = self.expandpath(dirname,here=True)
    return self.execute("%s %s%s"%(self.mkdrcmd,self.mkdrurl,dirname),verb=verb)
  
  def chmod(self,file,perm=None,**kwargs):
    verb = kwargs.get('verb',self.verbosity)
    if not perm: perm = self.chmdprm
    return self.execute("%s %s %s%s"%(self.chmdcmd,perm,self.chmdurl,file),verb=verb)
  

class Local(StorageSystem):
  
  def __init__(self,path,verb=0,ensure=False):
    super(Local,self).__init__(path,verb=verb,ensure=ensure)
    if ensure:
      self.ensuredir(self.path,verb=verb)
  

class EOS(StorageSystem):
  
  def __init__(self,path,verb=False,ensure=False):
    super(EOS,self).__init__(path,verb=verb)
    if not self.mounted: # EOS is mounted on lxplus
      # https://cern.service-now.com/service-portal?id=kb_article&n=KB0001998
      self.cpcmd   = 'xrdcp -f'
      self.chmdprm = '2777'
      self.cpurl   = "root://eoscms.cern.ch/"
      self.fileurl = "root://eoscms/"
      #self.prefix  = "root://eoscms.cern.ch/"
    self.tmpurl    = '/tmp/$USER/'
    if ensure:
      self.ensuredir(self.path)
  

class Castor(StorageSystem):
  
  def __init__(self,path,verb=False,ensure=False):
    super(Castor,self).__init__(path,verb=verb,ensure=ensure)
    self.lscmd   = 'nsls'
    self.cpcmd   = 'rfcp'
    self.rmcmd   = 'nsrm -rf'
    self.mkdrcmd = 'rfmkdir -p'
    self.chmdcmd = 'rfchmod'
    self.chmdprm = '0777'
    self.chmdurl = ''
    self.tmpurl  = '/tmp/$USER/'
    self.prefix  = ""
    self.fileurl = "rfio://"
    if ensure:
      self.ensuredir(self.path)
  

class T2_IIHE(StorageSystem):
  
  def __init__(self,path,verb=False,ensure=False):
    super(Castor,self).__init__(path,verb=verb,ensure=ensure)
    self.cpcmd   = 'dccp -d 2'
    self.chmdcmd = 'chmod'
    self.chmdprm = '0777'
    self.chmdurl = ''
    self.tmpurl  = '/scratch/$USER/'
    self.prefix  = ""
    self.fileurl = "dcap://"
    # check /pnfs/iihe/'
    if ensure:
      self.ensuredir(self.path)
  

class T3_PSI(StorageSystem):
  
  def __init__(self,path,verb=0,ensure=False):
    super(T3_PSI,self).__init__(path,verb=verb,ensure=False)
    self.rmcmd   = 'uberftp -rm'
    self.rmurl   = 'gsiftp://t3se01.psi.ch/'
    self.mkdrcmd = "LD_LIBRARY_PATH='' PYTHONPATH='' gfal-mkdir -p"
    self.mkdrurl = 'gsiftp://t3se01.psi.ch/'
    self.cpcmd   = 'xrdcp -f'
    self.cpurl   = "root://t3dcachedb03.psi.ch/"
    self.tmpdir  = '/scratch/$USER/'
    self.fileurl = "root://t3dcachedb03.psi.ch/"
    if ensure:
      self.ensuredir(self.path)
  

class BatchSystem(object):
  __metaclass__ = ABCMeta
  
  def __init__(self,verb=1):
    self.verbosity = verb
    
  def execute(self,cmd,dry=False,verb=None):
    if verb==None:
      verb = self.verbosity
    return execute(cmd,dry=dry,verb=verb)
  
  @abstractmethod
  def submit(self,script,**kwargs):
    """Submit a script with some optional parameters."""
    raise NotImplementedError("BatchSystem.submit is an abstract method.")
  
  @abstractmethod
  def status(self,**kwargs):
    """Check status of queued or running jobs."""
    raise NotImplementedError("BatchSystem.status is an abstract method.")
  
  @abstractmethod
  def jobs(self,**kwargs):
    """Get job status, return JobList object."""
    raise NotImplementedError("BatchSystem.jobs is an abstract method.")
  

class HTCondor(BatchSystem):
  # https://research.cs.wisc.edu/htcondor/manual/quickstart.html
  # https://twiki.cern.ch/twiki/bin/view/ABPComputing/LxbatchHTCondor
  
  def __init__(self,verb=False):
    super(HTCondor,self).__init__(verb=verb)
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
    """Get job status, return JobList object."""
    subcmd = "condor_q -format '%s ' Owner -format '%s ' ClusterId -format '%s ' ProcId -format '%s ' JobStatus -format '%s\n' Args"
    rows   = self.execute(subcmd,verb=self.verbosity-1)
    jobs   = JobList()
    if rows and self.verbosity>=1:
      print ">>> %10s %10s %8s %8s   %s"%('user','jobid','taskid','status','args')
    for row in rows.split('\n'):
      values = row.split()
      if len(values)<5: continue
      user   = values[0]
      jobid  = values[1]
      taskid = values[2]
      status = self.statusdict.get(int(values[3]),'?')
      args   = ' '.join(values[4:])
      if self.verbosity>=1:
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
    return [j for j in jobs if j.getstatus()=='r']
  
  def failed(self):
    return [j for j in jobs if j.getstatus()=='f']
  

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
      self.getstatus()
  
  def getstatus(self):
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

