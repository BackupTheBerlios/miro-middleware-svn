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
#include "VideoControl.h"

#include <qlayout.h>
#include <qwidget.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;


struct FeatureName
{
  Video::CameraFeature feature;
  const char * name;
};

static FeatureName featureName[] = {
  { Video::BRIGHTNESS,              "Brightness" },
  { Video::EXPOSURE,                "Exposure" },
  { Video::FOCUS,                   "Focus" },
  { Video::GAIN,                    "Gain" },
  { Video::GAMMA,                   "Gamma" },
  { Video::HUE,                     "Hue" },
  { Video::IRIS,                    "Iris" },
  { Video::SATURATION,              "Saturation" },
  { Video::SHARPNESS,               "Sharpness" },
  { Video::SHUTTER,                 "Shutter" },
  { Video::TEMPERATURE,             "Temperature" },
  { Video::TRIGGER,                 "Trigger" },
  { Video::WHITE_BALANCE_BLUE,      "White Balance Blue" },
  { Video::WHITE_BALANCE_RED,       "White Balance Red" },
  { Video::WHITE_BALANCE_SPEED,     "White Balance Speed" },
  { Video::WHITE_BALANCE_DELAY,     "White Balance Delay" },
  { Video::CONTRAST,                "Contrast" },
  { Video::TIMEOUT,                 "Timeout" },
  { Video::STROBE_DURATION,         "Strobe Duration" },
  { Video::STROBE_DELAY,            "Strobe Delay" },
  { Video::BACKLIGHT_COMPENSATION,  "Backlight Compensation" },
  { Video::FLICKERLESS_MODE,        "Flickerless Mode" },
  { Video::DYNAMIC_NOISE_REDUCTION, "Dynamic Noise Reduction" },
  { Video::COMPRESSION,             "Compression" }
};


string
VideoControl::feature2Name(Video::CameraFeature _feature)
{
  for (unsigned int i=0; i<sizeof(featureName)/sizeof(featureName[0]); ++i)
    if (_feature == featureName[i].feature)
      return string(featureName[i].name);
  return string(); // suppress compile warning
}


Video::CameraFeature
VideoControl::name2Feature(string _name)
{
  for (unsigned int i=0; i<sizeof(featureName)/sizeof(featureName[0]); ++i)
    if (_name == string(featureName[i].name))
      return featureName[i].feature;
  return Video::BRIGHTNESS; // suppress compile warning
}


int
VideoControl::feature2Pos(Video::CameraFeature _feature)
{
  for (unsigned int i=0; i<sizeof(featureName)/sizeof(featureName[0]); ++i)
    if (_feature == states_[i].feature)
      return i;
  return 0; // suppress compile warning
}

	
VideoControl::VideoControl(Miro::Client & _client,
			   QWidget * _parent,
			   const char * _name)
  :  QDialog(_parent, _name)
{
  if ( !_name )
    setName( "QtVideoControl" );

  setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
  QGridLayout * layout2 = new QGridLayout( this, 1, 1, 11, 6, "layout2"); 

  video_control_ = _client.resolveName<Video::CameraControl>(_name);
  Video::FeatureSetVector_var features;
  video_control_->getFeatureDescription(features);

  states_ = new FeatureState[features->length()];

  // create control widgets
  // connect them also
  QGridLayout * layout = new QGridLayout( 0, features->length(), 1, 0, 6, "layout");
  layout->setColStretch(1, 1);
  for (unsigned int i=0; i<features->length(); ++i) {
    
    if (features[i].hasAutoMode) {
      QCheckBox * checkBox = new QCheckBox( this, feature2Name(features[i].feature).c_str() );
      checkBox->setText("auto");
      layout->addWidget( checkBox, i, 0 );
      connect( checkBox, SIGNAL( stateChanged(int) ), this, SLOT( autoChange(int) ) );
    } else {
      QSpacerItem * spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
      layout->addItem( spacer, i, 0 );
    }

    SliderFloat * slider = new SliderFloat(features[i].minValue, features[i].maxValue, this, feature2Name(features[i].feature).c_str() );
    slider->setOrientation( QSlider::Horizontal );
    slider->setValue(features[i].value);
    layout->addWidget( slider, i, 1 );
    connect( slider, SIGNAL( valueChangedFloat(double) ), this, SLOT( valueChange(double) ) );

    QLineEdit * lineEdit = new QLineEdit( this, feature2Name(features[i].feature).c_str() );
    std::ostringstream ostr;
    ostr << features[i].value;
    lineEdit->setText(ostr.str().c_str());
    lineEdit->setReadOnly(true);
    layout->addWidget( lineEdit, i, 2 );

    QLabel * textLabel = new QLabel( this, feature2Name(features[i].feature).c_str() );
    textLabel->setText(feature2Name(features[i].feature).c_str());
    layout->addWidget( textLabel, i, 3 );

    states_[i].feature = features[i].feature;
    states_[i].autoMode = false;
    states_[i].value = features[i].value;
    states_[i].slider = slider;
    states_[i].lineEdit = lineEdit;
  }

  layout2->addLayout( layout, 0, 0 );

  resize( QSize(500, 78).expandedTo(minimumSizeHint()) );
  clearWState( WState_Polished );
}


VideoControl::~VideoControl()
{
  // no need to delete child widgets, Qt does it all for us
}


void
VideoControl::autoChange(int _state)
{
  Video::CameraFeature feature = name2Feature(sender()->name());

  Video::FeatureSet set;
  if (_state == QButton::On) {
    set.autoMode = true;
    states_[feature2Pos(feature)].slider->setEnabled(false);
    states_[feature2Pos(feature)].lineEdit->setEnabled(false);
  } else {
    set.autoMode = false;
    states_[feature2Pos(feature)].slider->setEnabled(true);
    states_[feature2Pos(feature)].lineEdit->setEnabled(true);
  }
  set.value = states_[feature2Pos(feature)].slider->value();
  video_control_->setFeature(feature, set);
}


void
VideoControl::valueChange(double _value)
{
  Video::CameraFeature feature = name2Feature(sender()->name());

  std::ostringstream ostr;
  ostr << _value;
  states_[feature2Pos(feature)].lineEdit->setText(ostr.str().c_str());

  Video::FeatureSet set;
  set.autoMode = false;
  set.value = _value;
  video_control_->setFeature(feature, set);
}




// ///////////////////////////


SliderFloat::SliderFloat(double _min, double _max, QWidget * parent, const char * name) :
  Super(parent, name),
  min_(_min),
  max_(_max)
{
  setMinValue(0);
  setMaxValue(ticks_);
}


void
SliderFloat::setValue(double _value)
{
  Super::setValue((int)((_value/(max_-min_)) * ticks_));
}


double
SliderFloat::value()
{
  return ((double)Super::value()/(double)ticks_) * (max_-min_) + min_;
}


void
SliderFloat::valueChange()
{
  emit valueChangedFloat(((double)Super::value()/(double)ticks_) * (max_-min_) + min_);
}
