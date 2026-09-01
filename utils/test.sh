#!/bin/bash

make || echo "No makefile found" exit 1
declare -i total=0
declare -i failures=0
declare -i success=0
declare -i exec_fail=0
declare -i creat_fail=0

while IFS= read -r line;do
    total+=1
	echo -n "$line:			"
    if  ./woody_woodpacker "$line" > create_error.log 2>&1; then
        echo  -n "[CREATION] SUCCESS"
        timeout 1 bash -c 'exec -a "$0" ./woody "$1"' "TEST_FOR_$(basename "$line")" "--help"< /dev/null > woody-out 2>&1
        echo "....WOODY...." > command-out  2>&1
        timeout 1 bash -c 'exec -a "$0" "$1" "$2"' "TEST_FOR_$(basename "$line")" "$line" "--help"< /dev/null >> command-out 2>&1
        if  diff woody-out command-out > /dev/null 2>&1 ; then
	        echo "	[EXECUTION] SUCCESS"
            success+=1
        else
	        echo "	[EXECUTION] FAILURE"
			diff woody-out command-out
            failures+=1
			exec_fail+=1
        fi
    else
		echo -n "$line:		" >> create_error.log
        echo "[CREATION] FAILURE"
        failures+=1
		creat_fail+=1
    fi
done < ./utils/test-64-bits.txt

echo
echo "Ran $total tests"
percent=$(( success * 100 / total ))
echo "SUCCESS: $success/$total [ $percent% ]"
percent=$(( failures * 100 / total ))
echo "FAILURES: $failures/$total [ $percent% ]"
echo "	CREATION FAILURES: $creat_fail"
echo "	EXECUTION FAILURES: $exec_fail"

