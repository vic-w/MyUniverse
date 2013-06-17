#pragma once

struct GlbCommand
{
    int command;
    int ctrl1_ret0;
    int iParam1;
    int iParam2;
    double fParam1;
    double fParam2;
    char cParam1[512];
    char cParam2[512];
};


#define GLB_CMD_PLAY            (1)
#define GLB_CMD_PAUSE           (2)