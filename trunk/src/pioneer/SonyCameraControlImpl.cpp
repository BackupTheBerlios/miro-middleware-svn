/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#include "pioneer/SonyCameraControlImpl.h"
#include "pioneer/SonyMessage.h"
#include "pioneer/PioneerConnection.h"
#include "psos/PsosMessage.h"

#include "miro/Exception.h"
#include "miro/Angle.h"
#include "miro/Log.h"

namespace Sony
{
  const float SonyCameraControlImpl::minFocalLength = 5.4;
  const float SonyCameraControlImpl::maxFocalLength = 64.8;
  const unsigned short SonyCameraControlImpl::zoom_dev_max = 0x03ff;
  const unsigned short SonyCameraControlImpl::focus_inf = 0x1000;
  const unsigned short SonyCameraControlImpl::focus_near = 0x9fff;

  SonyCameraControlImpl::SonyCameraControlImpl(Pioneer::Connection& _connection,
				const Pioneer::Parameters& _params) :
    CameraControlImpl(_params.cameraParams),
    connection(_connection),
    currentAutoExposure_(Miro::SonyCameraControl::AE_AUTO),
    currentIris_(Miro::SonyCameraControl::IRIS_F14),
    currentGain_(0)
  {
    MIRO_LOG_CTOR("Sony::SonyCameraControlImpl");
    pal_ntsc = _params.cameraParams.format == "pal"? 0 : 1;

    currentFocalLength_ = maxFocalLength
			* tan(zoomParameters_.rangeMin/2) / tan(currentZoom_/2);
    lastFocusValue_ = focus_inf;
    

    // Calculation of hyperbolic coefficients (thanks to Mathematica).
    double zoom_min = zoomParameters_.rangeMin;
    double zoom_max = zoomParameters_.rangeMax;
    zoom_a = zoom_dev_max * ( - 15 * zoom_min * zoom_min * zoom_min
			  + 49 * zoom_min * zoom_min * zoom_max
			  - 49 * zoom_min * zoom_max * zoom_max
			  + 15 * zoom_max * zoom_max * zoom_max )
		      / ( 4 * (zoom_min + zoom_max) * (zoom_min + zoom_max) );
    zoom_b = (    3 * zoom_min * zoom_min
	       - 10 * zoom_min * zoom_max
	       +  3 * zoom_max * zoom_max ) / ( 2 * (zoom_min + zoom_max) );
    zoom_c = zoom_dev_max * ( 5 * zoom_min - 3 * zoom_max )
			  / ( 2 * (zoom_min + zoom_max) );

    MIRO_LOG_CTOR_END("Sony::SonyCameraControlImpl");
  }

  // IDL interface implementation:
  //-------------------------------------------------------------------------
  // from CameraControl.idl
  //-------------------------------------------------------------------------
  void
  SonyCameraControlImpl::setZoom(CORBA::Float value)
		throw(Miro::EOutOfBounds)
  {
    Super::setZoom(value);
    currentFocalLength_ = maxFocalLength
			* tan(zoomParameters_.rangeMin/2) / tan(value/2);
    Sony::Message msg;
    msg.zoom(zoom2dev(currentZoom_));
    connection.sendCamera(msg);
  }

  void
  SonyCameraControlImpl::setFocus(CORBA::Float value)
		throw(Miro::EOutOfBounds)
  {
    // current maximum CCD <-> lens distance
    float d_max = focusParameters_.rangeMin * minFocalLength
		/ (focusParameters_.rangeMin - minFocalLength)
		- minFocalLength + currentFocalLength_;
    // current minimum focusable distance
    float min_focus = d_max * currentFocalLength_
		    / (d_max - currentFocalLength_);
    if (value < min_focus)
      throw Miro::EOutOfBounds();

    autoFocusSet_ = false;
    Sony::Message msg;
    msg.autofocus(autoFocusSet_);
    connection.sendCamera(msg);
    currentFocus_ = value;
    lastFocusValue_ = focus2dev(currentFocus_);
    msg.focus(lastFocusValue_);
    connection.sendCamera(msg);
  }

  float
  SonyCameraControlImpl::getFocus()
		throw()
  {
    if (autoFocusSet_)
      return -1;

    // The resulting focus might have changed due to a new zoom value,
    // so we have to recalculate it.
    // current minimum/maximum CCD <-> lens distance
    float d_min = currentFocalLength_;
    float d_max = focusParameters_.rangeMin * minFocalLength
		/ (focusParameters_.rangeMin - minFocalLength)
		- minFocalLength + currentFocalLength_;
    // CCD <-> lens distance
    float d = (lastFocusValue_ - focus_inf) * (d_max - d_min)
	    / (focus_near - focus_inf) + d_min;
    return d * currentFocalLength_ / (d - currentFocalLength_);
  }

  Miro::FocusRangeIDL
  SonyCameraControlImpl::getFocusRange()
		throw()
  {
    // current maximum CCD <-> lens distance
    float d_max = focusParameters_.rangeMin * minFocalLength
		/ (focusParameters_.rangeMin - minFocalLength)
		- minFocalLength + currentFocalLength_;
    // current minimum focusable distance
    float min_focus = d_max * currentFocalLength_
		    / (d_max - currentFocalLength_);
    Miro::FocusRangeIDL range;
    range.min = min_focus;
    range.max = INFINITY;
    return range;
  }

  void
  SonyCameraControlImpl::setAutoFocus(CORBA::Boolean value)
		throw(Miro::EDevIO)
  {
    Super::setAutoFocus(value);
    Sony::Message msg;
    msg.autofocus(autoFocusSet_);
    connection.sendCamera(msg);
  }

  void
  SonyCameraControlImpl::setShutter(Miro::TimeIDL& value)
		throw(Miro::EOutOfBounds)
  {
    Super::setShutter(value);

    switch (currentAutoExposure_) {
      case Miro::SonyCameraControl::AE_MANUAL:
      case Miro::SonyCameraControl::AE_FIXED_IRIS:
	setAutoExposure(Miro::SonyCameraControl::AE_MANUAL);
	break;
      default:
	setAutoExposure(Miro::SonyCameraControl::AE_FIXED_SHUTTER);
    }

    Sony::Message msg;
    msg.shutter(shutter2dev(currentShutter_));
    connection.sendCamera(msg);
  }

  //-------------------------------------------------------------------------
  // from SonyCameraControl.idl
  //-------------------------------------------------------------------------
  void
  SonyCameraControlImpl::setAutoExposure
		(Miro::SonyCameraControl::AutoExposureMode ae)
		throw(Miro::EOutOfBounds)
  {
    char mode;
    switch (ae) {
      case Miro::SonyCameraControl::AE_AUTO:
	mode = 0x00;
	break;
      case Miro::SonyCameraControl::AE_MANUAL:
	mode = 0x03;
	break;
      case Miro::SonyCameraControl::AE_FIXED_SHUTTER:
	mode = 0x0a;
	break;
      case Miro::SonyCameraControl::AE_FIXED_IRIS:
	mode = 0x0b;
	break;
      case Miro::SonyCameraControl::AE_BRIGHT:
	mode = 0x0d;
	break;
      default:
	throw Miro::EOutOfBounds();
    }
    currentAutoExposure_ = ae;
    Sony::Message msg;
    msg.autoexposure(mode);
    connection.sendCamera(msg);
  }

  Miro::SonyCameraControl::AutoExposureMode
  SonyCameraControlImpl::getAutoExposure()
		throw()
  {
    return currentAutoExposure_;
  }

  void
  SonyCameraControlImpl::setIris(Miro::SonyCameraControl::IrisMode iris)
		throw(Miro::EOutOfBounds)
  {
    unsigned char dev;
    switch (iris) {
      case Miro::SonyCameraControl::IRIS_CLOSE: dev = 0x00; break;
      case Miro::SonyCameraControl::IRIS_F28:   dev = 0x01; break;
      case Miro::SonyCameraControl::IRIS_F22:   dev = 0x02; break;
      case Miro::SonyCameraControl::IRIS_F19:   dev = 0x03; break;
      case Miro::SonyCameraControl::IRIS_F16:   dev = 0x04; break;
      case Miro::SonyCameraControl::IRIS_F14:   dev = 0x05; break;
      case Miro::SonyCameraControl::IRIS_F11:   dev = 0x06; break;
      case Miro::SonyCameraControl::IRIS_F9_6:  dev = 0x07; break;
      case Miro::SonyCameraControl::IRIS_F8:    dev = 0x08; break;
      case Miro::SonyCameraControl::IRIS_F6_8:  dev = 0x09; break;
      case Miro::SonyCameraControl::IRIS_F5_6:  dev = 0x0a; break;
      case Miro::SonyCameraControl::IRIS_F4_8:  dev = 0x0b; break;
      case Miro::SonyCameraControl::IRIS_F4:    dev = 0x0c; break;
      case Miro::SonyCameraControl::IRIS_F3_4:  dev = 0x0d; break;
      case Miro::SonyCameraControl::IRIS_F2_8:  dev = 0x0e; break;
      case Miro::SonyCameraControl::IRIS_F2_4:  dev = 0x0f; break;
      case Miro::SonyCameraControl::IRIS_F2:    dev = 0x10; break;
      case Miro::SonyCameraControl::IRIS_F1_8:  dev = 0x11; break;
      default:
	throw Miro::EOutOfBounds();
    }

    switch (currentAutoExposure_) {
      case Miro::SonyCameraControl::AE_MANUAL:
      case Miro::SonyCameraControl::AE_FIXED_SHUTTER:
	setAutoExposure(Miro::SonyCameraControl::AE_MANUAL);
	break;
      default:
	setAutoExposure(Miro::SonyCameraControl::AE_FIXED_IRIS);
    }

    currentIris_ = iris;
    Sony::Message msg;
    msg.iris(dev);
    connection.sendCamera(msg);
  }

  Miro::SonyCameraControl::IrisMode
  SonyCameraControlImpl::getIris()
		throw()
  {
    return currentIris_;
  }

  CORBA::Short
  SonyCameraControlImpl::setGain(CORBA::Short gain)
		throw(Miro::EOutOfBounds)
  {
    unsigned char dev;
    if (gain < -3) {
      throw Miro::EOutOfBounds();
    } else if (gain < -1.5) {
      gain = -3; dev = 0x00;
    } else if (gain < 1.5) {
      gain =  0; dev = 0x01;
    } else if (gain < 4.5) {
      gain =  3; dev = 0x02;
    } else if (gain < 7.5) {
      gain =  6; dev = 0x03;
    } else if (gain < 10.5) {
      gain =  9; dev = 0x04;
    } else if (gain < 13.5) {
      gain = 12; dev = 0x05;
    } else if (gain < 16.5) {
      gain = 15; dev = 0x06;
    } else if (gain <= 18) {
      gain = 18; dev = 0x07;
    } else
      throw Miro::EOutOfBounds();

    setAutoExposure(Miro::SonyCameraControl::AE_MANUAL);
    currentGain_ = gain;
    Sony::Message msg;
    msg.gain(dev);
    connection.sendCamera(msg);
    return currentGain_;
  }

  CORBA::Short
  SonyCameraControlImpl::getGain()
		throw()
  {
    return currentGain_;
  }

  Miro::GainRangeIDL
  SonyCameraControlImpl::getGainRange()
		throw()
  {
    Miro::GainRangeIDL range;
    range.min = -3;
    range.max = 18;
    return range;
  }

  //-------------------------------------------------------------------------
  // helper functions:
  //-------------------------------------------------------------------------
  unsigned short
  SonyCameraControlImpl::zoom2dev(float zoom)
  {
    // The relation between zoom angle and protocol value isn't linear, it
    // has to be approximated by a hyperbolic function dev = a/(zoom+b)+c.
    // The coefficients a, b and c are tuned so that zoom2dev returns 0
    // for zoom_max, 1023 for zoom_min and 512 for 3/8*(zoom_max+zoom_min),
    // which makes a pretty good approximation.
    short dev = (unsigned short) (zoom_a / (zoom + zoom_b) + zoom_c);
    if (dev < 0)
      return 0;
    else if (dev > zoom_dev_max)
      return zoom_dev_max;
    else
      return dev;
  }

  unsigned short
  SonyCameraControlImpl::focus2dev(float focus)
  {
    assert (focusParameters_.rangeMin < focusParameters_.rangeMax);
    // The valid focus range and the protocol value mapping depend on the
    // current focal length (zoom) setting. This is all pretty ugly, so
    // you'd better leave focusing to the camera whenever possible.

    // current minimum/maximum CCD <-> lens distance
    float d_min = currentFocalLength_;
    float d_max = focusParameters_.rangeMin * minFocalLength
		/ (focusParameters_.rangeMin - minFocalLength)
		- minFocalLength + currentFocalLength_;
    // target CCD <-> lens distance
    float d = focus * currentFocalLength_ / (focus - currentFocalLength_);
    return focus_inf +
	    (unsigned short)( (d - d_min) / (d_max - d_min)
			      * (focus_near - focus_inf) );
  }

  // The shutter frequency tables for PAL (Sony EVI-D31) and
  // NTSC (Sony EVI-D30) are slightly different (0x00 and 0x05).
  static int shutter_table[][2] = {
    //  PAL,  NTSC
    {    50,    60 }, // 0x00
    {    60,    60 }, // 0x01
    {    75,    75 }, // 0x02
    {    90,    90 }, // 0x03
    {   100,   100 }, // 0x04
    {   120,   125 }, // 0x05
    {   150,   150 }, // 0x06
    {   180,   180 }, // 0x07
    {   215,   215 }, // 0x08
    {   250,   250 }, // 0x09
    {   300,   300 }, // 0x0a
    {   350,   350 }, // 0x0b
    {   425,   425 }, // 0x0c
    {   500,   500 }, // 0x0d
    {   600,   600 }, // 0x0e
    {   725,   725 }, // 0x0f
    {   850,   850 }, // 0x10
    {  1000,  1000 }, // 0x11
    {  1250,  1250 }, // 0x12
    {  1500,  1500 }, // 0x13
    {  1750,  1750 }, // 0x14
    {  2000,  2000 }, // 0x15
    {  2500,  2500 }, // 0x16
    {  3000,  3000 }, // 0x17
    {  3500,  3500 }, // 0x18
    {  4000,  4000 }, // 0x19
    {  6000,  6000 }, // 0x1a
    { 10000, 10000 }  // 0x1b
  };

  unsigned char
  SonyCameraControlImpl::shutter2dev(Miro::TimeIDL& shutter)
  {
    char dev;
    if (shutter.sec == 0) {
      dev = sizeof(shutter_table) / (2 * sizeof(int)) - 1;
      while (dev > 0) {
	if (shutter.usec <= ceil(1e6 / shutter_table[(int)dev][pal_ntsc]))
	  break;
	dev--;
      }
    } else {
      // shutter.sec>0 -> use the slowest possible setting.
      dev = 0x00;
    }
    shutter.sec = 0;
    shutter.usec = (int)(1e6 / shutter_table[(int)dev][pal_ntsc]);
    return dev;
  }
}
