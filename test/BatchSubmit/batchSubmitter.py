#!/usr/bin/env python

import fileinput,sys,os,subprocess,re,time

## Change only this part

dataset = '/MinimumBias/Run2012C-PromptReco-v1/RECO'
remark = ''	# adds this string at the end of root filename	
runnum = sys.argv[1]
globalTag = 'GR_P_V40::All'
numEvents = '-1'
queue = '1nw'
destination = '/store/caf/user/jluetic/'
splitLen = 1	# number of root files from the original dataset in each job

submit = False	# run the script without submitting jobs (for testing)

## Setting enviroment

path=os.path.dirname(os.path.abspath(__file__))+'/'
#print 'Creating list of runs for: '+dataset
#return_code = subprocess.call("dbs search --query='find run where dataset="+dataset+" and run.numlss>50' > "+path+"/Run_list", shell=True)
#print return_code
#print 'Done.'

py = open(path+'Data_MinBias_RECO.py', 'r').read().split('\n')
script = open(path+'submit.sh','r').read().split('\n')

newDirName=path+runnum+remark
os.system('mkdir '+newDirName)
os.chdir(newDirName)

## Create file list
print 'Creating list of files for run: ' + runnum
sp = subprocess.call("dbs search --query='find file where dataset="+dataset+" and run="+runnum+"' > "+newDirName+"/File_list",shell=True)
print 'Done.'

File_List = open(newDirName+'/File_list','r').read().split('\n')  
File_List = File_List[4:-1] 

outputBase = 'Data_MB_RECO'
at = 1
for lines in range(0, len(File_List), splitLen):
    # First, get the list slice
    outputData = File_List[lines:lines+splitLen]
    # Now open the output file, join the new slice with newlines
    # and write it out. Then close the file.
    output =  newDirName+'/'+outputBase+ '_' + str(at) + '.py'
    tfile = open(output,'w')
    rootFN = 'pixelTree_MB'+remark+'_'+runnum+'_'+str(at)+'.root'
    for line in py:
		#tfile.write(re.sub('-1', '1000', line))
		tfile.write(re.sub('-1', numEvents, re.sub('#GLOBALTAG', '\"'+globalTag+'\"', line)))
                #tfile.write(line),
                if line.startswith('#FILENAMES'):
                        tfile.write('\n\t\'')
                        tfile.write(('\',\n\t\'').join(outputData))
                        tfile.write('\'\n')
		if line.startswith('rootFileName'):
			tfile.write('\"'+rootFN+'\"')
		tfile.write('\n')
    
    outputsh = newDirName+'/'+outputBase +'_'+str(at) + '.sh'
    
    nscript=open(outputsh,'w')
    for line in script:
   		#nscript.write(re.sub('XXXX', 'pixelTree_JetHT_'+runnum+'_'+str(at)+'.root', line)) 
   		#nscript.write(re.sub(r'/store/group/comm_pixel/PixelTree/r31/Collisions/','/store/caf/user/jluetic/',re.sub('XXXX', 'pixelTree_MB'+remark+'T_'+runnum+'_'+str(at)+'.root', line))) 
   		nscript.write(re.sub('DESTINATION',destination,re.sub('XXXX', rootFN, re.sub('PATH',path,line)))) 
		if line.startswith('cmsRun'):
			nscript.write(output)
		nscript.write('\n')

    tfile.close()
    nscript.close()

    command = 'bsub -R \"pool>30000\" -q '+queue+' -J '+str(at)+' < '+ outputsh
    if submit:
    	p = subprocess.call(command, shell=True)
    print(command)


    # Increment the counter
    at += 1



