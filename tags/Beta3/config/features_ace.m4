## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/features_ace.m4
##
##############################################################################

AC_DEFUN(AC_FEATURES_COMPILE_ACE,
[
	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for options in ACE/TAO)

# empty program (nothing needed)
cat << \EOF > conftest.cpp
int main() {}
EOF
# ACE/TAO-like makefile for library compilation
cat << \EOF > conftest.Makefile.lib
NAME = conftest
INSTALL=
VDIR = conftest.obj/
VSHDIR = conftest.shobj/
LSRC  = conftest.cpp
default: all
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
EOF
# empty program (nothing needed)
cat << \EOF > conftest.bin.cpp
int main() {}
EOF
# ACE/TAO-like makefile for binary compilation
cat << \EOF > conftest.Makefile.bin
BIN = conftest.bin
INSTALL=
VDIR = conftest.obj/
VSHDIR = conftest.shobj/
LSRC  = conftest.bin.cpp
default: all
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
EOF
# run the compilation
make -f conftest.Makefile.lib > conftest.run
make -f conftest.Makefile.bin >> conftest.run
# cut out the desired features
# I'm quite sure, that I earn the useless use of cat/echo/back-ticks award, 
# but hey, it works.
ac_feature_acetao_symbols=`for i in \`cat conftest.run \`; do expr $i : '\(-D.*\)'; done | grep -v ^$ | sort -u`
# ac_feature_acetao_warnings=`for i in \`cat conftest.run \`; do expr $i : '\(-W.*\)'; done | grep -v ^$ | sort -u`
ac_feature_acetao_libs=`for i in \`cat conftest.run \`; do expr $i : '\(-l.*\)'; done | grep -v ^$ | sort -u`
ac_feature_acetao_libdirs=`for i in \`cat conftest.run \`; do expr $i : '\(-L.*\)'; done | grep -v ^$ | sort -u`
ac_feature_acetao_incldirs=`for i in \`cat conftest.run \`; do expr $i : '\(-I.*\)'; done | grep -v ^$ | sort -u`
# eliminate some features we already know
for i in $ACE_CFLAGS $TAO_CFLAGS $QT_CFLAGS
do
	if test "x$i" != "x"; then
		ac_feature_acetao_incldirs=`echo $ac_feature_acetao_incldirs | sed s~$i~~`
	fi
done
ac_feature_acetao_incldirs=`echo $ac_feature_acetao_incldirs | sed 's/ \/* //g'`

for i in $ACE_LDFLAGS $TAO_LDFLAGS $QT_LDFLAGS
do
	if test "x$i" != "x"; then
		ac_feature_acetao_libdirs=`echo $ac_feature_acetao_libdirs | sed s~$i~~`
	fi
done
ac_feature_acetao_libdirs=`echo $ac_feature_acetao_libdirs | sed 's/ \/* //g'`
# without different qt-version to avoid linking again both
for i in $ACE_LIBS $TAO_LIBS
do
	if test "x$i" != "x"; then
		ac_feature_acetao_libs=`echo $ac_feature_acetao_libs | sed s~$i~~`
	fi
done
ac_feature_acetao_libs=`echo $ac_feature_acetao_libs | sed 's/-lqt-mt//' | sed 's/-lqt//'`

	AC_SUBST(INHERITED_LDFLAGS, ["$ac_feature_acetao_libdirs"])
	AC_SUBST(INHERITED_CPPFLAGS, ["$ac_feature_acetao_incldirs"])
	AC_SUBST(INHERITED_CFLAGS, ["$ac_feature_acetao_incldirs"])
	AC_SUBST(INHERITED_LIBS, ["$ac_feature_acetao_libs"])

# ac_feature_acetao_warnings=`echo $ac_feature_acetao_warnings`
ac_feature_acetao_symbols=`echo $ac_feature_acetao_symbols`

#	AC_SUBST(INHERITED_WARNINGS, ["$ac_feature_acetao_warnings"])
	AC_SUBST(INHERITED_SYMBOLS, ["$ac_feature_acetao_symbols"])

	AC_MSG_RESULT([yes])
	AC_LANG_POP()
])
