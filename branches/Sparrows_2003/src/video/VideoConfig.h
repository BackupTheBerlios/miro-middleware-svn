/*! \file VideoConfig.h
 *  \brief Defininitions and constants for the VideoDevice Settings
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.1  2003/03/09 17:01:43  muehlenf
 * *** empty log message ***
 *
 *
 */

#ifndef VIDEOCONFIG_H
#define VIDEOCONFIG_H

#include <string>

namespace Video
{
    enum VideoFormat
    {
        formatAuto = 0,
        formatPAL,
        formatNTSC,
        formatSECAM
    };

#define numFormatEntries  (formatSECAM + 1)

    enum VideoPalette
    {
        paletteGrey = 0,
        paletteRGB,
        paletteRGBA,
        paletteBGR,
        paletteABGR,
        paletteYUV
    };

#define numPaletteEntries  (paletteYUV + 1)

    enum VideoSubfield
    {
        subfieldAll = 0,
        subfieldOdd,
        subfieldEven
    };

#define numSubfieldEntries  (subfieldEven + 1)

    enum VideoSource
    {
        sourceComposite1 = 0,
        sourceComposite2,
        sourceComposite3,
        sourceSVideo,
        sourceTuner,
        sourceUSB,
        source1394
    };

#define numSourceEntries  (source1394 + 1)

    VideoFormat getFormat(const std::string & fmt);
    VideoSource getSource(const std::string & src);
    VideoPalette getPalette(const std::string & pal);
    VideoSubfield getSubfield(const std::string & sub);
};

#endif /* VIDEOCONFIG_H */


