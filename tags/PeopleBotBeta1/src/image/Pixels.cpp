#include "Pixels.h"

namespace MiroImage {

pixelLookupManager	pixelLookup;

pixelLookupManager::pixelLookupManager()
	{
	byte2RealLookup = new realType[256];
	for (integerType i=0; i<256; i++)
		byte2RealLookup[i] = (realType)i/255.0;

	realRedLookup = new realType[256];
	for (integerType i=0; i<256; i++)
		realRedLookup[i] = (realType)i/(3*255.0);
	realGreenLookup = new realType[256];
	for (integerType i=0; i<256; i++)
		realGreenLookup[i] = (realType)i/(3*255.0);
	realBlueLookup = new realType[256];
	for (integerType i=0; i<256; i++)
		realBlueLookup[i] = (realType)i/(3*255.0);
	
	redLookup = new byte[256];
	for (integerType i=0; i<256; i++)
		redLookup[i] = i/3;
	greenLookup = new byte[256];
	for (integerType i=0; i<256; i++)
		greenLookup[i] = i/3;
	blueLookup = new byte[256];
	for (integerType i=0; i<256; i++)
		blueLookup[i] = i/3;
	}

pixelLookupManager::~pixelLookupManager()
	{
	delete[] byte2RealLookup;

	delete[] realRedLookup;
	delete[] realGreenLookup;
	delete[] realBlueLookup;

	delete[] redLookup;
	delete[] greenLookup;
	delete[] blueLookup;
	}

//	void pixelHSV32::hsv2rgb(uint8&, uint8&, uint8&, uint16, uint8, uint8)
//	void pixelHSV32::hsv2rgb(uint8& r, uint8& g, uint8& b, uint16 h, uint8 s, uint8 v)
void pixelHSV32::hsv2rgb(uint8& r, uint8& g, uint8& b, const pixelHSV32& pix)
	{
	realType	red, green, blue;
	
	realType	hue = ((realType)pix.h * 360.0)/(realType)kMaxComponent16;
	realType	sat = (realType)pix.s/kMaxComponentReal;
	realType	val = (realType)pix.v/kMaxComponentReal;

	int			i;
	realType	f, p, q, t;


	if (sat == 0)
		red = green = blue = val;
	else
		{
		if (hue == 360)
			hue = 0;
		hue = hue/60;
		
		i = (int)hue;
		f = hue - (double)i;
		p = val * (1 - sat);
		q = val * (1 - (sat * f));
		t = val * (1- (sat * (1 - f)));
		switch (i)
			{
			case 0:
				red = val; green = t; blue = p;
				break;
			case 1:
				red = q; green = val; blue = p;
				break;
			case 2:
				red = p; green = val; blue = t;
				break;
			case 3:
				red = p; green = q; blue = val;
				break;
			case 4:
				red = t; green = p; blue = val;
				break;
			case 5:
			default:
				red = val;
				green = p;
				blue = q;
				break;
			}
		}

	r = (uint8)(red * kMaxComponentReal);
	g = (uint8)(green * kMaxComponentReal);
	b = (uint8)(blue * kMaxComponentReal);
	}
	
//	rgb ==> hsv conversion
void pixelHSV32::rgb2hsv(pixelHSV32& pix, uint8 r, uint8 g, uint8 b)
	{
	long	hue;
	long	saturation, value;
	long	red = r, green = g, blue = b;
	
	long	max = red;
	long	min = red;

	//	find maximum
	if (green > max)
		max = green;
	if (blue > max)
		max = blue;
	
	//	find minimum
	if (green < min)
		min = green;
	if (blue < min)
		min = blue;
	//	we got v
	value = max;
	
	//	now we calculate s
	if ((max != 0) && (max != min))
		{
		long	delta = max - min;

		saturation = (long)((kMaxComponentReal * (double)delta)/(double)max);
		if (red == max)
			hue = (60 * (green - blue))/delta;
		else if (green == max)
			hue = 120 + (60 * (blue - red))/delta;
		else if (blue == max)
			hue = 240 + (60 * (red - green))/delta;

		if (hue < 0)
			hue += 360;
		}
	else
		{
		saturation = 0;
		hue = 0;
		}
	if (hue >= 360)
		hue = 359;

/*
	if (saturation > kMaxComponent8)
		saturation = kMaxComponent8;
	else if (saturation < 0)
		saturation = 0;

	if (value > kMaxComponent8)
		value = kMaxComponent8;
	else if (value < 0)
		value = 0;
*/
	hue = (hue * kMaxComponent16)/ 360;

	pix.h = (uint16)hue;
	pix.s = saturation;
	pix.v = value;
	}
}
