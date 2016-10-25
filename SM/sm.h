#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stddef.h>
#include <stdbool.h>

#include "sm_def.h"

void SM_init( struct stateMachine *stateMachine,
				  struct state *initialState, struct state *errorState );

enum SM_handleEventRetVals
{
			SM_errArg = -2,
			SM_errorStateReached,
			SM_stateChanged,
			SM_stateLoopSelf,
			SM_noStateChange,
			SM_finalStateReached,
};

int SM_handleEvent( struct stateMachine *stateMachine,
						struct event *event );

struct state *SM_currentState( struct stateMachine *stateMachine );

struct state *SM_previousState( struct stateMachine *stateMachine );

bool SM_stopped( struct stateMachine *stateMachine );

void entryAction_handle(const STATE *state, struct event *event, void *entryAction);

void exitAction_handle(const STATE *state, struct event *event, void *exitAction);

void transitionAction_handle(const STATE *oldStateData, struct event *event, const STATE *newStateData, void *transAction);

bool guard_handle(void *condition, struct event *event, void *guard);



int sm_run();

#endif // STATEMACHINE_H
