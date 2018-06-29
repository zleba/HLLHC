#!/bin/zsh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_stored:$LD_LIBRARY_PATH
dirName=`grep  Outputdir  confgs/${1}.txt | sed 's/Outputdir *=//' |  tr -d '[:space:]' `

mkdir -p $dirName

./../ntupleQCD confgs/${1}.txt
