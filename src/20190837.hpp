#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

double value[4096];
int policy[4095];
int binarystate;
int stable;
int converged;

void improve_policy();
void evaluate_policy();
void policy_iteration(const Eigen::Vector<int, 12>& state);
int tobinary(const Eigen::Vector<int, 12>& state);
int countsquares(int state);

/// DO NOT CHANGE THE NAME AND FORMAT OF THIS FUNCTION
double getOptimalValue(const Eigen::Vector<int, 12>& state){
  policy_iteration();
  return value[tobinary(state)];
}

/// DO NOT CHANGE THE NAME AND FORMAT OF THIS FUNCTION
int getOptimalAction(const Eigen::Vector<int, 12>& state){
  policy_iteration();
  return policy[tobinary(state)];
}

void policy_iteration(){
  //store state-value and state-action in array and represent state in binary for index
  stable=0;
  converged=0;

  //initialize values
  value[4095] = 0; //4095 is terminal state
  for(int i=0; i<4095; i++){
    value[i]=2; //initialize to 2
  }
  //initialize policy
  for(int i=0; i<4095; i++){
    //search for possible action(empty spaces) from given state and choose first available action
    for(int j=0; j<12; j++){
      if(!(i&(1<<j)))
      policy[i] = j;
      break;
    }
  }

  while(!stable){
    stable=1;
    //Evaluate on-policy value recursively starting from given state
    while(!converged){
      converged = 1;
      evaluate_policy();
    }
    converged=0;

    //policy improvement: set policy as argmax over current value function
    improve_policy();
  }
}

void evaluate_policy(){
  int reward;
  int numstates;
  double valuesum;
  int squares_prev;
  int squares_after;
  int stateaction;
  int nextstate;

  for(int state=4094; state>=0; state--){ //for each state
    //do action according to policy
    stateaction = state | (1<<policy[state]);
    //check for reward
    squares_prev = countsquares(state);
    squares_after = countsquares(stateaction);
    reward = squares_after-squares_prev;

    // given an action, the reward is decided
    if(reward){
      nextstate = stateaction;
      if(value[state] != (reward + value[nextstate])){
        value[state] = reward + value[nextstate]; //update value
        converged = 0;
      }
    }
    else{
      for(int i=0; i<12; i++){ //update V(s') first
        if(!(stateaction&(1<<i))){ //for all s'
          nextstate = stateaction | (1<<i);
        }
      }
    }

    numstates =0;
    valuesum=0;
    for(int i=0; i<12; i++){ //loop to sum over V(s')
      if(!(stateaction&(1<<i))){
        nextstate = stateaction | (1<<i);
        valuesum += value[nextstate];
        numstates++;
      }
    }

    if(value[state] != (reward + valuesum/numstates)){
      value[state] = reward + valuesum/numstates; //update value
      converged = 0;
    }
  }
}


//policy improvement
void improve_policy(){
  int reward;
  int numstates;
  double valuesum;
  double actionvalue;
  double maxactionvalue;
  int squares_prev;
  int squares_after;
  int stateaction;
  int nextstate;
  int argmax;
  int stable;

  //update policy by taking argmax over new values
  for(int state=0; state<4095; state++){ //for each state
    maxactionvalue=-1;
    for(int j=0; j<12; j++){
      if(!(state&(1<<j))){  //for each possible action
        //do action
        stateaction = state | (1<<j);
        //check for reward
        squares_prev = countsquares(state);
        squares_after = countsquares(stateaction);
        reward = squares_after-squares_prev;
        if(reward){
          nextstate = stateaction;
          actionvalue = reward + value[nextstate];
        }
        else{
          numstates=0;
          valuesum=0;
          for(int k=0; k<12; k++){
            if(!(stateaction&(1<<k))){ //for all s'
              nextstate = stateaction | (1<<k);
              valuesum += value[nextstate];
              numstates++;
            }
          }
          actionvalue = valuesum/numstates; //since no reward
        }
        if(actionvalue>maxactionvalue){
          maxactionvalue = actionvalue;
          argmax = j;
        }
      }
    }
    if(policy[state] != argmax){
      policy[state] = argmax;
      stable = 0;
    }
  }
}


//auxillary functions
int tobinary(const Eigen::Vector<int, 12>& state){
  int binarystate=0;
  for(int i=0; i<12; i++){
    binarystate |= (state(i) << i);
  }
  return binarystate;
  //value = value[binarystate]
  //action = action[binarystate];
}

int countsquares(int state){
  int count = 0;
  if((state&325) == 325){
    count++; //upper left square
  }
  if((state&1290) == 1290){
    count++; //upper right square
  }
  if((state&660) == 660){
    count++; //bottom left square
  }
  if((state&2600) == 2600){
    count++; //bottom right square
  }
  return count;
}