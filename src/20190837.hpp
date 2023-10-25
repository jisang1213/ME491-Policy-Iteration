#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

//global variables
double value[4096]; //stores values for states from 0 to 4095
int policy[4095]; //stores policy for states from 0 to 4094
int stable;
int converged;

int numeval = 0;

//prototypes
void policy_iteration();
void evaluate_policy();
void improve_policy();
int tobinary(const Eigen::Vector<int, 12>& state);
int countsquares(int state);
double expectedvalue(int state);


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
  for(int state=0; state<4096; state++){
    value[state]=0;
  }
  //initialize policy
  for(int state=0; state<4095; state++){
    //search for possible action(empty spaces) from given state and choose first available action
    for(int action=0; action<12; action++){
      if(!(state&(1<<action))){//if space is empty
        policy[state] = action; //initialize policy to first available action
        break;
      }
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
    if(stable==0){
      std::cout<<"policy unstable"<<std::endl;
    }
    else{
      std::cout<<"policy stable"<<std::endl;
    }
  }

//  std::cout<<"policy iterated\n"<<std::endl;
//  for(int i=4095; i>=0; i--) {
//    std::cout<<value[i]<<", ";
//  }

  std::cout << "times evaluated: \n" << numeval << std::endl;
  std::cout<<"END\n"<<std::endl;

}

void evaluate_policy(){
  int reward;
  int squares_prev;
  int squares_after;
  int stateaction;
  int nextstate;
  double expectedfuturevalue;

  for(int state=4094; state>=0; state--){ //for each state
  //for(int state=0; state<4095; state++){ //TEST
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
      expectedfuturevalue = expectedvalue(stateaction);
      if(value[state] != expectedfuturevalue){
        value[state] = expectedfuturevalue; //update value
        converged = 0;
      }
    }
  }
  std::cout<<"policy evaluated"<<std::endl;
  numeval++;
}

double expectedvalue(int state){
  int reward;
  int squares_prev;
  int squares_after;
  int stateaction;
  int count=0;
  double sum=0;
  double average;

  //base case:
  if(state==4095){
    return 0;
  }
  
  //recursion:
  squares_prev = countsquares(state);
  //opponent plays until turn terminates. return sum of values weighted by probability
  for(int action=0; action<12; action++){
    if(!(state&(1<<action))){//if action is available
      //do action
      stateaction = state | (1<<action);
      squares_after = countsquares(stateaction);
      reward = squares_after-squares_prev;
      if(reward){
        sum += expectedvalue(stateaction);
      }
      else{
        sum+=value[stateaction];
      }
      count++;
    }
  }
  average = sum/count;

  return average;
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
          expectedfuturevalue = expectedvalue(stateaction);
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
  std::cout<<"policy improved"<<std::endl;
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