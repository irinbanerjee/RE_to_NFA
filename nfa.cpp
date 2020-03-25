#include <bits/stdc++.h>
using namespace std;
#define max 100

class NFA{
    public:
    int node_count=0;
    string transition_matrix[max][max];
    int final_state=0;
    int start_state=0;


    //initialize the NFA
    void init(){
        for(int i=0;i<node_count;++i){
            for(int j=0;j<node_count;++j){
                transition_matrix[i][j]="NIL";
            }
        }
    }

    //Display transition matrix
    void display(){
        cout<<"\t";
        for(int i=0;i<node_count;++i)cout<<i<<"\t";
        cout<<"\n";
        for(int i=0;i<node_count;++i){
            cout<<i<<"\t";
            for(int j=0;j<node_count;++j){
                cout<<transition_matrix[i][j]<<"\t";
            }
            cout<<"\n";
        }
    }
};


//one symbol 
NFA init_nfa(string s){
    NFA nfa;
    nfa.node_count=2;
    nfa.final_state=1;
    nfa.init();
    nfa.transition_matrix[0][1]=s;
    return nfa;
};

// For regular expression st
NFA and_logic(NFA nfa1,NFA nfa2){
    NFA nfa;
    nfa.node_count=nfa1.node_count+nfa2.node_count-1;
    nfa.final_state=nfa.node_count-1;
    nfa.init();
    for(int i=0;i<nfa1.node_count;++i){
        for(int j=0;j<nfa1.node_count;++j){
            nfa.transition_matrix[i][j]=nfa1.transition_matrix[i][j];
        }
    }
    int start=nfa1.node_count-1;
    for(int i=0;i<nfa2.node_count;++i){
        for(int j=0;j<nfa2.node_count;++j){
            nfa.transition_matrix[start+i][start+j]=nfa2.transition_matrix[i][j];
        }
    }
    return nfa;

};

//For regular expression s|t
NFA or_logic(NFA nfa1,NFA nfa2){
    NFA nfa;
    nfa.node_count=nfa1.node_count+nfa2.node_count+2;
    nfa.final_state=nfa.node_count-1;
    nfa.init();
    nfa.transition_matrix[0][1]="ESN";
    nfa.transition_matrix[0][nfa1.node_count+1]="ESN";

    int start=1;
    for(int i=0;i<nfa1.node_count;++i){
        for(int j=0;j<nfa1.node_count;++j){
            nfa.transition_matrix[start+i][start+j]=nfa1.transition_matrix[i][j];
        }
    }
    start=nfa1.node_count+1;
    for(int i=0;i<nfa2.node_count;++i){
        for(int j=0;j<nfa2.node_count;++j){
            nfa.transition_matrix[start+i][start+j]=nfa2.transition_matrix[i][j];
        }
    }

    nfa.transition_matrix[nfa1.node_count][nfa.final_state]="ESN";
    nfa.transition_matrix[nfa.final_state-1][nfa.final_state]="ESN";
    return nfa;

};

//For regular expression s*
NFA star_logic(NFA nfa1){
    NFA nfa;
    nfa.node_count=nfa1.node_count+2;
    nfa.final_state=nfa.node_count-1;
    nfa.init();
    nfa.transition_matrix[0][1]="ESN";
    int start=1;
    for(int i=0;i<nfa1.node_count;++i){
        for(int j=0;j<nfa1.node_count;++j){
            nfa.transition_matrix[start+i][start+j]=nfa1.transition_matrix[i][j];
        }
    }
    nfa.transition_matrix[nfa.final_state-1][nfa.final_state]="ESN";
    nfa.transition_matrix[nfa.final_state-1][1]="ESN";
    nfa.transition_matrix[0][nfa.final_state]="ESN";
    return nfa;

};

int main(){
    NFA obj0=init_nfa("a");
    NFA obj1=init_nfa("b");
    NFA obj2=or_logic(obj0,obj1);
    NFA obj3=star_logic(obj2);
    obj3.display();
    return 0;
}
