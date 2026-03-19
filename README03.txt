Problem :
This assignment extends previous assignments with the following changes:

- Ignore N and K values present in the input file.
- Even indexed assignments are solved using ChatGPT.
- Odd indexed assignments are solved using Gemini.
- c1 = cost per ChatGPT prompt.
- c2 = cost per Gemini prompt.
- Subscription is group-wise (total prompts per day).

Two scenarios are considered:
Case A:
Only one assignment can be solved per day.

Case B:
Multiple assignments can be solved per day if prompt limits allow and dependencies are satisfied.
Solutions are shared only on the next day.

Objectives:
For both Case A and Case B, the program computes:

1. Earliest number of days required to complete all assignments for a given subscription scheme.
2. Minimum subscription scheme required to complete all assignments within m days.
The program also handles infeasible scenarios.

Algorithms Implemented:
- DFS (Depth First Search)
- DFBB (Depth First Branch and Bound)
- A* Search

Compilation:
g++ assg03.cpp -o assg03

Execution:
assg03 input.txt c1 c2 chatLimit gemLimit m

Parameters:
input.txt  -> input file
c1         -> ChatGPT prompt cost
c2         -> Gemini prompt cost
chatLimit  -> ChatGPT prompts per day
gemLimit   -> Gemini prompts per day
m          -> target completion days

Example:
assg03 input.txt 2 3 5 5 8