// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Pixels_h
#define Pixels_h

#include "MiroImage.h"

namespace MiroImage 
{
  class pixelGray8;
  class pixelGray16;
  class pixelRealGray;
  class pixelRGB16before15;
  class pixelRGB16behind15;
  class pixelRGB24;
  class pixelRGB24LE;
  class pixelRGB32before24;
  class pixelRGB32behind24;
  class pixelRGB32before24LE;
  class pixelRGB32behind24LE;
  class pixelRealRGB;
  class pixelHSV32;
  class pixelRealHSV;

  const uint8		kMaxComponent8(0xff);
  const uint16	kMaxComponent16(0xffff);
  const realType	kMaxComponentReal(kMaxComponent8);

  typedef enum pixeltype
    {
      unknownPixel,
      gray8Pixel,
      gray16Pixel,
      grayRealPixel,
      rgb16before15Pixel,
      rgb16behind15Pixel,
      rgb24Pixel,
      rgb24LEPixel,
      rgb32before24Pixel,
      rgb32behind24Pixel,
      rgb32before24LEPixel,
      rgb32behind24LEPixel,
      rgbRealPixel,
      hsv32Pixel,
      hsvRealPixel
    } pixeltype;

  class pixelGray8
  {
  public:
    pixelGray8();
    pixelGray8(const uint8&);
    pixelGray8(const pixelGray8& v);
    pixelGray8(const pixelGray16& v);
    pixelGray8(const pixelRGB16before15& v);
    pixelGray8(const pixelRGB16behind15& v);
    pixelGray8(const pixelRGB24& v);
    pixelGray8(const pixelRGB24LE& v);
    pixelGray8(const pixelRGB32before24& v);
    pixelGray8(const pixelRGB32behind24& v);
    pixelGray8(const pixelRGB32before24LE& v);
    pixelGray8(const pixelRGB32behind24LE& v);
    pixelGray8(const pixelHSV32& v);

    pixelGray8&		operator=(const uint8&);
    pixelGray8&		operator=(const pixelGray8& v);
    pixelGray8&		operator=(const pixelGray16& v);
    pixelGray8&		operator=(const pixelRGB16before15& v);
    pixelGray8&		operator=(const pixelRGB16behind15& v);
    pixelGray8&		operator=(const pixelRGB24& v);
    pixelGray8&		operator=(const pixelRGB24LE& v);
    pixelGray8&		operator=(const pixelRGB32before24& v);
    pixelGray8&		operator=(const pixelRGB32behind24& v);
    pixelGray8&		operator=(const pixelRGB32before24LE& v);
    pixelGray8&		operator=(const pixelRGB32behind24LE& v);
    pixelGray8&		operator=(const pixelHSV32& v);
		
    static	pixeltype	pixelType()	{return gray8Pixel;}

    uint8		g;
  };


  class pixelGray16
  {
  public:
    pixelGray16();
    pixelGray16(const uint16&);
    pixelGray16(const pixelGray8& v);
    pixelGray16(const pixelGray16& v);
    pixelGray16(const pixelRGB24& v);
    pixelGray16(const pixelRGB32before24& v);

    pixelGray16&		operator=(const uint16&);
    pixelGray16&		operator=(const pixelGray8& v);
    pixelGray16&		operator=(const pixelGray16& v);
    pixelGray16&		operator=(const pixelRGB24& v);
    pixelGray16&		operator=(const pixelRGB32before24& v);

    static	pixeltype	pixelType()	{return gray16Pixel;}

    uint16		g;
  };

  class pixelRealGray
  {
  public:
    pixelRealGray();
    pixelRealGray(const shortRealType&);
    pixelRealGray(const pixelGray8& v);
    pixelRealGray(const pixelGray16& v);
    pixelRealGray(const pixelRealGray& v);
    pixelRealGray(const pixelRGB24& v);
    pixelRealGray(const pixelRGB32before24& v);
    pixelRealGray(const pixelRealRGB& v);
    pixelRealGray(const pixelHSV32& v);
    //	pixelRealGray(const pixelRealHSV& v);

    pixelRealGray&		operator=(const shortRealType&);
    pixelRealGray&		operator=(const pixelGray8& v);
    pixelRealGray&		operator=(const pixelGray16& v);
    pixelRealGray&		operator=(const pixelRealGray& v);
    pixelRealGray&		operator=(const pixelRGB24& v);
    pixelRealGray&		operator=(const pixelRGB32before24& v);
    pixelRealGray&		operator=(const pixelRealRGB& v);
    pixelRealGray&		operator=(const pixelHSV32& v);
    //	pixelRealGray&		operator=(const pixelRealHSV& v);

    static	pixeltype	pixelType()	{return grayRealPixel;}

    shortRealType		g;
  };


  class pixelRGB16before15
  {
  public:
    pixelRGB16before15();
    pixelRGB16before15(const uint16&);
    pixelRGB16before15(const pixelGray8& v);
    pixelRGB16before15(const pixelRGB16before15& v);
    pixelRGB16before15(const pixelRGB24& v);
    pixelRGB16before15(const pixelRGB32before24& v);

    pixelRGB16before15&		operator=(const uint16&);
    pixelRGB16before15&		operator=(const pixelGray8& v);
    pixelRGB16before15&		operator=(const pixelRGB16before15& v);
    pixelRGB16before15&		operator=(const pixelRGB24& v);
    pixelRGB16before15&		operator=(const pixelRGB32before24& v);

    static	pixeltype	pixelType()	{return rgb16before15Pixel;}

    uint16		pad:1;
    uint16		r:5;
    uint16		g:5;
    uint16		b:5;
  };


  class pixelRGB16behind15
  {
  public:
    pixelRGB16behind15();
    pixelRGB16behind15(const uint16&);
    pixelRGB16behind15(const pixelGray8& v);
    pixelRGB16behind15(const pixelRGB16behind15& v);
    pixelRGB16behind15(const pixelRGB24& v);
    pixelRGB16behind15(const pixelRGB32before24& v);

    pixelRGB16behind15&		operator=(const uint16&);
    pixelRGB16behind15&		operator=(const pixelGray8& v);
    pixelRGB16behind15&		operator=(const pixelRGB16behind15& v);
    pixelRGB16behind15&		operator=(const pixelRGB24& v);
    pixelRGB16behind15&		operator=(const pixelRGB32before24& v);

    static	pixeltype	pixelType()	{return rgb16behind15Pixel;}

    uint16		r:5;
    uint16		g:5;
    uint16		b:5;
    uint16		pad:1;
  };


  class pixelRGB24
  {
  public:
    pixelRGB24();
    pixelRGB24(uint8, uint8, uint8);
    pixelRGB24(const pixelGray8& v);
    pixelRGB24(const pixelGray16& v);
    pixelRGB24(const pixelRGB16before15& v);
    pixelRGB24(const pixelRGB16behind15& v);
    pixelRGB24(const pixelRGB24& v);
    pixelRGB24(const pixelRGB24LE& v);
    pixelRGB24(const pixelRGB32before24& v);
    pixelRGB24(const pixelRGB32behind24& v);
    pixelRGB24(const pixelRGB32before24LE& v);
    pixelRGB24(const pixelRGB32behind24LE& v);
    pixelRGB24(const pixelHSV32& v);

    pixelRGB24&		operator=(const pixelGray8& v);
    pixelRGB24&		operator=(const pixelGray16& v);
    pixelRGB24&		operator=(const pixelRGB16before15& v);
    pixelRGB24&		operator=(const pixelRGB16behind15& v);
    pixelRGB24&		operator=(const pixelRGB24& v);
    pixelRGB24&		operator=(const pixelRGB24LE& v);
    pixelRGB24&		operator=(const pixelRGB32before24& v);
    pixelRGB24&		operator=(const pixelRGB32behind24& v);
    pixelRGB24&		operator=(const pixelRGB32before24LE& v);
    pixelRGB24&		operator=(const pixelRGB32behind24LE& v);
    pixelRGB24&		operator=(const pixelHSV32& v);

    static	pixeltype	pixelType()	{return rgb24Pixel;}

    uint8		r;
    uint8		g;
    uint8		b;
  };


  class pixelRGB24LE
  {
  public:
    pixelRGB24LE();
    pixelRGB24LE(const pixelGray8& v);
    pixelRGB24LE(const pixelRGB24& v);
    pixelRGB24LE(const pixelRGB24LE& v);
    pixelRGB24LE(const pixelRGB32before24& v);

    pixelRGB24LE&		operator=(const pixelGray8& v);
    pixelRGB24LE&		operator=(const pixelRGB24& v);
    pixelRGB24LE&		operator=(const pixelRGB24LE& v);
    pixelRGB24LE&		operator=(const pixelRGB32before24& v);

    static	pixeltype	pixelType()	{return rgb24LEPixel;}

    uint8		b;
    uint8		g;
    uint8		r;
  };


  class pixelRGB32before24
  {
  public:
    pixelRGB32before24();
    pixelRGB32before24(uint8, uint8, uint8);
    pixelRGB32before24(const uint32&);
    pixelRGB32before24(const pixelGray8& v);
    pixelRGB32before24(const pixelGray16& v);
    pixelRGB32before24(const pixelRGB16before15& v);
    pixelRGB32before24(const pixelRGB16behind15& v);
    pixelRGB32before24(const pixelRGB24& v);
    pixelRGB32before24(const pixelRGB24LE& v);
    pixelRGB32before24(const pixelRGB32before24& v);
    pixelRGB32before24(const pixelRGB32behind24& v);
    pixelRGB32before24(const pixelRGB32before24LE& v);
    pixelRGB32before24(const pixelRGB32behind24LE& v);
    pixelRGB32before24(const pixelHSV32& v);

    pixelRGB32before24&		operator=(const uint32&);
    pixelRGB32before24&		operator=(const pixelGray8& v);
    pixelRGB32before24&		operator=(const pixelGray16& v);
    pixelRGB32before24&		operator=(const pixelRGB16before15& v);
    pixelRGB32before24&		operator=(const pixelRGB16behind15& v);
    pixelRGB32before24&		operator=(const pixelRGB24& v);
    pixelRGB32before24&		operator=(const pixelRGB24LE& v);
    pixelRGB32before24&		operator=(const pixelRGB32before24& v);
    pixelRGB32before24&		operator=(const pixelRGB32behind24& v);
    pixelRGB32before24&		operator=(const pixelRGB32before24LE& v);
    pixelRGB32before24&		operator=(const pixelRGB32behind24LE& v);
    pixelRGB32before24&		operator=(const pixelHSV32& v);

    static	pixeltype	pixelType()	{return rgb32before24Pixel;}

    uint32		pad:8;
    uint32		r:8;
    uint32		g:8;
    uint32		b:8;
  };


  class pixelRGB32behind24
  {
  public:
    pixelRGB32behind24();
    pixelRGB32behind24(const uint32&);
    pixelRGB32behind24(const pixelGray8& v);
    pixelRGB32behind24(const pixelRGB24& v);
    pixelRGB32behind24(const pixelRGB32before24& v);
    pixelRGB32behind24(const pixelRGB32behind24& v);

    pixelRGB32behind24&		operator=(const uint32&);
    pixelRGB32behind24&		operator=(const pixelGray8& v);
    pixelRGB32behind24&		operator=(const pixelRGB24& v);
    pixelRGB32behind24&		operator=(const pixelRGB32before24& v);
    pixelRGB32behind24&		operator=(const pixelRGB32behind24& v);

    static	pixeltype	pixelType()	{return rgb32behind24Pixel;}

    uint32		r:8;
    uint32		g:8;
    uint32		b:8;
    uint32		pad:8;
  };


  class pixelRGB32before24LE
  {
  public:
    pixelRGB32before24LE();
    pixelRGB32before24LE(const uint32&);
    pixelRGB32before24LE(const pixelGray8& v);
    pixelRGB32before24LE(const pixelRGB24& v);
    pixelRGB32before24LE(const pixelRGB32before24& v);
    pixelRGB32before24LE(const pixelRGB32before24LE& v);

    pixelRGB32before24LE&		operator=(const uint32&);
    pixelRGB32before24LE&		operator=(const pixelGray8& v);
    pixelRGB32before24LE&		operator=(const pixelRGB24& v);
    pixelRGB32before24LE&		operator=(const pixelRGB32before24& v);
    pixelRGB32before24LE&		operator=(const pixelRGB32before24LE& v);

    static	pixeltype	pixelType()	{return rgb32before24LEPixel;}

    uint32		pad:8;
    uint32		b:8;
    uint32		g:8;
    uint32		r:8;
  };


  class pixelRGB32behind24LE
  {
  public:
    pixelRGB32behind24LE();
    pixelRGB32behind24LE(const uint32&);
    pixelRGB32behind24LE(const pixelGray8& v);
    pixelRGB32behind24LE(const pixelRGB24& v);
    pixelRGB32behind24LE(const pixelRGB32before24& v);
    pixelRGB32behind24LE(const pixelRGB32behind24LE& v);

    pixelRGB32behind24LE&		operator=(const uint32&);
    pixelRGB32behind24LE&		operator=(const pixelGray8& v);
    pixelRGB32behind24LE&		operator=(const pixelRGB24& v);
    pixelRGB32behind24LE&		operator=(const pixelRGB32before24& v);
    pixelRGB32behind24LE&		operator=(const pixelRGB32behind24LE& v);

    static	pixeltype	pixelType()	{return rgb32behind24LEPixel;}

    uint32		b:8;
    uint32		g:8;
    uint32		r:8;
    uint32		pad:8;
  };


  class pixelRealRGB
  {
  public:
    pixelRealRGB();
    pixelRealRGB(const shortRealType&, const shortRealType&, const shortRealType&);
    pixelRealRGB(const pixelGray8& v);
    pixelRealRGB(const pixelGray16& v);
    pixelRealRGB(const pixelRealGray& v);
    pixelRealRGB(const pixelRGB24& v);
    pixelRealRGB(const pixelRGB32before24& v);
    pixelRealRGB(const pixelRealRGB& v);
    pixelRealRGB(const pixelHSV32& v);
    //	pixelRealRGB(const pixelRealHSV& v);

    pixelRealRGB&		operator=(const pixelGray8& v);
    pixelRealRGB&		operator=(const pixelGray16& v);
    pixelRealRGB&		operator=(const pixelRealGray& v);
    pixelRealRGB&		operator=(const pixelRGB24& v);
    pixelRealRGB&		operator=(const pixelRGB32before24& v);
    pixelRealRGB&		operator=(const pixelRealRGB& v);
    pixelRealRGB&		operator=(const pixelHSV32& v);
    //	pixelRealRGB&		operator=(const pixelRealHSV& v);

    static	pixeltype	pixelType()	{return rgbRealPixel;}

    shortRealType		r;
    shortRealType		g;
    shortRealType		b;
  };

  class pixelHSV32
  {
  public:
    pixelHSV32();
    pixelHSV32(const uint32&);
    pixelHSV32(uint16, uint8, uint8);
    pixelHSV32(const pixelGray8& v);
    pixelHSV32(const pixelRGB24& v);
    pixelHSV32(const pixelRGB32before24& v);
    pixelHSV32(const pixelHSV32& v);

    pixelHSV32&		operator=(const uint32&);
    pixelHSV32&		operator=(const pixelGray8& v);
    pixelHSV32&		operator=(const pixelRGB24& v);
    pixelHSV32&		operator=(const pixelRGB32before24& v);
    pixelHSV32&		operator=(const pixelHSV32& v);
		
    realType	hue();
    uint16		integerHue();

    static	pixeltype	pixelType()	{return hsv32Pixel;}

    static void hsv2rgb(uint8& r, uint8& g, uint8& b, const pixelHSV32&);
    static void rgb2hsv(pixelHSV32&, uint8 r, uint8 g, uint8 b);

    uint32		h:16;
    uint32		s:8;
    uint32		v:8;
  };

  typedef class pixelGray8			gray;
  typedef class pixelRGB24			rgb24;
  //	typedef class pixelRGB24			rgb;
  typedef class pixelRGB32before24	rgb32;
  //	typedef class pixelHSV24			hsv;
  typedef class pixelHSV32			hsv32;

  class pixelLookupManager
  {
  public:
    pixelLookupManager();
    ~pixelLookupManager();
		
    realType	byte2Real(byte);
    realType	rgb2RealGray(byte, byte, byte);
    byte		rgb2Gray(byte, byte, byte);

    realType*	byte2RealLookup;

    realType*	realRedLookup;
    realType*	realGreenLookup;
    realType*	realBlueLookup;

    byte*		redLookup;
    byte*		greenLookup;
    byte*		blueLookup;
  };

  extern	pixelLookupManager	pixelLookup;

#include "pixels.ih"

  const	rgb24	colorBlackRGB24(0, 0, 0);
  const	rgb24	colorWhiteRGB24(kMaxComponent8, kMaxComponent8, kMaxComponent8);

  const	rgb32	colorBlackRGB32(0, 0, 0);
  const	rgb32	colorWhiteRGB32(kMaxComponent8, kMaxComponent8, kMaxComponent8);

  const	hsv32	colorBlackHSV(0, 0, 0);
  const	hsv32	colorWhiteHSV(0, 0, kMaxComponent8);

} // namespace MiroImage

#endif // PIXELS_H
