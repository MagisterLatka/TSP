#include <ios>
#include <iostream>
#include <iterator>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <set>

using namespace std;

typedef pair<int,int> pii;
typedef tuple<int, int, int> tii;

vector<tii> points;

const int MX = 100;
int iterations = 200;
int ant_count = 300;
double evaporation = 0.9;
int n;

double feromone_trail[MX][MX] {};
double all_distance[MX][MX] {};
double mini_lenght = numeric_limits<double>::infinity();
vector<int> mini_path;
int mini_idx;

double a_p = 1;
double b_p = 6;

double avg_fer;

int next_vertex(int a, bool vis[]){
	double total_length = 0;
	double total_feromoes = 1;
	for(int i = 0; i < n;i ++){
		if(vis[i])
			continue;
		total_length += all_distance[a][i];
		total_feromoes += feromone_trail[a][i];
	}
	double probabilites[MX] {};
	for(int i = 0; i < n; i++){
		if(vis[i])
			continue;
		/* cout << feromone_trail[a][i]/total_feromoes << " " << all_distance[a][i]/total_length << endl; */
		probabilites[i] = pow(feromone_trail[a][i]/total_feromoes, a_p) + pow(all_distance[a][i]/total_length, b_p);
		/* cout << probabilites[i] << endl; */
	}
	double sum = 0;
	for(int i = 0; i < n; i++){
		sum += probabilites[i];
	}
	/* cout << "sum  " << sum << endl; */
	double prob = rand()%100;
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
		if(prob - probabilites[vert] < 0)
			break;
		prob -= probabilites[vert];
		/* cout << prob << " | "; */
		vert++;
	}
	avg_fer += feromone_trail[a][vert];
	/* cout << endl << "wybrany: " << vert << " "<< endl; */

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
			feromone_trail[i][k] = 0;
		}
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);
	srand(time(NULL));
	zero();
	cin >> n;
	for(int i = 0; i < n; i++){
		int a, b, idx;
		cin >> idx >> a >> b;
		points.push_back({a, b, idx});	
	}

	for(int i = 0; i < n; i++){
		for(int k = 0; k < n; k++){
			all_distance[i][k] = distance(i, k);
		}
	}

	while(iterations --> 0){
		avg_fer = 0;
		double avg = 0;
		vector<vector<int>> all_paths;
		for(int ant_nb = 0; ant_nb < ant_count; ant_nb++){
			vector <int> ord;
			set<int> saul_goodman;
			bool vis[MX] {};
			int start = rand()%n;
			vis[start] = true;
			ord.push_back(start);
			int curr_vert = start;
			for(int proc_vert = 1; proc_vert < n; proc_vert ++){
				int next_vert = next_vertex(curr_vert, vis);
				ord.push_back(next_vert);
				saul_goodman.insert(next_vert);
				curr_vert = next_vert;
				vis[curr_vert] = true;
			}
			if(saul_goodman.size() != n-1){
				cout << "adios amigos " << endl;
				return 0;
			}
			ord.push_back(start);
			all_paths.push_back(ord);
		}
		/* cout << endl; */
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
				feromone_trail[path[i]][path[i+1]] += (1000/length);
			}
			n++;
		}
		cout << "--------  " << avg/ant_count << "\t" << avg_fer << endl;
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
	cout << distance(0, 1);
}
