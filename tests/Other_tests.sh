#!/bin/bash

#Скрипт для тестирования прочих функций
#Для корректной работы тестера необходимо скачать sed

cd ..
cd ./cmake-build-debug/

ok_cnt=0
tests=2

../UDBMS_CLIENT/cmake_build_debug/UDBMS_CLIENT< ../tests/Other_tests_input


if [ -e config ]
	then
	{
	 echo 'ОК (Config exists)'
	 ok_cnt=$(( $ok_cnt + 1))
	 
	 currpath=$(sed '' config)
	 if [ $currpath = $(pwd) ]
	 	then
	  	{
	  		echo 'OK (Default path is true)'
	  		ok_cnt=$(( $ok_cnt + 1))
	  	} >> report; fi
	} >> report; fi

echo $ok_cnt'/'$tests 'tests passed' >> report

echo '0'





