/**
 * C++ Implementation of the geotrans library.
 *
 * ABSTRACT
 *
 *    This component provides conversions between geodetic coordinates
 *    (latitude and longitudes) and Universal Transverse Mercator (UTM)
 *    projection (zone, hemisphere, easting, and northing) coordinates.
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found, the error code is combined with the current error code using
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes see <b>Static Public Attributes</b>
 *
 * REFERENCES
 *
 *    TRANSVERSE MERCATOR originated from :<br>
 *                      U.S. Army Topographic Engineering Center<br>
 *                      Geospatial Information Division<br>
 *                      7701 Telegraph Road<br>
 *                      Alexandria, VA  22310-3864<br>
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * @author Bechter Stephan <stephan.bechter@student.tugraz.at
 */

#include "GpsUTM.h"

namespace GPS
{
	//***************************************************************************************************************************
	UTM::UTM()
	{
	  utm_a_        = 6378137.0;          // Semi-major axis of ellipsoid in meters
	  utm_f_        = 1 / 298.2572235631; // Flattening of ellipsoid
	  utm_override_ = 0;                  // Zone override flag
	  
	}
	
	//***************************************************************************************************************************
	long UTM::setUTMParameters( double a, double f, long override )
	{
	  double inv_f = 1 / f;
	  long Error_Code = UTM_NO_ERROR;
	
	  if (a <= 0.0)
	  { /* Semi-major axis must be greater than zero */
	    Error_Code |= UTM_A_ERROR;
	  }
	  if ( ( inv_f < 250 ) || ( inv_f > 350 ) )
	  { /* Inverse flattening must be between 250 and 350 */
	    Error_Code |= UTM_INV_F_ERROR;
	  }
	  if ( ( override < 0 ) || ( override > 60 ) )
	  {
	    Error_Code |= UTM_ZONE_OVERRIDE_ERROR;
	  }
	  if ( !Error_Code )
	  { /* no errors */
	    utm_a_ = a;
	    utm_f_ = f;
	    utm_override_ = override;
	  }
	  return ( Error_Code );
	}
	
	//***************************************************************************************************************************
	void UTM::getUTMParameters( double &a, double &f, long &override )
	{
	  a = utm_a_;
	  f = utm_f_;
	  override = utm_override_;
	} /* END OF Get_UTM_Parameters */
	
	//***************************************************************************************************************************
	long UTM::ConvertGeodeticToUTM ( double Latitude, double Longitude,
	                                 long &zone, char &hemisphere, double &easting, double &northing )
	{ 
	  long Lat_Degrees;
	  long Long_Degrees;
	  long temp_zone;
	  long Error_Code = UTM_NO_ERROR;
	  double Origin_Latitude = 0;
	  double Central_Meridian = 0;
	  double False_Easting = 500000;
	  double False_Northing = 0;
	  double Scale = 0.9996;
	
	  if ( ( Latitude < MIN_LAT ) || ( Latitude > MAX_LAT ) )
	  { /* Latitude out of range */
	    Error_Code |= UTM_LAT_ERROR;
	  }
	  if ( ( Longitude < -1 * M_PI ) || ( Longitude > ( 2 * M_PI ) ) )
	  { /* Longitude out of range */
	    Error_Code |= UTM_LON_ERROR;
	  }
	  if ( !Error_Code )
	  { /* no errors */
	    if ( Longitude < 0 )
	      Longitude += ( 2 * M_PI ) + 1.0e-10;
	    Lat_Degrees  = ( long )( Latitude * 180.0 / M_PI );
	    Long_Degrees = ( long )( Longitude * 180.0 / M_PI );
	
	    if ( Longitude < M_PI )
	      temp_zone = ( long )( 31 + ( ( Longitude * 180.0 / M_PI ) / 6.0 ) );
	    else
	      temp_zone = ( long )( ( ( Longitude * 180.0 / M_PI ) / 6.0 ) - 29 );
	    if ( temp_zone > 60 )
	      temp_zone = 1;
	    /* UTM special cases */
	    if ( ( Lat_Degrees > 55 ) && ( Lat_Degrees < 64 ) && ( Long_Degrees > -1 )
	        && ( Long_Degrees < 3 ) )
	      temp_zone = 31;
	    if ( ( Lat_Degrees > 55 ) && ( Lat_Degrees < 64 ) && ( Long_Degrees > 2 )
	        && ( Long_Degrees < 12 ) )
	      temp_zone = 32;
	    if ( ( Lat_Degrees > 71 ) && ( Long_Degrees > -1 ) && ( Long_Degrees < 9 ) )
	      temp_zone = 31;
	    if ( ( Lat_Degrees > 71 ) && ( Long_Degrees > 8 ) && ( Long_Degrees < 21 ) )
	      temp_zone = 33;
	    if ( ( Lat_Degrees > 71 ) && ( Long_Degrees > 20 ) && ( Long_Degrees < 33 ) )
	      temp_zone = 35;
	    if ( ( Lat_Degrees > 71 ) && ( Long_Degrees > 32 ) && ( Long_Degrees < 42 ) )
	      temp_zone = 37;
	
	    if ( utm_override_ )
	    {
	      if ( (temp_zone == 1 ) && ( utm_override_ == 60 ) )
	        temp_zone = utm_override_;
	      else if ( ( temp_zone == 60 ) && ( utm_override_ == 1 ) )
	        temp_zone = utm_override_;
	      else if ( ( ( temp_zone - 1 ) <= utm_override_ ) && ( utm_override_ <= ( temp_zone + 1 ) ) )
	        temp_zone = utm_override_;
	      else
	        Error_Code = UTM_ZONE_OVERRIDE_ERROR;
	    }
	    if ( !Error_Code )
	    {
	      if ( temp_zone >= 31 )
	        Central_Meridian = ( 6 * temp_zone - 183 ) * M_PI / 180.0;
	      else
	        Central_Meridian = ( 6 * temp_zone + 177 ) * M_PI / 180.0;
	      zone = temp_zone;
	      if ( Latitude < 0 )
	      {
	        False_Northing = 10000000;
	        hemisphere = 'S';
	      }
	      else
	        hemisphere = 'N';
	      SetTransverseMercatorParameters( utm_a_, utm_f_, Origin_Latitude,
	                                       Central_Meridian, False_Easting, False_Northing, Scale );
	      ConvertGeodeticToTransverseMercator( Latitude, Longitude, easting, northing );
	      if ( ( easting < MIN_EASTING ) || ( easting > MAX_EASTING ) )
	        Error_Code = UTM_EASTING_ERROR;
	      if ( ( northing < MIN_NORTHING ) || ( northing > MAX_NORTHING ) )
	        Error_Code |= UTM_NORTHING_ERROR;
	    }
	  }
	  return ( Error_Code );
	}
	
	//***************************************************************************************************************************
	long UTM::ConvertUTMToGeodetic( long Zone, char Hemisphere, double Easting, double Northing,
	                                double &latitude, double &longitude)
	{
	  long Error_Code = UTM_NO_ERROR;
	  double Origin_Latitude = 0;
	  double Central_Meridian = 0;
	  double False_Easting = 500000;
	  double False_Northing = 0;
	  double Scale = 0.9996;
	
	  if ( ( Zone < 1 ) || ( Zone > 60 ) )
	    Error_Code |= UTM_ZONE_ERROR;
	  if ( ( Hemisphere != 'S' ) && ( Hemisphere != 'N' ) )
	    Error_Code |= UTM_HEMISPHERE_ERROR;
	  if ( ( Easting < MIN_EASTING ) || ( Easting > MAX_EASTING ) )
	    Error_Code |= UTM_EASTING_ERROR;
	  if ( ( Northing < MIN_NORTHING ) || ( Northing > MAX_NORTHING ) )
	    Error_Code |= UTM_NORTHING_ERROR;
	  if ( !Error_Code )
	  { /* no errors */
	    if ( Zone >= 31 )
	      Central_Meridian = ( ( 6 * Zone - 183 ) * M_PI / 180.0 );
	    else
	      Central_Meridian = ((6 * Zone + 177) * M_PI / 180.0 );
	    if ( Hemisphere == 'S' )
	      False_Northing = 10000000;
	    SetTransverseMercatorParameters( utm_a_, utm_f_, Origin_Latitude,
	                                     Central_Meridian, False_Easting, False_Northing, Scale);
	    if ( ConvertTransverseMercatorToGeodetic( Easting, Northing, latitude, longitude ) )
	      Error_Code |= UTM_NORTHING_ERROR;
	    if ( ( latitude < MIN_LAT ) || ( latitude > MAX_LAT ) )
	    { /* Latitude out of range */
	      Error_Code |= UTM_NORTHING_ERROR;
	    }
	  }
	  return ( Error_Code );
	}
}; // end namespace GPS
