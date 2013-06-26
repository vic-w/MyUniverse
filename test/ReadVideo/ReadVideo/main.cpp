#include "opencv.hpp"

void main()
{
    IplImage* Image=0;
    CvCapture* Cap = cvCaptureFromFile("»»¥¯”Í¡÷.WMV");
//    CvCapture* Cap = cvCaptureFromFile("input.avi");

    if(cvGrabFrame(Cap))
    do
    {
        Image = cvRetrieveFrame(Cap);
        cvNamedWindow("image");
        cvShowImage("image", Image);
        cvWaitKey();
    }
    while(cvGrabFrame(Cap));

}
