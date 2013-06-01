#include <windows.h>
#include "glbCore.h"
#include "resource.h"

int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow )
{
    glbCreateWindow(hInstance);

    GlbImage Image = glbLoadImage("a001.dds");

    do
    {
        glbDrawImage(Image);
        
    }while(glbUpdateWindow(0));

    glbReleaseImage(&Image);
    glbDestoryWindow(hInstance);
    return 0;
}
