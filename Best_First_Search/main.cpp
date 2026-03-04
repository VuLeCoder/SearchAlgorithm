#include <iostream>
#include <queue>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <queue>
using namespace std;

unordered_map<string, vector<string>> adj;
unordered_map<string, string> father;
unordered_map<string, int> weight;

// Biến lưu để in bảng
struct row{
	string currVertex;
	string adjVertices;
	string visitedVertices;
	string vertexQueue;
};
vector<row> steps;

void reset();
pair<string, string> inputAdjacencyList();
pair<string, string> inputEdgeList();
bool bestfs(string start, string finish);
void printTable();
void printCycle();

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    reset();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    auto [u, v] = inputEdgeList();
	bool isFind = bestfs(u, v);
	if(!isFind) {
        cout << "Khong tim thay duong di\n";
		return 0;
	}

    printTable();
    printCycle();

    return 0;
}

void reset() {
	adj.clear();
	father.clear();
	weight.clear();
	steps.clear();
}

pair<string, string> inputAdjacencyList() {

}

pair<string, string> inputEdgeList() {
	int n, m; cin >> n >> m;
	string u, v;
	for(int i=0; i<m; ++i) {
		cin >> u >> v;
		adj[u].push_back(v);
	}

    int w;
    for(int i=0; i<n; ++i) {
        cin >> u >> w;
        weight[u] = w;
    }
	
	cin >> u >> v;
	return {u, v};
}

string getVertex(priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq) {
    string ans = "";
    while(!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        ans += top.second + "-" + to_string(top.first);

        if(!pq.empty()) {
            ans += ", ";
        }
    }
    return ans;
}

bool bestfs(string start, string finish) {
    string tmp = start + '-' + to_string(weight[start]);
    row r = {"", "", tmp, tmp};
    steps.push_back(r);

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({weight[start], start});
    father[start] = "-1";

    while(!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        r.currVertex = u;
		r.adjVertices = "";

        if(u == finish) {
            r.currVertex += '-' + to_string(weight[u]);
            r.adjVertices = "Tim thay";
            r.vertexQueue = "";
            r.visitedVertices = "";
            steps.push_back(r);
            return true;
        }

        for(string v : adj[u]) {
            r.adjVertices += v + '-' + to_string(weight[v]) + ", ";

            if(father.find(v) == father.end()) {
				r.visitedVertices += ", " + v + '-' + to_string(weight[v]);
				father[v] = u;
				
				pq.push({weight[v], v});
			}
        }
        if(!r.adjVertices.empty()) {
            r.adjVertices.pop_back(); r.adjVertices.pop_back();
        }
        
        r.vertexQueue = getVertex(pq);
        r.currVertex += '-' + to_string(weight[u]);
        steps.push_back(r);
    }

    return false;
}

void printTable() {
    size_t maxCurr = string("Current").length();
    size_t maxAdj = string("Adj").length();
    size_t maxVisited = string("Visited").length();
    size_t maxQueue = string("Queue").length();

    for (const auto& r : steps) {
        maxCurr = max(maxCurr, r.currVertex.length());
        maxAdj = max(maxAdj, r.adjVertices.length());
        maxVisited = max(maxVisited, r.visitedVertices.length());
        maxQueue = max(maxQueue, r.vertexQueue.length());
    }

    int padding = 3;
    maxCurr += padding;
    maxAdj += padding;
    maxVisited += padding;
    maxQueue += padding;

    cout << left
         << setw(maxCurr) << "Current"
         << setw(maxAdj) << "Adj"
         << setw(maxVisited) << "Visited"
         << setw(maxQueue) << "Queue" << "\n";

    cout << string(maxCurr + maxAdj + maxVisited + maxQueue, '-') << "\n";

    for (const auto& r : steps) {
        cout << left
             << setw(maxCurr) << r.currVertex
             << setw(maxAdj) << r.adjVertices
             << setw(maxVisited) << r.visitedVertices
             << setw(maxQueue) << r.vertexQueue
             << "\n";
    }
}

void printCycle() {
    string cur = steps.back().currVertex;
    while (cur.back() != '-') {
        cur.pop_back();
    }
    cur.pop_back();
    
    vector<string> path;
    while(cur != "-1") {
        path.push_back(cur);
        cur = father[cur];
    }

    reverse(path.begin(), path.end());

    cout << "\nDuong di: ";
    for(int i=0; i<path.size(); ++i) {
        cout << path[i];
        if(i != path.size() - 1) cout << " -> ";
    }
}
