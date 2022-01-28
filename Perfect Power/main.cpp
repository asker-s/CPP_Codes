// C++ program to print all prime factors
#include <bits/stdc++.h>
using namespace std;

// A function to print all prime
// factors of a given number n
size_t check_pp(long long int start, long long end)
{
set <int> arr;
arr.clear();
        if (start == 1) {
                arr.insert(1);
        }
        for (long long i = 2; i <= sqrt(end); i++) {
        long long int b = 2;
                while(pow(i,b) <= end){
                        if (pow(i,b) >= start) {
                                arr.insert(pow(i,b));
                        }
                b++;
                }
        }
        return arr.size();
}


int main(){

long int read_input;
long long int x,y;

cout << "Intervals:" << endl;

while ((read_input = scanf("%lld %lld",&x,&y)) != EOF) {

        if (read_input != 2 || x < 1 || y < 1 || x > y) {
                printf("Invalid input.\n");
                return 0;
        }
        //printing out y - x + 1 - number of perfect powers
        printf("[%lld;%lld] -> %lld\n",x,y,y-x+1-check_pp(x,y));
}

return 0;
}
