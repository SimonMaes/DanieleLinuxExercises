#!/bin/bash

#echo -e '\n >splint..\n'
#splint main.c
echo -e '\n >cppcheck..\n'
cppcheck main.c
echo -e '\n >make..\n'
make
