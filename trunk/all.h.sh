##############################################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2005
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/all.h.sh
##
##############################################################################

# helper script for PCH support.

# restore config options
path=`dirname $0`
options=`cat $path/all.h.in`

# process all precompiled files
echo -n "" > pch/all.h.tmp1
# the lines we're looking for looks like this:    # 65 "/usr/include/assert.h" 3 4
# look for such a line
# test if it includes one of the search paths
# first sed expression kills first part
# second one the crap at the end
# grep out the absolute pathnames and be sure that they are header files

# no need to add -I`dirname $i` -I./src -I./ to the search path, as this would only
# include files which are in the list anyway
gcc -E -x c++ $options `find . -name \*.cpp -o -name \*.h` 2>/dev/null | grep "^# [0-9]* " | sed 's/^#\ [0-9]*\ \"//' | sed 's/\"[\ 0-9]*$//' | grep ^/.*\.h[h]*$ >> pch/all.h.tmp1

# sort and delete duplicates
sort -u < pch/all.h.tmp1 > pch/all.h.tmp2

# create grep directive depending on the restored config options
# and convert again into an include directive
used_options=
for i in $options; do
    tmp=`echo -- $i | sed 's/^--\ //' | grep ^-I | sed 's/^-I//'`
    if test x$tmp != x; then
	used_options="$used_options $tmp"
    fi
done

echo -n "" > pch/all.h
for j in `cat pch/all.h.tmp2`; do
    for i in $used_options; do
	if echo $j | grep -q $i; then
	    echo "#include <"$j">" >> pch/all.h
	fi
    done
done
