#include "pattern_matching_aho.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----- Initilaize the FSM veriables
int pm_init(pm_t *fsm)
{
    if (fsm == NULL)
    {
        return -1;
    }
    fsm->newstate = 0;
    fsm->zerostate = (pm_state_t *)malloc(sizeof(pm_state_t));
    if (fsm->zerostate == NULL)
    {
        return -1;
    }

    //----- initilize the zerostate parameters:
    fsm->zerostate->_transitions = (slist_t *)malloc(sizeof(slist_t));
    if (fsm->zerostate->_transitions == NULL)
    {
        return -1;
    }
    slist_init(fsm->zerostate->_transitions);
    fsm->zerostate->depth = 0;
    fsm->zerostate->id = 0;
    fsm->zerostate->output = NULL;
    fsm->zerostate->fail = NULL;

    fsm->newstate++;

    return 0;
}

//----- Return the destination state if there is an edge that connect the source to the destination state with specific symbol
__global__ pm_state_t *pm_goto_get(pm_state_t *state, unsigned char symbol)
{
    int findIndication = 0; // '0' symbol not found, '1' found.
    slist_node_t *transitionPtr = slist_head(state->_transitions);
    pm_labeled_edge_t *edge;
    while (transitionPtr != NULL)
    {
        edge = (pm_labeled_edge_t *)slist_data(transitionPtr);
        if (edge->label == symbol)
        {
            findIndication = 1;
            break;
        }
        transitionPtr = slist_next(transitionPtr);
    }

    if (findIndication == 1)
    {
        return edge->state;
    }
    else
    {
        return NULL;
    }
}

//----- Set new edge that connect two states with symbol
int pm_goto_set(pm_state_t *from_state, unsigned char symbol, pm_state_t *to_state)
{
    //----- initilize the new lable
    pm_labeled_edge_t *newLableToAdd = (pm_labeled_edge_t *)malloc(sizeof(pm_labeled_edge_t));
    if (newLableToAdd == NULL)
    {
        return -1;
    }
    newLableToAdd->label = symbol;
    newLableToAdd->state = to_state;

    //----- add the new state to the 'from_state' as a transition
    slist_append(from_state->_transitions, newLableToAdd);
    // printf("%d -> %c -> %d\n",from_state->id,symbol,to_state->id);

    return 0;
}

//----- Add new lable (if not exists) to the FSM
int pm_addstring(pm_t *fsm, unsigned char *stringToAdd, size_t n)
{
    if (fsm == NULL || (fsm->newstate + n) > PM_CHARACTERS || stringToAdd == NULL || n <= 0 || strlen((const char *)stringToAdd) != n)
    {
        return -1;
    }

    int charIndexInString = 0;
    pm_state_t *statePtr = fsm->zerostate;
    while (charIndexInString < n && fsm->newstate != 1)
    {
        if (pm_goto_get(statePtr, stringToAdd[charIndexInString]) != NULL)
        {
            statePtr = pm_goto_get(statePtr, stringToAdd[charIndexInString]);
            charIndexInString++;
        }
        else
        {
            break;
        }
    }

    //----- verify that we don't pass the all string
    if (statePtr == fsm->zerostate || (charIndexInString + 1) <= n)
    {
        for (int i = charIndexInString; i < n; i++)
        {
            pm_state_t *to_state = (pm_state_t *)malloc(sizeof(pm_state_t));
            if (to_state == NULL)
            {
                return -1;
            }
            // printf("Allocating state %d\n",fsm->newstate);
            to_state->fail = NULL;

            //----- initilize new state parameters
            to_state->depth = statePtr->depth + 1;
            to_state->id = fsm->newstate;
            fsm->newstate++;
            to_state->_transitions = (slist_t *)malloc(sizeof(slist_t));
            if (to_state->_transitions == NULL)
            {
                return -1;
            }
            to_state->output = (slist_t *)malloc(sizeof(slist_t));
            if (to_state->output == NULL)
            {
                return -1;
            }
            slist_init(to_state->_transitions);
            slist_init(to_state->output);

            //----- if it's the last character of the string add the string to stat's output
            if (i + 1 == n)
            {
                //----- add new node to the outputs list
                slist_append(to_state->output, stringToAdd);
            }
            pm_goto_set(statePtr, stringToAdd[i], to_state);
            statePtr = to_state;
        }
    }
    return -1;
}

//----- Initilaize all FSM state's failure state
int pm_makeFSM(pm_t *fsm)
{
    if (fsm == NULL)
    {
        return -1;
    }

    if (fsm->zerostate->_transitions == NULL)
    {
        return 0;
    }

    slist_t *statesQueue = (slist_t *)malloc(sizeof(slist_t));
    if (statesQueue == NULL)
    {
        return -1;
    }

    slist_init(statesQueue);

    //----- initilize the zerostate transitions failure states
    slist_node_t *statePtr = slist_head(fsm->zerostate->_transitions);
    while (statePtr != NULL)
    {
        slist_append(statesQueue, ((pm_labeled_edge_t *)slist_data(statePtr))->state);
        ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail = fsm->zerostate;
        statePtr = slist_next(statePtr);
    }

    pm_state_t *fatherState;
    //----- initilize the rest states failure states
    while (statesQueue->head != NULL)
    {
        fatherState = (pm_state_t *)slist_pop_first(statesQueue);
        if (fatherState != NULL && fatherState->_transitions != NULL)
        {
            statePtr = slist_head(fatherState->_transitions);
            while (statePtr != NULL)
            {
                slist_append(statesQueue, ((pm_labeled_edge_t *)slist_data(statePtr))->state);
                while (fatherState != NULL)
                {
                    if (pm_goto_get(fatherState->fail, ((pm_labeled_edge_t *)slist_data(statePtr))->label) != NULL)
                    {
                        ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail = pm_goto_get(fatherState->fail,
                                                                                               ((pm_labeled_edge_t *)slist_data(statePtr))->label);
                        // printf("Setting f(%d) = %d\n",
                        // ((pm_labeled_edge_t *)slist_data(statePtr))->state->id,((pm_labeled_edge_t *)slist_data(statePtr))->state->fail->id);
                        break;
                    }
                    if (fatherState->fail == fsm->zerostate)
                    {
                        ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail = fsm->zerostate;
                        break;
                    }
                    fatherState = fatherState->fail;
                }
                slist_append_list(((pm_labeled_edge_t *)slist_data(statePtr))->state->output, ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail->output);
                statePtr = slist_next(statePtr);
            }
        }
    }
    free(statesQueue);
    return 0;
}

int pm_makeFSM_dfa(pm_t *fsm)
{
    if (fsm == NULL)
    {
        return -1;
    }

    if (fsm->zerostate->_transitions == NULL)
    {
        return 0;
    }

    slist_t *statesQueue = (slist_t *)malloc(sizeof(slist_t));
    if (statesQueue == NULL)
    {
        return -1;
    }

    slist_init(statesQueue);

    //----- initilize the zerostate transitions failure states
    slist_node_t *statePtr = slist_head(fsm->zerostate->_transitions);
    while (statePtr != NULL)
    {
        slist_append(statesQueue, ((pm_labeled_edge_t *)slist_data(statePtr))->state);
        ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail = fsm->zerostate;
        statePtr = slist_next(statePtr);
    }

    pm_state_t *fatherState;
    //----- initilize the rest states failure states
    while (statesQueue->head != NULL)
    {
        fatherState = (pm_state_t *)slist_pop_first(statesQueue);
        if (fatherState != NULL && fatherState->_transitions != NULL)
        {
            statePtr = slist_head(fatherState->_transitions);
            while (statePtr != NULL)
            {
                slist_append(statesQueue, ((pm_labeled_edge_t *)slist_data(statePtr))->state);
                while (fatherState != NULL)
                {
                    if (pm_goto_get(fatherState->fail, ((pm_labeled_edge_t *)slist_data(statePtr))->label) != NULL)
                    {
                        ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail = fsm->zerostate;
                        // printf("Setting f(%d) = %d\n",
                        //    ((pm_labeled_edge_t *)slist_data(statePtr))->state->id, ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail->id);
                        break;
                    }
                    if (fatherState->fail == fsm->zerostate)
                    {
                        ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail = fsm->zerostate;
                        break;
                    }
                    fatherState = fatherState->fail;
                }
                slist_append_list(((pm_labeled_edge_t *)slist_data(statePtr))->state->output, ((pm_labeled_edge_t *)slist_data(statePtr))->state->fail->output);
                statePtr = slist_next(statePtr);
            }
        }
    }
    free(statesQueue);
    return 0;
}

//----- Search if a given String is contains the FSM labls
__global__ slist_t *pm_fsm_search(pm_state_t *curState, unsigned char *string, size_t stringLength)
{
    if (curState == NULL || string == NULL || strlen((const char *)string) != stringLength)
    {
        return NULL;
    }

    if (slist_head(curState->_transitions) == NULL || ((pm_labeled_edge_t *)slist_data(slist_head(curState->_transitions)))->state->fail == NULL)
    {
        return NULL;
    }

    slist_t *matchsList = (slist_t *)malloc(sizeof(slist_t));
    if (matchsList == NULL)
    {
        return NULL;
    }
    slist_init(matchsList);

    slist_node_t *outputPtr;

    for (size_t i = 0; i < stringLength; i++)
    {
        while (pm_goto_get(curState, string[i]) == NULL)
        {
            if (curState->depth == 0)
            {
                return NULL;
            }
            if (curState->fail->depth == 0)
            {
                return NULL;
            }

            curState = curState->fail;
        }

        if (pm_goto_get<<< 1 , 1>>>(curState, string[i]) != NULL)
        {
            curState = pm_goto_get<<< 1 , 1>>>(curState, string[i]);
            if (curState->output->size != 0)
            {
                outputPtr = slist_head(curState->output);
                while (outputPtr != NULL)
                {
                    pm_match_t *matchToAdd = (pm_match_t *)malloc(sizeof(pm_match_t));
                    if (matchToAdd == NULL)
                    {
                        return NULL;
                    }
                    matchToAdd->end_pos = i;
                    matchToAdd->fstate = curState;
                    matchToAdd->start_pos = i - strlen((const char *)slist_data(outputPtr)) + 1;
                    matchToAdd->pattern = (char *)slist_data(outputPtr);
                    slist_append<<< 1 , 1>>>(matchsList, matchToAdd);

                    //----- print the matches
                    printf("Pattern: %s, start at: %d, ends at: %d, last state = %d\n",
                           matchToAdd->pattern, matchToAdd->start_pos, matchToAdd->end_pos, matchToAdd->fstate->id);

                    outputPtr = slist_next(outputPtr);
                }
            }
        }
    }

    return matchsList;
}

//----- Destroy the FSM
void pm_destroy(pm_t *fsm)
{
    if (fsm == NULL)
    {
        return;
    }

    //-----If the FSM just created and didn't add any string to it.
    if (fsm->newstate == 1)
    {
        if (fsm->zerostate->_transitions != NULL)
            free(fsm->zerostate->_transitions);
        free(fsm->zerostate);
        return;
    }
    slist_t *statesQueue = (slist_t *)malloc(sizeof(slist_t));
    if (statesQueue == NULL)
    {
        return;
    }

    slist_init<<< 1 , 1>>>(statesQueue);

    slist_append<<< 1 , 1>>>(statesQueue, fsm->zerostate);

    pm_state_t *stateToFree;
    slist_node_t *transitionsPtr = slist_head(fsm->zerostate->_transitions);

    while (statesQueue->head != NULL)
    {
        stateToFree = (pm_state_t *)slist_pop_first(statesQueue);
        transitionsPtr = slist_head(stateToFree->_transitions);

        while (transitionsPtr != NULL)
        {
            slist_append<<< 1 , 1>>>(statesQueue, ((pm_labeled_edge_t *)slist_data(transitionsPtr))->state);
            transitionsPtr = slist_next(transitionsPtr);
        }

        slist_destroy(stateToFree->_transitions, SLIST_FREE_DATA);
        free(stateToFree->_transitions);

        if (stateToFree->output != NULL)
        {
            slist_destroy(stateToFree->output, SLIST_LEAVE_DATA);
            free(stateToFree->output);
        }

        free(stateToFree);
    }
    free(statesQueue);
}
