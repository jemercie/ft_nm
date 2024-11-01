#!/bin/bash

# Définition des couleurs
green="\e[32m"
red="\e[31m"
yellow="\e[93m"
endcolor="\e[0m"
filename=tests.txt
filename_bonus=tests_bonus.txt

# Liste les fichiers dans le répertoire ressources et les stocke dans tests.txt
ls ressources/ > "$filename"

echo "
	TESTS BONUS:
"

# Redirige stderr vers stderr.txt
exec 2>stderr.txt

# Lecture des arguments de tests_bonus.txt
while read -r line_args; do
    # Lecture des fichiers dans tests.txt
    while read -r line; do
        # Capturer la sortie de nm et ft_nm
        nm ressources/"$line" "$line_args" > test_result_nm.txt
        ../ft_nm ressources/"$line" "$line_args" > test_result_ft_nm.txt

        # Comparaison des résultats
        if ! diff -I 'round-trip*' test_result_nm.txt test_result_ft_nm.txt >trace; then
            echo -e "test : $yellow $line_args $red [NOT PASSED] $yellow : $line $endcolor"
        else
            echo -e "test : $yellow $line_args $green [PASSED] $yellow : $line $endcolor"
        fi
        
        # Nettoyage des fichiers temporaires
        rm test_result_nm.txt test_result_ft_nm.txt
    done < "$filename"
done < "$filename_bonus"

# Nettoyage final
rm "$filename"
