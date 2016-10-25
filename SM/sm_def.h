//
// Created by ubun on 16-5-17.
//

#ifndef LC_SM_DEF_H
#define LC_SM_DEF_H


#define STATES 1
#define STATE_NAME 2
#define STATE_DATA 3
#define STATE_PARE 4
#define STATE_ENTRY 5
#define STATE_NEXT 6
#define STATE_TRAN 7
#define ENTRY_ACT 8
#define EXIT_ACT 9

#define TRANS 10
#define TRANS_TYPE 20
#define TRANS_NAME 11
#define TRANS_COND 12
#define TRANS_GURD 13
#define TRANS_NUM 14
#define TRANS_ACT 15

#define EVENTS 16
#define EVENT_TYPE 21
#define EVENT_NAME 17
#define FROM_STATE 22
#define EVENT_COND 18
#define EVENT_ACT 23
#define TO_STATE 19

typedef struct event EVENT;
struct event
{
    int eventType;
    char *eventName;                         // EVENT_NAME

    const char *fromState;
    char *eventCond;                         // EVENT_COND
    void *eventAction;
    const char *toState;              // TO_STATE
};

struct state;
typedef struct transition TRANSITION;
struct transition
{
    int transType;
    char *transName;   // 與transType與eventType 一樣
    void *condition;
    void *guard;
    void *transAction;
    struct state *nextState;
};

typedef struct state STATE;
struct state
{
    char *stateName;
    void *stateData;  //保存狀態的一些屬性：狀態名等。
    struct state *parentState;
    struct state *entryState;
    struct transition *transitions[TRANS];  //一個狀態TRANS個轉移
    size_t numTransitions;
    void *entryAction;
    void *exitAction;
};

struct stateMachine
{
    struct state *currentState;
    struct state *previousState;
    struct state *errorState;
};



#define MAX_SIZE 250
extern STATE *state_all[MAX_SIZE];
extern int STATE_i;
extern TRANSITION *trans_all[MAX_SIZE];
extern int TRANS_i;
extern EVENT *event_all[MAX_SIZE];
extern int EVENT_i;




#endif //LC_SM_DEF_H
