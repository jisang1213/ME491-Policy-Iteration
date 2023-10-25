#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

//global variables
double value[4096]; //stores values
int policy[4095]; //stores policy
int stable;
int converged;

//prototypes
void policy_iteration();
void evaluate_policy();
void improve_policy();
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
  value[4095] = 0; //4095 is the terminal state
  for(int state=0; state<4095; state++){
    value[state]=2; //initialize values to 2 
  }
  //initialize policy
  for(int state=0; state<4095; state++){
    //search for possible action(empty spaces) from given state and choose first available action
    for(int j=0; j<12; j++){
      if(!(state&(1<<j))) //if space is empty
      policy[state] = j; //initialize policy to first available action
      break;
    }
  }

  while(!stable){
    stable=1;
    while(!converged){
      converged = 1;
      evaluate_policy(); //Evaluate on-policy value
    }
    converged=0;

    //policy improvement: set policy as argmax over current value function
    improve_policy();
  }
}

void evaluate_policy(){
  int reward;
  double expectedvalueofnextstate;
  int squares_prev;
  int squares_after;
  int squares_remaining;
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
      squares_remaining = 4-squares_after;
      expectedvalueofnextstate = squares_remaining - value[stateaction]; //user's expected value is remaining square - opponents value
      if(value[state] != (reward + expectedvalueofnextstate)){
        value[state] = reward + expectedvalueofnextstate; //update value
        converged = 0;
      }
    }
  }
}


//policy improvement
void improve_policy(){
  int reward;
  double expectedvalueofnextstate;
  double actionvalue;
  double maxactionvalue;
  int squares_prev;
  int squares_after;
  int squares_remaining;
  int stateaction;
  int nextstate;
  int argmax;
  int stable;

  //update policy by taking argmax over new values
  for(int state=0; state<4095; state++){ //for each state
    maxactionvalue=-1;
    for(int action=0; action<12; action++){
      if(!(state&(1<<action))){  //for each possible action
        //do action
        stateaction = state | (1<<action);
        //check for reward
        squares_prev = countsquares(state);
        squares_after = countsquares(stateaction);
        reward = squares_after-squares_prev;
        if(reward){
          nextstate = stateaction;
          actionvalue = reward + value[nextstate];
        }
        else{
          squares_remaining = 4-squares_after;
          expectedvalueofnextstate = squares_remaining - value[stateaction];
          actionvalue = expectedvalueofnextstate; //since no reward
        }
        if(actionvalue>maxactionvalue){
          maxactionvalue = actionvalue;
          argmax = action;
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
  //Access values using value[binarystate]
  //Access actions using policy[binarystate]
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