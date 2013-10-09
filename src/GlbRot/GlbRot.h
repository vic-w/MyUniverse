#pragma once

#include <vector>

using namespace std;

#ifdef GLBROT_EXPORTS
#define GLBROT_API __declspec(dllexport)
#else
#define GLBROT_API __declspec(dllimport)
#endif

#define PI (3.1415926f)
//#define N_CALIB  (12) //标定纬线的条数


class GlbPointGeo   
{ 
public:
    float m_lat;//维度
    float m_lng;//经度
    GlbPointGeo()
    {
        m_lat = 0;
        m_lng = 0;
    }
    GlbPointGeo(float lat, float lng)
    {
        m_lat = lat;
        m_lng = lng;
    }
    GlbPointGeo &operator =(GlbPointGeo p)    //要修改对象数据成员，返回值必须为对象的引用
    {
        m_lat = p.m_lat;
        m_lng = p.m_lng;
        return *this;            //返回修改后的左值对象
    }
	bool operator==(GlbPointGeo p)
	{
		return (m_lat == p.m_lat) && (m_lng == p.m_lng);
	}	
	
	bool operator!=(GlbPointGeo p)
	{
		return (m_lat != p.m_lat) || (m_lng != p.m_lng);
	}
}; 

class GlbPoint2d   
{ 
public:
    float m_x;
    float m_y;
    GlbPoint2d()
    {
        m_x = 0;
        m_y = 0;
    }
    GlbPoint2d(float x, float y)
    {
        m_x = x;
        m_y = y;
    }
    GlbPoint2d &operator =(GlbPoint2d p)    //要修改对象数据成员，返回值必须为对象的引用
    {
        m_x = p.m_x;
        m_y = p.m_y;
        return *this;            //返回修改后的左值对象
    }
}; 

class GlbPoint3d 
{ 
public:
    float m_x;
    float m_y;
    float m_z;
    GlbPoint3d()
    {
        m_x = 0;
        m_y = 0;
        m_z = 0;
    }
    GlbPoint3d(float x, float y, float z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }
    GlbPoint3d &operator =(GlbPoint3d p)    //要修改对象数据成员，返回值必须为对象的引用
    {
        m_x = p.m_x;
        m_y = p.m_y;
        m_z = p.m_z;
        return *this;            //返回修改后的左值对象
    }
    GlbPoint3d &operator +=(GlbPoint3d p)    //要修改对象数据成员，返回值必须为对象的引用
    {
        m_x += p.m_x;
        m_y += p.m_y;
        m_z += p.m_z;
        return *this;            //返回修改后的左值对象
    }	

    GlbPoint3d &operator *=(float f)    //要修改对象数据成员，返回值必须为对象的引用
    {
        m_x *= f;
        m_y *= f;
        m_z *= f;
        return *this;            //返回修改后的左值对象
    }    
    
    GlbPoint3d operator -()
    {
        GlbPoint3d minusPoint;
        minusPoint.m_x = -m_x;
        minusPoint.m_y = -m_y;
        minusPoint.m_z = -m_z;
        return minusPoint;
    }
}; 

class GlbCalib
{
    #define N_CALIB  (12) //标定纬线的条数
public:
    float latitude[N_CALIB+1];
};


typedef GlbPoint3d GlbPivot;
typedef GlbPoint2d GlbPointRound;

class GlbEularAngle
{
public:
    //旋转顺序严格按照此结构中的顺序（欧拉角->旋转矩阵则按照z-x-z的顺序）
    float m_1_Horz; //地球水平方向旋转
    float m_2_Vert; //地球沿0度经线的旋转
    float m_3_Axis; //地球轴向旋转
};

class GlbRotmat 
{ 
public:
    float r11;
    float r12;
    float r13;
    float r21;
    float r22;
    float r23;
    float r31;
    float r32;
    float r33;
}; 

class CMove
{
public:
    GlbPoint2d m_pFrom;
    GlbPoint2d m_pTo;
    long m_nTrackID;
};

//计算向量的长度（模）
GLBROT_API float glbVectorNorm(GlbPoint3d Vec);
//将向量变为单位向量
GLBROT_API void glbVectorNormalize(GlbPoint3d Vec, GlbPoint3d &Vec_dst);
//两个向量的点乘
GLBROT_API float glbDotMul(GlbPoint3d Vec1, GlbPoint3d Vec2);
//两个向量的叉乘
GLBROT_API void glbCrossMul(GlbPoint3d Vec1, GlbPoint3d Vec2, GlbPoint3d &VecDST);
//两个旋转矩阵的相乘
//第一个参数是将要做的旋转，第二个参数是原始的旋转矩阵，第三个参数是旋转后的矩阵
//Rotation × Origin = destination
GLBROT_API void glbRotmatMul(GlbRotmat Rotation, GlbRotmat Origin, GlbRotmat &destination);

//经纬坐标 转换为 直角坐标
GLBROT_API void glbPointGeo2PointRect(GlbPointGeo pGeo, GlbPoint3d &pRect);
//直角坐标 转换为 经纬坐标
GLBROT_API void glbPointRect2PointGeo(GlbPoint3d pRect, GlbPointGeo &pGeo);

//地球上的经纬坐标 转换为 屏幕上的经纬坐标
GLBROT_API void glbGlobePoint2ScreenPoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);
//屏幕上的经纬坐标 转换为 地球上的经纬坐标
GLBROT_API void glbScreenPoint2GlobePoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);

//直角坐标 转换为 圆饼坐标
GLBROT_API void glbPointRect2PointRound(GlbPoint3d p3, GlbPointRound &p2, GlbCalib calib);
//圆饼坐标 转换为 直角坐标
GLBROT_API void glbPointRound2PointRect(GlbPointRound p2, GlbPoint3d &p3, GlbCalib calib);

//生成初始旋转矩阵
GLBROT_API void glbCreateGlbRotmat(GlbRotmat &r);
//复制旋转矩阵
GLBROT_API void glbCloneGlbRotmat(GlbRotmat r, GlbRotmat &r_dst);

//计算屏幕上的点p1到p2的旋转角度
GLBROT_API float glbAngleBetweenPoints(GlbPoint3d p1, GlbPoint3d p2);
//计算屏幕上的点p1到p2的旋转轴
GLBROT_API void glbPivotBetweenPoints(GlbPoint3d p1, GlbPoint3d p2, GlbPivot &pivot);
//计算球面上∠ABC的值（以B为顶点的角度,方向为站在地球表面向右转的角度,取值范围0~360）
GLBROT_API float glbAngleABC(GlbPointGeo A, GlbPointGeo B, GlbPointGeo C);
//计算球面上多边形所占的立体角Ω（顺时针方向）,取值范围0~2*PI（乘以半径的平方即可得到多边形的面积）
GLBROT_API float glbGetSteradian(vector<GlbPointGeo> polygon);
//判断球面上两段劣弧AB和CD是否相交
GLBROT_API bool glbArcsIntersect(GlbPointGeo A, GlbPointGeo B, GlbPointGeo C, GlbPointGeo D);

//欧拉角 转换为 旋转矩阵
GLBROT_API void glbEularAngle2Rotmat(GlbEularAngle angle, GlbRotmat &r);
//旋转向量 转换为 旋转矩阵
GLBROT_API void glbRotVector2RotMat(GlbPoint3d angle, GlbRotmat &r);
//屏幕上的点p1移动到p2所表示的旋转矩阵
GLBROT_API void glbMovingPoints2RotMat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r);
//由转轴和转角所表示的旋转矩阵
GLBROT_API void glbAnglePivot2RotMat(GlbPivot pivot, float angle, GlbRotmat &r);
//将p点绕pivot轴旋转angle角度
GLBROT_API void glbPivotingPoint(GlbPoint3d p, GlbPivot pivot, float angle, GlbPoint3d &p_out);
//生成贴图时所需的两个正交的旋转轴
GLBROT_API void glbCreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPivot &pivot_h, GlbPivot &pivot_v);//生成p点到headPoint的旋转轴，以及其正交方向的旋转轴


//从ini中读取畸变矫正信息
GLBROT_API void glbInitDistort(GlbCalib &calib, char* calibFileName);
//鱼眼镜头径向畸变矫正(由正规的圆饼坐标转变到适应镜头显示的圆饼坐标）
GLBROT_API float glbDistortRadius(float Radius, GlbCalib calib);
//畸变矫正反变换
GLBROT_API float glbUnDistortRadius(float Radius, GlbCalib calib);

//int GetTopLayer(GlbPoint2d pTouch);
