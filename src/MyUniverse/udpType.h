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

const int GLB_CMD_PLAY                      = 1;
const int GLB_CMD_PAUSE                     = 2;

const int GLB_CMD_GET_ROTATECLOCKWISE       = 3;
const int GLB_CMD_SET_ROTATECLOCKWISE       = 4;

const int GLB_CMD_GET_ROTATING              = 5;
const int GLB_CMD_SET_ROTATING              = 6;

const int GLB_CMD_GET_ROTATERATE            = 7;
const int GLB_CMD_SET_ROTATERATE            = 8;

const int GLB_CMD_GET_POLELATITUDE          = 9;
const int GLB_CMD_SET_POLELATITUDE          = 10;

const int GLB_CMD_GET_POLELONGITUDE         = 11;
const int GLB_CMD_SET_POLELONGITUDE         = 12;

const int GLB_CMD_GET_CHAPTERANDPAGENAME    = 13;
const int GLB_CMD_SET_CHAPTERANDPAGENAME    = 14;
