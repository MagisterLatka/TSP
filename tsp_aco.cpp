#include <iostream>
#include <iterator>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>

using namespace std;

typedef pair<int,int> pii;
typedef tuple<int, int, int> tii;

vector<tii> points;

const int MX = 100;
int iterations = 100;
int ant_count = 100;
double evaporation = 0.9;
int n;

double feromone_trail[MX][MX] {};
double mini_lenght = numeric_limits<double>::infinity();
vector<int> mini_path;

int next_vertex(double prob, int a, bool vis[]){
	int sum = 0;
	for(int i = 0; i < n; i++){
		if(vis[i])
			continue;
		sum += feromone_trail[a][i];
	}
	cout << prob << endl;
	prob = (prob*sum)/99;
	cout << sum << " " << prob << endl;
	int vert = 0;
	while(prob > 0 and vert < n){
		if(vis[vert]){
			vert++;
			continue;
		}
		prob -= feromone_trail[a][vert];
		vert++;
	}

	return vert;
}

double distance(int a, int b){
	int x1, y1, idx;
	int x2, y2;
	tie(x1, y1, idx) = points[a];
	tie(x2, y2, idx) = points[b];
	double x = (x1-x2);
	x *= x;
	double y = (y1-y2);
	y *= y;
	return sqrt(x + y);
}

void decerease_feromones(double ev){
	for(int i = 0; i < MX; i++){
		for(int k = 0; k < MX; k++){
			feromone_trail[i][k] *= ev;
		}
	}
}

void zero(){
	for(int i = 0; i < n; i++){
		for(int k = 0; k < n; k++){
			feromone_trail[i][k] = 1;
		}
	}
}

int main(){
	srand(time(NULL));
	zero();
	cin >> n;
	for(int i = 0; i < n; i++){
		int a, b, idx;
		cin >> idx >> a >> b;
		points.push_back({a, b, idx});	
	}


	while(iterations --> 0){
		vector<vector<int>> all_paths;
		for(int ant_nb = 0; ant_nb < ant_count; ant_nb++){
			vector <int> ord;
			bool vis[MX];
			int start = rand()%n;
			vis[start] = true;
			ord.push_back(start);
			int curr_vert = start;
			for(int proc_vert = 1; proc_vert < n; proc_vert ++){
				int next_prob = rand()%100;
				int next_vert = next_vertex(next_prob, curr_vert, vis);
				ord.push_back(next_vert);
				curr_vert = next_vert;
				vis[curr_vert] = true;
			}
			ord.push_back(start);
			all_paths.push_back(ord);
		}
		decerease_feromones(evaporation);
		for(auto path : all_paths){
			double length = 0;
			for(int i = 0; i < path.size()-1; i++){
				length += distance(path[i], path[i+1]);
			}
			if(length < mini_lenght){
				mini_lenght = length;
				mini_path = path;
			}
			for(int i = 0; i < path.size()-1; i++){
				feromone_trail[path[i]][path[i+1]] += (1/length);
			}
		}

	}

	cout << mini_lenght << endl;
	for(int a : mini_path){
		cout << a+1 << " ";
	}

}
