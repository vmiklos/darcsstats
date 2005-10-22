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
#include <stdlib.h>
#if defined(_WIN32) && !defined(__CYGWIN32__) && !defined(__CYGWIN__)
#include <windows.h>
#endif

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
	char *ret;
	/* drop the / suffix */
	if(str[strlen(str)-1]=='/')
		str[strlen(str)-1]='\0';
	ret = strrstr(str, "/");
	if (ret!=NULL)
		return(ret);
	else
		return(str);
}

char *htmlize(char *string)
{
	static char buf[1024];
	char *ptr = NULL;
	char *nextptr = NULL;

	/* fill out buf with zeros */
	memset(buf, 0, 1024);

	/* handle < */
	ptr = strchr(string, '<');
	if (ptr != NULL)
	{
		nextptr = ptr + 1;
		*ptr = 0;
		strncpy(buf, string, 1023);
		strncat(buf, "&lt;", 1023 - strlen(buf));
		string = nextptr;
	}

	/* handle > */
	ptr = strchr(string, '>');
	if (ptr != NULL)
	{
		nextptr = ptr + 1;
		*ptr = 0;
		strncat(buf, string, 1023 - strlen(buf));
		strncat(buf, "&gt;", 1023 - strlen(buf));
		string = nextptr;
	}

	strncat(buf, string, 1023 - strlen(buf));
	return buf;
}

#if defined(_WIN32) && !defined(__CYGWIN32__) && !defined(__CYGWIN__)
int mkstemp(char* template)
{
	char temppath[512];
	if(GetTempPath(512,temppath)!=0)
	{
		if(GetTempFileName(temppath,"fil",0,template)!=0)
		{
				FILE *pFile;
				pFile=fopen(template,"w+");
				if(pFile!=NULL)
					return (int)pFile;
		}
	}
	return -1;
}
#endif
