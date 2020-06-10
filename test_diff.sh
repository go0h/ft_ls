#!/bin/bash

rm -rf my std diff_file

./ft_ls -la / > ../my
ls -la / > ../std
cp ../my my
cp ../std std

diff my std | cat -e > diff_file
