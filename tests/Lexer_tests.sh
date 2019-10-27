#!/bin/bash

cd ..
cd ./cmake-build-debug/

touch ../tests/Lexer_tests_res_ok
touch ../tests/Lexer_tests_res_err


exec 3>&1 1>> ../tests/Lexer_tests_res_ok
exec 4>&2 2>> ../tests/Lexer_tests_res_err

if ../UDBMS_CLIENT/cmake_build_debug/UDBMS_CLIENT< ../tests/Lexer_tests_input
then
	exec 1>&3
	exec 2>&4 
fi

str_cnt=1
res_ok_num=1
res_err_num=1
ok_cnt=0

while read LINE; do
    res_ok=`head -n $res_ok_num ../tests/Lexer_tests_res_ok | tail -n +$res_ok_num`
    res_err=`head -n $res_err_num ../tests/Lexer_tests_res_err | tail -n +$res_err_num`
	if [ "= $res_ok" == "$LINE" ] || [ "$res_err" == "$LINE" ]
	then
		ok_cnt=$(( $ok_cnt + 1 )) 
		echo 'Test' "$str_cnt" 'passed' >> report
	else
		echo '---------------------------------' >> report
    	echo 'Test' "$str_cnt" 'failed' >> report
    	echo 'Expected:' $LINE $'\n''Got:' "=" $res_ok '|' $res_err >> report
    	echo '---------------------------------' >> report
	fi
	
	if [ "${LINE:0:2}" == "= " ]
		then 
			res_ok_num=$(( $res_ok_num + 1 ))
		else
			res_err_num=$(( $res_err_num + 1 ))
	fi
	
	str_cnt=$(( $str_cnt + 1 ))
done < ../tests/Lexer_tests_sol

rm ../tests/Lexer_tests_res_ok
rm ../tests/Lexer_tests_res_err

echo $ok_cnt'/'$(( $str_cnt - 1 )) 'tests passed' >> report

echo '0'
