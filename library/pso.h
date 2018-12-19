//20170914
/* shared memory */
int shared_memory[VARSIZE];

/* buffer */
int buffer[(PROCSIZE + 2) * VARSIZE];
byte gcounter;
byte lcounter[(PROCSIZE + 2) * VARSIZE];

/* store_buffer */
chan queue[(PROCSIZE + 2) * VARSIZE] = [BUFFSIZE] of {int};

/* commit operation */
#define COMMIT_WRITE(i, j)\
{\
    queue[(i) * VARSIZE + (j)]?v ->\
    if\
      ::v != -1 ->\
        shared_memory[j] = v; gcounter--;\
        lcounter[(i) * VARSIZE + (j)]--;\
        v = 0; \
      ::else ->\
        gcounter--;\
        int k = 0;\
        do\
          ::(k != j && len(queue[(i) * VARSIZE + k]) > 0) ->\
            queue[(i) * VARSIZE + k]?v ->\
            if\
              ::v != -1 ->\
                shared_memory[k] = v;\
                gcounter--;\
                lcounter[(i) * VARSIZE + (k)]--;\
              ::else ->\
                gcounter--; k++;\
                if\
                  ::(k >= VARSIZE) -> break;\
                  ::else -> skip;\
                fi;\
            fi;\
          ::(k != j && len(queue[(i) * VARSIZE + k]) == 0) ->\
            int l = 0;\
            do\
            ::(l < k) ->\
              queue[(i) * VARSIZE + l]!(-1);\
              l++;\
              gcounter++;\
            :: else -> break;\
            od;\
            break;\
          ::else ->\
            k++;\
            if\
              ::(k >= VARSIZE) -> break;\
              ::else -> skip;\
            fi;\
        od;\
        k = 0;\
        v = 0;\
    fi;\
}

/* memory process */
int v;
active proctype mem()
{
  int i, j, r;
endmem:
  do
    ::atomic
      {
        (gcounter != 0) ->
        r = gcounter;
        do
          ::(j < VARSIZE && len(queue[(i + 2) * VARSIZE + j]) > 0) ->
            printf("%d, %d\n", i, j);
            COMMIT_WRITE(i + 2, j);
            break;
          ::(i < PROCSIZE - 1 && j >= VARSIZE) ->
            i++;
            j = 0;
          ::(j < VARSIZE && r > len(queue[(i + 2) * VARSIZE + j])) ->
            r = r - len(queue[(i + 2) * VARSIZE + j]);
            j++;
        od;
        i = 0;
	      j = 0;
        r = 0;
        printf("endmem\n");
      }
  od;
}

#define READ(s) ( (lcounter[(_pid) * VARSIZE + (s)] == 0) -> shared_memory[s] : buffer[(_pid) * VARSIZE + (s)] )

#define WRITE(s, v)\
{\
  atomic{\
    (len(queue[(_pid) * VARSIZE + (s)]) < BUFFSIZE);\
    printf("WRITE\n");\
    int temp = v;\
    queue[(_pid) * VARSIZE + (s)]!temp;\
    buffer[(_pid) * VARSIZE + (s)] = temp;\
    gcounter++;\
    lcounter[(_pid) * VARSIZE + (s)]++;\
    temp = 0;\
    printf("WRITE\n");\
  }\
}

#define FENCE()\
{\
  atomic{\
    printf("FENCE\n");\
    int iterator = 0;\
    do\
      ::(len(queue[(_pid) * VARSIZE + iterator]) > 0) ->\
        queue[(_pid) * VARSIZE + iterator]?v ->\
        if\
          ::v != -1 ->\
            shared_memory[iterator] = v;\
            lcounter[(_pid) * VARSIZE + (iterator)]--;\
          ::else -> skip;\
        fi;\
        gcounter--;\
      ::(len(queue[(_pid) * VARSIZE + iterator]) == 0) ->\
        iterator++;\
        if\
          ::(iterator >= VARSIZE) -> break;\
          ::else -> skip;\
        fi;\
    od;\
    iterator = 0;\
    v = 0;\
    printf("FENCE\n");\
  }\
}

#define TFENCE(s)\
{\
  atomic{\
    do\
      ::(len(queue[(_pid) * VARSIZE + (s)]) > 0) -> COMMIT_WRITE(_pid, (s));\
      ::else -> break;\
    od;\
  }\
}

#define AFENCE() skip;

#define RFENCE()\
{\
  atomic {\
    int iterator = 0;\
    do\
      ::true ->\
        (len(queue[(_pid) * VARSIZE + (iterator)]) < BUFFSIZE);\
        queue[(_pid) * VARSIZE + (iterator)]!-1;\
        gcounter++;\
        iterator++;\
        if\
          ::(iterator >= VARSIZE) -> break;\
          ::else -> skip;\
        fi;\
    od;\
    iterator = 0\
  }\
}

#define TFENCE(s)\
{\
  atomic{\
    do\
      ::(len(queue[(_pid) * VARSIZE + (s)]) > 0) -> COMMIT_WRITE(_pid, (s));\
      ::else -> break;\
    od;\
  }\
}

#define INIT(s, v) shared_memory[s] = v;

#define SVAR(p, s) ( lcounter[(p:_pid) * VARSIZE + (s)] == 0 -> shared_memory[s] : buffer[(p:_pid) * VARSIZE + (s)] )

#define GSVAR(s) shared_memory[s]
