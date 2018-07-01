fileDir=root://cmseos.fnal.gov//store/user/snowmass/noreplica/YR_Delphes/Delphes342pre15/
outDir=/nfs/dust/cms/user/zlebcr/hlLHC/fnalFiles

iFile=0
for  dirName in `ls fileLists`
do
    echo $dirName
    for fName in `cat fileLists/$dirName`
    do
        #echo $fName
        dirNameWhole=$fileDir/$dirName
        outDirNow=$outDir/${fName%.*}
        sed  's|INDIR|'$dirNameWhole'|g;s|INFILE|'$fName'|g;s|OUTDIR|'$outDirNow'|g;s|OUTFILE|'$fName'|g' baseConf.txt > confgs/${iFile}.txt
        ((++iFile))

    done
done
