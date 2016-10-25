#include "sm.h"
#include "../VM/vm.h"

#include <stdint.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static void goToErrorState( struct stateMachine *stateMachine,
                            struct event *const event );
static struct transition *getTransitionDomain( struct stateMachine *stateMachine,
                                         struct state *state, struct event *const event );

void SM_init( struct stateMachine *fsm,
                  struct state *initialState, struct state *errorState )
{
    if ( !fsm )
        return;

    fsm->currentState = initialState;
    fsm->previousState = NULL;
    fsm->errorState = errorState;
}

int SM_handleEvent( struct stateMachine *fsm,
                        struct event *event )
{
    if ( !fsm || !event )
        return SM_errArg;

    if ( !fsm->currentState )
    {
        goToErrorState( fsm, event );
        return SM_errorStateReached;
    }

    if ( !fsm->currentState->numTransitions )
        return SM_noStateChange;

    struct state *nextState = fsm->currentState;
    do {
        struct transition *transition = getTransitionDomain( fsm, nextState, event );

        /* 對於給定的事件，如果當前事件沒有轉移，監察父狀態有沒有轉移 */
        if ( !transition )
        {
            nextState = nextState->parentState;
            continue;
        }

        /* 對於一個轉移，必須要指定下一個狀態 */
        if ( !transition->nextState )
        {
            goToErrorState( fsm, event );
            return SM_errorStateReached;
        }

        nextState = transition->nextState;

        /* If the new state is a parent state, enter its entry state (if it has
         * one). Step down through the whole family tree until a state without
         * an entry state is found: */
        while ( nextState->entryState )
            nextState = nextState->entryState;

        /* Run exit action only if the current state is left (only if it does
         * not return to itself): */
        if ( nextState != fsm->currentState && fsm->currentState->exitAction )
            exitAction_handle(fsm->currentState, event, fsm->currentState->exitAction);


        /* Run transition action (if any): 處理轉移動作在推出動作之前*/
        if ( transition->transAction )
            transitionAction_handle(fsm->currentState, event, nextState, transition->transAction);

        /* 處理新狀態的進入動作 Call the new state's entry action if it has any (only if state does
         * not return to itself): */
        if ( nextState != fsm->currentState && nextState->entryAction )
            entryAction_handle(nextState, event, nextState->entryAction);

        fsm->previousState = fsm->currentState;
        fsm->currentState = nextState;

        /* If the state returned to itself: */
        if ( fsm->currentState == fsm->previousState )
            return SM_stateLoopSelf;

        if ( fsm->currentState == fsm->errorState )
            return SM_errorStateReached;

        /* If the new state is a final state, notify user that the state
         * machine has stopped: */
        if ( !fsm->currentState->numTransitions )
            return SM_finalStateReached;

        return SM_stateChanged;
    } while ( nextState );

    return SM_noStateChange;
}

struct state *SM_currentState( struct stateMachine *fsm )
{
    if ( !fsm )
        return NULL;

    return fsm->currentState;
}

struct state *SM_previousState( struct stateMachine *fsm )
{
    if ( !fsm )
        return NULL;

    return fsm->previousState;
}


static void goToErrorState( struct stateMachine *fsm,
                            struct event *const event )
{
    fsm->previousState = fsm->currentState;
    fsm->currentState = fsm->errorState;

    if ( fsm->currentState && fsm->currentState->entryAction )
        entryAction_handle(fsm->currentState, event, fsm->currentState->entryAction);
}

static struct transition *getTransitionDomain( struct stateMachine *fsm,
                                         struct state *state, struct event *const event )
{
    size_t i;

    for ( i = 0; i < state->numTransitions; ++i )
    {
        struct transition *t = state->transitions[ i ];

        /* A transition for the given event has been found: */
        if ( t->transType == event->eventType )
        {
            if ( !t->guard )
                return t;
                /* If transition is guarded, ensure that the condition is held: */
            else if (guard_handle(t->condition, event, t->guard))
                return t;
        }
    }

    /* No transitions found for given event for given state: */
    return NULL;
}

bool SM_stopped( struct stateMachine *stateMachine )
{
    if ( !stateMachine )
        return true;

    return stateMachine->currentState->numTransitions == 0;
}




void entryAction_handle(const STATE *state, struct event *event, void *entryAction)
{
    const char *stateName = (const char *)state->stateName;

    printf("entry.....:  Entering %s, ", stateName);
//    printf("entryAction: Entering: ");
//    vm_print(stateName);
    printf(" eventType: %d, ", event->eventType);


    const char *entryAction_print = (const char *)entryAction;
    printf("entryAction(JSstring):      %s, ", entryAction_print);

    printf("  eventCond : %s, \n", event->eventCond);
}


void exitAction_handle(const STATE *state, struct event *event, void *exitAction)
{
    const char *stateName = (const char *)state->stateName;

    printf( "exit......:  Exiting %s,  ", stateName );
//    printf("exitAction: Exiting: ");
//    vm_print(stateName);
    printf( " eventType: %d, ", event->eventType);


    const char *exitAction_print = (const char *)exitAction;
    printf("exitAction(JSstring):       %s, ", exitAction_print);

    printf( "  eventCond : %s, \n", event->eventCond);
}


void transitionAction_handle(const STATE *oldState, struct event *event, const STATE *newState, void *transAction)
{
    const char *transitionAction_print = (const char *)transAction;

    printf( "transition:  Event Name '%s', Event Cond '%s', ", event->eventName, event->eventCond );
    printf( "  eventType: %d, ", event->eventType);
    printf("tarnsitionAction(JSstring): %s\n", transitionAction_print);

    if (event->toState == 0) {
        fprintf( stderr, "There is no toState, but newState is %s\n",
                 (const char *)newState->stateName);
        exit( 1 );
    } else if (strcmp((const char *)newState->stateName, event->toState))
    {
        fprintf( stderr, "Unexpected state transition (to %s)\n",
                 (const char *)newState->stateName);
        exit( 1 );
    }
}


bool guard_handle(void *condition, struct event *event, void *guard)
{
    const char *guard_print = (const char *)guard;
    printf("with guard condition: %s\n", guard_print);

    return *(char*)condition == *event->eventCond;
}