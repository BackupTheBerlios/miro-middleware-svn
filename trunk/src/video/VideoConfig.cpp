#include "VideoConfig.h"
#include "miro/Exception.h"

namespace Video
{
  VideoFormat getFormat(const std::string & fmt)
  {
    if (fmt == "auto")	   
      return formatAuto;
    else if (fmt == "pal")
      return formatPAL;
    else if (fmt == "ntsc")
      return formatNTSC;
    else if (fmt == "secam")    
      return formatSECAM;
   
    throw Miro::Exception(string("Video::Parameters: unknown format: ") + fmt);
  }
  
  VideoSource getSource(const std::string & src)
  {
    if (src == "composite1")       
      return sourceComposite1;
    else if (src == "composite2")
      return sourceComposite2;
    else if (src == "composite3")
      return sourceComposite3;
    else if (src == "svideo")
      return sourceSVideo;
    else if (src == "tuner")
      return sourceTuner;
    else if (src == "usb")        
      return sourceUSB;
    else if (src == "1394")	
      return source1394;

    throw Miro::Exception(string("Video::Parameters: unknown source: ") + src);
  }
    
  VideoPalette getPalette(const std::string & pal)
  {
    if ((pal == "grey") ||
	(pal == "gray"))
      return paletteGrey;
    else if (pal == "rgb")	
      return paletteRGB;
    else if (pal == "rgba") 
      return paletteRGBA;
    else if (pal == "bgr")  
      return paletteBGR;
    else if (pal == "abgr") 
      return paletteABGR;
    else if (pal == "yuv")  	
      return paletteYUV;
	
    throw Miro::Exception(string("Video::Parameters: unknown palette: ") + pal);
  }
    
  VideoSubfield getSubfield(const std::string & sub)
  {
    if (sub == "all")	
      return subfieldAll;
    else if (sub == "odd")	
      return subfieldOdd;
    else if (sub == "even")	
      return subfieldEven;
    
    throw Miro::Exception(string("Video::Parameters: unknown subfield: ") + sub);
  }
};
