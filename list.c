/*
 *  list.c
 * 
 *  Copyright (c) 2005 by Miklos Vajna <vmiklos@frugalware.org>
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, 
 *  USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "darcsstats.h"
#include "list.h"

/* create a new list */
DSList* list_new()
{
	DSList *list = NULL;
	
	list = (DSList*)malloc(sizeof(DSList));
	if(list == NULL)
		return(NULL);
	list->data = NULL;
	list->prev = NULL;
	list->next = NULL;
	list->last = list;
	return(list);
}

/* return the last item of a list if it is not empty */
DSList* list_last(DSList *list)
{
	if (list == NULL)
		return NULL;
	else
		return list->last;
}

/* add new item to the list */
DSList* list_add(DSList *list, void *data)
{
	DSList *ptr, *lp;

	/* gr, haven't used list_new() yet */
	ptr = list;
	if(ptr == NULL)
	{
		ptr = list_new();
		if (!ptr)
			return(NULL);
	}

	lp = list_last(ptr);
	/* if this is not the first item */
	if(lp != ptr || lp->data != NULL)
	{
		lp->next = list_new();
		/* malloc failure */
		if(lp->next == NULL)
			return(NULL);
		lp->next->prev = lp;
		lp->last = NULL;
		lp = lp->next;
	}

	lp->data = data;
	ptr->last = lp;

	return(ptr);
}

/* count the items of a list */
int list_count(DSList *list)
{
	int i;
	DSList *lp;

	for(lp=list, i=0; lp; lp=lp->next, i++);
	return(i);
}

/* free each item of a list */
void list_free(DSList *list)
{
	DSList *ptr, *it = list;

	while(it) {
		ptr = it->next;
		free(it->data);
		free(it);
		it = ptr;
	}
}

/* test for existence of a patch in a DSList */
void *ispatch_in(char *needle, DSList *haystack)
{
	DSList *lp;

	for(lp = haystack; lp; lp = lp->next)
		if(lp->data && !strcmp(((patch_t*)lp->data)->author, needle))
			return(lp->data);
	return(NULL);
}

/* test for existence of a file in a DSList */
void *isfile_in(char *needle, DSList *haystack)
{
	DSList *lp;

	for(lp = haystack; lp; lp = lp->next)
		if(lp->data && !strcmp(((file_t*)lp->data)->name, needle))
			return(lp->data);
	return(NULL);
}

/* sorts a list (the algorithm used is mergesort)
 * the original idea is from
 * http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html */
DSList *list_sort(DSList *list, int(*cmp)(DSList *, DSList *))
{
	DSList *p, *q, *e, *tail;
	int insize, nmerges, psize, qsize, i;

	insize = 1;

	while (1)
	{
		p = list;
		list = NULL;
		tail = NULL;

		/* counts the merges's number we do while(p) */
		nmerges = 0;
		while (p)
		{
			/* there exists a merge to be done */
			nmerges++;
			/* step "insize" places along from p */
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++)
			{
				psize++;
				q = q->next;
				if (!q)
					break;
			}

			/* if q hasn't fallen off end, we have two lists to
			 * merge */
			qsize = insize;

			/* now we have two lists; merge them */
			while (psize > 0 || (qsize > 0 && q))
			{
				/* decide whether next element of merge comes
				 * from p or q */
				if (psize == 0)
				{
					/* p is empty; e must come from q. */
					e = q;
					q = q->next;
					qsize--;
				}
				else if (qsize == 0 || !q)
				{
					/* q is empty; e must come from p. */
					e = p;
					p = p->next;
					psize--;
				}
				else if ((*cmp)(p,q) <= 0)
				{
					/* first element of p is lower (or same)
					 * e must come from p. */
					e = p;
					p = p->next;
					psize--;
				}
				else
				{
					/* first element of q is lower
					 * e must come from q. */
					e = q;
					q = q->next;
					qsize--;
				}
				/* add the next element to the merged list */
				if (tail)
					tail->next = e;
				else
					list = e;
				tail = e;
			}
			/* now p has stepped "insize" places along,
			 * and q has too */
			p = q;
		}
		tail->next = NULL;

		/* if we have done only one merge, we're finished */
		if (nmerges <= 1)
			return list;
		/* otherwise repeat, merging lists twice the size */
		insize *= 2;
	}
}
