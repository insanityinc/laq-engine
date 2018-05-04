#!/bin/bash

#PBS -N LA_STREAMING_BENCHMARK
#PBS -l walltime=24:00:00
#PBS -l nodes=1:ppn=64
#PBS -m abe
#PBS -M a71874@alunos.uminho.pt

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

        for j in 1 2 4 8 12 16 24 32 48 64
        do
            export OMP_NUM_THREADS="${j}"
            # Test the current dataset
            python "test.py" "${i}" "${blSize}" "6"
        done
    done
done
