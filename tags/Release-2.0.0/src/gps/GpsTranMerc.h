/**
 * @file
 * Transmercato transformation
 *
 * @class TransMercato
 * @author Bechter Stephan <stephan.bechter@student.tugraz.at
 * C++ Implementation of the geotrans library.
 * 
 * ABSTRACT
 *
 *    This component provides conversions between Geodetic coordinates 
 *    (latitude and longitude) and Transverse Mercator projection coordinates
 *    (easting and northing).
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found the error code is combined with the current error code using 
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes are:
 *
 *       TRANMERC_NO_ERROR           : 
 *       TRANMERC_LAT_ERROR          : 
 *                                      
 *       TRANMERC_LON_ERROR          : Longitude outside of valid range
 *                                      (-180 to 360 degrees, and within
 *                                        +/-90 of Central Meridian)
 *       TRANMERC_EASTING_ERROR      : 
 *       TRANMERC_NORTHING_ERROR     : Northing outside of valid range
 *                                      (depending on ellipsoid and
 *                                       projection parameters)
 *       TRANMERC_ORIGIN_LAT_ERROR   : 
 *       TRANMERC_CENT_MER_ERROR     : 
 *       TRANMERC_A_ERROR            : 
 *       TRANMERC_INV_F_ERROR        : 
 *       TRANMERC_SCALE_FACTOR_ERROR : 	
 *       TM_LON_WARNING              : 
 *
 * REFERENCES
 *
 *    TRANSVERSE MERCATOR originated from :  
 *                      U.S. Army Topographic Engineering Center
 *                      Geospatial Information Division
 *                      7701 Telegraph Road
 *                      Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 **/

#ifndef _TRANMERC_H__
#define _TRANMERC_H__

#include <math.h>

namespace GPS
{ 
	///
	class TransMercato
	{
	  public:
	    /// No errors occurred in function
	    static int const TRANMERC_NO_ERROR           = 0x0000;
	    /// Latitude outside of valid range (-90 to 90 degrees)
	    static int const TRANMERC_LAT_ERROR          = 0x0001;
	    /// Longitude outside of valid range (-180 to 360 degrees, and within +/-90 of Central Meridian)
	    static int const TRANMERC_LON_ERROR          = 0x0002;
	    /// Easting outside of valid range (depending on ellipsoid and projection parameters)
	    static int const TRANMERC_EASTING_ERROR      = 0x0004;
	    /// Northing outside of valid range (depending on ellipsoid and projection parameters)
	    static int const TRANMERC_NORTHING_ERROR     = 0x0008;
	    /// Origin latitude outside of valid range (-90 to 90 degrees)
	    static int const TRANMERC_ORIGIN_LAT_ERROR   = 0x0010;
	    /// Central meridian outside of valid range (-180 to 360 degrees)
	    static int const TRANMERC_CENT_MER_ERROR     = 0x0020;
	    /// Semi-major axis less than or equal to zero
	    static int const TRANMERC_A_ERROR            = 0x0040;
	    /// Inverse flattening outside of valid range (250 to 350)
	    static int const TRANMERC_INV_F_ERROR        = 0x0080;
	    /// Scale factor outside of valid range (0.3 to 3.0)
	    static int const TRANMERC_SCALE_FACTOR_ERROR = 0x0100;
	    /// Distortion will result if longitude is more than 9 degrees from the Central Meridian
	    static int const TRANMERC_LON_WARNING        = 0x0200; 
	    
	  protected:
	    /// PI over 2
	    static double const PI_OVER                  = ( 3.14159265358979323846 / 2.0e0 ); 
	    /// 89.99 degrees in radians
	    static double const MAX_LAT                  = ( ( 3.14159265358979323846 * 89.99 ) / 180.0 ); 
	    /// 90 degrees in radians
	    static double const MAX_DELTA_LONG           = ( ( 3.14159265358979323846 * 90 ) / 180.0 );
	    /// Minimum scale factor
	    static double const MIN_SCALE_FACTOR         = 0.3;
	    /// Maximum scale factor
	    static double const MAX_SCALE_FACTOR         = 3.0;
	    
	  private:
	    // Ellipsoid Parameters, default to WGS 84
	    double TranMerc_a;     ///< Semi-major axis of ellipsoid i meters
	    double TranMerc_f;     ///< Flattening of ellipsoid
	    double TranMerc_es;    ///< Eccentricity (0.08181919084262188000) squared
	    double TranMerc_ebs;   ///< Second Eccentricity squared
	    
	    // Transverse_Mercator projection Parameters
	    double TranMerc_Origin_Lat;     ///< Latitude of origin in radians
	    double TranMerc_Origin_Long;    ///< Longitude of origin in radians
	    double TranMerc_False_Northing; ///< False northing in meters
	    double TranMerc_False_Easting;  ///< False easting in meters
	    double TranMerc_Scale_Factor;   ///< Scale factor
	    
	    // Isometeric to geodetic latitude parameters, default to WGS 84
	    double TranMerc_ap;
	    double TranMerc_bp;
	    double TranMerc_cp;
	    double TranMerc_dp;
	    double TranMerc_ep;
	    
	    // Maximum variance for easting and northing values for WGS 84.
	    double TranMerc_Delta_Easting;
	    double TranMerc_Delta_Northing;
	    
	
	    //****************************************************************************************************************
	    inline double DENOM( double Latitude )
	    {
	      return ( ( double ) ( sqrt( 1.e0 - TranMerc_es * pow( sin( Latitude ), 2 ) ) ) );
	    }
	    
	    //****************************************************************************************************************
	    inline double SPHTMD( double Latitude )
	    {
	      return ( ( double ) ( TranMerc_ap * Latitude
	                - TranMerc_bp * sin( 2.e0 * Latitude ) + TranMerc_cp * sin( 4.e0 * Latitude )
	                - TranMerc_dp * sin( 6.e0 * Latitude ) + TranMerc_ep * sin( 8.e0 * Latitude ) ) );
	    }
	    
	    //****************************************************************************************************************
	    inline double SPHSN( double Latitude )
	    {
	      return ( ( double ) ( TranMerc_a / sqrt( 1.e0 - TranMerc_es * pow( sin ( Latitude ), 2 ) ) ) );
	    }
	
	    //****************************************************************************************************************
	    inline double SPHSR( double Latitude )
	    {
	      return ( ( double ) ( TranMerc_a * ( 1.e0 - TranMerc_es ) / pow( DENOM( Latitude ), 3 ) ) );
	    }
	    
	  public:
	    //****************************************************************************************************************
	    /// default consturctor
	    TransMercato();
	    
	    //****************************************************************************************************************
	    /// virtual destructor
	    virtual ~TransMercato() {};
	    
	    //****************************************************************************************************************
	    ///
	    /// The function Set_Tranverse_Mercator_Parameters receives the ellipsoid
	    /// parameters and Tranverse Mercator projection parameters as inputs, and
	    /// sets the corresponding state variables. If any errors occur, the error
	    /// code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
	    /// returned.
	    /// @param[IN] a                Semi-major axis of ellipsoid, in meters
	    /// @param[IN] f                Flattening of ellipsoid
	    /// @param[IN] Origin_Latitude  Latitude in radians at the origin of the projection
	    /// @param[IN] Central_Meridian Longitude in radians at the center of the projection
	    /// @param[IN] False_Easting    Easting/X at the center of the projection
	    /// @param[IN] False_Northing   Northing/Y at the center of the projection
	    /// @param[IN] Scale_Factor     Projection scale factor
	    /// @return status @see errorvalues
	    long SetTransverseMercatorParameters( double a, double f, double Origin_Latitude, double Central_Meridian,
	                                          double False_Easting, double False_Northing, double Scale_Factor);
	
	    //****************************************************************************************************************
	    /// The function Get_Transverse_Mercator_Parameters returns the current
	    /// ellipsoid and Transverse Mercator projection parameters.
	    ///
	    /// @param[OUT] a                Semi-major axis of ellipsoid, in meters
	    /// @param[OUT] f                Flattening of ellipsoid
	    /// @param[OUT] Origin_Latitude  Latitude in radians at the origin of the projection
	    /// @param[OUT] Central_Meridian Longitude in radians at the center of the projection
	    /// @param[OUT] False_Easting    Easting/X at the center of the projection
	    /// @param[OUT] False_Northing   Northing/Y at the center of the projection
	    /// @param[OUT] Scale_Factor     Projection scale factor 
	    void GetTransverseMercatorParameters( double &a, double &f, double &Origin_Latitude, double &Central_Meridian,
	                                          double &False_Easting, double &False_Northing, double &Scale_Factor );
	    
	    //****************************************************************************************************************
	    /// The function Convert_Geodetic_To_Transverse_Mercator converts geodetic
	    /// (latitude and longitude) coordinates to Transverse Mercator projection
	    /// (easting and northing) coordinates, according to the current ellipsoid
	    /// and Transverse Mercator projection coordinates.  If any errors occur, the
	    /// error code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
	    /// returned.
	    /// @param[IN]  Latitude  Latitude in radians
	    /// @param[IN]  Longitude Longitude in radians
	    /// @param[OUT] Easting   Easting/X in meters
	    /// @param[OUT] Northing  Northing/Y in meters
	    /// @return status @see errorvalues
	    long ConvertGeodeticToTransverseMercator( double Latitude, double Longitude,
	                                              double &Easting, double &Northing );
	
	    //****************************************************************************************************************
	    /// The function Convert_Transverse_Mercator_To_Geodetic converts Transverse
	    /// Mercator projection (easting and northing) coordinates to geodetic
	    /// (latitude and longitude) coordinates, according to the current ellipsoid
	    /// and Transverse Mercator projection parameters.  If any errors occur, the
	    /// error code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
	    /// returned.
	    /// @param[IN]  Easting   Easting/X in meters
	    /// @param[IN]  Northing  Northing/Y in meters
	    /// @param[OUT] Latitude  Latitude in radians
	    /// @param[OUT] Longitude Longitude in radians
	    /// @return status @see errorvalues
	    long ConvertTransverseMercatorToGeodetic( double Easting, double Northing,
	                                              double &Latitude, double &Longitude );
	                                              
	};
}; // end namespace GPS

#endif // _TRANMERC_H__
