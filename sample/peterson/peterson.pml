#define PROCSIZE 2
#define VARSIZE 3
#define BUFFSIZE 5
#include "pso.h"
#define want0 0
#define want1 1
#define turn 2

proctype T0() {
  WRITE(want0, 1);
  WRITE(turn, 1);
  FENCE();
  if
  ::(READ(turn) == 0)  -> skip;
  ::(READ(want1) == 0) -> skip;
  fi;
CS: skip; /*Critical Section*/
  WRITE(want0, 0);
}

proctype T1() {
  WRITE(want1, 1);
  WRITE(turn, 0);
  FENCE();
  if
  ::(READ(turn) == 1)  -> skip;
  ::(READ(want0) == 0) -> skip;
  fi;
CS: skip; /*Critical Section*/
  WRITE(want1, 0);
}

init {
  atomic {
    run T0();
    run T1();
  }
}

ltl prop {! <> (T0@CS && T1@CS)}
