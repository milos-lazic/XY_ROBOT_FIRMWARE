#ifndef _LIST_H_
#define _LIST_H_


/* Name: struct listnode
 * Description: linked list node structure
 * Members: next       - pointer to next node
 *          prev       - pointer to previous node
 *          size       - size of stored data
 *          data       - pointer to stored data
 *          key        - key
 */
struct listnode
{
	struct listnode *next;          /* pointer to next node */
	struct listnode *prev;          /* pointer to previous node */
	size_t           size;          /* size of stored data */
	void            *data;          /* pointer to stored data */
	int              key;           /* key */
};


/* Name: struct sl_list
 * Description: single-linked list container structure
 * Members: head       - pointer to first node in single-linked list
 *          tail       - pointer to last node in single-linked list
 *          len        - length of single-linked list
 */
struct sl_list
{
	struct listnode        *head;
	struct listnode        *tail;
	long long unsigned int  len;
};

void sl_list_init(struct sl_list *plist);
size_t sl_list_get_len(struct sl_list *plist);
int sl_list_insert_begin(struct sl_list *plist, void *pdata, size_t size);
int sl_list_delete_begin(struct sl_list *plist);
int sl_list_insert_end(struct sl_list *plist, void *pdata, size_t size);
int sl_list_delete_end(struct sl_list *plist);
// undefined >>>>
int sl_list_insert_n(struct sl_list *plist, long long unsigned int npos, void *pdata, size_t size);
int sl_list_delete_n(struct sl_list *plist, long long unsigned int npos);
// <<<< undefined
struct listnode *sl_list_search(struct sl_list *plist, int key);

#endif