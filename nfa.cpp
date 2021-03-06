#include <bits/stdc++.h>
using namespace std;
#define max 117
#define stack_max 500

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


/*
Use of only these 3 operators are permitted : 
(1).
(2)|
(3)*
expressions like [1-3] should be written instead as 1|2|3
Each operator along with the operand must be enclosed within brackets
for example : ((a|b)*) ,(a)
Only regular expressions following the above rules will generate valid results
 */

class NFA_stack{
    NFA obj_stack[stack_max];
    int count=0;

    public:
    int is_empty(){
        if(count==0)return 1;
        return 0;
    }

    public:
    void push(NFA obj){
        obj_stack[count]=obj;
        count++;
    }

    public:
    NFA top(){
        return obj_stack[count-1];
    }

    public:
    void pop(){
        count--;
    }

};

NFA convert(string s){
    NFA_stack* operands = new NFA_stack();
    stack <string> operators;
    cout<<s.size()<<"\n";
    for(int i=0;i<s.size();++i){
        cout<<"start-"<<i<<"\n";
        if(s[i]!=')' && s[i]!='(' && s[i]!='&' && s[i]!='*' && s[i]!='|'){
            NFA nfa;
            nfa=init_nfa(string(1,s[i]));
            operands->push(nfa);
        }
        else if(s[i]=='('){
            operators.push("(");
        }
        else if (s[i]==')'){
            while(operators.top()!="("){
                string sym=operators.top();
                operators.pop();
                if(sym=="*"){
                    NFA nfa=operands->top();
                    operands->pop();
                    nfa=star_logic(nfa);;
                    operands->push(nfa);
                }
                else if(sym=="&"){
                    NFA op1,op2;
                    op1=operands->top();
                    operands->pop();
                    op2=operands->top();
                    operands->pop();
                    cout<<op1.node_count<<"-"<<op2.node_count<<"\n";
                    NFA nfa=and_logic(op2,op1);
                    operands->push(nfa);
                }
                else if(sym=="|"){
                    NFA op1,op2;
                    op1=operands->top();
                    operands->pop();
                    op2=operands->top();
                    operands->pop();
                    NFA nfa=or_logic(op2,op1);
                    operands->push(nfa);
                }
            }
            operators.pop();
        }
        else{
           operators.push(string(1,s[i])); 
        }
        cout<<"end-"<<i<<"\n";
    }
    return operands->top();
}


int main(){
    NFA obj0=convert("(((((((((0|1)|2)|3)|4)|5)|6))&.)&((((((((((0|1)|2)|3)|4)|5)|6)|7)|8)|9)&((((((((((0|1)|2)|3)|4)|5)|6)|7)|8)|9))))");
   obj0.display();
    return 0;
}

