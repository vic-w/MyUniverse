#ifdef GLBTOUCH_EXPORTS
#define GLBTOUCH_API __declspec(dllexport)
#else
#define GLBTOUCH_API __declspec(dllimport)
#endif

#include "GlbCoreType.h"

GLBTOUCH_API void glbListenTouchSignal(GlbWindow &window, int port);