// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoControlDialogImp_h
#define VideoControlDialogImp_h

#include "VideoControlDialog.h"

#include "miro/Client.h"
#include "idl/VideoControlC.h"

#include <vector>

class VideoControlDialogImp : public VideoControlDialog  
{
    Q_OBJECT
  
public: 
    VideoControlDialogImp(Miro::Client & _client,
                          QWidget * parent = 0,
		          const char * name = 0);
    ~VideoControlDialogImp();

public slots:
    virtual void ValueChangedBrightness(int value);
    virtual void ValueChangedExposure(int value);
    virtual void ValueChangedFocus(int value);
    virtual void ValueChangedGain(int value);
    virtual void ValueChangedGamma(int value);
    virtual void ValueChangedHue(int value);
    virtual void ValueChangedIris(int value);
    virtual void ValueChangedSaturation(int value);
    virtual void ValueChangedSharpness(int value);
    virtual void ValueChangedShutter(int value);
    virtual void ValueChangedTemperature(int value);
    virtual void ValueChangedTrigger(int value);
    virtual void ValueChangedWhiteBalanceBlue(int value);
    virtual void ValueChangedWhiteBalanceRed(int value);
    virtual void TextChangedBrightness(const QString&);
    virtual void TextChangedExposure(const QString&);
    virtual void TextChangedFocus(const QString&);
    virtual void TextChangedGain(const QString&);
    virtual void TextChangedGamma(const QString&);
    virtual void TextChangedHue(const QString&);
    virtual void TextChangedIris(const QString&);
    virtual void TextChangedSaturation(const QString&);
    virtual void TextChangedSharpness(const QString&);
    virtual void TextChangedShutter(const QString&);
    virtual void TextChangedTemperature(const QString&);
    virtual void TextChangedTrigger(const QString&);
    virtual void TextChangedWhiteBalanceBlue(const QString&);
    virtual void TextChangedWhiteBalanceRed(const QString&);
    virtual void AutoChangedBrightness(int);
    virtual void AutoChangedExposure(int);
    virtual void AutoChangedFocus(int);
    virtual void AutoChangedGain(int);
    virtual void AutoChangedGamma(int);
    virtual void AutoChangedHue(int);
    virtual void AutoChangedIris(int);
    virtual void AutoChangedSaturation(int);
    virtual void AutoChangedSharpness(int);
    virtual void AutoChangedShutter(int);
    virtual void AutoChangedTemperature(int);
    virtual void AutoChangedTrigger(int);
    virtual void AutoChangedWhiteBalanceBlue(int);
    virtual void AutoChangedWhiteBalanceRed(int);

protected:
    //------------------------------------------------------------------------
    class FeatureTable
    {
    private:
        std::string name_;
        short       min_value_;
	short       max_value_;
        short     * value_;
        QLineEdit * line_edit_;
	QSlider   * slider_;
	QCheckBox * check_box_;

    public:	
	FeatureTable(const std::string & name, 
	             short min_value, short max_value, short * value,
		     QLineEdit * line_edit, QSlider * slider, QCheckBox * check_box);
	
	const std::string & name() const { return name_; }
	short min_value() const { return min_value_; }
	short max_value() const { return max_value_; }
	short * value() const { return value_; }
	QLineEdit * line_edit() const { return line_edit_; }
	QSlider * slider() const { return slider_; }
	QCheckBox * check_box() const { return check_box_; }
    };

    void readFeatures();
    void writeFeatures();
    void updateDialog();
    
    FeatureTable * findFeature(const std::string & name);

    bool setState(FeatureTable * feature, int state);
    bool setValue(FeatureTable * feature, short value);
    bool updateValue(const std::string & name, short value);
    bool updateText(const std::string & name, const QString & text);
    bool updateState(const std::string & name, int state);

private:
    Miro::Client              client_;
    Video::Control_var        video_control_;
    Video::FeatureSet         features_;
    std::vector<FeatureTable> feature_table_;
    bool                      in_update_;
};

#endif /* VideoControlDialogImp_h */

