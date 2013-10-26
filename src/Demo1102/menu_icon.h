#pragma once
#include "GlbCore.h"
#include <map>
 
using namespace std;

class CMenuIcon
{
private:
	map<string, GlbImage> m_icons;
public:
	CMenuIcon();
	~CMenuIcon();
	GlbImage getImage(string name);
};