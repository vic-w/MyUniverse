#pragma once

#include <vector>
using std::vector;

class CCity
{
public:
	CCity(void);
	~CCity(void);
	int id;
	char* name;
	char* displayname;
	char* country;
	bool displaycountry; //true: 显示城市所在国家的信息；false: 显示城市的信息
	
	char* folder; //城市图集文件夹
	char* imgPath; //城市图片的相对路径
	vector<vector<char*>> images;



	float latitude; //[-90, 90]
	float longitude; //[-180, 180]
	float timezone; //[-12, 12]
	int population;
	char* description;

	char* temprature; //当前温度
	char* weatherCondition; //当前天气描述


	char* getLocalTimeString(); //返回当地的时间
	static char* getTimezoneDiffString(CCity city1, CCity city2); //返回两个城市之间的时差
	static char* getTimezoneDiffString(CCity city1); //返回两个城市之间的时差， 如果只有一个参数，则返回该城市与北京的时间差

	static vector<CCity> getCities();
	static bool updateXml();
};