/*
 *		File: structures.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All of the data structures used for the 2nd project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "structures.h"

/*		Hashtables		*/

/**
 * Internal function to calculate the hash for a key.
 * It's based on the DJB algorithm.
 */
unsigned int calc_hash(char* key) {
	unsigned int hash = 5381;
	int c;

	while((c = *(key++)) != 0) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

/**
 * Internal function to calculate the hash step for a key.
 * It's based on the Jenkins hash function.
 */
unsigned int calc_hash_step(char* key) {
	unsigned int hash = 0;
	int i = 0;

	while (key[i] != '\0') {
		hash += key[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}

/**
 *
 */
unsigned int* hashtable_calc_hashes(char* key, int size) {
	unsigned int* hashing = SecureMalloc(sizeof(unsigned int) * 3);

	hashing[0] = calc_hash(key);
	hashing[1] = calc_hash_step(key);
	hashing[2] = hashing[1] % size;

	if (hashing[2] == 0) {
		hashing[2] = 1;
	}

	return hashing;
}

/**
 * Creates a hashtable with the specified size and starting information
 * and returns a pointer to it.
 */
hashtable* hashtable_create(int size) {
	int i;
	hashtable* new_hash_t = SecureMalloc(sizeof(hashtable));

	new_hash_t->table = (hash_elem**)SecureMalloc(size * sizeof(hash_elem*));
	new_hash_t->size = size;
	new_hash_t->elem_num = 0;

	for(i = 0; i < size; i++) {
		new_hash_t->table[i] = NULL;
	}

	return new_hash_t;
}

/**
 *
 */
hash_elem* hashtable_element_create(void* data) {
	hash_elem* elem = SecureMalloc(sizeof(hash_elem));

	elem->data = data;
	elem->state = HASHTABLE_TAKEN;

	return elem;
}

/**
 *
 */
hashtable* hashtable_insert(hashtable* hash_t, void* data, char* key,
							char*(*get_key)(void*)) {
	unsigned int* hashing = hashtable_calc_hashes(key, hash_t->size);
	unsigned int h = hashing[0] % hash_t->size, phi = hashing[2];
	int i = 1;
	hash_elem* elem = hashtable_element_create(data);

	while (hash_t->table[h] != NULL) {
		if (hash_t->table[h]->state == HASHTABLE_DELETED) {
			free(hash_t->table[h]);
			break;
		}
		h = (hashing[0] + i * phi) % hash_t->size;
		i++;
	}
	hash_t->table[h] = elem;

	free(hashing);

	if (++hash_t->elem_num >= hash_t->size * HASHTABLE_MAX_LOAD) {
		hash_t = hashtable_expand(hash_t, get_key);
	}

	return hash_t;
}

/**
 *
 */
hash_elem* hashtable_get(hashtable* hash_t, char* key, char*(*get_key)(void*)) {
	unsigned int* hashing = hashtable_calc_hashes(key, hash_t->size);
	unsigned int h = hashing[0] % hash_t->size, hash_1 = hashing[0], phi = hashing[2];
	int i = 1;

	free(hashing);

	while (hash_t->table[h] != NULL) {
		if (hash_t->table[h]->state != HASHTABLE_DELETED
			&& strcmp(get_key(hash_t->table[h]->data), key) == 0) {
			return hash_t->table[h];
		}
		h = (hash_1 + i * phi) % hash_t->size;
		i++;
	}

	return NULL;
}

/**
 *
 */
void hashtable_remove(hashtable* hash_t, char* key, char*(*get_key)(void*),
					  void(*clear_data)(void*)) {
	hash_elem* remove_elem = hashtable_get(hash_t, key, get_key);

	if (remove_elem == NULL) {
		return;
	}

	remove_elem->state = HASHTABLE_DELETED;
	--hash_t->elem_num;
	clear_data(remove_elem->data);
}

/**
 *
 */
int hash_elem_dead(hash_elem* elem) {
	if (elem == NULL || elem->state == HASHTABLE_DELETED) {
		return 1;
	}

	return 0;
}

/**
 *
 */
void hashtable_reinsert(hash_elem* elem, hashtable* new_hash_t, char* key) {
	unsigned int* hashing = hashtable_calc_hashes(key, new_hash_t->size);
	int h = hashing[0] % new_hash_t->size, i = 1;
	int phi = hashing[2];

	if (phi == 0) {
		phi = 1;
	}

	while (new_hash_t->table[h] != NULL) {
		h = (hashing[0] + i * phi) % new_hash_t->size;
		i++;
	}
	new_hash_t->table[h] = elem;

	free(hashing);
}

/**
 *
 */
hashtable* hashtable_expand(hashtable* hash_t, char*(*get_key)(void*)) {
	int i;
	hashtable* new_hash_t = hashtable_create(GetPrime(hash_t->size * 2));

	for (i = 0; i < hash_t->size; i++) {
		if (hash_t->table[i] != NULL
			&& hash_t->table[i]->state != HASHTABLE_DELETED) {
			hashtable_reinsert(hash_t->table[i], new_hash_t,
					  		get_key(hash_t->table[i]->data));
		}
	}

	hashtable_destroy(hash_t);

	return new_hash_t;
}

/**
 *
 */
void hashtable_destroy(hashtable* hash_t) {
	free(hash_t->table);
	free(hash_t);
}

/*		Linked Lists		*/

/**
 *
 */
list_t* list_create() {
	list_t* new_list = SecureMalloc(sizeof(list_t));
	new_list->first = NULL;
	new_list->last = NULL;

	return new_list;
}

/**
 *
 */
node_t* list_insert(list_t* list, void* data) {
	node_t* new_node = SecureMalloc(sizeof(node_t));

	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = list->last;

	if (list->last == NULL) {
		list->first = new_node;
	} else {
		list->last->next = new_node;
	}

	list->last = new_node;

	return new_node;
}

/**
 *
 */
void list_remove(list_t* list, node_t* node_removal) {
	if (node_removal != NULL) {
		if (node_removal->prev != NULL) {
			node_removal->prev->next = node_removal->next;
		} else {
			list->first = node_removal->next;
		}
		if (node_removal->next != NULL) {
			node_removal->next->prev = node_removal->prev;
		} else {
			list->last = node_removal->prev;
		}

		free(node_removal);
	}
}

/**
 *
 */
void list_destroy(list_t* list) {
	node_t *p1, *p2 = list->first;

	if (list->first == NULL) {
		free(list);
		return;
	}

	do {
		p1 = p2;
		p2 = p2->next;
		free(p1);
	} while (p2 != NULL);

	free(list);
}

/*		Merge Sort Implementation		*/

/**
 *
 */
node_t* merge(node_t* first, node_t* second, int(*cmp)(void*, void*)) {
    if (first == NULL) {
        return second; /* first linked list is empty */
	}

    if (second == NULL) {
        return first; /* second linked list is empty */
	}

    /* gets the smaller value */
    if (cmp(first->data, second->data) < 0) {
        first->next = merge(first->next, second, cmp);
		first->next->prev = first;
		first->prev = NULL;

        return first;
    } else {
        second->next = merge(first, second->next, cmp);
		second->next->prev = second;
		second->prev = NULL;

        return second;
    }
}

/**
 *
 */
node_t* list_mergesort(node_t* head, int(*cmp)(void*, void*)) {
	node_t *second;

    if (head == NULL || head->next == NULL) {
        return head;
	}

    second = split(head);

    /* Recursion for left and right halves */
    head = list_mergesort(head, cmp);
    second = list_mergesort(second, cmp);

    return merge(head, second, cmp); /* merges the two sorted halves */
}

/**
 *
 */
node_t* split(node_t* head) {
    node_t *fast = head, *slow = head, *tmp;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    tmp = slow->next;
    slow->next = NULL;

    return tmp;
}
