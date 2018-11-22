#!/usr/bin/env bash

data="test_data.json"
name="name"
command="echo"
rsfind_bin=$1
out_radix=/tmp/rstest_

RED='\033[1;31m'
GRN='\033[0;32m'
YLW='\033[0;33m'
NC='\033[0m' # No Color

retcode=0
for i in $(seq 0 $(jq -r '.commands | length - 1' < $data)); do
    for folder_i in $(seq 0 $(jq -r '.folders | length - 1' < $data)); do
        folder=$(jq -r ".folders[$folder_i]" <$data)
        jq_args="-r --arg jqfolder $folder --argjson i $i"

        cmd_a=$(jq $jq_args '.commands[$i].a | gsub("\\$folder";$jqfolder)' < $data)
        cmd_b=$(jq $jq_args '.commands[$i].b | gsub("\\$folder";$jqfolder)' < $data)
        id=$(echo $cmd_a $cmd_b | md5sum | cut --bytes 1-10)
        out_a="${out_radix}_${i}_${id}_a"
        out_b="${out_radix}_${i}_${id}_b"
        sh -c "$cmd_a" >"$out_a" 2>"${out_a}_err"
        sh -c "$cmd_b" >"$out_b" 2>"${out_a}_err"

        cmp "$out_a" "$out_b" 2>/dev/null >/dev/null
        equal=$?
        if [ $equal -eq 0 ]; then
            printf "${GRN}TEST $i.$folder_i: ✅${NC} ${cmd_a} = ${cmd_b}\n"
            # Should print nothing
            diff $out_a $out_b
        else
            printf "${RED}TEST $i.$folder_i: ❌${NC} ${cmd_a} ≠ ${cmd_b}\n"
            printf "${YLW}diff $out_a $out_b${NC}\n"
            retcode=1
        fi
    done
done
exit $retcode
