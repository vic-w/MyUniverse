#include "..\GlbCore\GlbCore.h"

void main()
{
    glbCreateWindow(0);

    GlbImage Image = glbLoadImage("a001.dds");
    //GlbImage Image = glbLoadImage("earth.jpg");

    do
    {
        glbDrawImage(Image);
        
    }while(glbUpdateWindow(0));

    glbReleaseImage(&Image);
    glbDestoryWindow(0);
    return;
}
