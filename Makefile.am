
#/*!Name of the binary*/
bin_PROGRAMS=vapsh

vapsh_SOURCES=src/readlineIF.cc \
              src/hostapdIF.cc \
              src/vapsh.cc \
              inc/readlineIF.h \
              inc/hostapdIF.h \
              inc/vapsh.h

vapsh_CPPFLAGS=-Iinc \
               -I/usr/local \
               -I/usr/local/ACE-6.5.0/include

vapsh_CFLAGS=-g -Wall

vapsh_LDFLAGS=-L/usr/localACE-6.5.0/lib

vapsh_LDADD=-lACE -lreadline -lpthread -ltinfo


