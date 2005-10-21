/*
 *  util.c
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
#include <string.h>
#include <ctype.h>

#include "util.h"

/* finds the last occurrence of needle in haystack */
char *strrstr(char *haystack, char *needle)
{
	char *p;
	p = strstr(haystack, needle);
	if (p != NULL)
	{
		while(strstr(p+1, needle)) p=strstr(p + 1, needle);
		return p+strlen(needle);
	}
	else
		return NULL;
}

char* droplastword(char *str)
{
	/* jump to the end of the string */
	char *pch = (char*)(str + (strlen(str) - 1));
	/* drop the newline suffix is exists */
	if(*pch=='\n')
		pch--;
	/* remove spaces */
	while(!isspace(*pch))
		pch--;
	*pch = '\0';
	return(str);
}

/* returns the "name" of the repo */
char *reponame(char *str)
{
	/* drop the / suffix */
	if(str[strlen(str)-1]=='/')
		str[strlen(str)-1]='\0';
	return strrstr(str, "/");
}
