# Makefile for darcsstats
#
# Copyright (C) 2005 by Miklos Vajna <vmiklos@frugalware.org>
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

CFLAGS ?= -Wall -g -march=i686 -O2 -pipe

INSTALL = /usr/bin/install -c
DESTDIR =
bindir = /usr/bin
mandir = /usr/man/man1

darcsstats: list.o darcsstats.o util.o output.o

install: darcsstats
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(mandir)
	$(INSTALL) darcsstats $(DESTDIR)$(bindir)/darcsstats
	$(INSTALL) -m644 darcsstats.1 $(DESTDIR)$(mandir)/darcsstats.1

clean:
	rm -f darcsstats *.o
