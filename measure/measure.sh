#!/bin/bash
set -e
#set -x
for i in *
do
    if [[ ${i} == _* ]]; then # Skip dirs starting with underscore
        continue
    fi
    if [[ -d ${i} ]]; then
        echo "===== $i"
        cp ./makefile.report $i
        cd ${i}
            make clean
            make
            make -f makefile.report report
        cd -
    fi
done
