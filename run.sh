g++ tsp_aco_m.cpp -o bin/tsp_m -std=c++23 -O3
echo 'Berlin52'
./bin/tsp_m < tests/berlin52.in
echo 'Bier127'
./bin/tsp_m < tests/bier127.in
echo 'Tsp250'
./bin/tsp_m < tests/tsp250.in
echo 'Tsp500'
./bin/tsp_m < tests/tsp500.in
echo 'Tsp1000'
./bin/tsp_m < tests/tsp1000.in
