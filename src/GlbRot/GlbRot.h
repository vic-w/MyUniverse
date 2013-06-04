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

GLBROT_API void Geo2Rect(GlbPointGeo pGeo, GlbPoint3d &pRect);
GLBROT_API void Rect2Geo(GlbPoint3d pRect, GlbPointGeo &pGeo);

GLBROT_API void Square2Sphere(GlbPoint2d pSquare, float TexWidth, float TexHeight, GlbPoint3d &pSphere);
GLBROT_API void Sphere2Square(GlbPoint2d p1, float w, float h, GlbPoint3d &p0);

GLBROT_API void Sphere2Sphere(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);
GLBROT_API void aSphere2Sphere(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);

GLBROT_API void Sphere2Round(GlbPoint3d p2, float radius, GlbPoint2d &p3);
GLBROT_API void Round2Sphere(GlbPoint2d p3, float radius, GlbPoint3d &p2);

GLBROT_API void CreateGlbRotmat(GlbRotmat &r);//生成初始旋转矩阵
GLBROT_API void CloneGlbRotmat(GlbRotmat r, GlbRotmat &r_dst);

GLBROT_API float Point2Angle(GlbPoint3d p1, GlbPoint3d p2);//计算p1到p2的旋转角度
GLBROT_API void Point2Pivot(GlbPoint3d p1, GlbPoint3d p2, GlbPoint3d &pivot, bool bNormalize);//计算p1到p2的旋转轴

GLBROT_API void AngleGlbRotmat(GlbPoint3d angle, GlbRotmat &r);//angle的三个分量为绕x，绕y，绕z旋转的角度，依次进行旋转的矩阵
GLBROT_API void PointGlbRotmat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r);//计算从p1到p2的旋转矩阵
GLBROT_API void PivotRotmat(GlbPoint3d pivot, float angle, GlbRotmat &rot);
GLBROT_API void PivotRotPoint(GlbPoint3d p, GlbPoint3d pivot, float angle, GlbPoint3d &p_out);//将p点以pivot轴旋转angle角度
GLBROT_API void CreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPoint3d &pivot_h, GlbPoint3d &pivot_v);//生成p点到headPoint的旋转轴，以及其正交方向的旋转轴

//float DistortR(float R);//鱼眼镜头径向畸变矫正
//float aDistortR(float R);//畸变矫正反变换
//
//int GetTopLayer(GlbPoint2d pTouch);
