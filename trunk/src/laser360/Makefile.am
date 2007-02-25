## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/laser360/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = Laser360Service

lib_LIBRARIES = libmiroLaser360.a

libmiroLaser360_a_SOURCES = \
	Parameters.cpp \
	Laser360Connection.cpp \
	Laser360Task.cpp \
	ldObject.cpp \
	ldInOutObject.cpp \
	ldInOutHandler.cpp \
	ldSerial.cpp \
	ldBaseTelegram.cpp \
	ldScannerTelegram.cpp \
	ldFactory.cpp \
	ldFactoryInit.cpp \
	ldTimer.cpp \
	ldSerialCom.cpp \
	ldScannerControl.cpp

Parameters.h: Parameters.cpp
Parameters.cpp: Parameters.xml

if COND_PCH
libmiroLaser360_a_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif


laser_include_HEADERS = \
	Laser360Impl.h \
	Laser360Connection.h \
	Laser360Task.h \
	Parameters.h \
	ldCrcTable.h \
	ldUserTypes.h \
	ldTelegramCodes.h \
	ldBaseException.h \
	ldFactoryException.h \
	ldSerialException.h \
	ldTelegramException.h \
	ldObject.h \
	ldInOutObject.h \
	ldInOutHandler.h \
	ldSerial.h \
	ldBaseTelegram.h \
	ldScannerTelegram.h \
	ldFactory.h \
	ldFactoryInit.h \
	ldTimer.h \
	ldSerialCom.h \
	ldScannerControl.h

Laser360Service_SOURCES = \
	Laser360Impl.cpp \
	Laser360Impl.h \
	Laser360Connection.cpp \
	Laser360Connection.h \
	Laser360Task.cpp \
	Laser360Task.h \
	Laser360Service.cpp \
	Laser360Service.h \
	ldObject.cpp \
	ldObject.h \
	ldInOutObject.cpp \
	ldInOutObject.h \
	ldInOutHandler.cpp \
	ldInOutHandler.h \
	ldSerial.cpp \
	ldSerial.h \
	ldBaseTelegram.cpp \
	ldBaseTelegram.h \
	ldScannerTelegram.cpp \
	ldScannerTelegram.h \
	ldFactory.cpp \
	ldFactory.h \
	ldFactoryInit.cpp \
	ldFactoryInit.h \
	ldTimer.cpp \
	ldTimer.h \
	ldSerialCom.cpp \
	ldSerialCom.h \
	ldScannerControl.cpp \
	ldScannerControl.h

if COND_PCH
Laser360Service_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif

Laser360Service_LDADD = \
	libmiroLaser360.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: Laser360Service libmiroLaser360.a
	$(INSTALLLIBRARIES)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANLIBRARIES)
	$(CLEANPROGRAMS)
