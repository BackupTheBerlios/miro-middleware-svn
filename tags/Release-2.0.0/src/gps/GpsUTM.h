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

#ifndef _UTM_H__
#define _UTM_H__

#include "GpsTranMerc.h"

namespace GPS
{
	class UTM : public TransMercato
	{
	  private:
	    // private constants
	    /// -80.5 degrees in radians
	    static const double MIN_LAT               = ( ( -80.5 * 3.14159265358979323846 ) / 180.0 );
	    ///  84.5 degrees in radians
	    static const double MAX_LAT               = ( ( 84.5 * 3.14159265358979323846 ) / 180.0 );
	    static const int MIN_EASTING              = 100000;
	    static const int MAX_EASTING              = 900000;
	    static const int MIN_NORTHING             = 0;
	    static const int MAX_NORTHING             = 10000000;
	
	    // member variables
	    double utm_a_;          ///< Semi-major axis of ellipsoid in meters
	    double utm_f_;          ///< Flattening of ellipsoid
	    long   utm_override_;   ///< Zone override flag
	    
	  public:
	    // errorcodes
	    /// No errors occurred in function
	    static const int UTM_NO_ERROR             = 0x0000;
	    /// Latitude outside of valid range (-80.5 to 84.5 degrees)
	    static const int UTM_LAT_ERROR            = 0x0001;
	    /// Longitude outside of valid range (-180 to 360 degrees)
	    static const int UTM_LON_ERROR            = 0x0002;
	    /// Easting outside of valid range (100,000 to 900,000 meters)
	    static const int UTM_EASTING_ERROR        = 0x0004;
	    /// Northing outside of valid range (0 to 10,000,000 meters)
	    static const int UTM_NORTHING_ERROR       = 0x0008;
	    /// Zone outside of valid range (1 to 60)
	    static const int UTM_ZONE_ERROR           = 0x0010;
	    /// Invalid hemisphere ('N' or 'S')
	    static const int UTM_HEMISPHERE_ERROR     = 0x0020;
	    /// Zone outside of valid range (1 to 60) and within 1 of 'natural' zone
	    static const int UTM_ZONE_OVERRIDE_ERROR  = 0x0040;
	    /// Semi-major axis less than or equal to zero
	    static const int UTM_A_ERROR              = 0x0080;
	    /// Inverse flattening outside of valid range (250 to 350)
	    static const int UTM_INV_F_ERROR          = 0x0100;
	  
	  
	    UTM();
	    
	    virtual ~UTM() {};
	    
	    /// The function Set_UTM_Parameters receives the ellipsoid parameters and
	    /// UTM zone override parameter as inputs, and sets the corresponding state
	    /// variables.  If any errors occur, the error code(s) are returned by the 
	    /// function, otherwise UTM_NO_ERROR is returned.
	    /// @param[IN] a        Semi-major axis of ellipsoid, in meters
	    /// @param[IN] f        Flattening of ellipsoid
	    /// @param[IN] override UTM override zone, zero indicates no override
	    /// @return status
	    long setUTMParameters( double a, double f, long override );
	    
	    /// The function Get_UTM_Parameters returns the current ellipsoid
	    /// parameters and UTM zone override parameter.
	    /// @param[OUT] a        Semi-major axis of ellipsoid, in meters
	    /// @param[OUT] f        Flattening of ellipsoid
	    /// @param[OUT] override UTM override zone, zero indicates no override
	    /// @return status of the operation
	    /// @see errorcodes
	    void getUTMParameters( double &a, double &f, long &override );
	  
	    /// The function Convert_Geodetic_To_UTM converts geodetic (latitude and
	    /// longitude) coordinates to UTM projection (zone, hemisphere, easting and
	    /// northing) coordinates according to the current ellipsoid and UTM zone
	    /// override parameters.  If any errors occur, the error code(s) are returned
	    /// by the function, otherwise UTM_NO_ERROR is returned.
	    /// @param[IN]  Latitude   Latitude in radians
	    /// @param[IN]  Longitude  Longitude in radians
	    /// @param[OUT] zone       UTM zone
	    /// @param[OUT] hemisphere North or South hemisphere
	    /// @param[OUT] easting    Easting (X) in meters
	    /// @param[OUT] northing   Northing (Y) in meters
	    /// @return status of the operation
	    /// @see errorcodes
	    long ConvertGeodeticToUTM( double Latitude, double Longitude,
	                               long &zone, char &hemisphere, double &easting, double &northing );
	  
	    /// The function Convert_UTM_To_Geodetic converts UTM projection (zone, 
	    /// hemisphere, easting and northing) coordinates to geodetic(latitude
	    /// and  longitude) coordinates, according to the current ellipsoid
	    /// parameters.  If any errors occur, the error code(s) are returned
	    /// by the function, otherwise UTM_NO_ERROR is returned.
	    /// @param[IN] Zone       UTM zone
	    /// @param[IN] Hemisphere North or South hemisphere
	    /// @param[IN] Easting    Easting (X) in meters
	    /// @param[IN] Northing   Northing (Y) in meters
	    /// @param[OUT] latitude   Latitude in radians
	    /// @param[OUT] longitude  Longitude in radians
	    /// @return status of the operation
	    /// @see errorcodes
	    long ConvertUTMToGeodetic( long Zone, char Hemisphere, double Easting, double Northing,
	                                double &latitude, double &longitude);
	
	};
}; // end namespace GPS

#endif // _UTM_H__
