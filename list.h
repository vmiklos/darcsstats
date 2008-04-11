/*
 *  list.h
 * 
 *  Copyright (c) 2005-2006 by Miklos Vajna <vmiklos@vmiklos.hu>
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

typedef struct __dslist_t
{
	void*   data;
	struct __dslist_t* prev;
	struct __dslist_t* next;
	struct __dslist_t* last;
} DSList;

#define FREELIST(p) { list_free(p, 0); p = NULL; }
#define FREELISTPTR(p) { list_free(p, 1); p = NULL; }

DSList* list_new();
DSList* list_last(DSList* list);
DSList* list_add(DSList* list, void* data);
int list_count(DSList* list);
void list_free(DSList* list, int mode);
void *ispatch_in(char *needle, DSList *haystack);
void *isfile_in(char *needle, DSList *haystack);
void *isignore_in(char *needle, DSList *haystack);
DSList *list_sort(DSList *list, int(*cmp)(DSList *, DSList *));
