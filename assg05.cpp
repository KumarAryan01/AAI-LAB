#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Course {
    int id;
    int start;
    int deadline;
    int duration;
};

int main(int argc, char* argv[])
{
    if(argc < 2){
        cout << "Usage: assg05 <encoding>\n";
        return 0;
    }

    int encoding = stoi(argv[1]);

    string label;
    int rooms, courses;

    cin >> label >> rooms;
    cin >> label >> courses;

    vector<Course> C(courses);

    for(int i = 0; i < courses; i++)
        cin >> label >> C[i].id >> C[i].start >> C[i].deadline >> C[i].duration;

    int maxDay = 0;
    for(auto &c : C)
        if(c.deadline > maxDay)
            maxDay = c.deadline;

    int varID = 1;
    vector<vector<int>> clauses;

    vector<vector<vector<int>>> z;
    vector<vector<int>> x;
    vector<vector<int>> y;

    if(encoding == 1)
    {
        z.resize(courses, vector<vector<int>>(rooms, vector<int>(maxDay + 1, 0)));

        for(int i = 0; i < courses; i++)
        for(int r = 0; r < rooms; r++)
        for(int d = C[i].start; d <= C[i].deadline - C[i].duration + 1; d++)
            z[i][r][d] = varID++;
    }
    else
    {
        x.resize(courses, vector<int>(rooms));
        y.resize(courses, vector<int>(maxDay + 1));

        for(int i = 0; i < courses; i++)
        for(int r = 0; r < rooms; r++)
            x[i][r] = varID++;

        for(int i = 0; i < courses; i++)
        for(int d = C[i].start; d <= C[i].deadline - C[i].duration + 1; d++)
            y[i][d] = varID++;
    }

    if(encoding == 1)
    {
        for(int i = 0; i < courses; i++)
        {
            vector<int> clause;

            for(int r = 0; r < rooms; r++)
            for(int d = C[i].start; d <= C[i].deadline - C[i].duration + 1; d++)
                clause.push_back(z[i][r][d]);

            clauses.push_back(clause);
        }

        for(int i = 0; i < courses; i++)
        {
            vector<int> vars;

            for(int r = 0; r < rooms; r++)
            for(int d = C[i].start; d <= C[i].deadline - C[i].duration + 1; d++)
                vars.push_back(z[i][r][d]);

            for(int a = 0; a < vars.size(); a++)
            for(int b = a + 1; b < vars.size(); b++)
                clauses.push_back({-vars[a], -vars[b]});
        }

        for(int r = 0; r < rooms; r++)
        for(int i = 0; i < courses; i++)
        for(int k = i + 1; k < courses; k++)
        {
            for(int d1 = C[i].start; d1 <= C[i].deadline - C[i].duration + 1; d1++)
            for(int d2 = C[k].start; d2 <= C[k].deadline - C[k].duration + 1; d2++)
            {
                int s1 = d1;
                int e1 = d1 + C[i].duration - 1;

                int s2 = d2;
                int e2 = d2 + C[k].duration - 1;

                if(!(e1 < s2 || e2 < s1))
                    clauses.push_back({-z[i][r][d1], -z[k][r][d2]});
            }
        }
    }
    else
    {
        for(int i = 0; i < courses; i++)
        {
            vector<int> clause;
            for(int r = 0; r < rooms; r++)
                clause.push_back(x[i][r]);
            clauses.push_back(clause);
        }

        for(int i = 0; i < courses; i++)
        for(int r = 0; r < rooms; r++)
        for(int r2 = r + 1; r2 < rooms; r2++)
            clauses.push_back({-x[i][r], -x[i][r2]});

        for(int i = 0; i < courses; i++)
        {
            vector<int> clause;
            for(int d = C[i].start; d <= C[i].deadline - C[i].duration + 1; d++)
                clause.push_back(y[i][d]);
            clauses.push_back(clause);
        }

        for(int i = 0; i < courses; i++)
        {
            vector<int> vars;
            for(int d = C[i].start; d <= C[i].deadline - C[i].duration + 1; d++)
                vars.push_back(y[i][d]);

            for(int a = 0; a < vars.size(); a++)
            for(int b = a + 1; b < vars.size(); b++)
                clauses.push_back({-vars[a], -vars[b]});
        }

        for(int r = 0; r < rooms; r++)
        for(int i = 0; i < courses; i++)
        for(int k = i + 1; k < courses; k++)
        {
            for(int d1 = C[i].start; d1 <= C[i].deadline - C[i].duration + 1; d1++)
            for(int d2 = C[k].start; d2 <= C[k].deadline - C[k].duration + 1; d2++)
            {
                int s1 = d1;
                int e1 = d1 + C[i].duration - 1;

                int s2 = d2;
                int e2 = d2 + C[k].duration - 1;

                if(!(e1 < s2 || e2 < s1))
                    clauses.push_back({-x[i][r], -y[i][d1], -x[k][r], -y[k][d2]});
            }
        }
    }

    ofstream out("schedule.cnf");

    out << "p cnf " << varID - 1 << " " << clauses.size() << "\n";

    for(auto &c : clauses)
    {
        for(int v : c)
            out << v << " ";
        out << "0\n";
    }

    out.close();

    cout << "DIMACS CNF written to schedule.cnf\n";
}