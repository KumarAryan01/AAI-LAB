Compilation :
g++ assg02.cpp -o assg02

Input File Format
Each assignment is given as:

A <id> <prompt-count> <dependencies> 0

Execution Format

assg02 <input-file> <mode> <N> <K/M>

N  = number of students
K  = number of prompts per student per day
M  = number of days

Modes
Mode 1:
Given N and K, find minimum days required.
assg02 <input-file> 1 N K

Mode 2:
Given N and M, find minimum prompts per student.
assg02 <input-file> 2 N M

Mode 3:
Same as Mode 1 but with 6am solution-sharing delay.
assg02 <input-file> 3 N K

Mode 4:
Same as Mode 2 but with 6am solution-sharing delay.
assg02 <input-file> 4 N M





https://chatgpt.com/share/698fea44-aa04-8009-b693-48812e06e086
