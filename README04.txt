Input Format:
K <number of ports>
P <prices for each port>
V <id> <arrival> <departure> <charge time>

Compilation:
g++ assg04.cpp -o assg04 -IC:\z3-4.16.0-x64-win\include -LC:\z3-4.16.0-x64-win\bin -lz3 
g++ genTestcases.cpp -o genTestcases


Run:
genTestcases.exe > input.txt
assg04.exe < input.txt

Output:

Minimum total charging cost

Vehicle to port assignment

If no valid schedule exists, then it prints:
No feasible schedule exists.