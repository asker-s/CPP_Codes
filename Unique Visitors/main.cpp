#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <set>

using namespace std;

int main () {

    int id;

    int res;

    //map to hold visits key is their id and second is their counter how many times they visited
    map<int, int> server;

    //keeping the total number of visits ids doesnt have to be unique
    vector<int> total;

    //keeping only unique ids 
    set<int> unique;

    char sign;

    cout << "Requests:" << endl;

    while (1) {

    	//taking the requests
        cin >> sign;

        //checking for end of file
        if(cin.eof())
            break;

        //using map to hold
        map<int, int>::iterator it;

        if (sign == '+') {

        	//skipping input and input validation
            if ((res = scanf("%d",&id)) != 1) {

                cout << "Invalid input." << endl;

                return 0;
            }

            if (id < 0 || id > 99999) {

                cout << "Invalid input." << endl;

                return 0;
            }

            if (total.size() == 1000000)
            {
                cout << "Invalid input." << endl;

                return 0;
            }
            //if input is correct trying to find the id on the map if it exists 
            it = server.find(id);

            //if I dont find I insert new visit which is new uniqe visit id
            if (it == server.end()) {

                server.insert(pair<int,int>(id,1));
                //everytime I find unique id I keep their id on a vector(unique ids)
                total.push_back(id);
                cout << "> first visit" << endl;
            }
            else {
            	//if I find the id on the map then I increase it is second(which is a counter in my case for visits) 
                it->second += 1;
                total.push_back(id);
                cout << "> visit #" << it->second << endl;
            }
        }
        else if (sign == '?') {
        	//getting min and max
            int a, b;
            //skipping input and input validation
            if ((res = scanf("%d  %d",&a, &b) != 2)) {
                cout << "Invalid input." << endl;
                return 0;
            }

            if (a > b || b >= (int)total.size() || a < 0 )
            {
                cout << "Invalid input." << endl;
                return 0;
            }

            int i;
            //and startting from total[min] to total[max] as set will only old unique ones 
            for (i = a; i <= b; ++i)
            {
                unique.insert(total[i]);
            }
            //and input
            cout << "> " << unique.size() << " / " << b - a + 1 << endl;
            unique.clear();
        }
        else {
            cout << "Invalid input." << endl;
            return 0;
        }

        if (res == EOF)
            break;
    }
    return 0;
}
