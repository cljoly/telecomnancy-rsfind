#!/usr/bin/env bash

# You may pass a range of test like 1:3 or 9 as first argument
# test.sh 1:3 will run test 1 and 2 (not 3)

data="test_data.json"
name="name"
command="echo"
out_radix=/tmp/rstest_
test_range=$1

# Number of test passing / failing
nb_test=0
nb_fail_test=0

RED_BG='\033[1;41m'
GRN_BG='\033[0;42m'
RED='\033[1;31m'
GRN='\033[0;32m'
YLW='\033[0;33m'
REY='\033[0;34m'
NC='\033[0m' # No Color

retcode=0
for i in $(jq -r ".commands | keys | .[${test_range}] | numbers,(.[]?|numbers)" < $data); do
    printf "${REY}=========================${NC}\n"
    for folder_i in $(seq 0 $(jq -r '.folders | length - 1' < $data)); do
        printf "${REY}-------------------------${NC}\n"
        folder=$(jq -r ".folders[$folder_i]" <$data)
        jq_args="-r --arg jqfolder $folder --argjson i $i"

        cmd_a=$(jq $jq_args '.commands[$i].a | gsub("\\$folder";$jqfolder)' < $data)
        cmd_b=$(jq $jq_args '.commands[$i].b | gsub("\\$folder";$jqfolder)' < $data)
        id=$(echo $cmd_a $cmd_b | md5sum | cut --bytes 1-10)
        out_a="${out_radix}_${i}_${id}_a"
        out_b="${out_radix}_${i}_${id}_b"
        sh -c "$cmd_a" >"$out_a" 2>"${out_a}_err"
        # $cmd_a >"$out_a" 2>"${out_a}_err"
        # Check return code, 0 by default
        if [ $? -ne $(jq -r ".commands[${i}].return_code // 0" < $data) ]; then
            echo -e "${RED}☠️ Non expected exit status${NC}"
            echo NON EXPECTED EXIT STATUS >> $out_a
        fi
        sh -c "$cmd_b" >"$out_b" 2>"${out_a}_err"
        # $cmd_b >"$out_b" 2>"${out_a}_err"

        nb_test=`expr $nb_test + 1`
        cmp "$out_a" "$out_b" 2>/dev/null >/dev/null
        equal=$?
        if [ $equal -eq 0 ]; then
            printf "${GRN}TEST $i.$folder_i: ✅${NC} ${cmd_a} = ${cmd_b}\n"
            # Should print nothing
            diff $out_a $out_b
        else
            nb_fail_test=`expr $nb_fail_test + 1`
            printf "${RED}TEST $i.$folder_i: ❌${NC} ${cmd_a} ≠ ${cmd_b}\n"
            printf "${YLW}diff $out_a $out_b${NC}\n"
            if [ ${DIFF:-0} -eq 1 ]; then
                diff $out_a $out_b
            fi
            echo $STOP
            if [ ${STOP:-0} -eq 1 ]; then
                exit $retcode
            fi
        fi
    done
done

if [ $nb_fail_test -eq 0 ]; then
    echo -e "🎉 ${GRN_BG} ${nb_test}/${nb_test} passing ${NC}"
else
    echo -e "🔥 ${RED_BG} ${nb_fail_test}/${nb_test} failing ${NC}"
fi
for i in $(seq 1 $nb_fail_test); do
    printf "🔥"
done
echo ""
exit $retcode
