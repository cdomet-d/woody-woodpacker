#! /bin/bash

set -eo pipefail

find "$PWD"/.dir_build -type f -name stub.o &> /dev/null || echo "Could not find $PWD/.dir_build/stub.o" exit 1

declare -A vars=([key]=KEY_OFF [o_entry]=OENTRY_OFF [stub_vaddr]=STUB_VADDR_OFF [text]=TEXT_OFF [text_size]=TEXTSZ_OFF)
stub="$PWD/.dir_build/stub.o"

for i in ${!vars[@]}; do
    off=$(nm "$stub" | awk "/ $i\$/{print \$1}")
    res="#define ${vars[$i]} 0x"
    res+=$(echo $off | sed 's/^0*//')
    echo $res
done

