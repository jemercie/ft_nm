#!/bin/bash

pink="\e[38;2;255;182;193m"
blue="\e[38;2;102;178;255m"

test_fds="valgrind ../ft_nm ../ft_nm | grep 'Open file descriptor'"
test_leak_mandatory="valgrind ../ft_nm ../nm | grep 'no leaks are possible'"
test_leak_bonus="valgrind ../ft_nm -r ../ft_nm | grep 'no leaks are possible'"

stdout>tmp

if ! $test_fds > tmp_nm; then
    echo -e "  [PASSED]   : no open fd"
else
    echo -e "[NOT PASSED] : close fd"
fi

if $test_leak_mandatory > tmp ; then
    echo -e "  [PASSED]   : mandatory leak test : no leak"
else
    echo -e "[NOT PASSED] : mandatory leak test : check w valgrind"

fi

if $test_leak_bonus > tmp ; then
    echo -e "  [PASSED]   : bonus leak test : no leak"
else
    echo -e "[NOT PASSED] : bonus leak test : check w valgrind"
fi

