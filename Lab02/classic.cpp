
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
vvi A, B, C;
void init(int temp){
    for(int i = 0; i < temp; i++){
        for(int j = 0; j < temp; j++){
            A[i][j] = i*j+1;
        }
    }
    for(int i = 0; i < temp; i++){
        for(int j = 0; j < temp; j++){
            B[i][j] = i+j;
        }
    }
    for(int i = 0; i < temp; i++){
        for(int j = 0; j < temp; j++){
            C[i][j] = 0;
        }
    }
}

void mult1(int temp){
    for (int i = 0; i < temp; i++) {
        for (int j = 0; j < temp; j++) {  
            C[i][j] = 0;
            for (int k = 0; k < temp; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }  
        }  
    }
}

void test(int ini, int fin){
    int step = (fin-ini)/10;
    for(int i = 0; i < 10; i++){
        double time_spent = 0.0;
        init(ini + step*(i+1));

        clock_t begin = clock();
        mult1(ini + step*(i+1));
        clock_t end = clock();

        time_spent = (double)(end - begin)*1.0 / CLOCKS_PER_SEC;
        cout << ini + step*(i+1) << " " << time_spent<< "\n";
        if(ini + step*(i+1) >= fin) break;
    }    
}
int main() {
    cout.precision(10);
    A.assign(MX, vi(MX, 0));
    B.assign(MX, vi(MX, 0));
    C.assign(MX, vi(MX, 0));
    test(1000, 1000);
    return 0;
}