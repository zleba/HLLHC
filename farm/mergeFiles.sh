fileDir=/nfs/dust/cms/user/zlebcr/hlLHC/cernFiles

names=`ls -d $fileDir/TT* | grep -o "\-[^_]*" | sed 's/-//' | sort -u`
echo $names
#exit

for n in $names
do
    echo $n
    files=`find $fileDir/*$n* -name "example0_ntuple.root"`
    hadd $fileDir/${n}.root $files
done
