#include "XsensInertialSensorConnection.h"

#include <string>
#include <iostream>
#include <stdio.h>
//#include <ace/OS.h>

#include "miro/TimeHelper.h"
//#include "idl/TimeC.h"

namespace Inertial
{

const string XsensInertialSensorConnection::OUTPUT_MODE_CALIB_ORIENT = "CALIB-ORIENT";
const string XsensInertialSensorConnection::OUTPUT_MODE_ORIENT = "ORIENT";
const string XsensInertialSensorConnection::OUTPUT_MODE_CALIB = "CALIB"; 
const string XsensInertialSensorConnection::OUTPUT_SETTINGS_ACC_EULER = "ACC-EULER";
const string XsensInertialSensorConnection::OUTPUT_SETTINGS_ACC_GYR_EULER = "ACC-GYR-EULER";
const string XsensInertialSensorConnection::OUTPUT_SETTINGS_EULER = "EULER";
const string XsensInertialSensorConnection::OUTPUT_SETTINGS_CALIBRATED = "CALIBRATED";
const string XsensInertialSensorConnection::OUTPUT_SETTINGS_ACC = "ACC";


  // *******************************************************************************************************************
  /** Concret Implementation of the InertialSensorreader
  *
  */

  Inertial::XsensInertialSensorConnection::XsensInertialSensorConnection( ACE_Reactor & _reactor,
       InertialSensorImpl & _i_impl ) :
       reactor_( &_reactor),
       inertial_event_( new EventHandler( inertial_poll_task_ ) ),
       inertial_poll_task_( new InertialSensorPollTask( *this, *inertial_event_, _i_impl ) ),
       parameters_(*InertialParameters::instance ( ) ),
       syncInertialSensorScan ( ),
       syncInertialSensorScanCond ( syncInertialSensorScan )
  {

    std::cout << "Init Hardware! " << std::endl;
    std::cout << "outputmode " << parameters_.outputmode << " settings " << parameters_.outputsettings << std::endl;
    deviceName_     = parameters_.device;
    if ( parameters_.outputmode == OUTPUT_MODE_ORIENT )
      outputMode_   = OUTPUTMODE_ORIENT;
    else if (parameters_.outputmode == OUTPUT_MODE_CALIB_ORIENT) {
    	outputMode_ = OUTPUTMODE_CALIB_ORIENT;
    } else if (parameters_.outputmode == OUTPUT_MODE_CALIB) {
    	outputMode_ = OUTPUTMODE_CALIB;
    } else {
      outputMode_   = OUTPUTMODE_ORIENT;
    }

    if ( parameters_.outputsettings == OUTPUT_SETTINGS_EULER )
      outputSettings_ = OUTPUTSETTINGS_EULER;
    else if (parameters_.outputsettings == OUTPUT_SETTINGS_ACC_EULER) {
    	outputSettings_ = OUTPUTSETTINGS_ACC_EULER;
    } else if (parameters_.outputsettings == OUTPUT_SETTINGS_ACC_GYR_EULER) {
    	outputSettings_ = OUTPUTSETTINGS_ACC_GYR_EULER;
    } else if (parameters_.outputsettings == OUTPUT_SETTINGS_CALIBRATED) {
    	outputSettings_ = OUTPUTSETTINGS_CALIBRATED;
    } else if (parameters_.outputsettings == OUTPUT_SETTINGS_ACC) {
    	outputSettings_ = OUTPUTSETTINGS_ACC;
    } else {
      outputSettings_ = OUTPUTSETTINGS_EULER;
    }

    // Open and initialize serial port
    if (xbus.openPort(deviceName_.c_str()) != XBRV_OK)
    {
      printf("Cannot open COM port %s\n", deviceName_.c_str());
      std::cout << "no device" << std::endl;
      //  return XBRV_INPUTCANNOTBEOPENED;
    }

    if ( setSensorMode() == false )
      std::cout << "schass" << std::endl;
      //return XBRV_UNEXPECTEDMSG;
    std::cout << "Init Hardware klappt! " << std::endl;

    inertial_poll_task_->open( 0 );
  }

  Inertial::XsensInertialSensorConnection::~XsensInertialSensorConnection( )
  {
    xbus.close();
  }

  bool Inertial::XsensInertialSensorConnection::setSensorMode( )
  {
    // Put MTi/MTx in Config State
    if(xbus.writeMessage(MID_GOTOCONFIG) != XBRV_OK)
    {
      printf("No device connected\n");
      return false;
    }

    // Request DID
    unsigned long value = 0xFFFFFFFF;
    xbus.reqSetting(MID_REQDID, value);
    if ((value & DID_TYPEH_MASK) != DID_TYPEH_MTI_MTX)
    {
      printf("MTi / MTx has not been detected\n");
      return false;
    }

    if (xbus.setDeviceMode(outputMode_, outputSettings_) != XBRV_OK)
    {
      printf("Could not set device mode\n");
      return false;
    }
    
    short answer = xbus.writeMessage(MID_SETPERIOD, PERIOD_10HZ);
    std::cout << "wrote: " << answer << std::endl;

//    //short skip_factor = xbus.writeMessage(MID_REQOUTPUTSKIPFACTOR);
//    unsigned long value2 = 0xFFFF;
//    cout << "before req outputskipfactor " << value2 << endl;
//    xbus.reqSetting(MID_REQOUTPUTSKIPFACTOR, value2);
//    cout << "req outputskipfactor " <<  value2 << endl;
//    xbus.reqSetting(MID_REQPERIOD, value2);
//    cout << "req period " <<  value2 << endl;
//    xbus.reqSetting(MID_REQBAUDRATE, value2);
//    cout << "req baudrate " << value2 << endl;
//    if (xbus.writeMessage(MID_SETBAUDRATE, BAUDRATE_921K6) != XBRV_OK) {
//    	printf("Could not set baudrate\n");
//    }
  
    
    // Put MTi/MTx in Measurement State
    xbus.writeMessage(MID_GOTOMEASUREMENT);

    //reset heading   
//     short result = xbus.writeMessage(MID_RESETORIENTATION, RESETORIENTATION_HEADING, 0x02);
//     if (result != XBRV_OK) {
//    	 std::cout <<"Could not reset heading " << result << std::endl;
//    	 std::cout <<"error " << xbus.getLastDeviceError() << std::endl;
//     } else
//    	 std::cout << "orientation reset worked" << endl;
        
    
    return true;
  }

  bool Inertial::XsensInertialSensorConnection::readData( Miro::InertialSensorEventIDL &inertial_data )
  {
    short datalen;
    //while (xbus.readDataMessage(data, datalen) == XBRV_OK)
    if (xbus.readDataMessage(data, datalen) == XBRV_OK)
    {
    //    std::string huhu = asctime(localtime(0));
    //    time_t new_time = time(0);//Function from windows.h that allows it to sleep, this will be 5 seconds
    //    long current = long(new_time - old_time);  //get current seconds since run time
    //    std::cout << huhu << std::endl;

      //xbus.getValue(VALUE_SAMPLECNT, samplecounter, data, BID_MASTER);
      if ( !xbus.writeMessage( MID_RESETORIENTATION ) )
         std::cout << "okay: reset " << std::endl;
      float grad_2_rad = 0.017453293;
      if (outputSettings_ == OUTPUTSETTINGS_EULER) {
      	xbus.getValue(VALUE_ORIENT_EULER, fdata, data);

      	inertial_data.inertial_data.roll  = fdata[0] * grad_2_rad;
      	inertial_data.inertial_data.pitch = fdata[1] * grad_2_rad;
      	inertial_data.inertial_data.yaw   = fdata[2] * grad_2_rad;
      } else if (outputSettings_ == OUTPUTSETTINGS_ACC_EULER) {
      	xbus.getValue(VALUE_CALIB_ACC_ORIENT_EULER, fdata, data);
      	inertial_data.inertial_data.accX = fdata[0];
      	inertial_data.inertial_data.accY = fdata[1];
      	inertial_data.inertial_data.accZ = fdata[2];
      	inertial_data.inertial_data.roll = fdata[3] * grad_2_rad;
      	inertial_data.inertial_data.pitch = fdata[4] * grad_2_rad;
      	inertial_data.inertial_data.yaw = fdata[5] * grad_2_rad;
      } else if (outputSettings_ == OUTPUTSETTINGS_ACC_GYR_EULER) {
      	xbus.getValue(VALUE_CALIB_ACC_GYR_ORIENT_EULER, fdata, data);
      	inertial_data.inertial_data.accX = fdata[0];
      	inertial_data.inertial_data.accY = fdata[1];
      	inertial_data.inertial_data.accZ = fdata[2];
      	inertial_data.inertial_data.gyrX = fdata[3];
      	inertial_data.inertial_data.gyrY = fdata[4];
      	inertial_data.inertial_data.gyrZ = fdata[5];
      	inertial_data.inertial_data.roll = fdata[6] * grad_2_rad;
      	inertial_data.inertial_data.pitch = fdata[7] * grad_2_rad;
      	inertial_data.inertial_data.yaw = fdata[8] * grad_2_rad;
      }	else if (outputSettings_ == OUTPUTSETTINGS_ACC) {
      	inertial_data.inertial_data.accX = fdata[0];
      	inertial_data.inertial_data.accY = fdata[1];
      	inertial_data.inertial_data.accZ = fdata[2];
      } else if (outputSettings_ == OUTPUTSETTINGS_CALIBRATED) {
      	inertial_data.inertial_data.accX = fdata[0];
      	inertial_data.inertial_data.accY = fdata[1];
      	inertial_data.inertial_data.accZ = fdata[2];
      } else
      	std::cout << "output settings not supported" << std::endl;
      
      ACE_Time_Value now = ACE_OS::gettimeofday( );
      timeA2C( now, inertial_data.time );
      // DEBUG
      //std::cout << inertial_data.inertial_data.yaw << std::endl;
      return true;
    //    time ( &rawtime );
    //    ptm = gmtime ( &rawtime );
    //    strftime( readtime, 10, "%H %M", ptm );
    }
    else
      return false;

  }

}
