// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "VideoControlDialogImp.h"
#include <qslider.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <qcheckbox.h>
#include <algorithm>

//----------------------------------------------------------------------------
VideoControlDialogImp::FeatureTable::FeatureTable(const std::string & name,
                                                  short min_value,
                                                  short max_value,
						  short * value,
						  QLineEdit * line_edit,
						  QSlider * slider,
						  QCheckBox * check_box)
 :  name_(name),
    min_value_(min_value), max_value_(max_value),
    value_(value), line_edit_(line_edit), slider_(slider), check_box_(check_box)
{
    slider_->setRange(min_value, max_value);
}
	
//----------------------------------------------------------------------------
VideoControlDialogImp::VideoControlDialogImp(Miro::Client & client,
                                             QWidget * parent,
					     const char * name)
 :  VideoControlDialog(parent, name),
    client_(client),
    in_update_(false)
{
    feature_table_.push_back(FeatureTable("brightness", 128, 383, &features_.brightness, LCD_Brightness, Slider_Brightness, CB_Brightness));
    feature_table_.push_back(FeatureTable("exposure", 0, 511, &features_.exposure, LCD_Exposure, Slider_Exposure, CB_Exposure));
    feature_table_.push_back(FeatureTable("iris", 0, 100, &features_.iris, LCD_Iris, Slider_Iris, CB_Iris));
    feature_table_.push_back(FeatureTable("shutter", 0, 7, &features_.shutter, LCD_Shutter, Slider_Shutter, CB_Shutter));
    feature_table_.push_back(FeatureTable("gain", 0, 255, &features_.gain, LCD_Gain, Slider_Gain, CB_Gain));
    feature_table_.push_back(FeatureTable("hue", 0, 100, &features_.hue, LCD_Hue, Slider_Hue, CB_Hue));
    feature_table_.push_back(FeatureTable("saturation", 0, 255, &features_.saturation, LCD_Saturation, Slider_Saturation, CB_Saturation));
    feature_table_.push_back(FeatureTable("balance-blue", 0, 255, &features_.white_balance.first, LCD_WhiteBalanceBlue, Slider_WhiteBalanceBlue, CB_WhiteBalanceBlue));
    feature_table_.push_back(FeatureTable("balance-red", 0, 255, &features_.white_balance.second, LCD_WhiteBalanceRed, Slider_WhiteBalanceRed, CB_WhiteBalanceRed));
    feature_table_.push_back(FeatureTable("gamma", 0, 1, &features_.gamma, LCD_Gamma, Slider_Gamma, CB_Gamma));
    feature_table_.push_back(FeatureTable("temperature", 0, 100, &features_.temperature, LCD_Temperature, Slider_Temperature, CB_Temperature));
    feature_table_.push_back(FeatureTable("sharpness", 0, 255, &features_.sharpness, LCD_Sharpness, Slider_Sharpness, CB_Sharpness));
    feature_table_.push_back(FeatureTable("trigger", 0, 100, &features_.trigger, LCD_Trigger, Slider_Trigger, CB_Trigger));
    feature_table_.push_back(FeatureTable("focus", 0, 100, &features_.focus, LCD_Focus, Slider_Focus, CB_Focus));

    for (size_t i = 0; i < feature_table_.size(); i++)
    {
        feature_table_[i].line_edit()->setValidator(
		new QIntValidator(
			feature_table_[i].min_value(),
			feature_table_[i].max_value(),
			this
			)
		);
    }

    video_control_ = client.resolveName<Video::Control>("VideoControl");
    readFeatures();
    updateDialog();
}

//----------------------------------------------------------------------------
VideoControlDialogImp::~VideoControlDialogImp()
{
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::readFeatures()
{
    video_control_->getFeatures(features_);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::writeFeatures()
{
    if (!in_update_)
        video_control_->setFeatures(features_);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::updateDialog()
{
    if (!in_update_)
    {
        in_update_ = true;
        for (size_t i = 0; i < feature_table_.size(); i++)
	{
			short value = std::max(
							std::min(*feature_table_[i].value(),
								     feature_table_[i].max_value()),
							feature_table_[i].min_value());
             feature_table_[i].slider()->setValue(value);
	    setValue(&feature_table_[i], value);
	    feature_table_[i].check_box()->setChecked(*feature_table_[i].value() < 0);
	    setState(&feature_table_[i], *feature_table_[i].value() < 0);
	}
	in_update_ = false;
    }
}

//----------------------------------------------------------------------------
VideoControlDialogImp::FeatureTable *
VideoControlDialogImp::findFeature(const std::string & name)
{
    for (size_t i = 0; i < feature_table_.size(); i++)
    {
        if (feature_table_[i].name() == name)
	    return &(feature_table_[i]);
    }
    return 0;
}

//----------------------------------------------------------------------------
bool VideoControlDialogImp::setState(VideoControlDialogImp::FeatureTable * feature, int state)
{
	if (feature)
	{
		short new_value;
		if (state)
		{
			feature->line_edit()->setEnabled(false);
			feature->slider()->setEnabled(false);
			
			new_value = -1;
		}
		else
		{
			feature->line_edit()->setEnabled(true);
			feature->slider()->setEnabled(true);
			
			new_value = (short)feature->slider()->value();
		}
		if (!in_update_)
		{
			*(feature->value()) = new_value;
	         writeFeatures();
		}
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
bool
VideoControlDialogImp::setValue(VideoControlDialogImp::FeatureTable * feature,
                                short value)
{
    if (feature)
    {
        QString num;
        feature->line_edit()->setText(num.setNum(value));
	return true;
    }
    return false;
}

//----------------------------------------------------------------------------
bool VideoControlDialogImp::updateValue(const std::string & name, short value)
{
//    std::cout << "V: " << name.c_str() << " := " << value << std::endl;

    if (!in_update_)
    {
        FeatureTable * feature = findFeature(name);
        if (feature)
        {
            *(feature->value()) = value;

            in_update_ = true;
	    QString num;
            feature->line_edit()->setText(num.setNum(value));
	    in_update_ = false;

            writeFeatures();
	    return true;
        }
    
        qWarning(QString("VideoControlDialogImp::updateValue(") + QString(name.c_str()) + QString("): not found"));
    }
    return false;
}

//----------------------------------------------------------------------------
bool VideoControlDialogImp::updateText(const std::string & name, const QString & text)
{
    short value = text.toShort();
//    std::cout << "T: " << name.c_str() << " := " << value << std::endl;

    if (!in_update_)
    {
        FeatureTable * feature = findFeature(name);
        if (feature)
        {
	    if (value < feature->min_value()) value = feature->min_value();
	    if (value > feature->max_value()) value = feature->max_value();
	
            *(feature->value()) = value;

            in_update_ = true;
            feature->slider()->setValue(value);
	    in_update_ = false;

            writeFeatures();
	    return true;
        }
    
        qWarning(QString("VideoControlDialogImp::updateText(") + QString(name.c_str()) + QString("): not found"));
    }
    return false;
}

//----------------------------------------------------------------------------
bool VideoControlDialogImp::updateState(const std::string & name, int state)
{
    if (!in_update_)
    {
		FeatureTable * feature = findFeature(name);
		if (feature)
		{
			setState(feature, state);
			return true;
		}

        qWarning(QString("VideoControlDialogImp::updateState(") + QString(name.c_str()) + QString("): not found"));
	}
	return false;
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedBrightness(int value)
{
    updateValue("brightness", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedExposure(int value)
{
    updateValue("exposure", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedFocus(int value)
{
    updateValue("focus", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedGain(int value)
{
    updateValue("gain", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedGamma(int value)
{
    updateValue("gamma", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedHue(int value)
{
    updateValue("hue", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedIris(int value)
{
    updateValue("iris", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedSaturation(int value)
{
    updateValue("saturation", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedSharpness(int value)
{
    updateValue("sharpness", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedShutter(int value)
{
    updateValue("shutter", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedTemperature(int value)
{
    updateValue("temperature", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedTrigger(int value)
{
    updateValue("trigger", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedWhiteBalanceBlue(int value)
{
    updateValue("balance-blue", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::ValueChangedWhiteBalanceRed(int value)
{
    updateValue("balance-red", value);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedBrightness(const QString & text)
{
    updateText("brightness", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedExposure(const QString & text)
{
    updateText("exposure", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedFocus(const QString & text)
{
    updateText("focus", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedGain(const QString & text)
{
    updateText("gain", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedGamma(const QString & text)
{
    updateText("gamma", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedHue(const QString & text)
{
    updateText("hue", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedIris(const QString & text)
{
    updateText("iris", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedSaturation(const QString & text)
{
    updateText("saturation", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedSharpness(const QString & text)
{
    updateText("sharpness", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedShutter(const QString & text)
{
    updateText("shutter", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedTemperature(const QString & text)
{
    updateText("temperature", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedTrigger(const QString & text)
{
    updateText("trigger", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedWhiteBalanceBlue(const QString & text)
{
    updateText("balance-blue", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::TextChangedWhiteBalanceRed(const QString & text)
{
    updateText("balance-red", text);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedBrightness(int state)
{
    updateState("brightness", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedExposure(int state)
{
    updateState("exposure", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedFocus(int state)
{
    updateState("focus", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedGain(int state)
{
    updateState("gain", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedGamma(int state)
{
    updateState("gamma", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedHue(int state)
{
    updateState("hue", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedIris(int state)
{
    updateState("iris", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedSaturation(int state)
{
    updateState("saturation", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedSharpness(int state)
{
    updateState("sharpness", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedShutter(int state)
{
    updateState("shutter", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedTemperature(int state)
{
    updateState("temperature", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedTrigger(int state)
{
    updateState("trigger", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedWhiteBalanceBlue(int state)
{
    updateState("balance-blue", state);
}

//----------------------------------------------------------------------------
void VideoControlDialogImp::AutoChangedWhiteBalanceRed(int state)
{
    updateState("balance-red", state);
}


