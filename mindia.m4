
dnl ** WARNING: this is only the code, which has to be placed in aclocal.m4 !!!

AC_DEFUN(AC_CHECK_TMAKE,
[
  AC_MSG_CHECKING([check for tmake or qmake])
  my_tmake_path=`which tmake`
  my_qmake_path=`which qmake`
  if ! test "$my_qmake_path" = "" && test -x $my_qmake_path;
    then
      ac_found_tmake="yes"
      ac_tmake="$my_qmake_path"
    else
      if ! test "$my_tmake_path" = "" && test -x $my_tmake_path;
        then
          ac_found_tmake="yes"
          ac_tmake="$my_tmake_path"
        else
          ac_found_tmake="no"
          ac_tmake=""
      fi
  fi
  AC_MSG_RESULT([$ac_found_tmake (found $ac_tmake)])
])

