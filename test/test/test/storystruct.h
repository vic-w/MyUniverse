#pragma once
#include <string> 
#include <vector>

using namespace std;

class CStar
{
public:
	int m_ID;
	string m_Path;
	string m_Name;
	int m_bRotating;
	int m_bPlaying;
	int m_RotationRate;
	int m_bRotateClockwise;
	int m_FrameRate;
	int m_nFrames;
public:
	CStar()
	{
		m_ID = 0;
		m_Path = "";
		m_Name = "";
		m_bRotating = 0;
		m_bPlaying = 0;
		m_RotationRate = 2;
		m_bRotateClockwise = 0;
		m_FrameRate = 5;
		m_nFrames = 0;
	}
	//~CStar();
};

class CGalaxy
{
public:
	int m_ID;
	string m_Path;
	string m_Name;
	int m_nStars;
	vector<CStar> m_Stars;
public:
	CGalaxy();
	//~CGalaxy();
};

class CUniverse
{
public:
	string m_Path;
	int m_nGalaxies;
	vector<CGalaxy> m_Galaxies;
public:
	CUniverse(string path);
	//~CUniverse();
};

