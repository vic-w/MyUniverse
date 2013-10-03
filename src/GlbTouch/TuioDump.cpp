#include "TuioDump.h"
		
void TuioDump::addTuioObject(TuioObject *tobj) {
	//std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void TuioDump::updateTuioObject(TuioObject *tobj) {
	//std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() 
	//			<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
}

void TuioDump::removeTuioObject(TuioObject *tobj) {
	//std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void TuioDump::addTuioCursor(TuioCursor *tcur) {
	//std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void TuioDump::updateTuioCursor(TuioCursor *tcur) {
	//std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() 
	//			<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;

	//处理移动信号
	std::list<TuioPoint> path = tcur->getPath();
	std::list <TuioPoint>::iterator pathIter;
	if(path.size() >= 2 && tcur->getMotionSpeed()>0)
	{
		pathIter = path.end();
		----pathIter;

		GlbPoint2d p0,p1;
		int width = m_pWindow->m_width;
		int height = m_pWindow->m_height;
		p1.m_x = (tcur->getX()*2 - 1.0f)/height*width;
		p1.m_y = 1.0f - tcur->getY()*2;
		p0.m_x = (pathIter->getX()*2 - 1.0f)/height*width;
		p0.m_y = 1.0f - pathIter->getY()*2;

		m_pWindow->EnterTouchCS();

		GlbMove move;
		move.m_pFrom = p0;
		move.m_pTo = p1;
		move.m_nTrackID = tcur->getSessionID();
		bool bID_exist = false;
		for(std::vector<GlbMove>::iterator i=m_pWindow->m_moveSignal.begin(); i!=m_pWindow->m_moveSignal.end(); ++i)    
		{
			if( move.m_nTrackID == i->m_nTrackID)
			{
				i->m_pTo = move.m_pTo;
				bID_exist = true;
			}
		}
		if(!bID_exist)
		{
			m_pWindow->m_moveSignal.push_back(move);
		}

		m_pWindow->LeaveTouchCS();
	}

}

void TuioDump::removeTuioCursor(TuioCursor *tcur) {
	//std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
	//判断点击
	std::list<TuioPoint> path = tcur->getPath();
	std::list <TuioPoint>::iterator pathIter;
	//判断点击
	float x0,y0,x1,y1;
	long t0,t1;

	x0 = path.begin()->getX();
	y0 = path.begin()->getY();
	t0 = path.begin()->getStartTime().getTotalMilliseconds();

	x1 = path.back().getX();
	y1 = path.back().getY();
	t1 = TuioTime::getSessionTime().getTotalMilliseconds();


	//std::cout<<"x0:"<<x0<<" y0:"<<y0<<" t0:"<<t0<<std::endl;
	//std::cout<<"x1:"<<x1<<" y1:"<<y1<<" t1:"<<t1<<std::endl;
	//std::cout<<t1-t0<<std::endl;

	float d_th = 0.01f;
	long T_th = 1000;
	long t_th = 10;

	if( fabs(x0-x1)< d_th && fabs(y0-y1)<d_th && t1-t0>t_th && t1-t0<T_th)
	{
		GlbPoint2d pTouch;
		int width = m_pWindow->m_width;
		int height = m_pWindow->m_height;
		pTouch.m_x = (x0*2 - 1.0f)/height*width;
		pTouch.m_y = 1.0f - y0*2;

		m_pWindow->EnterTouchCS();
		m_pWindow->m_touchSignal.push_back(pTouch);
		m_pWindow->LeaveTouchCS();
	}
}

void  TuioDump::refresh(TuioTime frameTime) {
	//std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

DWORD WINAPI TuioThread(LPVOID lpParam)
{
	GlbWindow *pWindow = (GlbWindow *)lpParam;

	int port = pWindow->m_TuioPort;

	TuioDump dump(pWindow);

	TuioClient client(port);
	client.addTuioListener(&dump);
	client.connect(true);

	return 0;
}


