/*
 *  darcsstats.c
 * 
 *  Copyright (c) 2005-2006 by Miklos Vajna <vmiklos@frugalware.org>
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
#include <ctype.h>
#include <unistd.h>

#include "darcsstats.h"
#include "list.h"
#include "util.h"
#include "output.h"
#include "config.h"

/** @defgroup ds_patch Patch functions
 * @{
 * @brief Patch handling functions used by DarcsStats
 */

/** Allocates an initializes a patch.
 * @return patch pointer
 */
patch_t *patch_new(void)
{
	patch_t *patch = NULL;

	MALLOC(patch, sizeof(patch_t));

	patch->author[0] = '\0';
	patch->changes = 0;
	patch->lines = 0;
	return(patch);
}

/** Registers a new patch in the patch list.
 * If the patch already registered, just increments the number of
 * changes of the patch, otherwise adds the new patch item to the list
 * of patches.
 * @param patches the list of patches already registered
 * @param patch to register
 * @return the given patchlist
 */
DSList *patch_add(DSList *patches, patch_t *patch)
{
	patch_t *handle;
	if((handle=ispatch_in(patch->author, patches))!=NULL)
	{
		if(patch->lines > 0)
		{
			handle->lines++;
		}
		else
		{
			handle->changes++;
		}
		FREE(patch);
	}
	else
	{
		patch->changes++;
		patches = list_add(patches, patch);
	}
	return(patches);
}
/** @} */


/** @defgroup ds_file File functions
 * @{
 * @brief File handling functions used by DarcsStats
 */

/** Allocates an initializes a file.
 * @return file pointer
 */
file_t *file_new(void)
{
	file_t *file = NULL;

	MALLOC(file, sizeof(file_t));

	file->name[0] = '\0';
	file->changes = 0;
	return(file);
}

/** Registers a new file in the file list.
 * If the file already registered, just increments the number of
 * changes of the file, otherwise adds the new file item to the list
 * of files.
 * @param files the list of files already registered
 * @param file to register
 * @return the given filelist
 */
DSList *file_add(DSList *files, file_t *file)
{
	file_t *handle;
	if((handle=isfile_in(file->name, files))!=NULL)
	{
		handle->changes++;
		FREE(file);
	}
	else
	{
		file->changes++;
		files = list_add(files, file);
		handle=file;
	}
	return(files);
}

/** Compares file changes.
 * Supposed to be a helper function for list_sort().
 * @param a list item containing a file entry
 * @param b list item containing a file entry
 * @return the arithmetic difference of the changes of the two files
 */
int cmp(DSList *a, DSList *b)
{
	return(((file_t*)b->data)->changes - ((file_t*)a->data)->changes);
}
/** @} */

int main(int argc, char **argv)
{
	FILE* fp = NULL;		/* input/output file pointer*/
	DSList *patches;		/* main patchlist */
	DSList *files;			/* main filelist */
	/* total number of added lines, required for couting percentage */
	int alllines=0;
	char cwd[PATH_MAX];
	char repopath[PATH_MAX]; /* path to the darcs repo */
	char output[PATH_MAX]; /* output file */
	char cmd[PATH_MAX];
	char *clfile;

	/* temp vars */
	patch_t *patch;
	file_t *file;
	char line[512];
	char author[256];

	/* parameters */
	if (argc != 3)
	{
		printf("usage: %s repo_directory output.html\n", argv[0]);
		return(1);
	}
	strncpy(repopath, argv[1], PATH_MAX);
	strncpy(output, argv[2], PATH_MAX);

	/* save the cwd */
	getcwd(cwd, PATH_MAX);
	if (chdir(repopath) != 0)
	{
		printf("can't set directory to %s!\n", repopath);
		return(1);
	}
	/* extract the changelog to /tmp */
	clfile = strdup("/tmp/darcsstats_XXXXXX");
	mkstemp(clfile);
	snprintf(cmd, PATH_MAX, "darcs changes -v > %s", clfile);
	if (system(cmd) != 0)
	{
		printf("Can't extract the changelog!\n");
		return(1);
	}
	chdir(cwd);

	patches = list_new();
	files = list_new();

	if ((fp = fopen(clfile, "r")) == NULL)
	{
		perror("Could not open input file for reading");
		return(1);
	}

	while(!feof(fp))
	{
		if(fgets(line, 256, fp) == NULL)
		{
			break;
		}
		if(isupper(line[0]))
		{
			/* if NULL, that must be a line that is longer than
			 * 256 chars, we can simply ignore it */
			if (strrstr(line, "  ") != NULL)
			{
				patch = patch_new();
				/* for later usage at lines*/
				strncpy(author, strrstr(line, "  "), 254);
				/* drop the newline suffix */
				author[strlen(author)-1]='\0';
				strcpy(patch->author, author);
				patches = patch_add(patches, patch);
			}
		}
		else if(strstr(line, "    hunk ./") == line)
		{
			file = file_new();
			strncpy(file->name, droplastword(strrstr(line, " ./")),
				PATH_MAX);
			files = file_add(files, file);
		}
		if(strstr(line, "    +") == line ||
			strstr(line, "    -") == line)
		{
			patch = patch_new();
			strcpy(patch->author, author);
			patch->lines++;
			patches = patch_add(patches, patch);
			alllines++;
		}
	}
	/* delete the no more needed changelog */
	if(unlink(clfile))
	{
		printf("warning: could not remove tempfile %s\n", clfile);
	}
	if ((fp = fopen(output, "w")) == NULL)
	{
		perror("Could not open output file for writing");
		return(1);
	}
	print_header(fp, reponame(repopath));
	print_table(fp, patches, alllines);
	files = list_sort(files, cmp);
	print_flist(fp, files);
	print_stats(fp, patches, files);
	print_footer(fp, reponame(repopath));
	fclose(fp);
	FREELIST(patches);
	FREELIST(files)
	return(0);
}
