
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl MIRO
// ------------------------------
#ifndef miro_Export_H
#define miro_Export_H

#include "ace/config-all.h"

#if !defined (MIRO_HAS_DLL)
#  define MIRO_HAS_DLL 1
#endif /* ! MIRO_HAS_DLL */

#if defined (MIRO_HAS_DLL) && (MIRO_HAS_DLL == 1)
#  if defined (MIRO_BUILD_DLL)
#    define miro_Export ACE_Proper_Export_Flag
#    define MIRO_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MIRO_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MIRO_BUILD_DLL */
#    define miro_Export ACE_Proper_Import_Flag
#    define MIRO_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MIRO_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MIRO_BUILD_DLL */
#else /* MIRO_HAS_DLL == 1 */
#  define miro_Export
#  define MIRO_SINGLETON_DECLARATION(T)
#  define MIRO_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MIRO_HAS_DLL == 1 */

// Set MIRO_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MIRO_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MIRO_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MIRO_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MIRO_NTRACE */

#if (MIRO_NTRACE == 1)
#  define MIRO_TRACE(X)
#else /* (MIRO_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define MIRO_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (MIRO_NTRACE == 1) */

#endif /* miro_Export_H */

// End of auto generated file.
