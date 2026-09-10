#!/bin/bash

make || exit 1

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BOLD='\033[1m'
NC='\033[0m' # No Color

declare -i total=0
declare -i fail=0
declare -i success=0
declare -i exec_fail=0
declare -i creat_fail=0

mkdir -p ./tests/.logs/
rm ./tests/.logs/*
while IFS= read -r line; do
    total+=1
	if [ -z "$line" ]; then
    	printf "\n${BOLD}%-40s ${NC}\n" "XFAILS"
		continue
	fi
    printf "✧ %-40s" "$line"
    if ./woody_woodpacker "$line" < /dev/null > /dev/null; then
        echo -ne "${GREEN}[CREATE] ✔︎${NC}"
        timeout 1 bash -c 'exec -a "$0" ./woody "$1"' "TEST_FOR_$(basename "$line")" "--help" </dev/null >tests/woody-out 2>&1
        timeout 1 bash -c 'exec -a "$0" "$1" "$2"' "TEST_FOR_$(basename "$line")" "$line" "--help" </dev/null >tests/command-out 2>&1
        sleep 0.2
        if diff <(tail -n +3 tests/woody-out) tests/command-out >/dev/null 2>&1; then
            echo -e "  ${GREEN}[EXEC] ✔︎${NC}"
            success+=1
        else
            echo -e "  ${RED}[EXEC] FAILURE${NC}"
            diff -y <(tail -n +3 tests/woody-out) tests/command-out >./tests/.logs/exec_fail_"$(basename "$line")".log
            readelf -h "$line" > ./tests/.logs/readelf_"$(basename "$line")".log
            fail+=1
            exec_fail+=1
        fi
    else
        fail+=1
        creat_fail+=1
        readelf -h "$line" >>./tests/.logs/readelf_"$(basename "$line")".log 2>&1
        readelf -l "$line" >> ./tests/.logs/readelf_"$(basename "$line")".log 2>&1
    fi
done <./tests/tests.txt

rm tests/woody-out tests/command-out

echo

if [ $total -eq 0 ]; then 
	echo "No tests ran"
	exit 1
fi

echo -e "${BOLD}Ran $total tests${NC}"
percent=$((success * 100 / total))
printf "${BOLD}${GREEN}%-12s${NC} ${GREEN}%02d/%02d [ %3s%% ]${NC}\n" "SUCCESS ✔︎"  "$success" "$total" "$percent"

percent=$((fail * 100 / total))
printf "${BOLD}${RED}%-12s${NC} ${RED}%02d/%02d [ %3s%% ]${NC}\n" "FAILURE ✗"  "$fail" "$total" "$percent"

percent=$((creat_fail * 100 / fail))
printf "%3s${BOLD}${RED}%-12s${NC} ${RED}%02d/%02d [ %3s%% of failures ]${NC}\n" " " "CREATE FAIL"  "$creat_fail" "$total" "$percent"
printf "%3s${BOLD}${YELLOW}%-12s${NC} ${YELLOW}%02d/%02d [ %3s%% of failures ]${NC}\n" " " "CREATE XFAIL"  "$creat_fail" "17" "$percent"

percent=$((exec_fail * 100 / fail))
printf "%3s${BOLD}${RED}%-12s${NC} ${RED}%02d/%02d [ %3s%% of failures ]${NC}\n" " " "EXEC FAIL"  "$exec_fail" "$fail" "$percent"