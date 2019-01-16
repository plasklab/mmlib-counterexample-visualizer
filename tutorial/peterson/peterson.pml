#define PROCSIZE 2
#define VARSIZE 3
#define BUFFSIZE 5
#include "pso.h"
#define want0 0
#define want1 (want0 + 1)
#define turn (want1 + 1)

proctype T0() {
  WRITE(want0, 1);
  WRITE(turn, 1);
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
