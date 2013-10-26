#include "stdafx.h"
#include "GlbRot.h"
#include <math.h>
#include "opencv.hpp"
#include "Ktmfc.h"

//#define N_CALIB  (12) //标定纬线的条数
//float latitude_calib[N_CALIB+1];

float glbVectorNorm(GlbPoint3d Vec)
{
	return sqrt( Vec.m_x * Vec.m_x + Vec.m_y * Vec.m_y + Vec.m_z * Vec.m_z);
}
void glbVectorNormalize(GlbPoint3d Vec, GlbPoint3d &Vec_dst)
{
	float norm = glbVectorNorm(Vec);
	if (norm<0.00001f) norm=0.00001f;

	Vec_dst.m_x = Vec.m_x / norm;
	Vec_dst.m_y = Vec.m_y / norm;
	Vec_dst.m_z = Vec.m_z / norm;
}
float glbDotMul(GlbPoint3d Vec1, GlbPoint3d Vec2)
{
	return Vec1.m_x*Vec2.m_x + Vec1.m_y*Vec2.m_y + Vec1.m_z*Vec2.m_z;
}
void glbCrossMul(GlbPoint3d Vec1, GlbPoint3d Vec2, GlbPoint3d &VecDST)
{
	GlbPoint3d t;
	t.m_x = Vec1.m_y * Vec2.m_z - Vec2.m_y * Vec1.m_z;
	t.m_y = Vec1.m_z * Vec2.m_x - Vec1.m_x * Vec2.m_z;
	t.m_z = Vec1.m_x * Vec2.m_y - Vec2.m_x * Vec1.m_y;

	VecDST.m_x = t.m_x;
	VecDST.m_y = t.m_y;
	VecDST.m_z = t.m_z;
}

void glbPointGeo2PointRect(GlbPointGeo pGeo, GlbPoint3d &pRect)
{
	float latitude = pGeo.m_lat / 180.0f * PI;//纬度
	float longitude = pGeo.m_lng / 180.0f * PI;//经度

	pRect.m_x = cos(longitude)*cos(latitude);
	pRect.m_y = sin(latitude);
	pRect.m_z = -sin(longitude)*cos(latitude);
}

void glbPointRect2PointGeo(GlbPoint3d pRect, GlbPointGeo &pGeo)
{
	if(pRect.m_y > 1)
	{
		pRect.m_y = 1;
	}
	else if(pRect.m_y<-1)
	{
		pRect.m_y = -1;
	}
	float latitude = asin(pRect.m_y);
	float longitude;
	float r_xz = sqrt(pow(pRect.m_x, 2) + pow(pRect.m_z, 2));
	if( r_xz < 0.0001 )
	{
		longitude = 0;
	}
	else
	{
		longitude = acos(pRect.m_x/r_xz);
	}

	if(pRect.m_z>0)
	{
		longitude = -longitude;
	}
	longitude = longitude / PI * 180.0f;
	latitude = latitude / PI * 180.0f;

	pGeo.m_lng = longitude;
	pGeo.m_lat = latitude;
}

void glbGlobePoint2ScreenPoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2)
{
	GlbPoint3d t;

	t.m_x = p1.m_x *r.r11 + p1.m_y*r.r12 + p1.m_z*r.r13;
	t.m_y = p1.m_x *r.r21 + p1.m_y*r.r22 + p1.m_z*r.r23;
	t.m_z = p1.m_x *r.r31 + p1.m_y*r.r32 + p1.m_z*r.r33;

	p2.m_x = t.m_x;
	p2.m_y = t.m_y;
	p2.m_z = t.m_z;
}

void glbScreenPoint2GlobePoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2)
{
	CvMat *GlbRotmat = cvCreateMat(3, 3, CV_32FC1);

	GlbRotmat->data.fl[0] = r.r11;
	GlbRotmat->data.fl[1] = r.r12;
	GlbRotmat->data.fl[2] = r.r13;
	GlbRotmat->data.fl[3] = r.r21;
	GlbRotmat->data.fl[4] = r.r22;
	GlbRotmat->data.fl[5] = r.r23;
	GlbRotmat->data.fl[6] = r.r31;
	GlbRotmat->data.fl[7] = r.r32;
	GlbRotmat->data.fl[8] = r.r33;	

	cvInvert( GlbRotmat, GlbRotmat);

	r.r11 = GlbRotmat->data.fl[0] ;
	r.r12 = GlbRotmat->data.fl[1];
	r.r13 = GlbRotmat->data.fl[2];
	r.r21 = GlbRotmat->data.fl[3];
	r.r22 = GlbRotmat->data.fl[4];
	r.r23 = GlbRotmat->data.fl[5];
	r.r31 = GlbRotmat->data.fl[6];
	r.r32 = GlbRotmat->data.fl[7];
	r.r33 = GlbRotmat->data.fl[8];	

	glbGlobePoint2ScreenPoint(p1, r, p2);
	cvReleaseMat(&GlbRotmat);
}


void glbPointRect2PointRound(GlbPoint3d p3, GlbPointRound &p2, GlbCalib calib)
{
	if(p3.m_y > 1)
	{
		p3.m_y = 1;
	}
	else if(p3.m_y<-1)
	{
		p3.m_y = -1;
	}


	float latitude = asin(p3.m_y);
	float R = (PI/2.0f - latitude)/PI; //范围[0,1]
	float r_xz = sqrt(pow(p3.m_x, 2) + pow(p3.m_z, 2));
	if( r_xz < 0.0001 )
	{
		p2.m_x = 0.0f;
		p2.m_y = 0.0f;
		return;
	}	

	R = glbDistortRadius(R, calib);

	p2.m_x = p3.m_x/r_xz*R;
	p2.m_y = -p3.m_z/r_xz*R;
}

void glbPointRound2PointRect(GlbPoint2d p2, GlbPoint3d &p3, GlbCalib calib)
{
	float R = sqrt(p2.m_x * p2.m_x + p2.m_y *p2.m_y);
	float ix = p2.m_x / R;
	float iz = -p2.m_y / R;

	R = glbUnDistortRadius(R, calib);

	float iR = R;//范围[0,1]
	float latitude = PI/2.0f - iR * PI;
	p3.m_y = sin(latitude);

	if(R < 0.0001)
	{
		p3.m_x = 0;
		p3.m_z = 0;
		return;
	}

	p3.m_x = ix * cos(latitude);
	p3.m_z = iz * cos(latitude);

	return;
}

void glbCreateGlbRotmat(GlbRotmat &r)
{
	r.r11 = 1;
	r.r12 = 0;
	r.r13 = 0;
	r.r21 = 0;
	r.r22 = 1;
	r.r23 = 0;
	r.r31 = 0;
	r.r32 = 0;
	r.r33 = 1;
}

void glbEularAngle2Rotmat(GlbEularAngle angle, GlbRotmat &r)
{
    //欧拉角旋转顺序 Y1 - Z2 - Y3
    //参考：http://en.wikipedia.org/wiki/Euler_angles

    float a1 = angle.m_1_Horz/180.0f*PI;
    float a2 = angle.m_2_Vert/180.0f*PI;
    float a3 = angle.m_3_Axis/180.0f*PI;
    float c1 = cos(a1);
    float c2 = cos(a2);
    float c3 = cos(a3);
    float s1 = sin(a1);
    float s2 = sin(a2);
    float s3 = sin(a3);

    r.r11 = c1*c2*c3-s1*s3;
    r.r12 = -c1*s2;
    r.r13 = c3*s1+c1*c2*s3;
    r.r21 = c3*s2;
    r.r22 = c2;
    r.r23 = s2*s3;
    r.r31 = -c1*s3-c2*c3*s1;
    r.r32 = s1*s2;
    r.r33 = c1*c3-c2*s1*s3;
}

void glbRotVector2RotMat(GlbPoint3d angle, GlbRotmat &r)
{
	CvMat * rotVector1 = cvCreateMat(3, 1, CV_32FC1);
	rotVector1->data.fl[0] = angle.m_x;
	rotVector1->data.fl[1] = angle.m_y;
	rotVector1->data.fl[2] = angle.m_z;
	CvMat *GlbRotmat1 = cvCreateMat(3, 3, CV_32FC1);
	cvRodrigues2(rotVector1, GlbRotmat1);

	r.r11 = GlbRotmat1->data.fl[0];
	r.r12 = GlbRotmat1->data.fl[1];
	r.r13 = GlbRotmat1->data.fl[2];
	r.r21 = GlbRotmat1->data.fl[3];
	r.r22 = GlbRotmat1->data.fl[4];
	r.r23 = GlbRotmat1->data.fl[5];
	r.r31 = GlbRotmat1->data.fl[6];
	r.r32 = GlbRotmat1->data.fl[7];
	r.r33 = GlbRotmat1->data.fl[8];

	cvReleaseMat(&rotVector1);//cvReleaseMatHeader?
	cvReleaseMat(&GlbRotmat1);

}

float glbAngleBetweenPoints(GlbPoint3d p1, GlbPoint3d p2)
{
	//从p1转到p2
	if(p1.m_x == p2.m_x && p1.m_y == p2.m_y && p1.m_z == p2.m_z)
	{
		return 0.0f;
	}
	float fglbDotMul = glbDotMul(p1, p2);
	float m_p1 = glbVectorNorm(p1);
	float m_p2 = glbVectorNorm(p2);

	float d = fglbDotMul / m_p1 / m_p2;

	if(d>1)
	{
		d = 1;
	}
	else if(d<-1)
	{
		d=-1;
	}

	float a = fabs(acos( d ) / PI * 180.0f); //旋转角

	return a;
}

void glbMovingPoints2RotMat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r)
{
	//从p1转到p2
	if(p1.m_x == p2.m_x && p1.m_y == p2.m_y && p1.m_z == p2.m_z)
	{
        glbCreateGlbRotmat(r);
		return;
	}

	float a = glbAngleBetweenPoints(p1, p2); //计算旋转角度
	GlbPivot pivot;
	glbPivotBetweenPoints(p1, p2, pivot); //计算旋转轴

    glbAnglePivot2RotMat(pivot, a, r);
}

void glbAnglePivot2RotMat(GlbPoint3d pivot, float angle, GlbRotmat &r)
{
	angle = angle / 180.0f * PI;
	float x = pivot.m_x;
	float y = pivot.m_y;
	float z = pivot.m_z;

	r.r11 = cos(angle) + (1-cos(angle)) * x * x;
	r.r12 = (1-cos(angle))*x*y - sin(angle)*z;
	r.r13 = (1-cos(angle))*x*z + sin(angle)*y;

	r.r21 = (1-cos(angle))*y*x + sin(angle)*z;
	r.r22 = cos(angle)+(1-cos(angle))*y*y;
	r.r23 = (1-cos(angle))*y*z - sin(angle)*x;

	r.r31 = (1-cos(angle))*z*x - sin(angle)*y;
	r.r32 = (1-cos(angle))*z*y + sin(angle)*x;
	r.r33 = cos(angle) + (1-cos(angle))*z*z;
}

void glbPivotingPoint(GlbPoint3d p, GlbPivot pivot, float angle, GlbPoint3d &p_out)
{
	GlbRotmat rot;
	glbAnglePivot2RotMat(pivot, angle, rot);

	glbGlobePoint2ScreenPoint(p, rot, p_out);
}

void glbCreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPivot &pivot_h, GlbPivot &pivot_v)
{
	if(p.m_x==directPoint.m_x && p.m_y==directPoint.m_y && p.m_z==directPoint.m_z)
	{
		directPoint.m_x = 0;
		directPoint.m_y = 1;
		directPoint.m_z = 0;
	}

	if(p.m_y > 0.999999)
	{
		pivot_v.m_x = 1.0f;
		pivot_v.m_y = 0.0f;
		pivot_v.m_z = 0.0f;
		pivot_h.m_x = 0.0f;
		pivot_h.m_y = 0.0f;
		pivot_h.m_z = 1.0f;
	}
	else
	{
		glbCrossMul(p, directPoint, pivot_v);
		glbVectorNormalize(pivot_v, pivot_v);

		if(!bHeadDirect)
		{
			pivot_v.m_x = -pivot_v.m_x;
			pivot_v.m_y = -pivot_v.m_y;
			pivot_v.m_z = -pivot_v.m_z;
		}

		glbCrossMul(pivot_v, p, pivot_h);
		glbVectorNormalize(pivot_h, pivot_h);
	}
}

GLBROT_API void glbInitDistort(GlbCalib &calib, char* calibFileName)
{
    //读取纬度标定信息
	for(int i=0; i<N_CALIB+1; i++)
	{
		char Name[20],Default[20], Value[20];
		sprintf_s(Name, 20,"latitude%d", i);
		sprintf_s(Default, 20,"%f", i/(float)N_CALIB);
		//GetPrivateProfileString("CALIB", Name, Default, Value, 10, ".\\calib.ini");
		GetPrivateProfileString("CALIB", Name, Default, Value, 10, calibFileName);
		sscanf_s(Value,"%f", calib.latitude+i );
		//！！！！！todo:这里需加入判断维度标定值是否合理！！！！！
	}
}

GLBROT_API float glbDistortRadius(float Radius, GlbCalib calib)
{
	float ret=1.0f;
	float fnCalib = (float)(N_CALIB);

	for(int i=1; i<=N_CALIB; i++)
	{
		if(Radius <= i/fnCalib)
		{
			ret = (Radius - (i-1)/fnCalib) * (calib.latitude[i] - calib.latitude[i-1])*fnCalib + calib.latitude[i-1];
			break;
		}
	}
	return ret;
}

GLBROT_API float glbUnDistortRadius(float Radius, GlbCalib calib)
{
	float ret=1.0f;
	float fnCalib = (float)(N_CALIB);

	for(int i=1; i<=N_CALIB; i++)
	{
		if(Radius <= calib.latitude[i])
		{
			ret = (Radius - calib.latitude[i-1])/(calib.latitude[i] - calib.latitude[i-1]) / fnCalib + (i-1)/fnCalib;
			break;
		}
	}
	return ret;
}

void glbCloneGlbRotmat( GlbRotmat r, GlbRotmat &r_dst )
{
	r_dst.r11 = r.r11;
	r_dst.r12 = r.r12;
	r_dst.r13 = r.r13;

	r_dst.r21 = r.r21;
	r_dst.r22 = r.r22;
	r_dst.r23 = r.r23;

	r_dst.r31 = r.r31;
	r_dst.r32 = r.r32;
	r_dst.r33 = r.r33;

}

void glbPivotBetweenPoints( GlbPoint3d p1, GlbPoint3d p2, GlbPivot &pivot)
{
	if(p1 == p2)
	{
		pivot.m_x = 0;
		pivot.m_y = 1;
		pivot.m_z = 0;
		return;
	}

	GlbPoint3d axle;//旋转轴（p1叉乘p2）
	glbCrossMul(p1, p2, axle);

	float m_axle = 1;
	m_axle = glbVectorNorm(axle);
	
	//旋转轴单位向量
	pivot.m_x = axle.m_x / m_axle;
	pivot.m_y = axle.m_y / m_axle;
	pivot.m_z = axle.m_z / m_axle;
}

void glbRotmatMul( GlbRotmat mat1, GlbRotmat mat2, GlbRotmat &mat_dst )
{
	GlbRotmat t;
	t.r11 = mat1.r11 * mat2.r11 + mat1.r12 * mat2.r21 + mat1.r13 * mat2.r31;
	t.r12 = mat1.r11 * mat2.r12 + mat1.r12 * mat2.r22 + mat1.r13 * mat2.r32;
	t.r13 = mat1.r11 * mat2.r13 + mat1.r12 * mat2.r23 + mat1.r13 * mat2.r33;

	t.r21 = mat1.r21 * mat2.r11 + mat1.r22 * mat2.r21 + mat1.r23 * mat2.r31;
	t.r22 = mat1.r21 * mat2.r12 + mat1.r22 * mat2.r22 + mat1.r23 * mat2.r32;
	t.r23 = mat1.r21 * mat2.r13 + mat1.r22 * mat2.r23 + mat1.r23 * mat2.r33;

	t.r31 = mat1.r31 * mat2.r11 + mat1.r32 * mat2.r21 + mat1.r33 * mat2.r31;
	t.r32 = mat1.r31 * mat2.r12 + mat1.r32 * mat2.r22 + mat1.r33 * mat2.r32;
	t.r33 = mat1.r31 * mat2.r13 + mat1.r32 * mat2.r23 + mat1.r33 * mat2.r33;

	glbCloneGlbRotmat(t, mat_dst);
}

float glbAngleABC(GlbPointGeo A, GlbPointGeo B, GlbPointGeo C)
{
	//此函数的大致思路是：将B点转至北极，然后比较A点与C点的经度，得出∠ABC的值

	GlbPoint3d A3d,B3d,C3d, Polar3d(0,1,0);
	glbPointGeo2PointRect(A, A3d);
	glbPointGeo2PointRect(B, B3d);
	glbPointGeo2PointRect(C, C3d);

	//将B点转至北极
	float angle = glbAngleBetweenPoints(B3d, Polar3d);
	GlbPivot pivot;
	glbPivotBetweenPoints(B3d, Polar3d, pivot);

	//旋转后的AC点
	GlbPoint3d Ap3d, Cp3d;
	glbPivotingPoint(A3d, pivot, angle, Ap3d);
	glbPivotingPoint(C3d, pivot, angle, Cp3d);
	GlbPointGeo ApGeo, CpGeo;
	glbPointRect2PointGeo(Ap3d, ApGeo);
	glbPointRect2PointGeo(Cp3d, CpGeo);

	float returnValue = CpGeo.m_lng - ApGeo.m_lng;
	if(returnValue < 0)
	{
		returnValue += 360;
	}
	return returnValue;
}

//此函数将polygon中的重复的点去除
void glbEliminateIdenticalPointsInPolygon(vector<GlbPointGeo> polygon, vector<GlbPointGeo> &polygon_out)
{
	if(polygon.size()<2)
	{
		polygon_out = polygon;
		return;
	}
	vector<GlbPointGeo> polygon_;//临时变量
	polygon_.push_back(polygon[0]);//复制第一个点

	vector<GlbPointGeo>::iterator it;
	for(it=polygon.begin()+1; it!=polygon.end(); it++)
	{
		//判断是否有重合点
		if( *it != *(it-1) )
		{
			//复制没有重合的点
			polygon_.push_back(*it);
		}
	}

	//判断是否首尾相接
	if(*(polygon_.end()-1) == *(polygon_.begin()) )
	{
		polygon_.pop_back();
	}

	polygon_out = polygon_;
	return;
}

float glbGetSteradian(vector<GlbPointGeo> polygon)
{
	//计算方法参考 http://www.guokr.com/article/98934/

	//首先去除连续的相同点
	glbEliminateIdenticalPointsInPolygon(polygon, polygon);
	
	int nVertex = polygon.size();

	if(nVertex < 3)
	{
		return 0;
	}

	float SumInnerAngle = 0;

	int nTriangle = nVertex - 2;

	GlbPointGeo A,B,C;
	int i;
	for(i=0; i<nTriangle; i++)
	{
		A = polygon[i];
		B = polygon[i+1];
		C = polygon[i+2];

		SumInnerAngle += glbAngleABC(A, B, C) / 180.0f * PI;
	}

	//最后两个角
	A = polygon[i];
	B = polygon[i+1];
	C = polygon[0];
	SumInnerAngle += glbAngleABC(A, B, C) / 180.0f * PI;

	A = polygon[i+1];
	B = polygon[0];
	C = polygon[1];
	SumInnerAngle += glbAngleABC(A, B, C) / 180.0f * PI;

	float omiga = SumInnerAngle - (nVertex-2) * PI;

	return omiga;
}

bool glbArcsIntersect(GlbPointGeo A, GlbPointGeo B, GlbPointGeo C, GlbPointGeo D)
{
    //参考 http://geospatialmethods.org/spheres/GCAIntersect.html


    //将四个点转换为直角坐标
    GlbPoint3d A3d,B3d,C3d,D3d;
    glbPointGeo2PointRect(A, A3d);
    glbPointGeo2PointRect(B, B3d);
    glbPointGeo2PointRect(C, C3d);
    glbPointGeo2PointRect(D, D3d);

    GlbPoint3d Ap3d(0,1,0),Bp3d,Cp3d,Dp3d; //Ap 就是A'（A prime)，表示A转动后的位置
    GlbPivot pivot;

    //将A转至北极点，计算其他点的位置
    float angle = glbAngleBetweenPoints(A3d, Ap3d);
    glbPivotBetweenPoints(A3d, Ap3d, pivot);
    glbPivotingPoint(B3d, pivot, angle, Bp3d);
    glbPivotingPoint(C3d, pivot, angle, Cp3d);
    glbPivotingPoint(D3d, pivot, angle, Dp3d);

    GlbPivot Pivot_AB, Pivot_CD;

    //计算AB和CD所在大圆的轴
    glbPivotBetweenPoints(Ap3d, Bp3d, Pivot_AB);
    glbPivotBetweenPoints(Cp3d, Dp3d, Pivot_CD);

    //AB和CD所在的大圆有两个交点（即与两个轴垂直的第三轴，用叉乘计算得到）
    GlbPoint3d Intersect1, Intersect2;
    glbPivotBetweenPoints(Pivot_AB, Pivot_CD, Intersect1);
    Intersect2 = -Intersect1;

    //将两交点转换成经纬度坐标
    GlbPointGeo Intersect1Geo, Intersect2Geo, BpGeo, CpGeo, DpGeo;
    glbPointRect2PointGeo(Intersect1, Intersect1Geo);
    glbPointRect2PointGeo(Intersect2, Intersect2Geo);
    glbPointRect2PointGeo(Bp3d, BpGeo);
    glbPointRect2PointGeo(Cp3d, CpGeo);
    glbPointRect2PointGeo(Dp3d, DpGeo);

    //分别判断两个交点和B'点的位置关系(以下实现的方法太恶心）
    float B_lng = BpGeo.m_lng;
    float C_lng = CpGeo.m_lng;
    float D_lng = DpGeo.m_lng;
    float CD_lng = D_lng - C_lng; //C到D在经度上的跨度
    float DC_lng = -CD_lng;//D到C的跨度
    bool C_is_front = false;
    if(CD_lng < 0)
    {
        CD_lng += 360.0f;
    }
    if(DC_lng < 0)
    {
        DC_lng += 360.0f;
    }
    float min_CD_lng = CD_lng;//CD的绝对跨度
    if(CD_lng > DC_lng)
    {
        C_is_front = true;
        min_CD_lng = DC_lng;
    }
    float mid_CD_lng;//CD的中点
    if(C_is_front)//CD的中点，是处在后面的点加上CD绝对跨度的一半
    {
        mid_CD_lng = D_lng + min_CD_lng/2.0f;
    }
    else
    {
        mid_CD_lng = C_lng + min_CD_lng/2.0f;
    }

    if(mid_CD_lng > 180.0f)
    {
        mid_CD_lng -= 360.0f;
    }

    float B_mid = B_lng - mid_CD_lng;//B到CD中点的跨度
    float mid_B = -B_mid;
    if(B_mid < 0)
    {
        B_mid += 360.0f;
    }
    if(mid_B < 0)
    {
        mid_B += 360.0f;
    }

    float min_B_mid = B_mid>mid_B ? mid_B : B_mid;//B到CD中点的绝对跨度

    if(min_B_mid < min_CD_lng/2.0f)//B到CD中点的绝对跨度 小于 CD绝对跨度的一半， 说明B在CD的跨度范围内
    {
        if( fabs(BpGeo.m_lng - Intersect1Geo.m_lng) < 0.0001 )//交点1 与 B经度相同
        {
            if( BpGeo.m_lat < Intersect1Geo.m_lat )//交点1维度在B之上
            {
                return true;//两个弧相交
            }
        }
        if( fabs(BpGeo.m_lng - Intersect2Geo.m_lng) < 0.0001 )//交点2 与 B经度相同
        {
            if( BpGeo.m_lat < Intersect2Geo.m_lat )//交点2维度在B之上
            {
                return true;//两个弧相交
            }
        }
    }

    return false;//两个弧不相交
}

GLBROT_API void glbPointTex2PointGeo(
                                        GlbRotmat GlobeRotMat,
                                        GlbPointGeo pGeo,       //贴图的中心点
                                        bool bPointOnGlobe,     //中心点坐标是在 globe坐标系(true) or screen坐标系(false)
                                        GlbPointGeo pGeoDirect, //贴图方向的参考点
                                        bool bDirOnGlobe,       //方向参考点实在 globe坐标系(true) or screen坐标系(false)
                                        bool bHeadDirect,       //图片朝向参考点(ture) or 背向参考点(false)
                                        float width,            //贴图的宽度(单位:角度)
                                        float height,           //贴图的高度(单位:角度)
                                        GlbTexMode mode,
                                        GlbPointTex texPoint,
                                        bool bTexPoint3dOnGlobe,
                                        GlbPointGeo &texPointGeo)
{
    //计算图像“中心点”的位置
    GlbPoint3d pRect;//“中心点”的直角坐标
    glbPointGeo2PointRect(pGeo, pRect);//将“中心点”转换为直角坐标
    if(bPointOnGlobe)//如果“中心点”在地球上
    {
        glbGlobePoint2ScreenPoint(pRect, GlobeRotMat, pRect);//将“中心点”转换至屏幕上
    }

    GlbPivot pivot_v, pivot_h;//横向和纵向旋转轴
    GlbPoint3d pRectDirect;//“朝向点”的直角坐标
    glbPointGeo2PointRect(pGeoDirect, pRectDirect);//将“朝向点”转换为直角坐标
    if(bDirOnGlobe)//如果“朝向点”在地球上
    {
        glbGlobePoint2ScreenPoint(pRectDirect, GlobeRotMat, pRectDirect);//将“朝向点”转换至屏幕上
    }

    glbCreateNormPivot( //生成贴图转轴
        pRect, //“中心点”直角坐标
        pRectDirect, //“朝向点”直角坐标
        bHeadDirect, //朝向
        pivot_h, //生成贴图所需的纵向转轴（轴是纵向的，旋转将是横向的）
        pivot_v  //生成贴图所需的横向转轴
    );

    if(mode == GLB_TEX_BELT)//如果贴图是类型是“条幅”
    {
        pivot_h = pRectDirect;  //纵向转轴 = 球心和“朝向点”的连线
    }

    float angle_h = width * (texPoint.m_x - 0.5f); //贴图中心点到左上角的旋转角度
    float angle_v = -height * (texPoint.m_y - 0.5f);//贴图中心点到左上角的旋转角度

    GlbPoint3d p_img;//此关节点的直角坐标
    glbPivotingPoint(pRect, pivot_v, angle_v, p_img);//先从中心点纵向旋转
    glbPivotingPoint(p_img, pivot_h, angle_h, p_img);//再横向旋转

	if(bTexPoint3dOnGlobe)//如果“中心点”在地球上
    {
        glbScreenPoint2GlobePoint(p_img, GlobeRotMat, p_img);//将“中心点”转换至屏幕上
    }

	glbPointRect2PointGeo(p_img, texPointGeo);
}