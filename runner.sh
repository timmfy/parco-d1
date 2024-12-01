#!/bin/bash

# Retrieve arguments
nRange=("${@:1:13}")
tRange=("${@:14:7}")
bsRange=("${@:21:13}")
blockSize=16
numRuns=${34}
tests=${35}
genSym=${36}
profiling=${37}
group=${38}
metrics=("${@:39}")
summary_profiler_file="summary_profiler.txt"
summary_comparison_file="summary_comparison.txt"
> "$summary_profiler_file"
> "$summary_comparison_file"

pattern=$(IFS=\|; echo "${metrics[*]}")

if [[ $profiling == "none" ]]; then
    # Compile and run for the nRange
    echo "Profiling: $profiling"
    echo "Tests to run: $tests"
    for i in "${!nRange[@]}"; do
        if [[ ${nRange[$i]} -eq 1 ]]; then
            for j in "${!bsRange[@]}"; do
                if [[ ${bsRange[$j]} -eq 1 ]]; then
                    for k in "${!tRange[@]}"; do
                        if [[ ${tRange[$k]} -eq 1 ]]; then
                            N=$((2**$i))
                            blockSize=$((2**$j))
                            threads=$((2**$k))
                            echo "Number of runs: $numRuns"
                            echo "-------------------------"
                            echo "Compiling..."
                            echo "-------------------------"
                            make all N=$N
                            echo "-------------------------"
                            echo "Running tests..."
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize
                            echo "-------------------------"
                            echo "-------------------------" >> "$summary_comparison_file"
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize >> "$summary_comparison_file"
                            ./bin/main --block-size $blockSize --runs $numRuns --symm $genSym $tests --threads $threads | tail -n 3 >> "$summary_comparison_file"
                            make clean
                        fi
                    done
                fi
            done
        fi
    done
else
    # Compile and profile with likwid the selected test
    # for threads and sizes
    echo "Tests to run: $tests"
    for i in "${!nRange[@]}"; do
        if [[ ${nRange[$i]} -eq 1 ]]; then
            N=$((2**$i))
            for j in "${!tRange[@]}"; do
                if [[ ${tRange[$j]} -eq 1 ]]; then
                    for k in "${!bsRange[@]}"; do
                        if [[ ${bsRange[$k]} -eq 1 ]]; then
                            blockSize=$((2**$k))
                            threads=$((2**$j))
                            echo "Number of runs: $numRuns"
                            echo "-------------------------"
                            echo "Compiling..."
                            echo "-------------------------"
                            make all N=$N
                            echo "-------------------------"
                            echo "Running tests..."
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize
                            echo "-------------------------"
                            echo "-------------------------" >> "$summary_profiler_file"
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize >> "$summary_profiler_file"
                            likwid-perfctr -V 0 -C 0-$(($threads-1)) -g $group ./bin/main --block-size $blockSize --runs $numRuns --symm $genSym $tests --threads $threads  | grep "Sum\|STAT" | grep -v "Event\|_" | grep -Ei "$pattern|Sum" >> "$summary_profiler_file"
                            make clean
                        fi
                    done
                fi
            done
        fi
    done
fi
make clean
echo "Summary:"
summary_file=""
if [[ $profiling == "none" ]]; then
    summary_file="$summary_comparison_file"
    rm "$summary_profiler_file"
else
    summary_file="$summary_profiler_file"
    rm "$summary_comparison_file"
fi
cat "$summary_file"
