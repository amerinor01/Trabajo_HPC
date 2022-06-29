SRC_DIR=$HOME/Trabajo_HPC
TEST_DIR=$SRC_DIR/tests
OPT_DIR=$SRC_DIR/opt
JOB_DIR=$SRC_DIR/jobs

jobs=$(ls $JOB_DIR/*.pbs)

for i in $jobs; do
    echo $i
    for j in {1..10}; do
        echo $j
        qsub $i
    done
done
