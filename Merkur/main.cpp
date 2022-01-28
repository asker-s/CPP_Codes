#include <bits/stdc++.h>

using namespace std;

bool is_triangle(long long int a, long long int b, long long int c){
        if (a + b > c && a + c > b && b + c > a) {
                return true;
        }
        return false;
}

long long int count_aaa(vector<long long int> v){
        long long int count = v.size();
        return count;
}

long long int count_abb(vector<long long int> v, set<long long int> s){
        long long int count = 0;
        set<long long int>::iterator it;
        for (unsigned i = 0; i < v.size(); i++) {
                for (it = s.begin(); it != s.end(); it++) {
                        if (is_triangle(v[i],v[i],*it) == true && v[i] != *it) {
                                count++;
                        }
                }
        }
        return count;
}

long long int count_abc(vector<long long int> v){
        long long int count = 0;
        for (unsigned i = 0; i < v.size() - 2; ++i) {
            unsigned k = i + 2;
            for (unsigned j = i + 1; j < v.size(); ++j) {
                while (k < v.size() && v[i] + v[j] > v[k])
                    ++k;
                if (k > j)
                    count += k - j - 1;
            }
        }
        return count;
}

int main(){

vector<pair<long long int,long long int>> beam;
vector<long long int> beam_aaa;
set<long long int> beam_abc;
vector<long long int> beam_abc1;
set<long long int>::iterator it;
vector<long long int> beam_abb;
long long int x;
long long int read_input = 0;
long long int yordu;
long long int tmp;
long long int invalid = 0;
long long int result = 0;

printf("Beam lengths:\n");


while ((read_input = scanf("%lld",&x)) != EOF) {
        if (read_input != 1 || x < 1) {
                printf("Invalid input.\n");
                return 0;
        }
        if (beam.empty()) {
                beam.push_back(make_pair(x,0));
                beam_abc.insert(x);
        }
        else{
                yordu = 0;
                for (unsigned i = 0; i < beam.size(); i++) {
                        if (x == beam[i].first) {
                                yordu++;
                                tmp = i;
                        }
                }
                if (yordu >= 1) {
                        beam[tmp].second++;
                }
                else{
                        beam.push_back(make_pair(x,0));
                        beam_abc.insert(x);
                }
        }

}
if (beam.size() < 3) {
        for (unsigned a = 0; a < beam.size(); a++) {
                invalid += beam[a].second;
        }
        if (invalid < 1) {
                printf("Invalid input.\n");
                return 0;
        }
}
for (unsigned i = 0; i < beam.size(); i++) {
        if (beam[i].second >= 1) {
                beam_abb.push_back(beam[i].first);
        }
        if (beam[i].second >= 2){
                beam_aaa.push_back(beam[i].first);
        }
}

for (it = beam_abc.begin(); it != beam_abc.end(); it++) {
        beam_abc1.push_back(*it);
}

result += count_aaa(beam_aaa);
if (beam_abc1.size() >= 3) {
        result += count_abc(beam_abc1);
}
result += count_abb(beam_abb,beam_abc);
cout << "Triangles: " << result << endl;

return 0;
}
