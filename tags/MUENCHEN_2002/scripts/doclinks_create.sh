#!/bin/bash
#
# link all latex_ref-files automatically created by the
# doxygen-program to the normal doc/tex-directory.
# This way we don't have that much problems with tex-include-dirs.
#
# no warranty, may delete all you need
#
# (C) 2001 Gerd Mayer
#


if (test $MIRO_ROOT != "")
then
  cd $MIRO_ROOT/doc/tex/
  if (test -d latex_ref)
  then
    for i in latex_ref/*
    do
      if (test -f $i && test ! -e ./`basename $i`)
      then
        ln -s $i .
      fi
    done
  fi
else
  echo "MIRO_ROOT is not set. Nothing done!!"
fi
