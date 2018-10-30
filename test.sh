#!/usr/bin/env bash

folder="."
name="name"
command="echo"
rsfind_bin=$1
rsfind=(
"echo Testing test script"
"$rsfind_bin"
"$rsfind_bin $folder"
"$rsfind_bin $folder --name $name"
"$rsfind $folder --print"
"$rsfind $folder -t $name"
"$rsfind $folder -i"
"$rsfind $folder -l"
"$rsfind $folder --exec \"$command\""
)
origf=(
"echo Testing test script"
"find"
"find $folder"
"find $folder -name $name"
"find $folder -print"
"grep -r -l $folder -e $name"
"find $folder -type f -exec file \; | grep image | cut -d : -f 1"
"find $folder -l"
"find $folder -exec \"$command\" \;"
)
a=/tmp/rstest_a
b=/tmp/rstest_b

RED='\033[1;31m'
GRN='\033[0;32m'
NC='\033[0m' # No Color

retcode=0
for i in $(seq 0 3); do
  ${rsfind[$i]} >$a
  ${origf[$i]} >$b

  cmp $a $b 2>/dev/null >/dev/null
  equal=$?
  if [ $equal -eq 0 ]; then
    printf "${GRN}TEST $i: ✅${NC}\n"
    diff $a $b
  else
    printf "${RED}TEST $i: ❌${NC} ${rsfind[$i]}\n"
    diff $a $b
    retcode=1
    exit $retcode
  fi
  rm $a $b
done
exit $retcode
