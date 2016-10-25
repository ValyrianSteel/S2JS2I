//
// Created by ubun on 16-5-23.
//
#include "sm.h"
#include "../test_data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int sm_run()
{
    struct stateMachine fsm;
//    SM_init( &fsm, &s1, &sE );
    SM_init(&fsm, &s[1], &s[0]);

    int res;
    size_t i;

    /* Hand all but the last event to the state machine: */
    for ( i = 0; i < sizeof( events ) / sizeof( struct event ) - 1; ++i )
//    for (i = 0; i < EVENT_i; ++i)
    {
        res = SM_handleEvent( &fsm, &events[ i ] );
        printf("\n");
        if ( res == SM_stateLoopSelf )
        {
            /* Prevent segmentation faults (due to the following comparison)
             * (loops will not be tested in the first transition): */
            if ( i == 0 )
            {
                fputs( "Internal error. This should not happen.\n", stderr );
                exit( 4 );
            }

            if ( !strcmp( &events[ i ].toState,
                          &events[ i - 1 ].toState ) )
                puts( "State changed back to itself" );
            else
            {
                fputs( "State unexpectedly changed back to itself", stderr );
                exit( 5 );
            }
        }
            /* Apart from an occasional state loop, all other events handed to the
             * state machine should result in 'SM_stateChanged': */
        else if ( res != SM_stateChanged )
        {
            fprintf( stderr, "Unexpected return value from SM_handleEvent:"
                    " %d\n", res );
            exit( 2 );
        }
    }

    /* The last state change is expected to result in a transition to a final
     * state: */
    res = SM_handleEvent( &fsm, &events[ i ] );
    if ( res != SM_finalStateReached )
    {
        fprintf( stderr, "Unexpected return value from SM_handleEvent:"
                " %d\n", res );
        exit( 3 );
    }
    printf("\n");
    puts( "A final state was reached (as expected)" );

    return 0;
}