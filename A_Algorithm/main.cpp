#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <queue>
using namespace std;

unordered_map<string, vector<pair<string, int>>> adj;
unordered_map<string, string> father;
unordered_map<string, int> weight;

struct row{
	string currVertex;
	string adjVertiex;
	string k, h, g, f;
	string vertexQueue;
};
vector<row> steps;

void reset();
pair<string, string> inputEdgeList();
bool A_Algorithm(string start, string finish);
void printTable();
void printCycle();

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    reset();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    auto [start, finish] = inputEdgeList();
    bool isFind = A_Algorithm(start, finish);
    if(!isFind) {
        cout << "Khong thay duong di\n";
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

pair<string, string> inputEdgeList() {
    int n, m; cin >> n >> m;
    string u, v;
    int cost;
    for(int i=0; i<m; ++i) {
        cin >> u >> v >> cost;
        adj[u].push_back({v, cost});
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

bool A_Algorithm(string start, string finish) {
    string tmp = start + '-' + to_string(weight[start]);
    row r = {"", "", "", "", "", "", tmp};
    steps.push_back(r);

    unordered_map<string, int> g;
    g[start] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({g[start], start});
    father[start] = "-1";

    while(!pq.empty()) {
        auto [f, u] = pq.top();
        pq.pop();

        if(u == finish) {
            r = {u, "Tim thay", "", "", "", "", ""};
            steps.push_back(r);
            return true;
        }

        r.currVertex = u;
        for(auto&[v, k] : adj[u]) {
            r.adjVertiex = v;
            r.k = to_string(k);
            r.h = to_string(weight[v]);
            r.g = to_string(g[u] + k);
            r.f = to_string(g[u] + k + weight[v]);
            r.vertexQueue = "";

            steps.push_back(r);
            r = {"", "", "", "", "", "", ""};
            
            if(g.find(v) != g.end() && g[v] <= g[u] + k) {
                continue;
            }
            g[v] = g[u] + k;
            father[v] = u;
            pq.push({g[u] + k + weight[v], v});
        }
        string tmp = getVertex(pq);
        steps.back().vertexQueue = tmp;
    }

    return false;
}

void printSeparator(const vector<size_t>& width) {
    cout << "+";
    for(int i = 0; i < width.size(); ++i) {
        cout << string(width[i] + 2, '-') << "+";
    }
    cout << "\n";
}

void printTable() {
    vector<string> headers = {
        "TT", "TTK", "k(u,v)", "h(v)", "g(v)", "f(v)", "Danh sach L"
    };

    vector<size_t> width(7);
    for(int i = 0; i < 7; ++i)
        width[i] = headers[i].length();

    // Tìm độ rộng lớn nhất mỗi cột
    for(const auto &r : steps) {
        width[0] = max(width[0], r.currVertex.length());
        width[1] = max(width[1], r.adjVertiex.length());
        width[2] = max(width[2], r.k.length());
        width[3] = max(width[3], r.h.length());
        width[4] = max(width[4], r.g.length());
        width[5] = max(width[5], r.f.length());
        width[6] = max(width[6], r.vertexQueue.length());
    }

    // In header
    printSeparator(width);
    cout << "|";
    for(int i = 0; i < 7; ++i)
        cout << " " << left << setw(width[i]) << headers[i] << " |";
    cout << "\n";
    printSeparator(width);

    // In dữ liệu
    for(const auto &r : steps) {
        if(r.currVertex.length() > 0) {
            printSeparator(width);
        }

        cout << "| "
             << left << setw(width[0]) << r.currVertex << " | "
             << left << setw(width[1]) << r.adjVertiex << " | "
             << left << setw(width[2]) << r.k << " | "
             << left << setw(width[3]) << r.h << " | "
             << left << setw(width[4]) << r.g << " | "
             << left << setw(width[5]) << r.f << " | "
             << left << setw(width[6]) << r.vertexQueue << " |\n";
    }

    printSeparator(width);
}

void printCycle() {
    if(steps.empty()) return;

    string finish = steps.back().currVertex;
    if(finish == "") return;

    vector<string> path;
    string cur = finish;

    while(cur != "-1" && cur != "") {
        path.push_back(cur);
        cur = father[cur];
    }

    reverse(path.begin(), path.end());

    cout << "\nDuong di: ";
    for(int i = 0; i < path.size(); ++i) {
        cout << path[i];
        if(i != path.size() - 1)
            cout << " -> ";
    }
    cout << "\n";
}


