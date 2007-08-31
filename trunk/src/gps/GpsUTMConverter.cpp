/**
 * @file
 * @author Bechter Stephan <stephan.bechter@sbox.tugraz.at>
 * Converting GPS geodetic coordinates (Longitude and Latitude)
 * to UTM Coordinates using the WGS84 as standard ellipsoid.
 *
 * This UTM Coordinates are in meters east and north
 * @author Bechter Stephan <stephan.bechter@student.tugraz.at>
 **/

#include "GpsUTMConverter.h"
#include <iostream>
namespace GPS
{
	using namespace std;
    
	//***************************************************************************************************************************
	UTMConverter::UTMConverter()
	{
	  ellipsoid_         = WGS84;
	  actual_a_          = WGS84_A;
	  actual_f_          = WGS84_F;
	  origin_latitude_   = 0;
	  origin_longitude_  = 0;
	  origin_zone_       = 0;
	  origin_hemisphere_ = 'X';
	  origin_easting_    = 0;
	  origin_northing_   = 0;
	}
	
	//***************************************************************************************************************************
	double UTMConverter::deg2rad( double degrees )
	{
	  return degrees * M_PI / 180;
	}
	
	//***************************************************************************************************************************
	double UTMConverter::rad2deg( double radians )
	{
	  return radians / M_PI * 180;
	}
	
	//***************************************************************************************************************************
	long UTMConverter::setEllipsoid( EllipsoidType type )
	{
	  ellipsoid_ = type;
	  switch( ellipsoid_ )
	  {
	    case WGS84:
	      actual_a_  = WGS84_A;
	      actual_f_  = WGS84_F;
	      break;
	      
	    case BESSEL:
	      actual_a_  = BESSEL_A;
	      actual_f_  = BESSEL_F;
	      break;
	    
	    default:
	      actual_a_  = WGS84_A;
	      actual_f_  = WGS84_F;
	  }
	  if ( origin_latitude_ != 0 || origin_longitude_ != 0 )
	    setOrigin( origin_latitude_, origin_longitude_ );
	  return setUTMParameters( actual_a_, actual_f_, 0 );
	}
	
	//***************************************************************************************************************************
	long UTMConverter::setOrigin( double latitude, double longitude )
	{
	  origin_latitude_  = latitude;
	  origin_longitude_ = longitude;
	  long status = ConvertGeodeticToUTM( latitude, longitude, 
	                                      origin_zone_, origin_hemisphere_, origin_easting_, origin_northing_ );
	  return status;
	}
	
	//***************************************************************************************************************************
	long UTMConverter::getUTM( double latitude, double longitude, long &zone, char &hemisphere, double &easting, double &northing )
	{
	  long status;
	  status = ConvertGeodeticToUTM( latitude, longitude, zone, hemisphere, easting, northing );
	  if ( origin_latitude_ != 0 || origin_longitude_ != 0 )
	  {
	    easting = origin_easting_ - easting;
	    northing = origin_northing_ - northing;
	  }
	  return status;
	}
    
    //***************************************************************************************************************************
    long UTMConverter::getGeo( long zone, char hemisphere, double easting, double northing,
                               double &latitude, double &longitude )
    {
      long status;
      if ( origin_latitude_ != 0 || origin_longitude_ != 0 )
      {
        easting  += origin_easting_;
        northing += origin_northing_;
      }
      cout << easting << endl;
      cout << northing << endl;
      status = ConvertUTMToGeodetic( zone, hemisphere, easting, northing, latitude, longitude);
      return status;
    }
    
}; // end namespace GPS
