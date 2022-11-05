#!/bin/bash

python tools/assembler/assemble.py badge/src/main.s && 
truncate.exe -s +100 badge/src/main.hex
read -p "Press any key to resume ..."
