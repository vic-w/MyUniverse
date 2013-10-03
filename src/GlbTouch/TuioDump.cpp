#include "TuioDump.h"
		
void TuioDump::addTuioObject(TuioObject *tobj) {
	std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void TuioDump::updateTuioObject(TuioObject *tobj) {
	std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() 
				<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
}

void TuioDump::removeTuioObject(TuioObject *tobj) {
	std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void TuioDump::addTuioCursor(TuioCursor *tcur) {
	std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void TuioDump::updateTuioCursor(TuioCursor *tcur) {
	std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() 
				<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
}

void TuioDump::removeTuioCursor(TuioCursor *tcur) {
	std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
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


