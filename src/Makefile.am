## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2008 SAMSUNG Electronics
##

MAINTAINERCLEANFILES	= Makefile.in
CLEANFILES		= *.bak

INCLUDES		= -I$(top_builddir) \
    		  	   -I$(top_srcdir) \
			  	   -I$(top_srcdir)/src/include \
			  	   -I$(top_srcdir)/src \
					-I$(top_srcdir)/mcf \
				    -I$(top_srcdir)/mcf/include \
    -I$(top_srcdir)/mcf/gwes \
    -I$(top_srcdir)/mcf/gwes/gtk \
    -I$(top_srcdir)/mcf/gwes/win32 \
    -I$(top_srcdir)/mcf/gwes/efl \
    -I$(top_srcdir)/res \
			  	   -I@ISF_INCLUDEDIR@ \
			  	   -DISF_ICONDIR=\"@ISF_ICONDIR@\" \
			  	   -DISF_DEFAULT_DATADIR=\"@ISF_DEFAULT_DATADIR@\" \
				   -DFACTORYFS=\"@FACTORYFS_DIR@\" \
				   -DPREFIX=\"@PREFIX_DIR@\" \
				   -DISE_NAME=\"@ISE_NAME@\" \
				   -DX1_PROF \
				   -DX1_DEBUG \
				   -DDEFAULT_THEME=\"default\"\
				   -DIMG_PATH_PREFIX=\"/usr/share/isf/ise/@ISE_NAME@\"\
				   -D__EFL__ 
			  
noinst_HEADERS		=

moduledir		= @ISF_MODULEDIR@/Helper

module_LTLIBRARIES	= ise-default.la

setupmoduledir           = @ISF_MODULEDIR@/SetupUI
setupmodule_LTLIBRARIES  = ise-default-setup.la

ise_default_la_SOURCES	= isemain.cpp ise.cpp ise-default-setting.cpp languagesetting.cpp window_slide_animator.cpp \
					 ../mcf/mcferroradjustment.cpp \
				    ../mcf/utils/win32/mcfutils-win32.cpp \
				 ../mcf/utils/linux/mcfutils-linux.cpp \
				    ../mcf/mcfdebug.cpp \
   				 ../mcf/mcfutils.cpp \
				    ../mcf/mcfcontroller.cpp \
   				 ../mcf/mcfgwes.cpp \
				    ../mcf/mcfevents.cpp \
   				 ../mcf/gwes/win32/mcfgraphics-win32.cpp \
				    ../mcf/gwes/efl/mcfwindows-efl.cpp \
   				 ../mcf/gwes/efl/mcfgraphics-efl.cpp \
   				 ../mcf/gwes/efl/mcfevents-efl.cpp \
				    ../mcf/gwes/win32/mcfevents-win32.cpp \
   				 ../mcf/mcffeedback.cpp \
				    ../mcf/mcfuibuilder.cpp \
   				 ../mcf/mcfcore.cpp \
   				 ../mcf/mcfwindows.cpp \
				    ../mcf/mcfgraphics.cpp \
   				 ../mcf/mcfimageproxy.cpp \
				    ../mcf/mcffontproxy.cpp \
   				 ../mcf/mcfcontext.cpp \
				    ../mcf/mcfactionstate.cpp \
   				 ../mcf/mcfresourcecache.cpp \
				 ../res/mcfresource.cpp

 

ise_default_la_CXXFLAGS = -I$(top_srcdir)/res/ \
							@ELM_CFLAGS@ @SLP_UTILX_CFLAGS@ @HEYNOTI_CFLAGS@ @IMF_CFLAGS@ \
							@GLIB_CFLAGS@ @VCONF_CFLAGS@ @SENSOR_CFLAGS@ \
							@ISF_CFLAGS@ @ECOREX_CFLAGS@ @X11_CFLAGS@


ise_default_la_LDFLAGS	= -avoid-version \
		   	  			   -rpath $(moduledir) \
			  			   -module \
			 			   -L$(top_srcdir)/data \
						   @ELM_LIBS@ \
						   @SLP_UTILX_LIBS@ \
						   @HEYNOTI_LIBS@ \
						   @IMF_LIBS@ \
						   @VCONF_LIBS@ \
                                                   @GLIB_LIBS@ \
                                                   @SENSOR_LIBS@ \
						   @ISF_LIBS@ \
						   @ECOREX_LIBS@ \
						   @X11_LIBS@
		
ise_default_la_LIBADD	= \
									@ELM_LIBS@ @SLP_UTILX_LIBS@ @HEYNOTI_LIBS@ @IMF_LIBS@ @VCONF_LIBS@ \
									@SENSOR_LIBS@

                               
ise_default_setup_la_SOURCES  = efl_ise_setup.cpp languagesetting.cpp

ise_default_setup_la_CXXFLAGS = \
									-I$(top_srcdir)/src/include \
									-I$(top_srcdir)/res \
									-DISE_NAME=\"ise-default\" \
									-DIMG_PATH_PREFIX=\"@PREFIX_DIR@/share/isf/ise/ise-default\" \
									@ELM_CFLAGS@ @SLP_UTILX_CFLAGS@ @HEYNOTI_CFLAGS@ @IMF_CFLAGS@ \
									@GLIB_CFLAGS@ @VCONF_CFLAGS@ \
									@SENSOR_CFLAGS@ \
									-DSETUP_BUILD


ise_default_setup_la_LDFLAGS  = \
									-avoid-version -rpath $(moduledir) -module \
									@LIBTOOL_EXPORT_OPTIONS@ @LTLIBINTL@ -L$(top_srcdir)/data 

ise_default_setup_la_LIBADD  = \
									@ELM_LIBS@ @SLP_UTILX_LIBS@ @HEYNOTI_LIBS@ @IMF_LIBS@ @VCONF_LIBS@ \
									@SENSOR_LIBS@




