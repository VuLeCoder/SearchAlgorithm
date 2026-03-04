#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

// Biến cho thuật toán
unordered_map<string, vector<string>> adj;
unordered_map<string, string> father;
//unordered_map<string, bool> visited;

// Biến lưu để in bảng
struct row{
	string currVertex;
	string adjVertices;
	string visitedVertices;
	string vertexQueue;
};
vector<row> steps;

// Hàm
void reset();
pair<string, string> inputAdjacencyList();
pair<string, string> inputEdgeList();
bool bfs(string start, string finish);
void printTable();
void printCycle();

// Main
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
//	inputAdjacencyList();
	auto [u, v] = inputEdgeList();
	bool isFind = bfs(u, v);
	if(!isFind) {
		return 0;
	}
	
	printTable();
	printCycle();
	
	return 0;
}

void reset() {
	adj.clear();
	father.clear();
	
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
	
	cin >> u >> v;
	return {u, v};
}

void popVertex(string& vertexQ) {
	reverse(vertexQ.begin(), vertexQ.end());
	
	while(!vertexQ.empty() && vertexQ.back() != ' ') vertexQ.pop_back();
	if(!vertexQ.empty()) vertexQ.pop_back();
	
	reverse(vertexQ.begin(), vertexQ.end());
}

bool bfs(string start, string finish) {
	row r = {"", " ", start, start};
	steps.push_back(r);
	
	queue<string> q;
	q.push(start);
	father[start] = "-1";
	
	while(!q.empty()) {
		string u = q.front();
		q.pop();
		
		r.currVertex = u;
		r.adjVertices = "";
		popVertex(r.vertexQueue);
		
		if(u == finish) {
			r.adjVertices = "Da tim thay";
			r.visitedVertices = "";
			r.vertexQueue = "";
			steps.push_back(r);
			return true;
		}
		
		for(string v : adj[u]) {
			r.adjVertices += (" " + v);
			
			if(father.find(v) == father.end()) {
				r.visitedVertices += (" " + v);
				father[v] = u;
				
				r.vertexQueue += (v + " ");
				q.push(v);
			}
		}
		
		steps.push_back(r);
	}
	return false;
}

void printTable() {
    size_t w1 = 12, w2 = 12, w3 = 15, w4 = 12;

    for(const row& r : steps) {
        w1 = max(w1, r.currVertex.length());
        w2 = max(w2, r.adjVertices.length());
        w3 = max(w3, r.visitedVertices.length());
        w4 = max(w4, r.vertexQueue.length());
    }

    cout << left
         << setw(w1) << "Current"
         << " | " << setw(w2) << "Adjacency"
         << " | " << setw(w3) << "Visited"
         << " | " << setw(w4) << "Queue"
         << "\n";

    cout << string(w1, '-') << "-+-"
         << string(w2, '-') << "-+-"
         << string(w3, '-') << "-+-"
         << string(w4, '-') << "\n";

    for(const row& r : steps) {
        cout << left
             << setw(w1) << r.currVertex
             << " | " << setw(w2) << r.adjVertices
             << " | " << setw(w3) << r.visitedVertices
             << " | " << setw(w4) << r.vertexQueue
             << "\n";
    }
}

void printCycle() {
    vector<string> path;
    string cur = steps.back().currVertex;

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
