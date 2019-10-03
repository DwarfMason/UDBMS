#!/bin/bash

cd ..
cd ./cmake-build-debug/

rm ../tests/Create_table_res
touch ../tests/Create_table_res


exec 3>&1 1>> ../tests/Create_table_res
exec 4>&2 2>> ../tests/Create_table_res

if ./UDBMS< ../tests/Create_table_input
then
	exec 1>&3
	exec 2>&4
fi

str_cnt=1
ok_cnt=0

while read LINE; do
    buff=`head -n $str_cnt ../tests/Create_table_sol | tail -n +$str_cnt`
	if [ "$buff" == "$LINE" ]
	then
		ok_cnt=$(( $ok_cnt + 1 )) 
		echo 'OK' >> report
	else
    	echo 'Test' $str_cnt 'failed' >> report
	fi
	str_cnt=$(( $str_cnt + 1 ))
done < ../tests/Create_table_res

echo $ok_cnt'/'$(( $str_cnt - 1 )) 'tests passed' >> report

echo '0'
