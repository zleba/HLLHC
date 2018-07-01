fileDir=/pnfs/desy.de/cms/tier2/store/group/upgrade/delphes_output/YR_Delphes/Delphes342pre15_hadd/
outDir=/nfs/dust/cms/user/zlebcr/hlLHC/desyFiles


iFile=0
for  dirName in $fileDir/QCD_Pt-*
do
    echo $dirName
    for fName in `ls $dirName`
    do
        #echo $fName
        outDirNow=$outDir/${fName%.*}
        sed  's|INDIR|'$dirName'|g;s|INFILE|'$fName'|g;s|OUTDIR|'$outDirNow'|g;s|OUTFILE|'$fName'|g' baseConf.txt > confgs/${iFile}.txt
        ((++iFile))

    done
done
