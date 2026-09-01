#!/bin/bash

make || echo "No makefile found" exit 1
declare -i total=0
declare -i failures=0
declare -i success=0
declare -i exec_fail=0
declare -i creat_fail=0

mkdir -p ./utils/logs/
while IFS= read -r line;do
    total+=1
	printf "%-50s" "$line"
    if  ./woody_woodpacker "$line" > ./utils/logs/create_error.log 2>&1; then
        echo  -n "[CREATION] SUCCESS"
        timeout 1 bash -c 'exec -a "$0" ./woody "$1"' "TEST_FOR_$(basename "$line")" "--help"< /dev/null > woody-out 2>&1
        echo "....WOODY...." > command-out  2>&1
        timeout 1 bash -c 'exec -a "$0" "$1" "$2"' "TEST_FOR_$(basename "$line")" "$line" "--help"< /dev/null >> command-out 2>&1
        if  diff woody-out command-out > /dev/null 2>&1 ; then
	        echo "	[EXECUTION] SUCCESS"
            success+=1
        else
	        echo "	[EXECUTION] FAILURE"
			diff woody-out command-out > ./utils/logs/exec_fail_"$(basename "$line")".log
            failures+=1
			exec_fail+=1
        fi
    else
        echo "[CREATION] FAILURE"
        failures+=1
		creat_fail+=1
    fi
done < ./utils/test-64-bits.txt

rm woody-out command-out

echo
echo "Ran $total tests"
percent=$(( success * 100 / total ))
echo "SUCCESS: $success/$total [ $percent% ]"
percent=$(( failures * 100 / total ))
echo "FAILURES: $failures/$total [ $percent% ]"
percent=$(( creat_fail * 100 / total ))
echo "	CREATION FAILURES: $creat_fail  [ $percent% of total tests ]"
percent=$(( exec_fail * 100 / total ))
echo "	EXECUTION FAILURES: $exec_fail [ $percent% of total tests ]"

