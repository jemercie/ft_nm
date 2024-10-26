#!/bin/bash

#IFS='\n'

#colors:
green="\e[32m"
red="\e[31m"
yellow="\e[93m"
endcolor="\e[0m"
filename=tests.txt

ls ressources/ > $filename

while read -r line; do

	nm ressources/$line > test_result_nm.txt
	./ft_nm ressources/$line > test_result_ft_nm.txt

	if ! diff -I 'round-trip*' test_result_nm.txt test_result_ft_nm.txt 
	then
		echo -e "test :$yellow $line $red NOT PASSED $endcolor"
	else
		echo -e "test $yellow $line $green PASSED $endcolor"
	fi
	rm test_result_nm.txt test_result_ft_nm.txt
done < $filename
rm $filename
