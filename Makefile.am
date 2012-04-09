## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2008 SAMSUNG
##

AUX_DIST                = $(ac_aux_dir)/config.guess \
			$(ac_aux_dir)/config.sub \
			$(ac_aux_dir)/install-sh \
			$(ac_aux_dir)/ltconfig \
			$(ac_aux_dir)/ltmain.sh \
			$(ac_aux_dir)/missing \
			$(ac_aux_dir)/mkinstalldirs

EXTRA_DIST		= config.rpath \
			  mkinstalldirs \
			  bootstrap

AUTOMAKE_OPTIONS 	= gnu
SUBDIRS 		= m4 po src data

MAINTAINERCLEANFILES 	= Makefile.in \
			  aclocal.m4 \
			  configure \
			  $(AUX_DIST) 

CLEANFILES		= *.bak 
			  

ACLOCAL_AMFLAGS		= -I m4
