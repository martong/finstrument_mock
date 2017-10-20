#!/bin/bash
if [ "$#" -eq 0 ]; then
	echo "pls state the name of the target dir"
	exit 1
fi

mkdir $1

for i in __O*/report.txt; do
	cp --parents $i $1
done
cp *.py $1
echo "*.py" >> $1/.gitignore
echo "*.pyc" >> $1/.gitignore

