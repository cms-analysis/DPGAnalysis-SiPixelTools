#!/bin/bash
obj=$1
#root -l macros/run_new_layer4_janos_mod_barrel.C\(\"$file\"\)
if [[ -f $obj ]];then
    if [[ $2 = res ]]; then
	root -l macros/res_test.C\(\"$obj\"\) -b -q
    else
	root -l run_new_RMSvsZ_JustPlot.C\(\"$obj\"\) -b -q
    fi
else
    if [[ $2 = res ]]; then
	for file in $obj/*.root; do
	    root -l macros/res_test.C\(\"$file\"\) -b -q
	done
    else
	for file in $obj/*.root; do
            root -l run_new_RMSvsZ_JustPlot.C\(\"$file\"\) -b -q
        done
    fi
fi
