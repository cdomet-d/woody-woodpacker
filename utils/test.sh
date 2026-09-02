#!/bin/bash

make || exit 1

declare -i total=0
declare -i failures=0
declare -i success=0
declare -i exec_fail=0
declare -i creat_fail=0

mkdir -p ./utils/logs/
rm ./utils/logs/*
while IFS= read -r line; do
	total+=1
	printf "%-50s" "$line"
	if ./woody_woodpacker "$line" < /dev/null > /dev/null 2>&1; then
		echo -n "[CREATION] SUCCESS"
		timeout 1 bash -c 'exec -a "$0" ./woody "$1"' "TEST_FOR_$(basename "$line")" "--help" </dev/null >utils/woody-out 2>&1
		echo "....WOODY...." >utils/command-out 2>&1
		timeout 1 bash -c 'exec -a "$0" "$1" "$2"' "TEST_FOR_$(basename "$line")" "$line" "--help" </dev/null >>utils/command-out 2>&1
		sleep 0.2
		if diff utils/woody-out utils/command-out >/dev/null 2>&1; then
			echo "	[EXECUTION] SUCCESS"
			success+=1
		else
			echo "	[EXECUTION] FAILURE"
			diff utils/woody-out utils/command-out >./utils/logs/exec_fail_"$(basename "$line")".log
			failures+=1
			exec_fail+=1
		fi
	else
		echo "[CREATION] SUCCESS"
		failures+=1
		creat_fail+=1
	fi
done <./utils/home.txt

rm utils/woody-out utils/command-out

echo
echo "Ran $total tests"
percent=$((success * 100 / total))
echo "SUCCESS: $success/$total [ $percent% ]"
percent=$((failures * 100 / total))
echo "FAILURES: $failures/$total [ $percent% ]"
percent=$((creat_fail * 100 / total))
echo "	CREATION FAILURES: $creat_fail  [ $percent% of total tests ]"
percent=$((exec_fail * 100 / total))
echo "	EXECUTION FAILURES: $exec_fail [ $percent% of total tests ]"
