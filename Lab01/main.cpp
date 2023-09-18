
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <climits>
#include <limits>
#include <time.h>
#include <math.h>
#include <bitset>
#include <numeric>
#include <assert.h>
#include <functional>
#define vi vector<int>
#define vvi vector<vi>
#define vl vector<long long>
#define vvl vector<vl>
using namespace std;
const int MOD = 1e9 + 7;

const int MX = 8000;
int t;
long long n, k;
double A[MX][MX], x[MX], y[MX];
void init(int temp){
    for(int i = 0; i < temp; i++){
        for(int j = 0;j < temp; j++){
            A[i][j] = i*temp+j+1;
        }
    }
    for(int i = 0; i < temp; i++){
        x[i] = 100 + i;
    }
    for(int i = 0; i < temp; i++) {
        y[i] = 0;
    }
}

void f1(int temp){
    for (int i = 0; i < temp; i++){
        for (int j = 0; j < temp; j++){
            y[i] += A[i][j]*x[j];
        }
    }
}

void f2(int temp){
    for (int j = 0; j < temp; j++){
        for (int i = 0; i < temp; i++){
            y[i] += A[i][j]*x[j];
        }
    }
}

void test(int ini, int fin){
    double time_spent = 0.0;
    vector<pair<double,double>> bucle1(10);
    vector<pair<double,double>> bucle2(10);
    int step = (fin-ini)/10;
    for(int i = 0; i < 10; i++){
        clock_t begin = clock();
        // primer bucle
        f1(ini + step*(i+1));
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        // printf("Bucle 1 => %f seconds", time_spent);
        bucle1[i] = {ini + step*(i+1), time_spent};

        init(ini + step*(i+1));

        begin = clock();
        // segundo bucle
        f2(ini + step*(i+1));
        end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        // printf("\nBucle 2 => %f seconds", time_spent);
        bucle2[i] = {ini + step*(i+1), time_spent};
    }
    cout << "bucle1:\n";
    for(auto x : bucle1) cout << x.first << " " << x.second << "\n";
    cout << "\nbucle2:\n";
    for(auto x : bucle2) cout << x.first << " " << x.second << "\n";
    
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    init(MX);
    cout << "test 1\n";
    test(0, 2500);
    cout << "\ntest 2\n";
    test(2500, 8000);
    return 0;
}