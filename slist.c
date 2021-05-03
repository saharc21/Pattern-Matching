#include "slist.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** Initialize a single linked list
	\param list - the list to initialize */
__global__ void slist_init(slist_t *list)
{ //init the first list
    if (list == NULL)
    {
        return;
    }
    slist_head(list) = NULL;
    slist_tail(list) = NULL;
    slist_size(list) = 0;
}

/** Destroy and de-allocate the memory hold by a list
	\param list - a pointer to an existing list
	\param dealloc flag that indicates whether stored data should also be de-allocated */
void slist_destroy(slist_t *list, slist_destroy_t flag)//free the list 
{
    if (list == NULL ||slist_head(list)==NULL|| (flag != SLIST_LEAVE_DATA && flag != SLIST_FREE_DATA))
        return;
    slist_node_t *temp = slist_head(list);
    {
        while (temp != NULL)
        {
            slist_head(list) = slist_next(slist_head(list));
            if (flag == SLIST_FREE_DATA) //free the data in the nodes and after that the nodes themsel
            {
                free(slist_data(temp));
            }
            free(temp);
            temp = slist_head(list);
            slist_size(list)--;
        }
    }
}

/** Pop the first element in the list
	\param list - a pointer to a list
	\return a pointer to the data of the element, or NULL if the list is empty */
void *slist_pop_first(slist_t *list) //if there is information in first data, return pointer to the data and delete the first node. else, return null.(don't forget decrease the size of list)
{
    if (list == NULL||slist_head(list) == NULL )
        return NULL;
    slist_node_t *temp = slist_head(list);
    if ( slist_data(slist_head(list)) != NULL)
    {
        void *freeData = slist_data(slist_head(list));
        slist_head(list) = slist_next(slist_head(list));
        if(slist_head(list)==NULL)
            slist_tail(list)=NULL;
        free(temp);
        slist_size(list) = slist_size(list) - 1;
        return freeData;
    }
    return NULL;
}

/** Append data to list (add as last node of the list)
	\param list - a pointer to a list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure */
__global__ int slist_append(slist_t *list, void *data) //create new node with new data ("void * data") and add it to the exist list from the 'tail side'
{
    if (data == NULL || list == NULL ) 
        return -1;
    slist_node_t *add = (slist_node_t *)malloc(sizeof(slist_node_t));
    if (add == NULL)//if the allocate is failed
        return -1;
    slist_next(add) = NULL;
    slist_data(add) = data;

    if (slist_head(list) == NULL)//if there is no exist nodes in the list 
    {
        slist_head(list) = add;
        slist_tail(list) = add;
    }
    else
    {
        slist_next(slist_tail(list)) = add;
        slist_tail(list) = add;
    }
    slist_size(list)++;
    return 0;
}

/** Prepend data to list (add as first node of the list)
	\param list - a pointer to list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure
*/
int slist_prepend(slist_t *list, void *data)//create new node with new data ("void * data") and add it to the exist list from the 'head side'
{
    if (data == NULL || list == NULL) //if the allocate is failed
        return -1;
    slist_node_t *add = (slist_node_t *)malloc(sizeof(slist_node_t));//A new node that will join the exist list
    if (add == NULL)
        return -1;

    slist_data(add) = data;
    slist_next(add) = slist_head(list);
    slist_head(list) = add;
    if (slist_next(slist_head(list)) == NULL)
        slist_tail(list) = slist_head(list);

    slist_size(list) = slist_size(list) + 1;
    return 0;
}

/** \brief Append elements from the second list to the first list, use the slist_append function.
	you can assume that the data of the lists were not allocated and thus should not be deallocated in destroy 
	(the destroy for these lists will use the SLIST_LEAVE_DATA flag)
	\param to a pointer to the destination list
	\param from a pointer to the source list
	\return 0 on success, or -1 on failure
*/
int slist_append_list(slist_t *source, slist_t *destanation)//connect one list to an addional list 
{
    if (source == NULL || destanation == NULL)
        return -1;

    if (slist_head(source) == NULL && slist_head(destanation) == NULL)
        return 0;

    if (slist_head(source) != NULL && slist_head(destanation) == NULL)
        return 0;

    if (slist_head(source) == NULL && slist_head(destanation) != NULL)
    {
        slist_init(source);
        slist_node_t *temp = slist_head(destanation);
        while (temp != NULL)
        {
            slist_append(source, slist_data(temp));
            temp = slist_next(temp);
        }
        return 0;
    }
    if (slist_head(source) != NULL && slist_head(destanation) != NULL)
    {
        slist_node_t *temp = slist_head(destanation);
        while (temp != NULL)
        {
            slist_append(source, slist_data(temp));
            temp = slist_next(temp);
        }
        return 0;
    }
    return -1;
}
