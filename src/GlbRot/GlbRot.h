#pragma once

#ifdef GLBROT_EXPORTS
#define GLBROT_API __declspec(dllexport)
#else
#define GLBROT_API __declspec(dllimport)
#endif

#define PI (3.1415926f)
#define N_CALIB  (12) //标定纬线的条数


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
}; 

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

GLBROT_API float VectorNorm(GlbPoint3d Vec);
GLBROT_API void VectorNormalize(GlbPoint3d Vec, GlbPoint3d &Vec_dst);

GLBROT_API float DotMul(GlbPoint3d Vec1, GlbPoint3d Vec2);
GLBROT_API void CrossMul(GlbPoint3d Vec1, GlbPoint3d Vec2, GlbPoint3d &VecDST);
GLBROT_API void GlbRotmatMul(GlbRotmat mat1, GlbRotmat mat2, GlbRotmat &mat_dst);

//经纬坐标 转换为 直角坐标
GLBROT_API void glbPointGeo2PointRect(GlbPointGeo pGeo, GlbPoint3d &pRect);
//直角坐标 转换为 经纬坐标
GLBROT_API void glbPointRect2PointGeo(GlbPoint3d pRect, GlbPointGeo &pGeo);

//原始素材（方形）坐标 转换为 球面上的直角坐标（此函数没有调用）
GLBROT_API void Square2Sphere(GlbPoint2d pSquare, float TexWidth, float TexHeight, GlbPoint3d &pSphere);
//直角坐标 转换为 原始素材坐标
GLBROT_API void Sphere2Square(GlbPoint2d p1, float w, float h, GlbPoint3d &p0);

//地球上的经纬坐标 转换为 屏幕上的经纬坐标
GLBROT_API void glbGlobePoint2ScreenPoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);
//屏幕上的经纬坐标 转换为 地球上的经纬坐标
GLBROT_API void glbScreenPoint2GlobePoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);

//直角坐标 转换为 圆饼坐标
GLBROT_API void glbPointRect2PointRound(GlbPoint3d p2, float radius, GlbPoint2d &p3);
//圆饼坐标 转换为 直角坐标
GLBROT_API void glbPointRound2PointRect(GlbPoint2d p3, float radius, GlbPoint3d &p2);

//生成初始旋转矩阵
GLBROT_API void glbCreateGlbRotmat(GlbRotmat &r);
//复制旋转矩阵
GLBROT_API void glbCloneGlbRotmat(GlbRotmat r, GlbRotmat &r_dst);

//计算屏幕上的点p1到p2的旋转角度
GLBROT_API float glbAngleBetweenPoints(GlbPoint3d p1, GlbPoint3d p2);
//计算屏幕上的点p1到p2的旋转轴
GLBROT_API void glbPivotBetweenPoints(GlbPoint3d p1, GlbPoint3d p2, GlbPoint3d &pivot);

//欧拉角 转换为 旋转矩阵
GLBROT_API void glbEularAngle2Rotmat(GlbEularAngle angle, GlbRotmat &r);
//旋转向量 转换为 旋转矩阵
GLBROT_API void glbRotVector2RotMat(GlbPoint3d angle, GlbRotmat &r);
//屏幕上的点p1移动到p2所表示的旋转矩阵
GLBROT_API void glbMovingPoints2RotMat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r);
//由转轴和转角所表示的旋转矩阵
GLBROT_API void glbAnglePivot2RotMat(GlbPoint3d pivot, float angle, GlbRotmat &r);
//将p点绕pivot轴旋转angle角度
GLBROT_API void PivotRotPoint(GlbPoint3d p, GlbPoint3d pivot, float angle, GlbPoint3d &p_out);
//生成贴图时所需的两个正交的旋转轴
GLBROT_API void CreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPoint3d &pivot_h, GlbPoint3d &pivot_v);//生成p点到headPoint的旋转轴，以及其正交方向的旋转轴

//float DistortR(float R);//鱼眼镜头径向畸变矫正
//float aDistortR(float R);//畸变矫正反变换
//
//int GetTopLayer(GlbPoint2d pTouch);
