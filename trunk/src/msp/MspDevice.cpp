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


#include "msp/MspDevice.h"

namespace Msp
{
  const unsigned long sonarAddr[25] = 
  {
    0x2011,0x2012,0x2013,0x2014,  // msp 0 chain 1
    0x2101,0x2102,0x2103,0x2104,  // msp 1 chain 0
    0x2111,0x2112,0x2113,0x2114,  // msp 1 chain 1
    0x2201,0x2202,0x2203,0x2204,  // msp 2 chain 0
    0x2211,0x2212,0x2213,0x2214,  // msp 2 chain 1
    0x2001,0x2002,0x2003,0x2004,  // msp 0 chain 0
    0x0000
  };

  const unsigned long mspAddr[] = 
  {
    0x20, 0x21, 0x22, 0x30, 0x31, 0x32, 0x33, 0x00
  };

  const int irNum2Row[IR_MSPS][IR_PER_MSP] = 
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 }, // msp 0, enclusure 
    { 0, 0, 0, 0, 0, 0, 0, 0 }, // msp 1, enclusure 
    { 0, 0, 0, 0, 0, 0, 0, 0 }, // msp 2, enclusure 
    { 1, 1, 2, 1, 1, 1, 2, 1 }, // msp 3, base 
    { 1, 1, 2, 1, 1, 1, 2, 1 }, // msp 4, base 
    { 1, 1, 2, 1, 1, 1, 2, 1 }, // msp 5, base 
    { 1, 1, 2, 1, 1, 1, 2, 1 }  // msp 6, base 
  };

  const int irNum2Col[IR_MSPS][IR_PER_MSP] = 
  {
    { 20, 21, 22, 23,  0,  1,  2,  3 }, // msp 0, enclosure: row 0 
    {  4,  5,  6,  7,  8,  9, 10, 11 }, // msp 1, enclosure: row 0 
    { 12, 13, 14, 15, 16, 17, 18, 19 }, // msp 2, enclosure: row 0 
    { 23,  0,  0,  1,  2,  3,  1,  4 }, // msp 3, row 1, 1, 2, 1, 1, 1, 2, 1 
    {  5,  6,  2,  7,  8,  9,  3, 10 }, // map 4, row 1, 1, 2, 1, 1, 1, 2, 1 
    { 11, 12,  4, 13, 14, 15,  5, 16 }, // map 5, row 1, 1, 2, 1, 1, 1, 2, 1 
    { 17, 18,  6, 19, 20, 21,  7, 22 }  // map 6, row 1, 1, 2, 1, 1, 1, 2, 1 
  };

  const int tactileNum2Row[TACTILE_MSPS][TACTILE_PER_MSP] = 
  {
    { 0, 1, 0, 1, 0, 1, 0, 1 }, // msp 0, enclusure 
    { 0, 1, 0, 1, 0, 1, 0, 1 }, // msp 1, enclusure 
    { 0, 1, 0, 1, 0, 1, 0, 1 }, // msp 2, enclusure 
    { 2, 3, 2, 3, 2, 3, 2, 3 }, // msp 3, base 
    { 2, 3, 2, 3, 2, 3, 2, 3 }, // msp 4, base 
    { 2, 3, 2, 3, 2, 3, 2, 3 }, // msp 5, base 
    { 2, 3, 2, 3, 2, 3, 2, 3 }	// msp 6, base 
  };

  const int tactileNum2Col[TACTILE_MSPS][TACTILE_PER_MSP] = 
  {
    { 10, 10, 11, 11,  0,  0,  1,  1 }, // msp 0, enclosure 
    {  2,  2,  3,  3,  4,  4,  5,  5 }, // msp 1, enclosure 
    {  6,  6,  7,  7,  8,  8,  9,  9 }, // msp 2, enclosure 
    { 15, 15,  0,  0,  1,  1,  2,  2 }, // msp 3, base 
    {  3,  3,  4,  4,  5,  5,  6,  6 }, // map 4, base 
    {  7,  7,  8,  8,  9,  9, 10, 10 }, // map 5, base 
    { 11, 11, 12, 12, 13, 13, 14, 14 }  // map 6, base 
  };
};
