// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_BuiltIn_h
#define Miro_BuiltIn_h

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if GCC_MAJOR_VERSION >= 3
#define MIRO_PREFETCH(x, y, z) __builtin_prefetch(x, y, z)
#define MIRO_LIKELY(c)         __builtin_expect(!!(c),1)
#define MIRO_UNLIKELY(c)       __builtin_expect(!!(c),0)
#else 
#define MIRO_PREFETCH(x, y, z)
#define MIRO_LIKELY(c)
#define MIRO_UNLIKELY(c)
#endif

#endif // Miro_BuiltIn_h
