#!/bin/bash

source scripts/start.sh
echo "-------------------------"
./scripts/runner.sh ${nRange[@]} ${tRange[@]} ${bsRange[@]} $numRuns $tests $genSym $profiling $group ${metrics[@]}


