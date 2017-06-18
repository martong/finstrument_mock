#!/bin/bash
set -e
#set -x
for i in *
do
    if [[ ${i} == _* ]]; then
        continue
    fi
    if [[ -d ${i} ]]; then
        echo "===== $i"
        cd ${i}
            make clean
        cd -
    fi
done
