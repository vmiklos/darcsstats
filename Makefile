# Makefile for darcsstats
#
# Copyright (C) 2005-2006 by Miklos Vajna <vmiklos@vmiklos.hu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#

OS ?= unix
CFLAGS ?= -Wall -march=i686 -O2 -pipe

ifeq ($(OS),win)
	CC = i386-mingw32msvc-gcc
	LDFLAGS=-mwindows
	EXEEXT=.exe
endif

INSTALL = /usr/bin/install -c
DESTDIR =
bindir = /usr/bin
mandir = /usr/man/man1

OBJS = list.o darcsstats.o util.o output.o

darcsstats$(EXEEXT): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

doc: HEADER.html Changelog

HEADER.html: README
	ln -s README HEADER.txt
	asciidoc -a toc -a numbered -a sectids HEADER.txt
	rm HEADER.txt

Changelog: _darcs/inventory
	darcs changes > Changelog

man:
	doxygen Doxyfile

html: man
	mkdir -p html
	man2html darcsstats.1 >html/darcsstats.1.html
	man2html darcsstats.3 >html/darcsstats.3.html
	for i in man/man3/*; do \
		man2html $$i |sed '1 d;2 d' >html/`basename $$i`.html; \
	done

install: darcsstats
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(mandir)
	$(INSTALL) darcsstats $(DESTDIR)$(bindir)/darcsstats
	$(INSTALL) -m644 darcsstats.1 $(DESTDIR)$(mandir)/darcsstats.1

clean:
	rm -f *.o

distclean:
	rm -f darcsstats$(EXEEXT)
	rm -rf html man
