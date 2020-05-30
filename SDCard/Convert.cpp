#include <bits/stdc++.h>

#define pb push_back

using namespace std;

const string param1 = "#param1";
const string param2 = "#param2";

int n = 3;
vector<int> from = {2, 3, 4};
int s1 = 5, s2 = 6;
vector<vector<string>> v;
map<string, string> define_dict;

void add(string s) {
	vector<string> tek;
	s += ';';
	int l = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ';') {
			tek.pb(s.substr(l, i - l));
			l = i + 1;
		}
	}
	v.pb(tek);
}

void read_param() {
	cin >> n;
	if (n == -1) {
		n = 3;
		return;
	}
	cin >> s1 >> s2;
	s1--;
	s2--;
	from.assign(n, 0);
	for (auto &e : from) {
		cin >> e;
		e--;
	}
}

pair<string, int> get(string &s, int i) {
	int l = -1, r = -1;
	while (i < s.size() && s[i] == ' ') i++;
	l = i;
	while (i < s.size() && s[i] != ' ') i++;
	r = i - 1;
	return {s.substr(l, r - l + 1), r};
}

string get_comment(string &s, int i) {
	while (s[i] != '/') i++;
	return s.substr(i + 3, s.size() - (i + 3));
}

pair<string, string> get2(string &s) {
	pair<string, int> p1, p2;
	p1 = get(s, 7);
	p2 = get(s, p1.second + 1);
	return {p2.first, get_comment(s, p2.second + 1)};
}

void read_define() {
	ifstream myfile ("../gh_LoggerEvents.h");
	string s;
	pair<string, string> p;
	if (myfile.is_open()) {
		while (getline (myfile, s)) {
			if (s.size() > 7 && s[0] == '#') { // "#define - len = 7
				p = get2(s);
				define_dict[p.first] = p.second;
			}
		}
		myfile.close();
	}
}

vector<int> z_function(string s) {
	vector<int> z(s.size());
	z[0] = 0;
	int n = s.size(), l = 0, r = 0;
	for (int i = 1; i < s.size(); i++) {
		if (i <= r) {
			z[i] = min(z[i - l], r - i + 1);
		}
		while (s[z[i]] == s[z[i] + i] && z[i] + i < n) {
			z[i]++;
		}
		if (z[i] + i > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
	return z;
}

void check(string &s, string &s1, string &s2) {
	string tek = s;
	vector<int> z1 = z_function(param1 + ";" + tek);
	vector<int> z2 = z_function(param2 + ";" + tek);
	s = "";
	int i = 0;
	while (i < tek.size()) {
		if (z1[i + param1.size() + 1] == param1.size()) {
			s += s1;
			i += param1.size();
			continue;
		}
		if (z2[i + param2.size() + 1] == param2.size()) {
			s += s2;
			i += param2.size();
			continue;
		}
		s += tek[i];
		i++;
	}
}

void solve_change_define() {
	for (int i = 0; i < v.size(); i++) {
		for (auto e : from) {
			if (define_dict.find(v[i][e]) != define_dict.end()) {
				v[i].pb(define_dict[v[i][e]]);
				check(v[i][v[i].size() - 1], v[i][s1], v[i][s2]);
			}
		}
	}
}

void write() {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			cout << v[i][j];
			if (j != v[i].size() - 1) cout << ";";
		}
		cout << "\n";
	}
}

signed main() {
	read_param();
	read_define();
	freopen("in.csv", "rt", stdin);
	freopen("out.csv", "wt", stdout);
	string s;
	while (getline(cin, s, '\n')) {
		if (s != "") add(s);
	}
	solve_change_define();
	write();
    return 0;
}

