#!/bin/bash 

set -eo pipefail

make || echo "No makefile found" exit 1

while IFS= read -r line;do
	echo -n "$line"
	./woody_woodpacker "$line"
done < ./utils/test