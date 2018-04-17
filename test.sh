#!/bin/bash
set -x


for blSize in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
do
    mkdir -p "results/${blSize}"
    export BSIZE="${blSize}"

    for q in "6"
    do
        echo "dataset,k-best,average,std_dev" > "results/${blSize}/query_${q}_time.csv"
        echo "dataset,k-best,average,std_dev" > "results/${blSize}/query_${q}_memory.csv"
    done

    for i in "1"
    do
        export DATASET="${i}"
        make delete
        make -j4
        time engine/bin/load
        # Test the current dataset
        python "test.py" "${i}" "${blSize}" "6"
    done
done
