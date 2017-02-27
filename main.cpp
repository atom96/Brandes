#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <mutex>
#include <thread>
#include <fstream>
#include <cassert>

using namespace std;

vector<double> BC;

vector<mutex> mut_for_BC;


void brandes(vector<int>* W, size_t num_of_nodes, vector<vector<int>>* neighbours) {
    for(auto s: *W) {
        vector<vector<int>> P(num_of_nodes);
        vector<double> sigma(num_of_nodes);
        vector<double> d(num_of_nodes);
        vector<double> delta(num_of_nodes);
        stack<int> S;
        for(size_t w = 0; w < num_of_nodes; ++w) {
            P[w] = vector<int>();
            sigma[w] = 0;
            d[w] = -1;
            delta[w] = 0;
        }

        sigma[s] = 1;
        d[s] = 0;


        queue<int> Q;
        Q.push(s);

        while(!Q.empty()) {
            auto v = Q.front();
            Q.pop();
            S.push(v);
            for (auto w: (*neighbours)[v]) {
                if (d[w] < 0) {
                    Q.push(w);
                    d[w] = d[v] + 1;
                }

                if (d[w] == d[v] + 1) {
                    sigma[w] += sigma[v];
                    P[w].push_back(v);
                }
            }
        }

        while(!S.empty()) {
            auto w = S.top();
            S.pop();

            for (auto v: P[w]) {
                delta[v] += (sigma[v] / sigma[w]) * (1 + delta[w]);
            }

            if (w != s) {
                mut_for_BC[w].lock();
                BC[w] += delta[w];
                mut_for_BC[w].unlock();
            }
        }
    }
}

vector<vector<int>> split_set(int num_of_threads, vector<int> V) {
    vector<vector<int>> sets_for_threads(num_of_threads);
    int current_thread = 0;
    int remaing_threads = num_of_threads;
    size_t num_of_nodes = V.size();

    while(num_of_nodes > 0) {
        int to_current = num_of_nodes / remaing_threads;
        if(to_current == 0) {
            to_current = 1;
        }
        num_of_nodes -= to_current;
        remaing_threads--;
        vector<int> current_set;

        for(int i = 0; i < to_current; ++i) {
            current_set.push_back(num_of_nodes + i);
        }

        sets_for_threads[current_thread++] = current_set;
    }

    return move(sets_for_threads);
}


void run_threads(int num_of_threads, size_t num_of_nodes, vector<vector<int>>& sets_for_threads, vector<vector<int>>& neighbours) {
    vector<thread*> threads(num_of_threads);

    for(int i = 0; i < num_of_threads; ++i) {
        thread* t = new thread{brandes, &sets_for_threads[i], num_of_nodes, &neighbours};
        threads[i] = t;
    }

    for(int i = 0; i < num_of_threads; ++i) {
        threads[i]->join();
    }

    for(int i = 0; i < num_of_threads; ++i) {
        delete(threads[i]);
    }
}

void perform_algorithm(int num_of_threads, size_t num_of_nodes, vector<vector<int>>& neighbours) {
    vector<int> V(num_of_nodes);

    BC.resize(num_of_nodes);
    mut_for_BC = vector<mutex>(num_of_nodes);

    for(size_t i = 0; i < num_of_nodes; ++i) {
        V[i] = i;
    }

    auto sets_for_threads = split_set(num_of_threads, V);

    run_threads(num_of_threads, num_of_nodes, sets_for_threads, neighbours);
}

int main(int argc,  char** argv) {
    int a,b;
    map<int, int> nodes_map;
    vector<vector<int>> neighbours;
    int curr = 0;
    int num_of_threads = stoi(argv[1]);
    ifstream data;
    ofstream output;

    assert(argc == 4);
    data.open(argv[2]);
    while(data>>a>>b) {
        if(nodes_map.find(a) == nodes_map.end()) {
            nodes_map[a] = curr++;
            neighbours.push_back(vector<int>());
        }
        if(nodes_map.find(b) == nodes_map.end()) {
            nodes_map[b] = curr++;
            neighbours.push_back(vector<int>());
        }
        neighbours[nodes_map[a]].push_back(nodes_map[b]);
    }

    data.close();

    perform_algorithm(num_of_threads, neighbours.size(), neighbours);


    output.open(argv[3]);

    for(auto node: nodes_map) {
        if(!neighbours[node.second].empty()) {
            output<<node.first<<" "<<BC[node.second]<<endl;
        }
    }
    output.close();

    return 0;
}
