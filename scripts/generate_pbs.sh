#!/bin/bash
# Call the execution script with the parsed parameters

source scripts/start.sh
cat <<EOL > parco-d1-job.pbs

#PBS -N parco-d1-job
#PBS -o ./parco-d1-job.out
#PBS -e ./parco-d1-job.err
#PBS -q short_cpuQ
#PBS -l walltime=0:01:00
#PBS -l select=1:ncpus=64:mem=1gb
module load gcc91
module load likwid-4.3.4
export OMP_NUM_THREADS=64
gcc() {
    gcc-9.1.0 "$@"
}
cd ${PWD}
echo "Number of runs: $numRuns"
echo "-------------------------"
./scripts/runner.sh ${nRange[@]} ${tRange[@]} ${bsRange[@]} $numRuns $tests $genSym $profiling $group ${metrics[@]}
EOL

