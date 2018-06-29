#!/bin/zsh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_stored:$LD_LIBRARY_PATH
./../ntupleQCD confgs/${1}.txt
