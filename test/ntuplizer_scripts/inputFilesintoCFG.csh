#!/bin/csh
set limit=$1
echo "Will write $limit files in each template ..."

rm -f template*

sed '1,/INPUTFILE/d'<pixelNtuplizer_RealData_template>Btemplate_cfg.py
sed '/INPUTFILE/,$ d' <pixelNtuplizer_RealData_template>Atemplate_cfg.py

cp Atemplate_cfg.py template1_cfg.py

set i=1
set linenumber=0
set totallines=0
set totallinenumber=0

foreach fileline ( `more $2` )
  @ totallines++
end

foreach fileline ( `more $2` )
  @ linenumber++
  @ totallinenumber++
  
  if (($linenumber != $limit) && ($totallinenumber != $totallines)) then
  echo $fileline,>>template${i}_cfg.py
  endif
  
  if (($linenumber == $limit) && ($totallinenumber != $totallines)) then
  echo $fileline>>template${i}_cfg.py
  cat Btemplate_cfg.py>>template${i}_cfg.py
  echo template${i}_cfg.py done !
  @ i++
  cp Atemplate_cfg.py template${i}_cfg.py
  set linenumber=0
  endif
  
  if (($totallinenumber == $totallines)) then
  echo $fileline>>template${i}_cfg.py
  cat Btemplate_cfg.py>>template${i}_cfg.py
  echo template${i}_cfg.py done !
  endif 
end

rm -f Btemplate_cfg.py
rm -f Atemplate_cfg.py





