
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
               -I/usr/local/readline-8.0-rc1/include \
               -I/home/naushada/sw-packages/ACE_wrappers

vapsh_CFLAGS=-g -Wall

vapsh_LDFLAGS=-L/home/naushada/sw-packages/ACE_wrappers/lib \
              -L/usr/local/readline-8.0-rc1/lib

vapsh_LDADD=-lACE -lreadline -lpthread -ltinfo


