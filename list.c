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
