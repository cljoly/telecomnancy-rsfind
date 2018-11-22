#!/usr/bin/env bash

folder="."
name="name"
command="echo"
rsfind_bin=$1
rsfind=(
# 1
"echo Testing test script"
# 2
"$rsfind_bin"
# 3
"$rsfind_bin $folder"
# 4
"$rsfind_bin $folder --name $name"
# 5
"$rsfind_bin $folder --print"
# 6
"$rsfind_bin $folder -t $name"
# 7
"$rsfind_bin $folder -i"
# 8
"$rsfind_bin $folder -l"
# 9
"$rsfind_bin $folder --exec \"$command\""
# 10
"echo Fin des tests 🎉"
)
origf=(
# 1
"echo Testing test script"
# 2
"find"
# 3
"find $folder"
# 4
"find $folder -name $name"
# 5
"find $folder -print"
# 6
"grep -r -l $folder -e $name"
# 7
"find $folder -type f -exec file \; | grep image | cut -d : -f 1"
# 8
"find $folder -exec ls -l -d {} \\;"
# 9
"find $folder -exec \"$command\" \;"
# 10
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

    echo "'${cmd_a}' CMD_A"
    echo "'${cmd_b}' CMD_B"
  $cmd_a >$out_a 2>/dev/null
  $cmd_b >$out_b 2>/dev/null

  cmp $out_a $out_b 2>/dev/null >/dev/null
  equal=$?
  if [ $equal -eq 0 ]; then
    printf "${GRN}TEST $i: ${cmd_a} ✅${NC}\n"
    diff $out_a $out_b
  else
    printf "${RED}TEST $i: ❌${NC} ${cmd_a}\n"
    echo "diff $out_a $out_b"
    # diff $a $b
    retcode=1
  fi
done
exit $retcode
