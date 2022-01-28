#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int HEALTHY_LIFE = 1e9;

struct planetState{
    bool infected, healing;

    // history data
    int parent;
    bool crewInfected;
    int remainingLife;

};

int N, M;
int s, t, l;
int K, D;

planetState* state;

vector<vector<int> > paths;
vector<int> walk;

int healerSrc;
int healerParent[35];
int healers[35];
queue<int> healerQ;

bool isNewStateBetter(planetState newSt, planetState oldSt){
    if(newSt.remainingLife < 0)
        return false;

    if(oldSt.parent == -1)
        return true;

    if(newSt.remainingLife > oldSt.remainingLife)
        return true;

    return false;
}


int getHealerIndex(int healer){
    for(int i = 0; i < D + 1; i++){

            if(healers[i] == healer)
            return i;
    }
            return -1;
}

void resetStates(){
    for(int i = 0; i < N; i++){
        state[i].parent = -1;
        state[i].crewInfected = false;
        state[i].remainingLife = HEALTHY_LIFE;
    }
}

bool bfs(int start, int target){
    resetStates();

    queue<int> q;
    q.push(start);
    state[start].parent = start;

    while(!q.empty()){
        int current = q.front();
        q.pop();

        if(current == target){
            int last = target;
            while(last != healerSrc){
                walk.push_back(last);
                last = state[last].parent;
            }

            return true;
        }

        if(current != start && state[current].healing == true){
            int healerIndex = getHealerIndex(current);

            if(healerParent[healerIndex] == -1){
                healerQ.push(current);
                healerParent[healerIndex] = healerSrc;
            }

            continue;
        }

        for(unsigned int i = 0; i < paths[current].size(); i++){
            int next = paths[current][i];

            planetState newSt;
            newSt.parent = current;
            newSt.crewInfected = state[current].crewInfected;
            newSt.remainingLife = state[current].remainingLife;

            if(state[current].crewInfected)
                newSt.remainingLife--;

            if(!state[current].crewInfected && state[next].infected){
                newSt.crewInfected = true;
                newSt.remainingLife = l;
            }

            if(isNewStateBetter(newSt, state[next])){
                state[next].parent = newSt.parent;
                state[next].crewInfected = newSt.crewInfected;
                state[next].remainingLife = newSt.remainingLife;

                q.push(next);
            }
        }
    }

    return false;
}

bool healerBfs(int start){
    bool pathExits = false;

    healerQ.push(start);

    int healerIndex = getHealerIndex(start);
    healerParent[healerIndex] = start;

    while(!healerQ.empty()){
        int current = healerQ.front();
        healerQ.pop();

        healerSrc = current;

        //cout<<"ITERATION"<<endl;
        if(bfs(current, t)){
            pathExits = true;
            break;
        }
    }

    if(!pathExits)
        return false;

    //cout<<"BACKTRACKING"<<endl;
    do{
        healerIndex = getHealerIndex(healerSrc);
        int oldSource = healerSrc;
        healerSrc = healerParent[healerIndex];
        bfs(healerSrc, oldSource);
    }while(healerSrc != s);

    return true;
}

int main()
{
    cin>>N>>M;

    state = new planetState[N];
    paths.resize(N);

    for(int i = 0; i < N; i++)
        state[i].infected = state[i].healing = false;

    cin>>s>>t>>l;

    cin>>K;

    int x, y;
    for(int i = 0; i < K; i++){
        cin>>x;
        state[x].infected = true;
    }

    cin>>D;

    for(int i = 0; i < 35; i++)
        healerParent[i] = -1;

    healers[0] = s;
    for(int i = 0; i < D; i++){
        cin>>x;
        state[x].healing = true;
        healers[i + 1] = x;
    }

    for(int i = 0; i < M; i++){
        cin>>x>>y;
        paths[x].push_back(y);
        paths[y].push_back(x);
    }

    if(healerBfs(s)){
        cout<<s;
        for(int i = walk.size() - 1; i >= 0 ; i--)
            cout<<" "<<walk[i];
        cout<<endl;
    }
    else
        cout<<-1<<endl;

    delete state;
    return 0;
}
