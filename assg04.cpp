#include <z3++.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
using namespace z3;
using namespace std;

struct Vehicle {
    int id;
    int arrival;
    int departure;
    int charge;
};

int main() {

    string temp;
    int K;
    cin >> temp >> K;   

    vector<int> price(K);

    cin >> temp;        
    for (int i = 0; i < K; i++)
        cin >> price[i];

    vector<Vehicle> vehicles;

    while (cin >> temp) {   
        Vehicle v;
        cin >> v.id >> v.arrival >> v.departure >> v.charge;
        vehicles.push_back(v);
    }

    int N = vehicles.size();

    if (N > K) {
        cout << "No feasible schedule exist.\n";
        return 0;
    }

    context ctx;
    solver s(ctx);

    vector<expr> port;
    expr total_cost = ctx.int_const("total_cost");
    expr cost_expr = ctx.int_val(0);

    for (int i = 0; i < N; i++) {
        port.push_back(ctx.int_const(("port_" + to_string(i)).c_str()));
        s.add(port[i] >= 1);
        s.add(port[i] <= K);
    }
    
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
            s.add(port[i] != port[j]);

    for (int i = 0; i < N; i++) {

        expr vehicle_cost = ctx.int_val(0);

        for (int k = 1; k <= K; k++) {

            int time_needed = (vehicles[i].charge + k - 1) / k;

            if (vehicles[i].arrival + time_needed <= vehicles[i].departure) {

                int cost_val = time_needed * price[k - 1];

                vehicle_cost = vehicle_cost +
                    ite(port[i] == k,
                        ctx.int_val(cost_val),
                        ctx.int_val(0));
            }
            else {
                s.add(port[i] != k);
            }
        }

        cost_expr = cost_expr + vehicle_cost;
    }

    s.add(total_cost == cost_expr);

    int bestCost = INT_MAX;
    model bestModel(ctx);

    while (s.check() == sat) {

        model m = s.get_model();
        int currentCost = m.eval(total_cost).get_numeral_int();

        if (currentCost < bestCost) {
            bestCost = currentCost;
            bestModel = m;
        }
        s.add(total_cost < currentCost);
    }

    if (bestCost == INT_MAX) {
        cout << "No feasible schedule exists.\n";
    }
    else {
        cout << "Minimum total charging cost: "
             << bestCost << endl;
        cout << "\nAssignments:\n";

        for (int i = 0; i < N; i++) {
            cout << "Vehicle "
                 << vehicles[i].id
                 << " -> Port "
                 << bestModel.eval(port[i]).get_numeral_int()
                 << endl;
        }
    }
    return 0;
}