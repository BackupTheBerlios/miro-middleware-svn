/** 
 * @file
 * @author Bechter Stephan <stephan.bechter@sbox.tugraz.at>
 * Converting GPS geodetic coordinates (Longitude and Latitude) 
 * to UTM Coordinates using the WGS84 as standard ellipsoid.
 * 
 * This UTM Coordinates are in meters east and north
 * @author Bechter Stephan <stephan.bechter@student.tugraz.at>
 **/
#ifndef _GPSUTMCONVERTER_H___
#define _GPSUTMCONVERTER_H___

#include "GpsUTM.h"

namespace GPS
{
	/**
	 */
	class UTMConverter : public UTM
	{
	  public:
	    /// Types of the ellipsoid
	    enum EllipsoidType
	    {
	      WGS84, ///< Internation WGS84 map format.
	      BESSEL ///< Optimized ellipsoid for germany
	    };
	  
	  private:
	    static double const WGS84_A  = 6378137.0;
	    static double const WGS84_F  = 1/298.257223563;
	    static double const BESSEL_A = 6377397.155;
	    static double const BESSEL_F = 1/299.15281;
	    
	    EllipsoidType ellipsoid_; ///< ellipsoid type @see EllipsoidType
	    double actual_a_;
	    double actual_f_;
	    double origin_latitude_;
	    double origin_longitude_;
	    long   origin_zone_;
	    char   origin_hemisphere_;
	    double origin_easting_;
	    double origin_northing_;
	    
	  public:
	    /// Initialisation of default values
	    UTMConverter();
	    
	    /// Virtual destructor
	    virtual ~UTMConverter() {};
		
			/// Converts degrees in radians
			/// @param degrees angle in degrees
			/// @return angel in radians
			double deg2rad( double degrees );
			
			/// Converts degrees in radians
			/// @param degrees angle in radians
			/// @return angle in degrees
			double rad2deg( double radians );
		
	    /// Set the ellipsoid: WGS84 (default), BESSEL
	    /// @param type Ellipsoid type ( UTMConverter::WGS84 )
	    /// @see EllipsoidType
	    long setEllipsoid( EllipsoidType type );
	    
	    /// Get the actual ellipsoid mode
	    /// @return actual ellipsoid type
	    /// @see EllipsoidType
	    EllipsoidType getEllipsoid() { return ellipsoid_; }
		  
			/// Set the "point of origin". If sed then all coordinates got with getUTM are
	    /// relative to this point.
	    /// @param latitude  Latitude in radians
	    /// @param longitude Longitude in radians
	    /// @see getUTM()
	    long setOrigin( double latitude, double longitude);
		  
	    /// Returns the coordinates for the given geodetic coordinates
	    /// @param[IN]  latitude   Latitude in radians
	    /// @param[IN]  longitude  Longitude in radians
	    /// @param[OUT] zone       UTM zone
	    /// @param[OUT] hemisphere North or South hemisphere
	    /// @param[OUT] easting    Easting/X in meters
	    /// @param[OUT] northing   Northing (Y) in meters
	    /// @return Status value
	    long getUTM( double latitude, double longitude, long &zone, char &hemisphere, double &easting, double &northing );
	    
      /// Returns the coordinates for the given geodetic coordinates
      /// @param[IN]  zone       UTM zone
      /// @param[IN]  hemisphere North or South hemisphere
      /// @param[IN]  easting    Easting/X in meters
      /// @param[IN]  northing   Northing (Y) in meters
      /// @param[OUT] latitude   Latitude in radians
      /// @param[OUT] longitude  Longitude in radians
      /// @return Status value
      long getGeo( long zone, char hemisphere, double easting, double northing,
                   double &latitude, double &longitude );
	};
}; // end namespace GPS

#endif // _GPSUTMCONVERTER_H___
