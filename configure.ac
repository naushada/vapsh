AC_INIT([vapsh], [1.0], [naushad.dln@gmail.com])
AC_CONFIG_AUX_DIR([build])
AM_INIT_AUTOMAKE([1.14.1 subdir-objects foreign dist-bzip2])
AC_PROG_CXX
AC_PROG_CC

AC_CONFIG_FILES([Makefile])

AC_CHECK_HEADERS([readline/readline.h], [],
                 [AC_MSG_ERROR([Unable to find header file readline.h])])

AC_CHECK_HEADERS([readline/history.h], [],
                 [AC_MSG_ERROR([Unable to find the header file history.h])])

#AC_CHECK_HEADERS([ace/Reactor.h], [],
#                 [AC_MSG_ERROR([Unable to find header file Reactor.h])])

# Checks for libraries.
#AC_SEARCH_LIBS([svc], [ACE], [], [
#  AC_MSG_ERROR([Unable to find svc()])
#])

AC_SEARCH_LIBS([readline], [readline], [], [
  AC_MSG_ERROR([Unable to find readline()])
])

AC_OUTPUT
