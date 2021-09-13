#!/bin/bash

function Usage() {
    echo "Usage: $0"
}

if [[ $# -ge 1 ]];then
    Usage
    echo "too many parameter"
    exit 1
fi

NowTime=`date +"%Y-%m-%d %H:%M:%S"`
eval `df -T -m  -x devtmpfs | tail -n +2 \
    | awk -v DiskSum=0 -v LeftSum=0 \
    '{printf("parsum["NR"]=%d;parleft["NR"]=%d;parname["NR"]=%s;parusage["NR"]=%s;", $3, $5, $7, $6);\
    DiskSum+=$3;LeftSum+=$5} END {printf("parnum=%d;DiskSum=%d;LeftSum=%d", NR, DiskSum, LeftSum)}'`


# 输出总的
diskusage=`echo "scale=2; 100-${LeftSum}*100/${DiskSum}" | bc`
echo "${NowTime} 0 all ${DiskSum} ${LeftSum} ${diskusage}%"

# 输出分区的
for (( i = 1; i <= ${parnum}; i++ ));do
    echo "$NowTime 1 ${parname[$i]} ${parsum[$i]} ${parleft[$i]} ${parusage[$i]}"
done

