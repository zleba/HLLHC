fileDir=/nfs/dust/cms/user/zlebcr/hlLHC/desyFiles

names=`ls -d $fileDir/QCD_Pt-* | grep -o "\-[^_]*" | sed 's/-//' | sort -u`

for n in $names
do
    echo $n
    files=`find $fileDir/*$n* -name "example0_ntuple.root"`
    hadd $fileDir/${n}.root $files
done
