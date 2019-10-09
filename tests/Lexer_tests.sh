#!/bin/bash

cd ..
cd ./cmake-build-debug/

touch ../tests/Lexer_tests_res_ok
touch ../tests/Lexer_tests_res_err


exec 3>&1 1>> ../tests/Lexer_tests_res_ok
exec 4>&2 2>> ../tests/Lexer_tests_res_err

if ./UDBMS< ../tests/Lexer_tests_input
then
	exec 1>&3
	exec 2>&4 
fi

str_cnt=1
ok_cnt=0

while read LINE; do
    res_ok=`head -n $str_cnt ../tests/Lexer_tests_res_ok | tail -n +$str_cnt`
    res_err=`head -n $str_cnt ../tests/Lexer_tests_res_err | tail -n +$str_cnt`
	if [ "* $res_ok" == "$LINE" ] | [ "$res_err" == "$LINE" ]
	then
		ok_cnt=$(( $ok_cnt + 1 )) 
		echo 'Test' "$str_cnt" 'passed' >> report
	else
		echo '---------------------------------' >> report
    	echo 'Test' "$str_cnt" 'failed' >> report
    	echo 'Expected:' $LINE $'\n''Got:' $res_ok '/' $res_err >> report
    	echo '---------------------------------' >> report
	fi
	str_cnt=$(( $str_cnt + 1 ))
done < ../tests/Lexer_tests_sol

rm ../tests/Lexer_tests_res_ok
rm ../tests/Lexer_tests_res_err

echo $ok_cnt'/'$(( $str_cnt - 1 )) 'tests passed' >> report

echo '0'
