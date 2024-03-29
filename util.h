/*
 *  util.h
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

#define MALLOC(p, b) { if((b) > 0) \
	{ p = malloc(b); if (!(p)) \
	{ fprintf(stderr, "malloc failure: could not allocate %d bytes\n", b); \
	exit(1); }} else p = NULL; }
#define FREE(p) { if (p) { free(p); (p) = NULL; }}

#if defined(_WIN32) && !defined(__CYGWIN32__) && !defined(__CYGWIN__)
int mkstemp(char* template);
#endif

char *strrstr(char *haystack, char *needle);
char* droplastword(char *str);
char *reponame(char *str);
char *htmlize(char *from);
