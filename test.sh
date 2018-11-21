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
"$rsfind_bin $folder --print"
"$rsfind_bin $folder -t $name"
"$rsfind_bin $folder -i"
"$rsfind_bin $folder -l"
"$rsfind_bin $folder --exec \"$command\""
"echo Fin des tests 🎉"
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
"echo Fin des tests 🎉"
)
a=/tmp/rstest_a
b=/tmp/rstest_b

RED='\033[1;31m'
GRN='\033[0;32m'
NC='\033[0m' # No Color

retcode=0
for i in $(seq 0 ${#rsfind[@]}); do
    cmd_a=${rsfind[$i]}
    cmd_b=${origf[$i]}
    id=$(echo ${rsfind[$i]} ${origf[$i]} | md5sum | cut --bytes 1-10)
    out_a="${a}_${id}"
    out_b="${b}_${id}"

    echo "${cmd_a} CMD_A"
    echo "${cmd_b} CMD_B"
  $cmd_a >$out_a
  $cmd_b >$out_b

  cmp $out_a $out_b 2>/dev/null >/dev/null
  equal=$?
  if [ $equal -eq 0 ]; then
    printf "${GRN}TEST $i: ${cmd_a} ✅${NC}\n"
    diff $out_a $out_b
  else
    printf "${RED}TEST $i: ❌${NC} ${cmd_a}\n"
    # diff $a $b
    retcode=1
  fi
done
exit $retcode
