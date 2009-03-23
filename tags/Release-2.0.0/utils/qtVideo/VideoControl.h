// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//

#ifndef VideoControl_h
#define VideoControl_h

#include "miro/Client.h"
#include "idl/VideoControlC.h"

#include <qdialog.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qlabel.h>

#include <vector>
#include <string>


class SliderFloat : public QSlider
{
  Q_OBJECT

  typedef QSlider Super;

 public:
  SliderFloat(double _min, double _max, QWidget * parent, const char * name = 0);

  virtual void setValue(double _value);
  virtual double value();

 signals:
  void valueChangedFloat(double);

 protected:
  virtual void valueChange();

 private:
  double min_;
  double max_;
  static const int ticks_ = 1000; // discretize to this number of values
};



class VideoControl : public QDialog
{
  Q_OBJECT
  
 public: 
  VideoControl(Miro::Client & _client,
	       QWidget * parent = 0,
	       const char * name = 0);
  ~VideoControl();

 public slots:
  void autoChange(int);
  void valueChange(double);

 protected:
  std::string feature2Name(Video::CameraFeature _feature);
  Video::CameraFeature name2Feature(std::string _name);
  int feature2Pos(Video::CameraFeature _feature);

 private:
  typedef struct FeatureState {
    Video::CameraFeature feature;
    bool autoMode;
    double value;
    QSlider * slider;
    QLineEdit * lineEdit;
  };

  FeatureState * states_;

  Video::CameraControl_var  video_control_;
};

#endif

