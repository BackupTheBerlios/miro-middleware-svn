#!/bin/bash
#
# delete all links created by the "doclinks_create.sh"-skript.
# normal files remain untouched.
#
# no warranty, may delete all you need
#
# (C) 2001 Gerd Mayer
#

if (test $MIRO_ROOT != "")
then
  cd $MIRO_ROOT/doc/tex/
  for i in *
  do
    if (test -h "$i")
    then
      rm -f "$i"
    fi
  done
else
  echo "MIRO_ROOT is not set. Nothing done!!"
fi

