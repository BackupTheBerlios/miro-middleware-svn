#ifndef platform_compatibility_h
#define platform_compatibility_h

// temporary platform compatiblity file until
// a better one can be created as part of build
// system (i.e. 'config.h' style)

#ifdef WIN32

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

#ifndef M_1_PI
#define M_1_PI (1.0/M_PI)
#endif // M_1_PI

#endif // WIN32

#endif // win32compat_h


