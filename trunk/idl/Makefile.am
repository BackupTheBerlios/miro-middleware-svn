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
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


SUFFIXES = .idl C.cpp C.h C.i.h S.cpp S.h S.i.h

.idlC.cpp:
	$(TAO_IDL) -in -Ge 0 -hc C.h -cs C.cpp -ci C.i.h -hs S.h -hT S_T.h -ss S.cpp -sT S_T.cpp -si S.i.h -st S_T.i.h -Sc -Wp,-I$(top_srcdir)/idl -I$(top_srcdir)/idl $<

C.cppC.h: ;

C.hC.i.h: ;

C.i.hS.cpp: ;

S.cppS.h: ;

S.hS.i.h: ;

lib_LTLIBRARIES = libmiroIDL.la


globalsources = \
	Time.idl \
	Exception.idl \
	WorldPoint.idl \
	WorldPoint3D.idl \
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
	Laser.idl \
	Pan.idl \
	Tilt.idl \
	PanTilt.idl \
	CanonPanTilt.idl \
	Buttons.idl \
	Video.idl \
	VideoControl.idl \
	CameraControl.idl \
	QuickCamControl.idl \
	Speech.idl \
	SphinxSpeech.idl \
	BehaviourEngine.idl \
	Joystick.idl \
	DistanceScan.idl \
	ArbiterEvent.idl \
	ObjectVector.idl \
	Compass.idl \
	CompassEvent.idl \
	Inclination.idl \
	Inclinometer.idl \
	InclinometerEvent.idl \
	FieldStrength.idl \
	Magnetometer.idl \
	MagnetometerEvent.idl \
	Thermometer.idl \
	ThermometerEvent.idl \
	Gps.idl \
	GpsData.idl \
	CtGps.idl

b21sources = \
	DtlkSpeech.idl \
	DirectedPerceptionPanTilt.idl \
	B21Buttons.idl \
	B21Motion.idl

pioneersources = \
	PioneerStall.idl \
	PioneerMotion.idl \
	CanonCameraControl.idl \
	SonyCameraControl.idl \
	Gripper.idl \
	TCM2.idl \
	TCM2Event.idl

sparrowsources = \
	Kicker.idl \
	SparrowPanTilt.idl \
	SparrowStall.idl \
	SparrowMotion.idl \
	SparrowAlive.idl

deprecatedsources = \
	CanonCamera.idl



if COND_B21
b21sources_used = $(b21sources)
endif

if COND_PIONEER
pioneersources_used = $(pioneersources)
endif

if COND_SPARROW99
sparrowsources_used = $(sparrowsources)
endif

if COND_DEPRECATED
deprecatedsources_used = $(deprecatedsources)
endif


sources = \
	$(globalsources) \
	$(b21sources_used) \
	$(pioneersources_used) \
	$(sparrowsources_used) \
	$(deprecatedsources_used)

builtsources = \
	$(sources:.idl=C.cpp) \
	$(sources:.idl=C.h) \
	$(sources:.idl=C.i.h) \
	$(sources:.idl=S.h) \
	$(sources:.idl=S.cpp) \
	$(sources:.idl=S.i.h)

libmiroIDL_la_SOURCES = $(builtsources)

#libmiroIDL_la_LIBADD = \
#	-lTAO \
#	-lACE

BUILT_SOURCES = $(builtsources)
CLEANFILES = $(builtsources)

all-local: libmiroIDL.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
