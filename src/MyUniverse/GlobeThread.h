#pragma once

#include "GlbCore.h"

enum STORY_TYPE
{
    DDS,
    JPG,
    PNG,
    AVI,
    WMA,
    FLODER
};

class CStoryPage
{
public:
    bool bMovie;
    CString pagePath;
    STORY_TYPE storyType;
};

DWORD WINAPI GlobeThread(LPVOID lpParam);
