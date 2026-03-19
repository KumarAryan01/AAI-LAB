#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

struct Assignment {
    int id;
    int promptCost;
    vector<int> dependencies;
};

vector<Assignment> assignmentList;
int totalAssignments;

int chatPromptCost;
int gemPromptCost;
int dailyChatLimit;
int dailyGemLimit;
int requiredDays;

long long dfsNodes;
long long dfbbNodes;
long long aStarNodes;

bool allAssignmentsDone(const vector<bool>& completed) {
    for (bool done : completed)
        if (!done) return false;
    return true;
}

bool dependencySatisfied(int index, const vector<bool>& completed) {
    for (int dep : assignmentList[index].dependencies)
        if (!completed[dep - 1])
            return false;
    return true;
}

vector<int> findAvailableAssignments(const vector<bool>& completed) {
    vector<int> available;
    for (int i = 0; i < totalAssignments; i++)
        if (!completed[i] && dependencySatisfied(i, completed))
            available.push_back(i);
    return available;
}

int estimateRemainingDays(const vector<bool>& completed) {
    int remainingWork = 0;
    for (int i = 0; i < totalAssignments; i++)
        if (!completed[i])
            remainingWork += assignmentList[i].promptCost;

    int totalDailyCapacity = dailyChatLimit + dailyGemLimit;
    if (totalDailyCapacity == 0)
        return INT_MAX;

    return (remainingWork + totalDailyCapacity - 1) / totalDailyCapacity;
}

bool processOneDay(vector<bool>& completed, int scenarioType) {
    int chatLeft = dailyChatLimit;
    int gemLeft  = dailyGemLimit;
    bool didWork = false;

    vector<int> available = findAvailableAssignments(completed);

    if (scenarioType == 1) {
        if (!available.empty()) {
            int index = available[0];

            if (assignmentList[index].id % 2 == 0) {
                if (chatLeft >= assignmentList[index].promptCost) {
                    completed[index] = true;
                    didWork = true;
                }
            }
            else {
                if (gemLeft >= assignmentList[index].promptCost) {
                    completed[index] = true;
                    didWork = true;
                }
            }
        }
    }
    else {
        for (int index : available) {
            if (assignmentList[index].id % 2 == 0) {
                if (chatLeft >= assignmentList[index].promptCost) {
                    chatLeft -= assignmentList[index].promptCost;
                    completed[index] = true;
                    didWork = true;
                }
            }
            else {
                if (gemLeft >= assignmentList[index].promptCost) {
                    gemLeft -= assignmentList[index].promptCost;
                    completed[index] = true;
                    didWork = true;
                }
            }
        }
    }

    return didWork;
}

int bestDFSResult;

void runDFS(vector<bool> state, int dayCount, int scenarioType) {
    dfsNodes++;

    if (allAssignmentsDone(state)) {
        bestDFSResult = min(bestDFSResult, dayCount);
        return;
    }

    if (dayCount >= bestDFSResult)
        return;

    vector<bool> nextState = state;

    if (!processOneDay(nextState, scenarioType))
        return;

    runDFS(nextState, dayCount + 1, scenarioType);
}

int bestDFBBResult;

void runDFBB(vector<bool> state, int dayCount, int scenarioType) {
    dfbbNodes++;

    if (dayCount >= bestDFBBResult)
        return;

    if (allAssignmentsDone(state)) {
        bestDFBBResult = dayCount;
        return;
    }

    vector<bool> nextState = state;

    if (!processOneDay(nextState, scenarioType))
        return;

    runDFBB(nextState, dayCount + 1, scenarioType);
}

struct SearchState {
    vector<bool> completed;
    int day;
};

struct CompareState {
    bool operator()(const SearchState& a, const SearchState& b) {
        return (a.day + estimateRemainingDays(a.completed)) >
               (b.day + estimateRemainingDays(b.completed));
    }
};

int runAStar(int scenarioType) {
    priority_queue<SearchState, vector<SearchState>, CompareState> pq;

    SearchState start;
    start.completed = vector<bool>(totalAssignments, false);
    start.day = 0;

    pq.push(start);

    while (!pq.empty()) {
        SearchState current = pq.top();
        pq.pop();
        aStarNodes++;

        if (allAssignmentsDone(current.completed))
            return current.day;

        vector<bool> nextState = current.completed;

        if (!processOneDay(nextState, scenarioType))
            continue;

        SearchState next;
        next.completed = nextState;
        next.day = current.day + 1;

        pq.push(next);
    }

    return -1;
}

void loadInput(string filename) {
    ifstream file(filename);
    string token;

    while (file >> token) {
        if (token == "%") {
            string skipLine;
            getline(file, skipLine);
        }
        else if (token == "N" || token == "K") {
            int ignoreValue;
            file >> ignoreValue;
        }
        else if (token == "A") {
            Assignment task;
            file >> task.id >> task.promptCost;

            int dep;
            while (file >> dep && dep != 0)
                task.dependencies.push_back(dep);

            assignmentList.push_back(task);
        }
    }

    totalAssignments = assignmentList.size();
}

void solveEarliestFinish(int scenarioType) {
    dfsNodes = dfbbNodes = aStarNodes = 0;
    bestDFSResult = bestDFBBResult = INT_MAX;

    vector<bool> initialState(totalAssignments, false);

    runDFS(initialState, 0, scenarioType);
    runDFBB(initialState, 0, scenarioType);
    int bestAStar = runAStar(scenarioType);

    cout << "Objective 1: Earliest Finish\n";
    cout << "DFS  : Finish Day = " << bestDFSResult << endl;
    cout << "DFBB : Finish Day = " << bestDFBBResult << endl;
    cout << "A*   : Finish Day = " << bestAStar << endl;

    cout << "\nNodes Expanded:\n";
    cout << "DFS  = " << dfsNodes << endl;
    cout << "DFBB = " << dfbbNodes << endl;
    cout << "A*   = " << aStarNodes << endl << endl;
}

void solveMinimumSubscription(int scenarioType) {
    cout << "Objective 2: Minimum Subscription (Finish within "
         << requiredDays << " days)\n";

    int low = 1, high = 50;
    int bestValue = INT_MAX;

    while (low <= high) {
        int mid = (low + high) / 2;

        dailyChatLimit = mid;
        dailyGemLimit  = mid;

        aStarNodes = 0;

        int finishDays = runAStar(scenarioType);

        if (finishDays != -1 && finishDays <= requiredDays) {
            bestValue = mid;
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    if (bestValue == INT_MAX) {
        cout << "No feasible subscription scheme.\n\n";
        return;
    }

    cout << "Minimum ChatGPT prompts per day = " << bestValue << endl;
    cout << "Minimum Gemini prompts per day  = " << bestValue << endl;
    cout << "Total Daily Subscription Cost   = "
         << bestValue * chatPromptCost +
            bestValue * gemPromptCost
         << endl << endl;
}

int main(int argc, char** argv) {

    if (argc < 7) {
        cout << "Usage:\n";
        cout << "assg03 input.txt c1 c2 chatLimit gemLimit targetDays\n";
        return 0;
    }

    string inputFile = argv[1];

    chatPromptCost = stoi(argv[2]);
    gemPromptCost  = stoi(argv[3]);
    dailyChatLimit = stoi(argv[4]);
    dailyGemLimit  = stoi(argv[5]);
    requiredDays   = stoi(argv[6]);

    loadInput(inputFile);

    cout << "CASE A\n";
   
    solveEarliestFinish(1);
    solveMinimumSubscription(1);

    
    cout << "CASE B\n";
    
    solveEarliestFinish(2);
    solveMinimumSubscription(2);

    return 0;
}