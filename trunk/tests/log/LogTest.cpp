// -*- c++ -*-
///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "miro/IO.h"
#include "miro/Log.h"


#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int
main(int argc, char * argv[])
{
  if (!::Miro::Log::compiledWithLog())
  {
    cerr << "Error: Miro not compiled with Log support\n";
    return 1;
  }
  if (!::Miro::Log::compiledWithDebug())
  {
    cerr << "Error: Miro not compiled with Debug support\n";
    return 1;
  }

  ::Miro::Log::mask(0xFFFFFFFF);
  ::Miro::Log::level(::Miro::Log::MAX_DBG_LEVEL);

  cout << "Testing log levels\n";
  MIRO_LOG(LL_EMERGENCY,"MIRO_LOG LL_EMERGENCY");
  MIRO_LOG(LL_ALERT,"MIRO_LOG LL_ALERT");
  MIRO_LOG(LL_CRITICAL,"MIRO_LOG LL_CRITICAL");
  MIRO_LOG(LL_ERROR,"MIRO_LOG LL_ERROR");
  MIRO_LOG(LL_WARNING,"MIRO_LOG LL_WARNING");
  MIRO_LOG(LL_NOTICE,"MIRO_LOG LL_NOTICE");

  cout << "Testing debug levels\n";
  MIRO_DBG(MIRO,LL_EMERGENCY,"MIRO_DBG LL_EMERGENCY");
  MIRO_DBG(MIRO,LL_ALERT,"MIRO_DBG LL_ALERT");
  MIRO_DBG(MIRO,LL_CRITICAL,"MIRO_DBG LL_CRITICAL");
  MIRO_DBG(MIRO,LL_ERROR,"MIRO_DBG LL_ERROR");
  MIRO_DBG(MIRO,LL_WARNING,"MIRO_DBG LL_WARNING");
  MIRO_DBG(MIRO,LL_NOTICE,"MIRO_DBG LL_NOTICE");
  MIRO_DBG(MIRO,LL_CTOR_DTOR,"MIRO_DBG LL_CTOR_DTOR");
  MIRO_DBG(MIRO,LL_DEBUG,"MIRO_DBG LL_DEBUG");
  MIRO_DBG(MIRO,LL_TRACE,"MIRO_DBG LL_TRACE");
  MIRO_DBG(MIRO,LL_PRATTLE,"MIRO_DBG LL_PRATTLE");

  cout << "Testing debug categories\n";
  MIRO_DBG(MIRO,LL_EMERGENCY,"MIRO");
  MIRO_DBG(VIDEO,LL_EMERGENCY,"VIDEO");
  MIRO_DBG(PSOS,LL_EMERGENCY,"PSOS");
  MIRO_DBG(SPHINX,LL_EMERGENCY,"SPHINX");
  MIRO_DBG(PIONEER,LL_EMERGENCY,"PIONEER");
  MIRO_DBG(CAN,LL_EMERGENCY,"CAN");
  MIRO_DBG(FAUL,LL_EMERGENCY,"FAUL");
  MIRO_DBG(SPARROW,LL_EMERGENCY,"SPARROW");
  MIRO_DBG(MCP,LL_EMERGENCY,"MCP");
  MIRO_DBG(ABUS,LL_EMERGENCY,"ABUS");
  MIRO_DBG(MSP,LL_EMERGENCY,"MSP");
  MIRO_DBG(SICK,LL_EMERGENCY,"SICK");
  MIRO_DBG(DTLK,LL_EMERGENCY,"DTLK");
  MIRO_DBG(DP,LL_EMERGENCY,"DP");
  MIRO_DBG(B21,LL_EMERGENCY,"B21");

  cout << "Testing category/loglevel masking\n";
  ::Miro::Log::mask(::Miro::Log::SPARROW);
  ::Miro::Log::level(::Miro::Log::LL_ERROR);
  MIRO_LOG(LL_ERROR, "Log: LL_ERROR (should appear)");
  MIRO_LOG(LL_WARNING, "Log: LL_WARNING (should not appear)");
  MIRO_DBG(B21, LL_ERROR, "Dbg: B21 LL_ERROR (should not appear)");
  MIRO_DBG(B21, LL_WARNING, "Dbg: B21 LL_WARNING (should not appear)");
  MIRO_DBG(SPARROW, LL_ERROR, "Dbg: SPARROW LL_ERROR (should appear)");
  MIRO_DBG(SPARROW, LL_WARNING, "Dbg: SPARROW LL_WARNING (should not appear)");
  cout << "Two messages should have appeared\n";

  return 0;
}
