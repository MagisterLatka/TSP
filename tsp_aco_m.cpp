#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <cfloat>
#include <chrono>

struct ConnectionData {
	double length;
	double invLength;
	double intensity;
	double deltaIntensity;
};
struct AntData {
	std::vector<int> order;
	std::vector<bool> visited;
};

const double c = 5.0, alpha = 1.0, beta = 5.0, decayRatio = 0.5, q = 1000.0, maxTime = 180.0;
const int maxNumberOfCycles = 1000, stagnationCount = 50;
std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_real_distribution<> probability(0.0, 1.0);

double getLength(const std::pair<int, int>& a, const std::pair<int, int>& b);
int findNextPoint(const std::vector<std::vector<ConnectionData>>& data, const std::vector<AntData>& ants, int n, int currentPos, int currentAnt);
double calcTourLength(const std::vector<std::vector<ConnectionData>>& data, const std::vector<int>& order);

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);

	int n;
	std::cin >> n;

	std::vector<std::vector<ConnectionData>> data(n);
	std::vector<std::pair<int, int>> positions(n);
	std::vector<AntData> ants(n);
	for (int i = 0; i < n; ++i) {
		int index, x, y;
		std::cin >> index >> x >> y;
		positions[index - 1] = std::make_pair(x, y);
		data[i].resize(n);
	}
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			double length = getLength(positions[i], positions[j]);
			data[i][j] = { length, 1.0 / length, c, 0.0 };
			data[j][i] = { length, 1.0 / length, c, 0.0 };
		}
	}

	auto start = std::chrono::high_resolution_clock::now();
	int numberOfCycles = maxNumberOfCycles, sameResults = 0;
	double shortestTour = DBL_MAX, timeSpent = 0.0;
	while (numberOfCycles-- > 0 && sameResults < stagnationCount) {//&& !stagnation
		bool foundBetter = false;
		for (int i = 0; i < n; ++i) {
			ants[i].order.clear();
			ants[i].order.push_back(i);
			ants[i].visited = std::move(std::vector<bool>(n, false));
			ants[i].visited[i] = true;

			for (int j = 0; j < n - 1; ++j) {
				int nextPoint = findNextPoint(data,	ants, n, i, ants[i].order.back());
				ants[i].visited[nextPoint] = true;
				ants[i].order.push_back(nextPoint);
			}

			double tourLength = calcTourLength(data, ants[i].order);
			if (tourLength < shortestTour) {
				shortestTour = tourLength;
				foundBetter = true;
			}

			for (int j = 0; j < ants[i].order.size() - 1; ++j) {
				data[ants[i].order[j]][ants[i].order[j + 1]].deltaIntensity += q / tourLength;
				data[ants[i].order[j + 1]][ants[i].order[j]].deltaIntensity += q / tourLength;
			}
			data[ants[i].order.back()][ants[i].order.front()].deltaIntensity += q / tourLength;
			data[ants[i].order.front()][ants[i].order.back()].deltaIntensity += q / tourLength;
		}
		if (!foundBetter) {
			++sameResults;
		}
		else
			sameResults = 0;

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				data[i][j].intensity = data[i][j].intensity * decayRatio + data[i][j].deltaIntensity;
				data[i][j].deltaIntensity = 0.0;
			}
		}

		auto stop = std::chrono::high_resolution_clock::now();
		timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
		if (timeSpent > maxTime * 1000.0) {
			break;
		}
	}
	std::cout << "Shortest tour found: " << shortestTour << " in time " << (timeSpent / 1000.0) << std::endl;
}
double getLength(const std::pair<int, int>& a, const std::pair<int, int>& b) {
	return std::pow(std::pow((double)a.first - (double)b.first, 2.0) + std::pow((double)a.second - (double)b.second, 2.0), 0.5);
}
int findNextPoint(const std::vector<std::vector<ConnectionData>>& data, const std::vector<AntData>& ants, int n, int currentAnt, int currentPos) {
	std::vector<double> nominator(n);
	double denominator = 0.0;
	for (int i = 0; i < n; ++i) {
		double value = 0.0;
		if (i != currentPos && !ants[currentAnt].visited[i]) {
			value = std::pow(data[currentPos][i].intensity, alpha) * std::pow(data[currentPos][i].invLength, beta);
		}
		nominator[i] = value;
		denominator += value;
	}
	double currentSum = 0.0, probabilityResult = probability(gen) * denominator;
	for (int i = 0; i < n; ++i) {
		if (currentSum + nominator[i] > probabilityResult) {
			return i;
		}
		currentSum += nominator[i];
	}
	return -1;
}
double calcTourLength(const std::vector<std::vector<ConnectionData>>& data, const std::vector<int>& order) {
	double total = 0.0;
	for (int i = 0; i < order.size() - 1; ++i) {
		total += data[order[i]][order[i + 1]].length;
	}
	total += data[order.back()][order.front()].length;
	return total;
}
