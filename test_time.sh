cmd=$1
echo "runtime" > $2.csv
for i in $(seq 1 $3)
do
    ts=$(date +%s%N)
    $cmd
    echo $(($(date +%s%N) - $ts)) >> $2.csv
done