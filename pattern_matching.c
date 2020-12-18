#include "slist.h"
#include "pattern_matching.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Initializes the fsm parameters (the fsm itself sould be allocated).  Returns 0 on success, -1 on failure. 
*  this function should init zero state
*/
int pm_init(pm_t *tree)//initilaize the pm_t
{
    if (tree == NULL)
        return -1;
    tree->newstate = 0;
    tree->zerostate = (pm_state_t *)malloc(sizeof(pm_state_t));
    if (tree->zerostate == NULL)
        return -1;
    tree->zerostate->depth = 0;
    tree->zerostate->fail = NULL;
    tree->zerostate->id = tree->newstate;
    tree->zerostate->output = NULL;
    tree->zerostate->_transitions = (slist_t *)malloc(sizeof(slist_t));
    if (tree->zerostate->_transitions == NULL)
        return -1;
    slist_init(tree->zerostate->_transitions);
    tree->newstate = tree->newstate + 1;
    return 0;
}

/* Adds a new string to the fsm, given that the string is of length n. 
   Returns 0 on success, -1 on failure.*/
int pm_addstring(pm_t *temp, unsigned char *word, size_t n)//creates the states according the text
{
    if (temp == NULL || temp->zerostate == NULL || word == NULL || strlen(word) != n)
        return -1;
    pm_state_t *runner = temp->zerostate;
    pm_state_t *new;
    for (int i = 0; i < n; i++)
    {
        if (pm_goto_get(runner, word[i]) == NULL)
        {
            new = (pm_state_t *)malloc(sizeof(pm_state_t));
            if (new == NULL)
                return -1;
            new->depth = runner->depth + 1;
            new->fail = NULL;
            new->id = temp->newstate;
            new->output = (slist_t *)malloc(sizeof(slist_t));
            if (new->output == NULL)
                return -1;
            slist_init(new->output);
            new->_transitions = (slist_t *)malloc(sizeof(slist_t));
            if (new->_transitions == NULL)
                return -1;
            slist_init(new->_transitions);
            pm_goto_set(runner, word[i], new);
            runner = new;
            temp->newstate++;
        }
        else
        {
            runner = pm_goto_get(runner, word[i]);
        }
    }
    if (slist_append(runner->output, word) == -1)
        return -1;
    return 0;
}

/* Finalizes construction by setting up the failrue transitions, as
   well as the goto transitions of the zerostate. 
   Returns 0 on success, -1 on failure.*/
int pm_makeFSM(pm_t *tempTree)//define about every state who is his failure state 
{
    if (tempTree == NULL)
        return -1;
    slist_t *fail = (slist_t *)malloc(sizeof(slist_t));
    if (fail == NULL)
        return -1;
    slist_init(fail);
    slist_node_t *transitionTemp;
    pm_state_t *tempState = tempTree->zerostate;
    pm_state_t *tempState2;
    pm_state_t *go;
    pm_labeled_edge_t *edge;
    slist_append(fail, tempState);
    while (slist_head(fail) != NULL)//every iteration we take one state and define his failure state 
    {
        tempState = slist_pop_first(fail);
        if (tempState->_transitions == NULL)
            continue;
        transitionTemp = slist_head(tempState->_transitions);
        while (transitionTemp != NULL)
        {
            if (slist_data(transitionTemp) == NULL)
                break;

            edge = slist_data(transitionTemp);
            tempState2 = edge->state;
            go = pm_goto_get(tempState->fail, edge->label);
            if (go != NULL)
            {
                tempState2->fail = go;
                printf("Setting f(%d)=%d\n", tempState2->id, tempState2->fail->id);
            }
            else
            {
                if (tempState2->depth == 1)
                {
                    tempState2->fail = tempState;
                    printf("Setting f(%d)=%d\n", tempState2->id, tempState2->fail->id);
                }

                else
                {
                    while (go == NULL)
                    {
                        tempState = tempState->fail;
                        if (tempState == NULL)
                        {
                            tempState2->fail = tempTree->zerostate;
                            break;
                        }
                        go = pm_goto_get(tempState->fail, edge->label);
                        tempState2->fail = go;
                    }
                    printf("Setting f(%d)=%d\n", tempState2->id, tempState2->fail->id);
                }
            }
            if (tempState2->fail->output != NULL && slist_head(tempState2->fail->output) != NULL)
                slist_append_list(tempState2->output, tempState2->fail->output);
            int x = slist_append(fail, tempState2);
            if (x != 0)
                return -1;
            transitionTemp = slist_next(transitionTemp);//step over the 'sons' of the current state 
        }
    }
    free(fail);
}

/* Set a transition arrow from this from_state, via a symbol, to a
   to_state. will be used in the pm_addstring and pm_makeFSM functions.
   Returns 0 on success, -1 on failure.*/
int pm_goto_set(pm_state_t *from_state, unsigned char symbol, pm_state_t *to_state)//connect one state to another with edge with symbol
{
    if (from_state->_transitions == NULL)//create transition for the from_state if he doesn't have 'sons'
    {
        from_state->_transitions = (slist_t *)malloc(sizeof(slist_t));
        if (from_state == NULL)
            return -1;
        slist_init(from_state->_transitions);
    }
    pm_labeled_edge_t *sym = (pm_labeled_edge_t *)malloc(sizeof(pm_labeled_edge_t));//temporary edge 
    if (sym == NULL)
        return -1;
    sym->label = symbol;
    sym->state = to_state;
    slist_append(from_state->_transitions, sym);
    printf("Allocating state %d \n %d -> %c -> %d \n", sym->state->id, from_state->id, sym->label, sym->state->id); //example: print "ALLOCATING state 3 3->b->4"
    return 0;
}

/* Returns the transition state.  If no such state exists, returns NULL. 
   will be used in pm_addstring, pm_makeFSM, pm_fsm_search, pm_destroy functions. */
pm_state_t *pm_goto_get(pm_state_t *state, unsigned char symbol)//check if the state has a "next state" with the current symbol
{
    if (state == NULL || state->_transitions == NULL || slist_head(state->_transitions) == NULL)
        return NULL;
    slist_node_t *tran1 = slist_head(state->_transitions);
    pm_labeled_edge_t *sym = slist_data(tran1);
    while (tran1 != NULL)
    {
        if (sym->label == symbol)
            return sym->state;
        tran1 = slist_next(tran1);
        if (tran1 != NULL)
            sym = slist_data(tran1);
    }
    return NULL;
}

/* Search for matches in a string of size n in the FSM. 
   if there are no matches return empty list */
slist_t *pm_fsm_search(pm_state_t *stateTemp, unsigned char *a, size_t size)
{
    if (a == NULL)
        return NULL;
    if (strlen(a) != size)
        return NULL;
    slist_t *matching = (slist_t *)malloc(sizeof(slist_t)); //list of matches
    if (matching == NULL)
        return NULL;
    slist_init(matching);
    pm_state_t *check; //get the result of "go to get"

    pm_state_t *runner = stateTemp;

    for (int i = 0; i < size; i++)
    {
        check = pm_goto_get(runner, a[i]);
        if (check == NULL && runner == stateTemp)
        {
            continue;
        }
        while (check == NULL)
        {
            if (runner == NULL)
                break;
            runner = runner->fail;
            check = pm_goto_get(runner, a[i]);
        }
        if (check != NULL)
        {
            runner = check;
            if (runner->output != NULL && slist_head(runner->output) != NULL)
            {

                slist_node_t *runner2 = slist_head(runner->output);
                while (runner2 != NULL)
                {
                    pm_match_t *temp2 = (pm_match_t *)malloc(sizeof(pm_match_t));
                    if (temp2 == NULL)
                        return NULL;
                    temp2->start_pos = i - strlen(runner2->data) + 1;
                    temp2->end_pos = i;
                    temp2->pattern = runner2->data;
                    temp2->fstate = runner;
                    printf("Pattern: %s, starts at: %d, ends at: %d, last state = %d\n", temp2->pattern, temp2->start_pos, temp2->end_pos, temp2->fstate->id);
                    int x = slist_append(matching, temp2);
                    if (x == -1)
                        return NULL;
                    runner2 = slist_next(runner2);
                }
            }
        }
    }
    return matching;
}

/* Destroys the fsm, deallocating memory. */
void pm_destroy(pm_t *tree)
{
    if (tree == NULL || tree->zerostate == NULL)//no free needed . we don't have allocated pm_t
        return;
    else
    {

        slist_node_t *transition;
        pm_state_t *temp = tree->zerostate;
        pm_state_t *remove;
        slist_t *des = (slist_t *)malloc(sizeof(slist_t));//temp list helps us free the states 
        if (des == NULL)
            return;
        pm_labeled_edge_t *edge;
        slist_init(des);
        slist_append(des, temp);
        while (slist_head(des) != NULL)
        {
            remove = slist_pop_first(des);
            transition = slist_head(remove->_transitions);
            while (transition != NULL)
            {
                edge = slist_data(transition);
                slist_append(des, edge->state);
                transition = slist_next(transition);
            }
            slist_destroy(remove->_transitions, SLIST_FREE_DATA);//free the transitions of a state
            if (remove->_transitions != NULL)
                free(remove->_transitions);
            slist_destroy(remove->output, SLIST_LEAVE_DATA);//free the output of a state
            if (remove->output != NULL)
                free(remove->output);
            free(remove);
        }
        slist_destroy(des, SLIST_FREE_DATA);//free the temp list in the end
        free(des);
    }
}
