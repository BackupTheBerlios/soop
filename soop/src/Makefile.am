bin_PROGRAMS = soop
soop_SOURCES = soop.c

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/libsclient $(all_includes)

# the library search path.
soop_LDFLAGS = $(all_libraries) 
soop_LDADD = $(top_builddir)/libsclient/libsclient.a -lm

METASOURCES = AUTO
