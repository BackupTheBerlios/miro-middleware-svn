// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "VideoParameters.h"
#include "VideoDevice.h"

#include <miro/TimeHelper.h>

#include <qdom.h>

#include <iostream>
#include <cmath>

namespace Video
{
  using std::ostream;

  Parameters::Parameters() :
    Super()
  {
    device = "/dev/v4l/video0";
	grabber = "bttv";
	format = "pal";		//	PAL
	source = "composite1";		//	Composite1
	palette = "rgb";	//  RGB24
	subfield = "all";	// odd / even
	width = 384;	//	PAL/4
	height = 288;
	connections = 16;
	buffers = 16;
	byteorder = 1;	//	0 == native / 1 == rgb
	pixelSize = 3;  //      size in bytes
	upsideDown=0;   //      0 == normal / 1 == 180 deg turn
  }

  void
  Parameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {        // the node was really a text element.
	    if (n1.nodeName() == "grabber") {
	      grabber = t.data();
	    }
	    if (n1.nodeName() == "format") {
	      format = t.data();
	    }
	    else if (n1.nodeName() == "source") {
	      source = t.data();
	    }
	    else if (n1.nodeName() == "palette") {
	      	palette = t.data() ;
		if ((palette == std::string("rgb")) ||
		    (palette == std::string("bgr"))) 
		  pixelSize=3;
		else if ((palette == std::string("rgba")) ||
			 (palette == std::string("abgr")))
		  pixelSize=4;
		else if (palette == std::string("gray")) 
		  pixelSize=1;

	    }
	    else if (n1.nodeName() == "subfield") {
	      	subfield = t.data() ;
	    }
	    else if (n1.nodeName() == "width") {
	      width = t.data().toInt();
	    }
	    else if (n1.nodeName() == "height") {
	      height = t.data().toInt();
	    }
	    else if (n1.nodeName() == "connections") {
	      connections = t.data().toInt();
	    }
	    else if (n1.nodeName() == "buffers") {
	      buffers = t.data().toInt();
	    }
	    else if (n1.nodeName() == "byteorder") {
	      if (t.data() == "native")
	      	byteorder = 0;
	      else
	      	byteorder = 1;
	    }	    
	    else if (n1.nodeName() == "upsideDown") {
	      if (t.data() == "yes")
		upsideDown=true;
	      else
	      upsideDown=false;
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  ostream&
  operator << (ostream& ostr, const Parameters& desc) 
  {
    ostr << static_cast<const Parameters::Super&>(desc) << endl
	 << "grabber = " << desc.grabber << endl
	 << "format = " << desc.format << endl
	 << "source = " << desc.source << endl
	 << "palette = " << desc.palette << endl
	 << "subfield = " << desc.subfield << endl
	 << "width = " << desc.width << endl
	 << "height = " << desc.height << endl
	 << "connections = " << desc.connections << endl
	 << "buffers = " << desc.connections << endl
	 << "byteorder = ";
	 if (desc.byteorder == 0)
	 	ostr << "native" << endl;
	 else
	 	ostr << "rgb" << endl;
	 ostr << "upsideDown = ";
	 if (desc.upsideDown!=0)
	   ostr << "yes" << endl;
	 else 
	 ostr << "no" << endl;
	 
	return ostr;
  }
};
