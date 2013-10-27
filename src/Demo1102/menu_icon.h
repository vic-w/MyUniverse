#pragma once
#include "GlbCore.h"
#include <map>
 
using namespace std;

class CMenuIcon
{
private:
	map<string, GlbImage> m_icons;
	map<string, GlbSize> m_size;
public:
	CMenuIcon();
	~CMenuIcon();
	GlbImage getImage(string name, GlbSize &size);
};