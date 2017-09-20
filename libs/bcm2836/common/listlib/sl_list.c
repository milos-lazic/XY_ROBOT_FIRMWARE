#include <stdlib.h>
#include <string.h>
#include "list.h"


/* Name: sl_list_init
 * Description: initializes a single-linked list object
 * Parameters: plist            - pointer to a single-linked list object
 * Return:     None
 * Note:       
 */
void sl_list_init(struct sl_list *plist)
{
	plist->head = NULL;
	plist->tail = NULL;
	plist->len = 0;
}


/* Name: sl_list_get_len
 * Description: get the length of a single-linked list
 * Parameters: plist            - pointer to a single-linked list object
 * Return:     list length
 * Note:       
 */
size_t sl_list_get_len(struct sl_list *plist)
{
	return plist->len;
}


/* Name: sl_list_insert_begin
 * Description: insert a node (and associated data) at the beginning
 *              of a single-linked list
 * Parameters: plist            - pointer to a single-linked list object
 *             pdata            - pointer to data to be stored in list
 *             size             - size of data to be stored in list
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Each call to this function allocates (malloc) memory
 *                 twice - first to dynamically obtain memory for a new 
 *                 node, and next to obtain memory (the size is determined
 *                 by the 'size' argument) to copy over the data pointed to
 *                 by the 'pdata' argument. 'free()' needs to be called
 *                 twice when a node is being deleted. 
 */
int sl_list_insert_begin(struct sl_list *plist,
                         void           *pdata,
                         size_t          size)
{
	struct listnode *p;

	if ( plist == NULL || pdata == NULL )
		return -1;

	// allocate memory for a node structure and assign it to temporary pointer
	p = (struct listnode *) malloc(sizeof(struct listnode));
	if (p == NULL)
		return -1;

	// allocate memrory for the data being stored
	p->data = malloc(size);
	if (p->data == NULL)
		goto out;

	// copy data from user-supplied pointer to the allocated memory area
	(void)memcpy(p->data, pdata, size);

	p->key = plist->len;
	p->size = size;
	p->prev = NULL;

	if (plist->len == 0)
	{
		p->next = NULL;
		plist->tail = p;
	}
	else
	{
		p->next = plist->head;
	}

	plist->head = p;
	plist->len++;

	return 0;

out:
	free(p);
	return -1;
}


/* Name: sl_list_insert_end
 * Description: insert a node (and associated data) at the end
 *              of a single-linked list
 * Parameters: plist            - pointer to a single-linked list object
 *             pdata            - pointer to data to be stored in list
 *             size             - size of data to be stored in list
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Each call to this function allocates (malloc) memory
 *                 twice - first to dynamically obtain memory for a new 
 *                 node, and next to obtain memory (the size is determined
 *                 by the 'size' argument) to copy over the data pointed to
 *                 by the 'pdata' argument. 'free()' needs to be called
 *                 twice when a node is being deleted. 
 */
int sl_list_insert_end(struct sl_list *plist, void *pdata, size_t size)
{
	struct listnode *p;

	if ( plist == NULL || pdata == NULL )
		return -1;

	// allocate memory for a node structure and assign it to temporary pointer
	p = (struct listnode *) malloc(sizeof(struct listnode));
	if (p == NULL)
		return -1;

	// allocate memrory for the data being stored
	p->data = malloc(size);
	if (p->data == NULL)
		goto out;

	// copy data from user-supplied pointer to the allocated memory area
	(void)memcpy(p->data, pdata, size);

	p->key = plist->len;
	p->size = size;
	p->next = NULL;
	p->prev = NULL;

	if (plist->len == 0)
	{
		plist->head = p;
	}
	else
	{
		plist->tail->next = p;
	}

	plist->tail = p;
	plist->len++;

	return 0;

out:
	free(p);
	return -1;
}


/* Name: sl_list_delete_begin
 * Description: delete (free) the first node (and associated data)
 *              of a single-linked list
 * Parameters: plist            - pointer to a single-linked list object
 * Return:     0 IF successful, -1 IF failed
 * Note:       
 */
int sl_list_delete_begin(struct sl_list *plist)
{
	struct listnode *pnode;
	void            *pdata;

	if (plist->len == 0 || plist == NULL)
		return -1;

	pnode = plist->head;
	pdata = pnode->data;

	plist->head = plist->head->next;
	if (plist->head == NULL)
		plist->tail = NULL;

	free(pdata);
	free(pnode);
	plist->len--;

	return 0;
}


/* Name: sl_list_delete_end
 * Description: delete (free) the last node (and associated data)
 *              of a single-linked list
 * Parameters: plist            - pointer to a single-linked list object
 * Return:     0 IF successful, -1 IF failed
 * Note:       
 */
int sl_list_delete_end(struct sl_list *plist)
{
	struct listnode *pnode;
	void            *pdata;

	if (plist->len == 0 || plist == NULL)
		return -1;

	if (plist->len == 1)
	{
		free(plist->tail->data);
		free(plist->tail);
		plist->tail = NULL;
		plist->head = NULL;
	}
	else
	{
		pnode = plist->head;
		while(pnode->next != plist->tail)
			pnode = pnode->next;

		free(plist->tail->data);
		free(plist->tail);

		pnode->next = NULL;
		plist->tail = pnode;
	}

	plist->len--;

	return 0;
}


/* Name: sl_list_search
 * Description: search for a node in a single-linked list using its key
 * Parameters: plist            - pointer to a single-linked list object
 *             key              - key to search for
 * Return:     pointer to list node IF successful, NULL if not found 
 * Note:       
 */
struct listnode *sl_list_search(struct sl_list *plist, int key)
{
	struct listnode *currnode = plist->head;

	while(1)
	{
		if (currnode == NULL)
			return NULL;

		if (currnode->key == key)
			return currnode;

		currnode = currnode->next;
	}
}