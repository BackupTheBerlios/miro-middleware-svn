## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/idl/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`

SUFFIXES = .idl C.cpp C.h C.i.h S.cpp S.h S.i.h

.idlC.cpp:
	$(TAO_IDL) -in -Ge 0 -hc C.h -cs C.cpp -ci C.i.h -hs S.h -hT S_T.h -ss S.cpp -sT S_T.cpp -si S.i.h -st S_T.i.h -Sc -Wp,-I$(top_srcdir)/idl -I$(top_srcdir)/idl $<


lib_LTLIBRARIES = libmiroIDL.la


globalsources = \
	Time.idl \
	Exception.idl \
	WorldPoint.idl \
	WorldVector.idl \
	Position.idl \
	Velocity.idl \
	MotionStatus.idl \
	Odometry.idl \
	Motion.idl \
	ClosedMotion.idl \
	SynchroMotion.idl \
	DifferentialMotion.idl \
	Stall.idl \
	Battery.idl \
	ScanDescription.idl \
	RangeEvent.idl \
	RangeSensor.idl \
	Pan.idl \
	Tilt.idl \
	PanTilt.idl \
	CanonPanTilt.idl \
	CanonCamera.idl \
	Buttons.idl \
	Video.idl \
	VideoControl.idl \
	Speech.idl \
	SphinxSpeech.idl \
	BehaviourEngine.idl \
	Joystick.idl

b21sources = \
	Laser.idl \
	DtlkSpeech.idl \
	DirectedPerceptionPanTilt.idl \
	B21Buttons.idl \
	B21Motion.idl

pioneersources = \
	PioneerStall.idl \
	PioneerMotion.idl \
	Gripper.idl

sparrowsources1 = \
	PioneerStall.idl \
	PioneerMotion.idl \
	Gripper.idl

sparrowsources2 = \
	Kicker.idl \
	SparrowPanTilt.idl \
	SparrowStall.idl \
	SparrowMotion.idl \
	SparrowAlive.idl

deprecatedsources = \
	ImageDefinitions.idl \
	Image.idl



if COND_B21
b21sources_used = $(b21sources)
endif

if COND_PIONEER
pioneersources_used = $(pioneersources)
endif

if COND_SPARROW99
if !COND_PIONEER
sparrowsources1_used = $(sparrowsources1)
endif
sparrowsources2_used = $(sparrowsources2)
endif

if COND_DEPRECATED
deprecatedsources_used = $(deprecatedsources)
endif


sources = \
	$(globalsources) \
	$(b21sources_used) \
	$(pioneersources_used) \
	$(sparrowsources1_used) \
	$(sparrowsources2_used) \
	$(deprecatedsources_used)

builtsources = \
	$(sources:.idl=C.cpp) \
	$(sources:.idl=C.h) \
	$(sources:.idl=C.i.h) \
	$(sources:.idl=S.h) \
	$(sources:.idl=S.cpp) \
	$(sources:.idl=S.i.h)

libmiroIDL_la_DEPENDENCIES = $(sources)
nodist_libmiroIDL_la_SOURCES = $(builtsources)

BUILT_SOURCES = $(builtsources)
CLEANFILES = $(builtsources)

nodist_idl_include_HEADERS = \
	$(sources:.idl=C.h) \
	$(sources:.idl=C.i.h) \
	$(sources:.idl=S.h) \
	$(sources:.idl=S.i.h)

EXTRA_DIST = \
	$(globalsources) \
	$(b21sources) \
	$(pioneersources) \
	$(sparrowsources1) \
	$(sparrowsources2) \
	$(deprecatedsources)

all-local:
	if test -L idl; then rm idl; fi
	$(LN_S) ./ idl
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
