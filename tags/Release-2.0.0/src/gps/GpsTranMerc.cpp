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
#include "GpsTranMerc.h"

namespace GPS
{
	//***************************************************************************************************************************
	TransMercato::TransMercato()
	{
	  // Ellipsoid Parameters, default to WGS 84
	  TranMerc_a              = 6378137.0;              /// Semi-major axis of ellipsoid i meters
	  TranMerc_f              = 1 / 298.257223563;      /// Flattening of ellipsoid
	  TranMerc_es             = 0.0066943799901413800;  /// Eccentricity (0.08181919084262188000) squared
	  TranMerc_ebs            = 0.0067394967565869;     /// Second Eccentricity squared
	    
	  // Transverse_Mercator projection Parameters
	  TranMerc_Origin_Lat     = 0.0;   /// Latitude of origin in radians
	  TranMerc_Origin_Long    = 0.0;   /// Longitude of origin in radians
	  TranMerc_False_Northing = 0.0;   /// False northing in meters
	  TranMerc_False_Easting  = 0.0;   /// False easting in meters
	  TranMerc_Scale_Factor   = 1.0;   /// Scale factor
	  
	  // Isometeric to geodetic latitude parameters, default to WGS 84
	  TranMerc_ap             = 6367449.1458008;
	  TranMerc_bp             = 16038.508696861;
	  TranMerc_cp             = 16.832613334334;
	  TranMerc_dp             = 0.021984404273757;
	  TranMerc_ep             = 3.1148371319283e-005;
	  
	  // Maximum variance for easting and northing values for WGS 84
	  TranMerc_Delta_Easting  = 40000000.0;
	  TranMerc_Delta_Northing = 40000000.0;
	}
	
	
	//********************************************************************************************************************
	long TransMercato::SetTransverseMercatorParameters( double a, double f, double Origin_Latitude,
	                                                    double Central_Meridian, double False_Easting,
	                                                    double False_Northing, double Scale_Factor)
	{
	  double tn;        /* True Meridianal distance constant  */
	  double tn2;
	  double tn3;
	  double tn4;
	  double tn5;
	  double dummy_northing;
	  double TranMerc_b; /* Semi-minor axis of ellipsoid, in meters */
	  double inv_f = 1 / f;
	  long Error_Code = TRANMERC_NO_ERROR;
	
	  if (a <= 0.0)
	  { /* Semi-major axis must be greater than zero */
	    Error_Code |= TRANMERC_A_ERROR;
	  }
	  if ((inv_f < 250) || (inv_f > 350))
	  { /* Inverse flattening must be between 250 and 350 */
	    Error_Code |= TRANMERC_INV_F_ERROR;
	  }
	  if ((Origin_Latitude < -1 * MAX_LAT) || (Origin_Latitude > MAX_LAT))
	  { /* origin latitude out of range */
	    Error_Code |= TRANMERC_ORIGIN_LAT_ERROR;
	  }
	  if ((Central_Meridian < -1 * M_PI) || (Central_Meridian > (2*M_PI)))
	  { /* origin longitude out of range */
	    Error_Code |= TRANMERC_CENT_MER_ERROR;
	  }
	  if ((Scale_Factor < MIN_SCALE_FACTOR) || (Scale_Factor > MAX_SCALE_FACTOR))
	  {
	    Error_Code |= TRANMERC_SCALE_FACTOR_ERROR;
	  }
	  if (!Error_Code)
	  { /* no errors */
	    TranMerc_a = a;
	    TranMerc_f = f;
	    TranMerc_Origin_Lat = 0;
	    TranMerc_Origin_Long = 0;
	    TranMerc_False_Northing = 0;
	    TranMerc_False_Easting = 0; 
	    TranMerc_Scale_Factor = 1;
	
	    /* Eccentricity Squared */
	    TranMerc_es = 2 * TranMerc_f - TranMerc_f * TranMerc_f;
	    /* Second Eccentricity Squared */
	    TranMerc_ebs = (1 / (1 - TranMerc_es)) - 1;
	
	    TranMerc_b = TranMerc_a * (1 - TranMerc_f);    
	    /*True meridianal constants  */
	    tn = (TranMerc_a - TranMerc_b) / (TranMerc_a + TranMerc_b);
	    tn2 = tn * tn;
	    tn3 = tn2 * tn;
	    tn4 = tn3 * tn;
	    tn5 = tn4 * tn;
	
	    TranMerc_ap = TranMerc_a * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
	                                + 81.e0 * (tn4 - tn5)/64.e0 );
	    TranMerc_bp = 3.e0 * TranMerc_a * (tn - tn2 + 7.e0 * (tn3 - tn4)
	                                       /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
	    TranMerc_cp = 15.e0 * TranMerc_a * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
	    TranMerc_dp = 35.e0 * TranMerc_a * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
	    TranMerc_ep = 315.e0 * TranMerc_a * (tn4 - tn5) / 512.e0;
	    ConvertGeodeticToTransverseMercator( MAX_LAT,
	                                         MAX_DELTA_LONG,
	                                         TranMerc_Delta_Easting,
	                                         TranMerc_Delta_Northing );
	    ConvertGeodeticToTransverseMercator( 0,
	                                         MAX_DELTA_LONG,
	                                         TranMerc_Delta_Easting,
	                                         dummy_northing );
	    TranMerc_Origin_Lat = Origin_Latitude;
	    if (Central_Meridian > M_PI)
	      Central_Meridian -= (2*M_PI);
	    TranMerc_Origin_Long = Central_Meridian;
	    TranMerc_False_Northing = False_Northing;
	    TranMerc_False_Easting = False_Easting; 
	    TranMerc_Scale_Factor = Scale_Factor;
	  } /* END OF if(!Error_Code) */
	  return ( Error_Code );
	}
	
	
	//***************************************************************************************************************************
	void TransMercato::GetTransverseMercatorParameters( double &a, double &f, double &Origin_Latitude, double &Central_Meridian,
	                                                    double &False_Easting, double &False_Northing, double &Scale_Factor )
	{
	  a                = TranMerc_a;
	  f                = TranMerc_f;
	  Origin_Latitude  = TranMerc_Origin_Lat;
	  Central_Meridian = TranMerc_Origin_Long;
	  False_Easting    = TranMerc_False_Easting;
	  False_Northing   = TranMerc_False_Northing;
	  Scale_Factor     = TranMerc_Scale_Factor;
	}
	
	
	//***************************************************************************************************************************
	long TransMercato::ConvertGeodeticToTransverseMercator( double Latitude, double Longitude,
	                                                        double &Easting, double &Northing )
	{
	  double c;       /* Cosine of latitude                          */
	  double c2;
	  double c3;
	  double c5;
	  double c7;
	  double dlam;    /* Delta longitude - Difference in Longitude       */
	  double eta;     /* constant - TranMerc_ebs *c *c                   */
	  double eta2;
	  double eta3;
	  double eta4;
	  double s;       /* Sine of latitude                        */
	  double sn;      /* Radius of curvature in the prime vertical       */
	  double t;       /* Tangent of latitude                             */
	  double tan2;
	  double tan3;
	  double tan4;
	  double tan5;
	  double tan6;
	  double t1;      /* Term in coordinate conversion formula - GP to Y */
	  double t2;      /* Term in coordinate conversion formula - GP to Y */
	  double t3;      /* Term in coordinate conversion formula - GP to Y */
	  double t4;      /* Term in coordinate conversion formula - GP to Y */
	  double t5;      /* Term in coordinate conversion formula - GP to Y */
	  double t6;      /* Term in coordinate conversion formula - GP to Y */
	  double t7;      /* Term in coordinate conversion formula - GP to Y */
	  double t8;      /* Term in coordinate conversion formula - GP to Y */
	  double t9;      /* Term in coordinate conversion formula - GP to Y */
	  double tmd;     /* True Meridional distance                        */
	  double tmdo;    /* True Meridional distance for latitude of origin */
	  long    Error_Code = TRANMERC_NO_ERROR;
	  double temp_Origin;
	  double temp_Long;
	
	  if ((Latitude < -1 * MAX_LAT) || (Latitude > MAX_LAT))
	  {  /* Latitude out of range */
	    Error_Code|= TRANMERC_LAT_ERROR;
	  }
	  if (Longitude > M_PI)
	    Longitude -= (2 * M_PI);
	  if ((Longitude < (TranMerc_Origin_Long - MAX_DELTA_LONG))
	      || (Longitude > (TranMerc_Origin_Long + MAX_DELTA_LONG)))
	  {
	    if (Longitude < 0)
	      temp_Long = Longitude + 2 * M_PI;
	    else
	      temp_Long = Longitude;
	    if (TranMerc_Origin_Long < 0)
	      temp_Origin = TranMerc_Origin_Long + 2 * M_PI;
	    else
	      temp_Origin = TranMerc_Origin_Long;
	    if ((temp_Long < (temp_Origin - MAX_DELTA_LONG))
	        || (temp_Long > (temp_Origin + MAX_DELTA_LONG)))
	      Error_Code|= TRANMERC_LON_ERROR;
	  }
	  if (!Error_Code)
	  { /* no errors */
	
	    /* 
	     *  Delta Longitude
	     */
	    dlam = Longitude - TranMerc_Origin_Long;
	
	    if (fabs(dlam) > (9.0 * M_PI / 180))
	    { /* Distortion will result if Longitude is more than 9 degrees from the Central Meridian */
	      Error_Code |= TRANMERC_LON_WARNING;
	    }
	
	    if (dlam > M_PI)
	      dlam -= (2 * M_PI);
	    if (dlam < -1 * M_PI)
	      dlam += (2 * M_PI);
	    if (fabs(dlam) < 2.e-10)
	      dlam = 0.0;
	
	    s = sin(Latitude);
	    c = cos(Latitude);
	    c2 = c * c;
	    c3 = c2 * c;
	    c5 = c3 * c2;
	    c7 = c5 * c2;
	    t = tan (Latitude);
	    tan2 = t * t;
	    tan3 = tan2 * t;
	    tan4 = tan3 * t;
	    tan5 = tan4 * t;
	    tan6 = tan5 * t;
	    eta = TranMerc_ebs * c2;
	    eta2 = eta * eta;
	    eta3 = eta2 * eta;
	    eta4 = eta3 * eta;
	
	    /* radius of curvature in prime vertical */
	    sn = SPHSN(Latitude);
	
	    /* True Meridianal Distances */
	    tmd = SPHTMD(Latitude);
	
	    /*  Origin  */
	    tmdo = SPHTMD (TranMerc_Origin_Lat);
	
	    /* northing */
	    t1 = (tmd - tmdo) * TranMerc_Scale_Factor;
	    t2 = sn * s * c * TranMerc_Scale_Factor/ 2.e0;
	    t3 = sn * s * c3 * TranMerc_Scale_Factor * (5.e0 - tan2 + 9.e0 * eta 
	                                                + 4.e0 * eta2) /24.e0; 
	
	    t4 = sn * s * c5 * TranMerc_Scale_Factor * (61.e0 - 58.e0 * tan2
	                                                + tan4 + 270.e0 * eta - 330.e0 * tan2 * eta + 445.e0 * eta2
	                                                + 324.e0 * eta3 -680.e0 * tan2 * eta2 + 88.e0 * eta4 
	                                                -600.e0 * tan2 * eta3 - 192.e0 * tan2 * eta4) / 720.e0;
	
	    t5 = sn * s * c7 * TranMerc_Scale_Factor * (1385.e0 - 3111.e0 * 
	                                                tan2 + 543.e0 * tan4 - tan6) / 40320.e0;
	
	    Northing = TranMerc_False_Northing + t1 + pow(dlam,2.e0) * t2
	                + pow(dlam,4.e0) * t3 + pow(dlam,6.e0) * t4
	                + pow(dlam,8.e0) * t5; 
	
	    /* Easting */
	    t6 = sn * c * TranMerc_Scale_Factor;
	    t7 = sn * c3 * TranMerc_Scale_Factor * (1.e0 - tan2 + eta ) /6.e0;
	    t8 = sn * c5 * TranMerc_Scale_Factor * (5.e0 - 18.e0 * tan2 + tan4
	                                            + 14.e0 * eta - 58.e0 * tan2 * eta + 13.e0 * eta2 + 4.e0 * eta3 
	                                            - 64.e0 * tan2 * eta2 - 24.e0 * tan2 * eta3 )/ 120.e0;
	    t9 = sn * c7 * TranMerc_Scale_Factor * ( 61.e0 - 479.e0 * tan2
	                                             + 179.e0 * tan4 - tan6 ) /5040.e0;
	
	    Easting = TranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7 
	              + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;
	  }
	  return (Error_Code);
	}
	
	
	//***************************************************************************************************************************
	long TransMercato::ConvertTransverseMercatorToGeodetic( double Easting, double Northing,
	                                                        double &Latitude, double &Longitude )
	{
	  double c;       /* Cosine of latitude                          */
	  double de;      /* Delta easting - Difference in Easting (Easting-Fe)    */
	  double dlam;    /* Delta longitude - Difference in Longitude       */
	  double eta;     /* constant - TranMerc_ebs *c *c                   */
	  double eta2;
	  double eta3;
	  double eta4;
	  double ftphi;   /* Footpoint latitude                              */
	  int    i;       /* Loop iterator                   */
	  double s;       /* Sine of latitude                        */
	  double sn;      /* Radius of curvature in the prime vertical       */
	  double sr;      /* Radius of curvature in the meridian             */
	  double t;       /* Tangent of latitude                             */
	  double tan2;
	  double tan4;
	  double t10;     /* Term in coordinate conversion formula - GP to Y */
	  double t11;     /* Term in coordinate conversion formula - GP to Y */
	  double t12;     /* Term in coordinate conversion formula - GP to Y */
	  double t13;     /* Term in coordinate conversion formula - GP to Y */
	  double t14;     /* Term in coordinate conversion formula - GP to Y */
	  double t15;     /* Term in coordinate conversion formula - GP to Y */
	  double t16;     /* Term in coordinate conversion formula - GP to Y */
	  double t17;     /* Term in coordinate conversion formula - GP to Y */
	  double tmd;     /* True Meridional distance                        */
	  double tmdo;    /* True Meridional distance for latitude of origin */
	  long Error_Code = TRANMERC_NO_ERROR;
	
	  if ((Easting < (TranMerc_False_Easting - TranMerc_Delta_Easting))
	      ||(Easting > (TranMerc_False_Easting + TranMerc_Delta_Easting)))
	  { /* Easting out of range  */
	    Error_Code |= TRANMERC_EASTING_ERROR;
	  }
	  if ((Northing < (TranMerc_False_Northing - TranMerc_Delta_Northing))
	      || (Northing > (TranMerc_False_Northing + TranMerc_Delta_Northing)))
	  { /* Northing out of range */
	    Error_Code |= TRANMERC_NORTHING_ERROR;
	  }
	
	  if (!Error_Code)
	  {
	    /* True Meridional Distances for latitude of origin */
	    tmdo = SPHTMD(TranMerc_Origin_Lat);
	
	    /*  Origin  */
	    tmd = tmdo +  (Northing - TranMerc_False_Northing) / TranMerc_Scale_Factor; 
	
	    /* First Estimate */
	    sr = SPHSR(0.e0);
	    ftphi = tmd/sr;
	
	    for (i = 0; i < 5 ; i++)
	    {
	      t10 = SPHTMD (ftphi);
	      sr = SPHSR(ftphi);
	      ftphi = ftphi + (tmd - t10) / sr;
	    }
	
	    /* Radius of Curvature in the meridian */
	    sr = SPHSR(ftphi);
	
	    /* Radius of Curvature in the meridian */
	    sn = SPHSN(ftphi);
	
	    /* Sine Cosine terms */
	    s = sin(ftphi);
	    c = cos(ftphi);
	
	    /* Tangent Value  */
	    t = tan(ftphi);
	    tan2 = t * t;
	    tan4 = tan2 * tan2;
	    eta = TranMerc_ebs * pow(c,2);
	    eta2 = eta * eta;
	    eta3 = eta2 * eta;
	    eta4 = eta3 * eta;
	    de = Easting - TranMerc_False_Easting;
	    if (fabs(de) < 0.0001)
	      de = 0.0;
	
	    /* Latitude */
	    t10 = t / (2.e0 * sr * sn * pow(TranMerc_Scale_Factor, 2));
	    t11 = t * (5.e0  + 3.e0 * tan2 + eta - 4.e0 * pow(eta,2)
	               - 9.e0 * tan2 * eta) / (24.e0 * sr * pow(sn,3) 
	                                       * pow(TranMerc_Scale_Factor,4));
	    t12 = t * (61.e0 + 90.e0 * tan2 + 46.e0 * eta + 45.E0 * tan4
	               - 252.e0 * tan2 * eta  - 3.e0 * eta2 + 100.e0 
	               * eta3 - 66.e0 * tan2 * eta2 - 90.e0 * tan4
	               * eta + 88.e0 * eta4 + 225.e0 * tan4 * eta2
	               + 84.e0 * tan2* eta3 - 192.e0 * tan2 * eta4)
	          / ( 720.e0 * sr * pow(sn,5) * pow(TranMerc_Scale_Factor, 6) );
	    t13 = t * ( 1385.e0 + 3633.e0 * tan2 + 4095.e0 * tan4 + 1575.e0 
	                * pow(t,6))/ (40320.e0 * sr * pow(sn,7) * pow(TranMerc_Scale_Factor,8));
	    Latitude = ftphi - pow(de,2) * t10 + pow(de,4) * t11 - pow(de,6) * t12 
	               + pow(de,8) * t13;
	
	    t14 = 1.e0 / (sn * c * TranMerc_Scale_Factor);
	
	    t15 = (1.e0 + 2.e0 * tan2 + eta) / (6.e0 * pow(sn,3) * c * 
	                                        pow(TranMerc_Scale_Factor,3));
	
	    t16 = (5.e0 + 6.e0 * eta + 28.e0 * tan2 - 3.e0 * eta2
	           + 8.e0 * tan2 * eta + 24.e0 * tan4 - 4.e0 
	           * eta3 + 4.e0 * tan2 * eta2 + 24.e0 
	           * tan2 * eta3) / (120.e0 * pow(sn,5) * c  
	                             * pow(TranMerc_Scale_Factor,5));
	
	    t17 = (61.e0 +  662.e0 * tan2 + 1320.e0 * tan4 + 720.e0 
	           * pow(t,6)) / (5040.e0 * pow(sn,7) * c 
	                          * pow(TranMerc_Scale_Factor,7));
	
	    /* Difference in Longitude */
	    dlam = de * t14 - pow(de,3) * t15 + pow(de,5) * t16 - pow(de,7) * t17;
	
	    /* Longitude */
	    ( Longitude ) = TranMerc_Origin_Long + dlam;
	    while ( Latitude > ( 90.0 * M_PI / 180.0 ) )
	    {
	      Latitude = M_PI - Latitude;
	      Longitude += M_PI;
	      if (Longitude > M_PI)
	        Longitude -= (2 * M_PI);
	    }
	
	    while (Latitude < (-90.0 * M_PI / 180.0))
	    {
	      Latitude = - (Latitude + M_PI);
	      Longitude += M_PI;
	      if (Longitude > M_PI)
	        Longitude -= (2 * M_PI);
	    }
	    if (Longitude > (2*M_PI))
	      Longitude -= (2 * M_PI);
	    if (Longitude < -1 * M_PI)
	      Longitude += (2 * M_PI);
	
	    if (fabs(dlam) > (9.0 * M_PI / 180))
	    { /* Distortion will result if Longitude is more than 9 degrees from the Central Meridian */
	      Error_Code |= TRANMERC_LON_WARNING;
	    }
	  }
	  return (Error_Code);
	}
}; // end namespace GPS
