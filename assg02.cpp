#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;

struct Task {
    int id;
    int cost;
    vector<int> deps;
};

vector<Task> tasks;
unordered_map<int, vector<int>> graph;
unordered_map<int, int> indeg;
unordered_map<int, int> need;

bool canFinish(int students, int K, int days, bool delay) {

    queue<int> q;
    unordered_map<int,int> deg = indeg;
    vector<vector<int>> nextDay(days + 2);

 // Step 1: Initialize Queue
Push tasks with no dependencies.

    for(auto &t : tasks)
        if(deg[t.id] == 0)
            q.push(t.id);

    int done = 0;

//Step 2: Day-by-Day Simulation


    for(int day = 1; day <= days; day++) {

        vector<int> left(students, K);
        queue<int> temp;
// Used for tasks that couldn't be assigned today.

Step 3: Assign Tasks
try to assign available tasks.
For each task:
Try assigning to some student
If student has enough prompts:

        while(!q.empty()) {
            int cur = q.front(); q.pop();
            bool assigned = false;

//If student has enough prompts

            for(int i = 0; i < students; i++) {
                if(left[i] >= need[cur]) {
                    left[i] -= need[cur];
                    assigned = true;
                    done++;

Step 4: Unlock Children (check delay / If No Delay)

                    for(int nxt : graph[cur]) {
                        deg[nxt]--;
                        if(deg[nxt] == 0) {
                            if(delay) nextDay[day + 1].push_back(nxt);
                            else q.push(nxt);
                        }
                    }
                    break;
                }
            }

            if(!assigned)
                temp.push(cur);
        }

        q = temp;
        for(int x : nextDay[day])
            q.push(x);
    }

    return done == tasks.size();
}

int main(int argc, char** argv) {
    if(argc < 5) {
        cout << "Usage: assg02 <input-file> <mode> <N> <K/M>\n";
        return 0;
    }

    string file = argv[1];
    int mode = stoi(argv[2]);
    int students = stoi(argv[3]);
    int val = stoi(argv[4]);
    bool delay = (mode >= 3);

    ifstream in(file);
    string s;
    while(in >> s) {
        if(s == "%") {
            string t; getline(in,t);
        } else if(s == "A") {
            Task t;
            in >> t.id >> t.cost;
            need[t.id] = t.cost;
            int d;
            while(in >> d && d != 0) {
                t.deps.push_back(d);
                graph[d].push_back(t.id);
                indeg[t.id]++;
            }
            tasks.push_back(t);
        }
    }

MODE 1 & 3 — Minimum Days

    if(mode == 1 || mode == 3) {
        int K = val;
        for(int d = 1; ; d++) {
            if(canFinish(students, K, d, delay)) {
                cout << "Minimum days = " << d << "\n";
                break;
            }
        }
    }


MODE 2 & 4 — Minimum Prompts
Binary Search.

    else {
        int days = val;
        int lo = 1, hi = 0;
        for(auto &t : tasks) hi += t.cost;

        int ans = hi;
        while(lo <= hi) {
            int mid = (lo + hi) / 2;

If works → try smaller.
Else → increase.
            if(canFinish(students, mid, days, delay)) {
                ans = mid;
                hi = mid - 1;
            } else lo = mid + 1;
        }

        cout << "Minimum prompts per student = " << ans << "\n";
    }
}

