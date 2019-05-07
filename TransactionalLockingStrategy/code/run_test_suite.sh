#! /bin/bash

# set -x

mkdir -p r04
../run_tests.sh -d r04 -i 10000 -x 1000000 -r 20 -m 0       >  r04/results_04.txt
../run_tests.sh -d r04 -i 10000 -x 1000000 -r 20 -c 8 -m 1  >> r04/results_04.txt
../run_tests.sh -d r04 -i 10000 -x 1000000 -r 20 -c 8 -m 2  >> r04/results_04.txt
../run_tests.sh -d r04 -i 10000 -x 1000000 -r 20 -c 8 -m 3  >> r04/results_04.txt

mkdir -p r05
../run_tests.sh -d r05 -i 100000 -x 1000000 -r 20 -m 0       >  r05/results_05.txt
../run_tests.sh -d r05 -i 100000 -x 1000000 -r 20 -c 8 -m 1  >> r05/results_05.txt
../run_tests.sh -d r05 -i 100000 -x 1000000 -r 20 -c 8 -m 2  >> r05/results_05.txt
../run_tests.sh -d r05 -i 100000 -x 1000000 -r 20 -c 8 -m 3  >> r05/results_05.txt

mkdir -p r06
../run_tests.sh -d r06 -i 1000000 -x 1000000 -r 20 -m 0       >  r06/results_06.txt
../run_tests.sh -d r06 -i 1000000 -x 1000000 -r 20 -c 8 -m 1  >> r06/results_06.txt
../run_tests.sh -d r06 -i 1000000 -x 1000000 -r 20 -c 8 -m 2  >> r06/results_06.txt
../run_tests.sh -d r06 -i 1000000 -x 1000000 -r 20 -c 8 -m 3  >> r06/results_06.txt

mkdir -p r07
../run_tests.sh -d r07 -i 10000000 -x 1000000 -r 20 -m 0       >  r07/results_07.txt
../run_tests.sh -d r07 -i 10000000 -x 1000000 -r 20 -c 8 -m 1  >> r07/results_07.txt
../run_tests.sh -d r07 -i 10000000 -x 1000000 -r 20 -c 8 -m 2  >> r07/results_07.txt
../run_tests.sh -d r07 -i 10000000 -x 1000000 -r 20 -c 8 -m 3  >> r07/results_07.txt
