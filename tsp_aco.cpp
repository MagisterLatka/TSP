#include <ios>
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
int iterations = 2000;
int ant_count = 300;
double evaporation = 0.8;
int n;

double feromone_trail[MX][MX] {};
double mini_lenght = numeric_limits<double>::infinity();
vector<int> mini_path;
int mini_idx;

int next_vertex(double prob, int a, bool vis[]){
	double sum = 0;
	for(int i = 0; i < n; i++){
		if(vis[i])
			continue;
		sum += feromone_trail[a][i];
	}
	/* cout << "sum  " << sum << endl; */
	/* cout << prob << " "; */
	prob = (prob*sum)/100.0;
	int vert = 0;
	/* cout << prob << endl; */
	while( vert < n){
		/* cout << "co"; */
		if(vis[vert]){
			vert++;
			continue;
		}
		if(prob - feromone_trail[a][vert] < 0)
			break;
		/* cout << feromone_trail[a][vert] << " -> "; */
		prob -= feromone_trail[a][vert];
		/* cout << prob << " | "; */
		vert++;
	}
	/* cout << vert << endl;; */

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
	for(int i = 0; i < MX; i++){
		for(int k = 0; k < MX; k++){
			feromone_trail[i][k] = 1;
		}
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	srand(time(NULL));
	zero();
	cin >> n;
	for(int i = 0; i < n; i++){
		int a, b, idx;
		cin >> idx >> a >> b;
		points.push_back({a, b, idx});	
	}


	while(iterations --> 0){
		double avg = 0;
		vector<vector<int>> all_paths;
		for(int ant_nb = 0; ant_nb < ant_count; ant_nb++){
			vector <int> ord;
			bool vis[MX] {};
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
		int n = 0;
		for(auto path : all_paths){
			double length = 0;
			for(int i = 0; i < path.size()-1; i++){
				length += distance(path[i], path[i+1]);
			}
			avg += length;
			/* cout << length << endl; */
			/* for(int a : path){ */
			/* 	cout << a << " "; */
			/* } */
			/* cout << endl; */
			if(length < mini_lenght){
				mini_lenght = length;
				mini_path = path;
				mini_idx = n;
			}
			for(int i = 0; i < path.size()-1; i++){
				/* cout << (100000/length) << endl; */
				feromone_trail[path[i]][path[i+1]] += (100000/length);
			}
			n++;
		}
		cout << "--------  " << avg/ant_count << endl;
	}
	
	cout << mini_idx << endl;
	cout << mini_lenght << endl;
	for(int a : mini_path){
		cout << a+1 << " ";
	}

	cout << endl;
	for(int i = 0; i < n; i++){
		for(int k =0; k < n; k++){
			cout << feromone_trail[i][k] << " ";
		}
		cout << endl;
	}

}
