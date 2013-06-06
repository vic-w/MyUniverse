#pragma once

#include "GlbCore.h"
#include <vector>

using std::vector;

enum STORY_TYPE
{
    DDS,
    JPG,
    PNG,
    AVI,
    WMA,
    FOLDER
};

class CStoryPage
{
public:
    bool bEmpty;
    bool bMovie;
    CString pagePath;
    STORY_TYPE storyType;
    vector<CString> FrameNames;
    int nFrames;
    int nCurFrame;
};

DWORD WINAPI GlobeThread(LPVOID lpParam);

void DrawStoryPage();
