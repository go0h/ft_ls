#!/bin/bash

MY="my"
STD="std"
DIFF="diff_file"

rm -rf ${MY} ${STD} ${DIFF}

./ft_ls -la ${1} ${2} > ../my
ls -la ${1} ${2} > ../std
cp ../${MY} ${MY}
cp ../${STD} ${STD}

diff ${MY} ${STD} | cat -e > ${DIFF}
cat ${DIFF}
