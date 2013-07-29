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
    WMV,
    FOLDER,
    NONE
};

class CStoryPage
{
public:
    //页面文件结构
    bool bEmpty;//页面文件为空
    bool bMovie;//页面是一个动画
    CString pagePath;//页面文件的完整路径（目录+文件名）
    STORY_TYPE storyType;//DDS,JPG,PNG,AVI,WMV,FOLDER

    //序列帧专用
    vector<CString> FrameNames;//序列帧（FOLDER）格式时，用来存放每一帧的文件名
    int nFrames;//序列帧的总帧数
    int nCurFrame;//当前的帧号

    //动画控制
    bool bPlaying;//播放动画
    int frameRate;//动画播放速度

    //旋转控制
    bool bRotating;//自转
    bool bClockwise; //自转方向
    int rotationRate;//自转速度
};

DWORD WINAPI GlobeThread(LPVOID lpParam);
DWORD WINAPI TimingThread(LPVOID lpParam);

void DrawStoryPage(GlbWindow mainWindow);
