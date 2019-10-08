#!/bin/bash

cd ..
cd ./cmake-build-debug/

rm ../tests/Lexer_tests_res
touch ../tests/Lexer_tests_res


exec 3>&1 1>> ../tests/Lexer_tests_res
exec 4>&2 2>> ../tests/Lexer_tests_res

if ./UDBMS< ../tests/Lexer_tests_input
then
	exec 1>&3
	exec 2>&4
fi

str_cnt=1
ok_cnt=0

while read LINE; do
    buff=`head -n $str_cnt ../tests/Lexer_tests_sol | tail -n +$str_cnt`
	if [ "$buff" == "$LINE" ]
	then
		ok_cnt=$(( $ok_cnt + 1 )) 
		echo 'OK' >> report
	else
		echo '---------------------------------' >> report
    	echo 'Test' "$str_cnt" 'failed' >> report
    	echo 'Expected:' $buff $'\n''Got:' $LINE >> report
    	echo '---------------------------------' >> report
	fi
	str_cnt=$(( $str_cnt + 1 ))
done < ../tests/Lexer_tests_res

echo $ok_cnt'/'$(( $str_cnt - 1 )) 'tests passed' >> report

echo '0'
