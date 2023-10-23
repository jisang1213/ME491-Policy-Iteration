#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

int countsquares(Eigen::Vector<int, 12>& state);
int gameover(Eigen::Vector<int, 12>& state);

/// DO NOT CHANGE THE NAME AND FORMAT OF THIS FUNCTION
double getOptimalValue(const Eigen::Vector<int, 12>& state){
  // return the optimal value given the state
  /// TODO
  int greedyaction;
  Eigen::Vector<int, 12> stateaction;
  Eigen::Vector<int, 12> nextstate;
  int squares_before;
  int squares_after;
  int reward;
  int emptyspaces;
  double optimalvalue=0;

  //If terminal state, return 0.0
  if(gameover(state)){
    return 0.0;
  }

  //greedy action = getOptimalAction(s)
  greedyaction = getOptimalAction(state);
  //find current number of squares
  squares_before = countsquares(state);

  //do action
  stateaction = state;
  stateaction(greedyaction) = 1;

  //check if scored
  squares_after = countsquares(stateaction);

  if(squares_after>squares_before){
    reward = 1;
    //edge case: game ends on action
    if(gameover(stateaction)){
      return reward;
    }
    //game continues with another turn
    return reward + getOptimalValue(stateaction);
  }

  //V* = sum over s' of P(s'|s,a-greedy)[R+getOptimalValue(s')] where R=0
  //since opponent places randomly, divide sum by number of empty spaces to get average
  emptyspaces=0;
  for(int i=0; i<12; i++){
    if(stateaction(i)==0){
      nextstate = stateaction;
      nextstate(i)=1;
      emptyspaces++;
      optimalvalue += getOptimalValue(nextstate);
    }
  }

  optimalvalue = optimalvalue/emptyspaces;

  return optimalvalue;  // return optimal value
}

/// DO NOT CHANGE THE NAME AND FORMAT OF THIS FUNCTION
int getOptimalAction(const Eigen::Vector<int, 12>& state){
  // return one of the optimal actions given the state.
  // the action should be represented as a state index, at which a line will be drawn.
  /// TODO
  Eigen::Vector<int, 12> stateaction;
  Eigen::Vector<int, 12> nextstate;
  int squares_before;
  int squares_after;
  int reward;
  int emptyspaces;
  double maxactionvalue=0;
  double actionvalue;
  int optimalaction;

  //Loop over a:
  //argmax over a of sum over s' of P(s'|s,a)[R + getOptimalValue(s')]  (Loop over s')
  squares_before = countsquares(state);
  for(int i=0; i<12; i++){
    if(state(i)==0){
      actionvalue = 0;
      //do action
      stateaction = state;
      stateaction(i) = 1;

      //check if scored
      squares_after = countsquares(stateaction);

      if(squares_after>squares_before){
        reward = 1;
        //edge case: game ends on action
        if(gameover(stateaction)){
          actionvalue = reward;
        }
        else{
          actionvalue = reward + getOptimalValue(stateaction);
        }
      }
      else{
        //V* = sum over s' of P(s'|s,a-greedy)[R+getOptimalValue(s')] where R=0
        //since opponent places randomly, divide sum by number of empty spaces to get average
        emptyspaces=0;
        for(int j=0; j<12; j++){
          if(stateaction(j)==0){
            nextstate = stateaction;
            nextstate(j)=1;
            emptyspaces++;
            actionvalue += getOptimalValue(nextstate);
          }
        }
        actionvalue = actionvalue/emptyspaces;
      }
      if(actionvalue>maxactionvalue){
        maxactionvalue = actionvalue;
        optimalaction = i;
      }
    }
  }

  return optimalaction;  // return optimal action
}

int gameover(const Eigen::Vector<int, 12>& state){
  int end = 1;
  for(int i=0; i<12; i++){
    if(state(i)==0){
      end = 0;
    }
  }
  return end;
}

int countsquares(const Eigen::Vector<int, 12>& state){
  int count = 0;
  if(state(0)==1 && state(0)==state(8) && state(8)==state(2) && state(2)==state(6)){
    count++;
  }
  if(state(1)==1 && state(1)==state(10) && state(10)==state(3) && state(3)==state(8)){
    count++;
  }
  if(state(5)==1 && state(5)==state(9) && state(9)==state(3) && state(3)==state(11)){
    count++;
  }
  if(state(4)==1 && state(4)==state(7) && state(7)==state(2) && state(2)==state(9)){
    count++;
  }
  return count;
}