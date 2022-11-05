#!/bin/bash

python tools/assembler/assemble.py badge/src/main.asm && 
truncate.exe -s +100 badge/src/main.hex && 
stty -F /dev/ttyS5 9600 && 
cat badge/src/main.hex >> /dev/ttyS5
read -p "Press any key to resume ..."
