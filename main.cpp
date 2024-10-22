#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>

constexpr int MX = 100007;
double** dist = new double[MX][MX];
std::pair<double, double> pos[MX];
std::bitset<MX> visited;

int main()
{
	std::cout << "Podaj n:" << std::endl;
	int n;
	std::cin >> n;

	if (n > MX)
	{
		std::cout << "Za duze n\n";
		return 1;
	}

	for (int i = 0; i < n; ++i)
	{
		std::cout << "Podaj wspolrzedne punktu: ";
		std::cin >> pos[i].first >> pos[i].second;
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			dist[i][j] = sqrt(pow(pos[i].first - pos[j].first, 2.0) + pow(pos[i].second - pos[j].second, 2.0));
		}
	}

	std::cout << "Podaj indeks startowego wierzcholka: ";
	int start;
	std::cin >> start;
	visited[start] = true;


	while (!visited.all())
	{

	}
}
