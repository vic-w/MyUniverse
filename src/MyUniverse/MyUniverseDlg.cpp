﻿
// MyUniverseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyUniverse.h"
#include "MyUniverseDlg.h"
#include "afxdialogex.h"
#include "GlobeThread.h"
#include "udpThread.h"
#include "libxml.h"
#include "opencv.hpp"
#include "Ktmfc.h"
#include "StorytellerComp.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CRITICAL_SECTION g_StoryPage_CS;
extern CStoryPage g_StoryPage;

extern CRITICAL_SECTION g_GlobeRotMat_CS;
extern GlbRotmat g_GlobeRotMat;
extern GlbEularAngle g_StorytellerEularAngle;
extern GlbEularAngle g_GlobeEularAngle;
extern GlbEularAngle g_OffsetEularAngle;

int g_bMainThreadActive;
int g_bGlbThreadActive;
int g_bTimingThreadActive;
int g_bUdpThreadActive;

#ifdef _DEBUG
#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )
#endif

char* u2g(char *inbuf);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// 用于输入授权码的 CInpuLicenseDlg 对话框

class CInpuLicenseDlg : public CDialogEx
{
public:
    CInpuLicenseDlg();

    // 对话框数据
    enum { IDD = IDD_INPUTLICENSE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
public:
//	void SaveLicenseKey(CString key);
	afx_msg void OnBnClickedOk();
};

CInpuLicenseDlg::CInpuLicenseDlg() : CDialogEx(CInpuLicenseDlg::IDD)
{
}

void CInpuLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInpuLicenseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInpuLicenseDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CInpuLicenseDlg::OnBnClickedOk()
{
	char* inputkey = new char[32+1];
	UINT nChars = GetDlgItemText(IDC_EDIT1, inputkey, 32+1);
	if (nChars > 0)
	{
		FILE* file = fopen("license.dat", "w");	
		fwrite( inputkey, sizeof( char ), strlen(inputkey), file );
		fclose(file);
	}
	delete[] inputkey;

	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


// CMyUniverseDlg 对话框

CMyUniverseDlg::CMyUniverseDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CMyUniverseDlg::IDD, pParent)
    , m_edit_rotx(0)
    , m_edit_roty(0)
    , m_edit_rotz(0)
    , m_slider_rotx(0)
    , m_slider_roty(0)
    , m_slider_rotz(0)
    , m_story_path(_T(""))
    , m_chapter_value(_T(""))
    , m_page_value(_T(""))
    , m_frame_rate(0)
    , m_rotating_UI(FALSE)
    , m_rotationRate_UI(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMyUniverseDlg::~CMyUniverseDlg()
{

}

void CMyUniverseDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_m_edit_rotx, m_edit_rotx);
    DDV_MinMaxInt(pDX, m_edit_rotx, 0, 360);
    DDX_Text(pDX, IDC_m_edit_roty, m_edit_roty);
    DDV_MinMaxInt(pDX, m_edit_roty, 0, 360);
    DDX_Text(pDX, IDC_m_edit_rotz, m_edit_rotz);
    DDV_MinMaxInt(pDX, m_edit_rotz, 0, 360);
    DDX_Slider(pDX, IDC_m_slider_rotx, m_slider_rotx);
    DDX_Slider(pDX, IDC_m_slider_roty, m_slider_roty);
    DDX_Slider(pDX, IDC_m_slider_rotz, m_slider_rotz);
    DDX_Text(pDX, IDC_EDIT_SOTRY_PATH, m_story_path);
    DDV_MaxChars(pDX, m_story_path, 512);
    DDX_Control(pDX, IDC_COMBO_CHAPTER, m_chapter_select);
    DDX_CBString(pDX, IDC_COMBO_CHAPTER, m_chapter_value);
    DDV_MaxChars(pDX, m_chapter_value, 512);
    DDX_Control(pDX, IDC_COMBO_PAGE, m_page_select);
    DDX_CBString(pDX, IDC_COMBO_PAGE, m_page_value);
    DDX_Text(pDX, IDC_EDIT_FRAME_RATE, m_frame_rate);
    DDV_MinMaxInt(pDX, m_frame_rate, 0, 100);
    DDX_Check(pDX, IDC_CHECK_AUTO_ROT, m_rotating_UI);
    DDX_Control(pDX, IDC_m_slider_rotz, m_slider_rotz_ctrl);
    DDX_Control(pDX, IDC_m_edit_rotz, m_edit_rotz_ctrl);
    DDX_Control(pDX, IDC_m_slider_rotx, m_slider_rotx_ctrl);
    DDX_Control(pDX, IDC_m_slider_roty, m_slider_roty_ctrl);
    DDX_Text(pDX, IDC_m_rotationRate, m_rotationRate_UI);
    DDX_Control(pDX, IDC_UNCLOCKWISE, m_unclockwise_UI);
    DDX_Control(pDX, IDC_CLOCKWISE, m_clockwise_UI);
}

BEGIN_MESSAGE_MAP(CMyUniverseDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CMyUniverseDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CMyUniverseDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_m_slider_rotx, &CMyUniverseDlg::OnNMCustomdrawSliderRotx)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_m_slider_roty, &CMyUniverseDlg::OnNMCustomdrawSliderRoty)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_m_slider_rotz, &CMyUniverseDlg::OnNMCustomdrawSliderRotz)
    ON_EN_CHANGE(IDC_m_edit_rotx, &CMyUniverseDlg::OnEnChangeEditRotx)
    ON_EN_CHANGE(IDC_m_edit_roty, &CMyUniverseDlg::OnEnChangeEditRoty)
    ON_EN_CHANGE(IDC_m_edit_rotz, &CMyUniverseDlg::OnEnChangeEditRotz)
    //ON_EN_CHANGE(IDC_EDIT1, &CMyUniverseDlg::OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CMyUniverseDlg::OnBnClickedButtonBrowse)
    ON_CBN_SELCHANGE(IDC_COMBO_CHAPTER, &CMyUniverseDlg::OnCbnSelchangeComboChapter)
    ON_CBN_SELCHANGE(IDC_COMBO_PAGE, &CMyUniverseDlg::OnCbnSelchangeComboPage)
    ON_BN_CLICKED(IDC_BUTTON_STEP, &CMyUniverseDlg::OnBnClickedButtonStep)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMyUniverseDlg::OnBnClickedButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CMyUniverseDlg::OnBnClickedButtonPause)
    ON_EN_CHANGE(IDC_EDIT_FRAME_RATE, &CMyUniverseDlg::OnEnChangeEditFrameRate)
    ON_BN_CLICKED(IDC_CHECK_AUTO_ROT, &CMyUniverseDlg::OnBnClickedCheckAutoRot)

    //自定义消息
    ON_MESSAGE(WM_GLB_UPDATEDATA,OnGlbUpdateData)  
    ON_MESSAGE(WM_GLB_UDPREADONEPAGE, OnGlbUdpReadOnePage)
    ON_EN_CHANGE(IDC_m_rotationRate, &CMyUniverseDlg::OnEnChangemrotationrate)
    ON_BN_CLICKED(IDC_UNCLOCKWISE, &CMyUniverseDlg::OnBnClickedUnclockwise)
    ON_BN_CLICKED(IDC_CLOCKWISE, &CMyUniverseDlg::OnBnClickedClockwise)
	ON_BN_CLICKED(IDC_ROT_RESET, &CMyUniverseDlg::OnBnClickedRotReset)
END_MESSAGE_MAP()


// CMyUniverseDlg 消息处理程序

BOOL CMyUniverseDlg::OnInitDialog()
{
    InitializeCriticalSection(&g_StoryPage_CS);//初始化临界区
    InitializeCriticalSection(&g_GlobeRotMat_CS);//初始化临界区
    g_bMainThreadActive = 1; //主线程开始活动，结束是在~CMyUniverseDlg()函数中

    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

	
    /*if(!glbKtMFC()) //glbKtMFC在这里调用会失败，原因不明，需要调试
    {
		
    }*/

    //读取ini文件中的配置
    char lpStoryPath[512];
    GetPrivateProfileString("MyUniverseCfg", "StoryPath", "", lpStoryPath, 512, ".\\config.ini");
    m_story_path = lpStoryPath;
    UpdateData(PUT_DATA);
    ReadChapterStruct();

    //初始化旋转矩阵
    g_StorytellerEularAngle.m_1_Horz = 0;
    g_StorytellerEularAngle.m_2_Vert = 90;
    g_StorytellerEularAngle.m_3_Axis = 0;    

    // 初始化界面上的值
    m_edit_roty = (int)g_StorytellerEularAngle.m_2_Vert;
    m_slider_roty = (int)(m_edit_roty/360.0*100);
    UpdateData(PUT_DATA);

    
    g_OffsetEularAngle.m_1_Horz = 0;
    g_OffsetEularAngle.m_2_Vert = -90;
    g_OffsetEularAngle.m_3_Axis = 0;

    StorytellerEular2GlobeEular(g_StorytellerEularAngle, g_OffsetEularAngle, g_GlobeEularAngle);
    glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);

    CreateThread(0, 0, GlobeThread, (LPVOID)this,0,0);//启动OpenGL显示线程
    CreateThread(0, 0, TimingThread, (LPVOID)this,0,0);//启动时间控制线程
    CreateThread(0, 0, UdpThread, (LPVOID)this,0,0);//启动时间控制线程

	if(!glbKtMFC()) //如果glbKtMFC在这里调用，就能成功，原因不明，需要调试
    {
		CInpuLicenseDlg dlgInputLic;
		dlgInputLic.DoModal();
		
		exit(0); //TODO : fix memory leak!
    }

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyUniverseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyUniverseDlg::OnPaint()
{
    //UpdateData(PUT_DATA);
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyUniverseDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMyUniverseDlg::OnBnClickedOk()
{
    return;
}


void CMyUniverseDlg::OnBnClickedCancel()
{
    if( 2 == MessageBox("真的要退出吗？", NULL, MB_OKCANCEL))
    {
        return;
    }
    else
    {
        g_bMainThreadActive = 0;//主线程即将退出（开始于OnInitDialog()函数中）

        while( g_bGlbThreadActive || g_bTimingThreadActive)//等待子线程退出
        {
            MSG msg;
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            Sleep(1);
        }

        CDialogEx::OnCancel();//真正退出
    }
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(GET_DATA);
    m_edit_rotx = (int)(m_slider_rotx/100.0*360);
    UpdateData(PUT_DATA);
    GlobeRotate(m_edit_rotx, m_edit_roty, m_edit_rotz, g_GlobeRotMat);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(GET_DATA);
    m_edit_roty = (int)(m_slider_roty/100.0*360);
    UpdateData(PUT_DATA);
    GlobeRotate(m_edit_rotx, m_edit_roty, m_edit_rotz, g_GlobeRotMat);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(GET_DATA);
    m_edit_rotz = (int)(m_slider_rotz/100.0*360);
    UpdateData(PUT_DATA);
    GlobeRotate(m_edit_rotx, m_edit_roty, m_edit_rotz, g_GlobeRotMat);
    *pResult = 0;
}


void CMyUniverseDlg::OnEnChangeEditRotx()
{
    UpdateData(GET_DATA);
    m_slider_rotx = (int)(m_edit_rotx/360.0*100);
    UpdateData(PUT_DATA);
}


void CMyUniverseDlg::OnEnChangeEditRoty()
{
    UpdateData(GET_DATA);
    m_slider_roty = (int)(m_edit_roty/360.0*100);
    UpdateData(PUT_DATA);
}


void CMyUniverseDlg::OnEnChangeEditRotz()
{
    UpdateData(GET_DATA);
    m_slider_rotz = (int)(m_edit_rotz/360.0*100);
    UpdateData(PUT_DATA);
}

void CMyUniverseDlg::OnBnClickedButtonBrowse()
{
    char szPath[MAX_PATH];     //存放选择的目录路径
    CString str;
    ZeroMemory(szPath, sizeof(szPath));  
 
    BROWSEINFO bi;  
    bi.hwndOwner = m_hWnd;  
    bi.pidlRoot = NULL;  
    bi.pszDisplayName = szPath;  
    bi.lpszTitle = "请选择课程目录：";  
    bi.ulFlags = 0;  
    bi.lpfn = NULL;  
    bi.lParam = 0;  
    bi.iImage = 0;  

    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);  
    if(lp && SHGetPathFromIDList(lp, szPath))  
    {
       m_story_path = szPath;
    }
    UpdateData(0);
    ReadChapterStruct();
    ::WritePrivateProfileString("MyUniverseCfg","StoryPath",szPath,".\\config.ini");
}

void CMyUniverseDlg::ReadChapterStruct()
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    m_chapter_select.ResetContent();
    if(m_story_path.GetLength() != 0)
    m_story_path += "\\*";

	hFind = FindFirstFile(m_story_path, &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox (_T("Invalid file handle.\n"));
	}
	else
	{
		do
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
				//printf ("这是一个目录\n");
                CString chapterName = FindFileData.cFileName;
                if(chapterName != "." && chapterName != "..")
                {
                    m_chapter_select.AddString(FindFileData.cFileName);
                }
			}
		}while (FindNextFile(hFind, &FindFileData) != 0);

		DWORD dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			//printf ("FindNextFile error. Error is %u\n", dwError);
            //AfxMessageBox(_T("ERROR_NO_MORE_FILES"));
		}
	}
    m_chapter_select.SetCurSel(0);
    ReadPageStruct();
}

CString CMyUniverseDlg::GetPageStructPath(CString StoryPath, CString ChapterName)
{
    CString page_struct_path = StoryPath;
    int pathLength = page_struct_path.GetLength();
    char lastChar = *(page_struct_path.GetBuffer()+pathLength-1);
    if(lastChar != '\\')
    page_struct_path +="\\";
    page_struct_path += ChapterName;
    page_struct_path += "\\";
    return page_struct_path;
}

void CMyUniverseDlg::ReadPageStruct()//次处支持：folder，dds，jpg，avi，wmv
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    m_page_select.ResetContent();//清空二级菜单（页面）的内容

    UpdateData(GET_DATA);//从界面上获取m_chapter_value

    m_page_struct_path = GetPageStructPath(m_story_path, m_chapter_value);

    CString XML_FilePath = FindXMLFilePath(m_page_struct_path);//判断是否有xml文件存在

    if(XML_FilePath == "")
    {
	    hFind = FindFirstFile(m_page_struct_path+"*", &FindFileData);
	
	    if(hFind == INVALID_HANDLE_VALUE)
	    {
		    //AfxMessageBox ("Invalid file handle.\n");
	    }
	    else
	    {
		    do
		    {
                CString chapterName = FindFileData.cFileName;
                if(chapterName != "." && chapterName != "..")
                {
                    char* suffix = chapterName.GetBuffer() + chapterName.GetLength() -4;
			        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
				        //printf ("这是一个目录\n");
                        m_page_select.AddString(FindFileData.cFileName);
			        }
                    else if( _stricmp(suffix,".jpg") ==0
                          || _stricmp(suffix,".bmp") ==0
                          || _stricmp(suffix,".png") ==0
                          || _stricmp(suffix,".dds") ==0
                          || _stricmp(suffix,".avi") ==0
                          || _stricmp(suffix,".wmv") ==0
                          )
                    {
                        m_page_select.AddString(FindFileData.cFileName);
                    }
                }
		    }while (FindNextFile(hFind, &FindFileData) != 0);

		    DWORD dwError = GetLastError();
		    FindClose(hFind);
		    if (dwError != ERROR_NO_MORE_FILES) 
		    {
			    //printf ("FindNextFile error. Error is %u\n", dwError);
                //AfxMessageBox(_T("ERROR_NO_MORE_FILES"));
		    }
	    }
    }
    else//there is one xml file!
    {
        xmlDocPtr doc; //文件指针
        xmlNodePtr cur;//节点指针
        doc = xmlParseFile(XML_FilePath);
        cur = xmlDocGetRootElement(doc);
        //AfxMessageBox((char*)cur->name);
        cur = cur->xmlChildrenNode;
        while(cur!=NULL)
        {
            if(xmlHasProp(cur,(xmlChar*)"imageName"))
            {
                xmlChar* szImageName = xmlGetProp(cur,(xmlChar*) "imageName");
                char* imageName_gb2312 = u2g((char*)szImageName);

                m_page_select.AddString(imageName_gb2312);
                
                free(imageName_gb2312);
                xmlFree(szImageName);
            }
            cur = cur->next;
        }
        xmlFreeDoc(doc);
    }

    m_page_select.SetCurSel(0);
    ReadOnePage(true);
}

void CMyUniverseDlg::ReadOnePage(bool bUpdateDataFromUI)
{
    if(bUpdateDataFromUI)
    {
        UpdateData(GET_DATA);//从界面上获取m_chapter_value和m_page_value
    }

    m_page_struct_path = GetPageStructPath(m_story_path, m_chapter_value);
    CString XML_FilePath = FindXMLFilePath(m_page_struct_path);//判断是否有xml文件存在

    
    CString pageFileName;
    if(XML_FilePath=="")
    {
        //以m_page_value作为文件名
        pageFileName = m_page_value;
        //AfxMessageBox(pagePath);
    }
    else
    {
        //从xml文件中查找文件名
        xmlDocPtr doc; //文件指针
        xmlNodePtr cur;//节点指针
        doc = xmlParseFile(XML_FilePath);
        cur = xmlDocGetRootElement(doc);
        //AfxMessageBox((char*)cur->name);
        cur = cur->xmlChildrenNode;
        while(cur!=NULL)
        {
            if(xmlHasProp(cur,(xmlChar*)"imageName"))
            {
                xmlChar* szImageName = xmlGetProp(cur,(xmlChar*) "imageName");
                xmlChar* imageName_gb2312 = (xmlChar*)u2g((char*)szImageName);
                xmlChar* imageName_fromDlg = (xmlChar*)(LPSTR)(LPCTSTR)m_page_value;
                if(!xmlStrcmp(imageName_gb2312, imageName_fromDlg))
                {
                    //读取文件名
                    char* g = u2g((char*)xmlGetProp(cur,(xmlChar*) "href") + 2);
                    pageFileName = g;
                    free(g);
                }
                free(imageName_gb2312);
                xmlFree(szImageName);
            }
            cur = cur->next;
        }
        xmlFreeDoc(doc);
    }

    CString pagePath = m_page_struct_path + pageFileName;
    char* suffix = pagePath.GetBuffer() + pagePath.GetLength() - 4;

    EnterCriticalSection(&g_StoryPage_CS);

    //清除全局数据
    g_StoryPage.bEmpty = 1;
    g_StoryPage.FrameNames.clear();
    g_StoryPage.bPlaying = 0;
    g_StoryPage.bRotating = 0;
    g_StoryPage.bClockwise = 0;
    g_StoryPage.rotationRate = 2;

    //控制界面变量
    m_rotating_UI = 0;
    m_rotationRate_UI = 2;
    m_unclockwise_UI.SetCheck(true);
    m_clockwise_UI.SetCheck(false);

    if( _stricmp(suffix,".jpg") == 0)
    {
        //AfxMessageBox(_T("this is a jpg file"));
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = JPG;
    }
    else if( _stricmp(suffix,".bmp") == 0)
    {
        //AfxMessageBox(_T("this is a bmp file"));
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = BMP;
    }
    else if( _stricmp(suffix,".png") == 0)
    {
        //AfxMessageBox(_T("this is a png file"));
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = PNG;
    }
    else if( _stricmp(suffix,".dds") == 0)
    {
        //AfxMessageBox(_T("this is a dds file"));
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = DDS;
    }
    else if( _stricmp(suffix,".avi") == 0)
    {
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 1;
        g_StoryPage.bPlaying = 1;
        g_StoryPage.frameRate = 30;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = AVI;
        g_StoryPage.nCurFrame = 0;
        m_frame_rate = g_StoryPage.frameRate;
    }
    else if( _stricmp(suffix,".wmv") == 0)
    {
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 1;
        g_StoryPage.bPlaying = 1;
        g_StoryPage.frameRate = 30;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = WMV;
        g_StoryPage.nCurFrame = 0;
        m_frame_rate = g_StoryPage.frameRate;
    }
    else
    {
        //AfxMessageBox(_T("this is a folder"));
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 1;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = FOLDER;
        g_StoryPage.nFrames = 0;	

        ReadFolderContent(pagePath, "dds");
        if(g_StoryPage.nFrames == 0)
        ReadFolderContent(pagePath, "jpg");
        if(g_StoryPage.nFrames == 0)
        ReadFolderContent(pagePath, "bmp");
        if(g_StoryPage.nFrames == 0)
        ReadFolderContent(pagePath, "png");
    }
    ReadStoryConfigXML();
    LeaveCriticalSection(&g_StoryPage_CS);
}

void CMyUniverseDlg::OnCbnSelchangeComboChapter()
{
    ReadPageStruct();
}


void CMyUniverseDlg::OnCbnSelchangeComboPage()
{
    ReadOnePage(true);
}

void CMyUniverseDlg::ReadFolderContent(CString folderPath, CString suffix)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    CString path = folderPath;
    path += "\\*.";
    path += suffix;
    hFind = FindFirstFile(path, &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox (_T("Invalid file handle.\n"));
	}
	else
	{
		do
		{
            CString FrameName = folderPath;
            FrameName += "\\";
            FrameName += FindFileData.cFileName;
            g_StoryPage.FrameNames.push_back(FrameName);
            g_StoryPage.nFrames++;
		}while (FindNextFile(hFind, &FindFileData) != 0);
    }
    g_StoryPage.nCurFrame = 0;
}

void CMyUniverseDlg::GlobeRotate(int Horz, int Vert, int Axis, GlbRotmat &r)
{
    EnterCriticalSection(&g_GlobeRotMat_CS);
    if(fabs(g_StorytellerEularAngle.m_1_Horz - Horz)>5)
    {
        g_StorytellerEularAngle.m_1_Horz = (float)Horz;
    }
    if(fabs(g_StorytellerEularAngle.m_2_Vert - Vert)>5)
    {
        g_StorytellerEularAngle.m_2_Vert = (float)Vert;
    }
    if(fabs(g_StorytellerEularAngle.m_3_Axis - Axis) > 5)//因为slider最小分辨率相当于3.6度角度
    {
        g_StorytellerEularAngle.m_3_Axis = (float)Axis;
    }
    StorytellerEular2GlobeEular(g_StorytellerEularAngle, g_OffsetEularAngle, g_GlobeEularAngle);
    glbEularAngle2Rotmat(g_GlobeEularAngle, r);
    LeaveCriticalSection(&g_GlobeRotMat_CS);
}

int code_convert(char* from_charset, char* to_charset, char* inbuf,
               int inlen, char* outbuf, int outlen)
{
    iconv_t cd;
    char** pin = &inbuf;  
    char** pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);  
    if(cd == 0)
       return -1;
 
    memset(outbuf,0,outlen);  
 
    if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen) == -1)
       return -1;  
 
    iconv_close(cd);
 
    return 0;  
}

char* u2g(char *inbuf)  
{
	if (strlen(inbuf) == 0)
	{
		return "";
	}
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
 
    return szOut;
}  

CString CMyUniverseDlg::FindXMLFilePath(CString pageStructPath)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFile(pageStructPath+"*.xml", &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox (_T("Invalid file handle.\n"));
        return "";
	}
	else
	{
        CString XML_FilePath = pageStructPath;
        CString XML_FileName = FindFileData.cFileName;
        XML_FilePath += XML_FileName;
        return XML_FilePath;
    }
}

void CMyUniverseDlg::ReadStoryConfigXML()
{
    //此函数调用了libxml2库，
    //使用方法参考：http://www.blogjava.net/wxb_nudt/archive/2007/11/18/161340.html
    //中文乱码问题参考：http://ling091.iteye.com/blog/295872

    //AfxMessageBox(m_page_struct_path);//本章节的目录名
    CString XML_FilePath = FindXMLFilePath(m_page_struct_path);
    if(XML_FilePath == "")
	{
		//AfxMessageBox (_T("Invalid file handle.\n"));
	}
	else
	{
        //AfxMessageBox(XML_FilePath);
        xmlDocPtr doc; //文件指针
        xmlNodePtr cur;//节点指针
        doc = xmlParseFile(XML_FilePath);
        cur = xmlDocGetRootElement(doc);
        //AfxMessageBox((char*)cur->name);
        cur = cur->xmlChildrenNode;
        while(cur!=NULL)
        {
            if(xmlHasProp(cur,(xmlChar*)"imageName"))
            {
                xmlChar* szAttr = xmlGetProp(cur,(xmlChar*) "imageName");
                //AfxMessageBox((char*)szAttr);
                xmlChar* imageName_xml = szAttr;
                xmlChar* imageName_xml_gb2312 = (xmlChar*)u2g((char*)imageName_xml);
                xmlChar* imageName_page = (xmlChar*)(LPSTR)(LPCTSTR)m_page_value;
                if(!xmlStrcmp(imageName_xml_gb2312, imageName_page))
                {
                    //AfxMessageBox((char*)filename_xml_gb2312);
                    //以确定查找到该Page的配置

                    //读取bPlaying
                    xmlChar* bPlaying_char = xmlGetProp(cur,(xmlChar*) "playing");
                    if(!xmlStrcmp(bPlaying_char, (xmlChar*)"true"))
                    {
                        g_StoryPage.bPlaying = 1;
                    }

                    //读取frameRate
                    xmlChar* frameRate_char = xmlGetProp(cur,(xmlChar*) "frameRate");
                    if(frameRate_char)
                    {
                        int frameRate = atoi((char*)frameRate_char);
                        g_StoryPage.frameRate = frameRate;
                        m_frame_rate = frameRate;
                        //UpdateData(PUT_DATA);
                    }                    
                    
                    //读取rotationRate
                    xmlChar* rotationRate_char = xmlGetProp(cur,(xmlChar*) "rotationRate");
                    if(rotationRate_char)
                    {
                        int rotationRate = atoi((char*)rotationRate_char);
                        g_StoryPage.rotationRate = rotationRate;
                        m_rotationRate_UI = rotationRate;
                        //UpdateData(PUT_DATA);
                    }

                    //读取rotateClockwise
                    xmlChar* rotateClockwise_char = xmlGetProp(cur,(xmlChar*) "rotateClockwise");
                    if(!xmlStrcmp(rotateClockwise_char, (xmlChar*)"true"))
                    {
                        g_StoryPage.bClockwise = 1;
                        m_clockwise_UI.SetCheck(true);
                        m_unclockwise_UI.SetCheck(false);
                        //UpdateData(PUT_DATA);
                    }
                    else
                    {
                        g_StoryPage.bClockwise = 0;
                        m_unclockwise_UI.SetCheck(true);
                        m_clockwise_UI.SetCheck(false);
                        //UpdateData(PUT_DATA);
                    }

                    //读取rotating
                    xmlChar* bRotating_char = xmlGetProp(cur,(xmlChar*) "rotating");
                    if(!xmlStrcmp(bRotating_char, (xmlChar*)"true"))
                    {
                        g_StoryPage.bRotating = 1;
                        m_rotating_UI = 1;
                        //UpdateData(PUT_DATA);
                    }
                    else
                    {
                        g_StoryPage.bRotating = 0;
                        m_rotating_UI = 0;
                        //UpdateData(PUT_DATA);
                    }
                }
                free(imageName_xml_gb2312);
                xmlFree(szAttr);
            }
            cur = cur->next;
        }
        xmlFreeDoc(doc);
    }
    UpdateData(PUT_DATA);
    return;
}

unsigned char* CMyUniverseDlg::convert (unsigned char *in, char *encoding)
{
    unsigned char *out;
    int ret,size,out_size,temp;
    xmlCharEncodingHandlerPtr handler;
    size = (int)strlen((const char *)in)+1;
    out_size = size*2-1;
    out = (unsigned char*)malloc((size_t)out_size);
    if (out) {
        handler = xmlFindCharEncodingHandler(encoding);
        if (!handler) {
            free(out);
            out = NULL;
        }
    }
    if (out) {
        temp=size-1;
        ret = handler->output(out, &out_size, in, &temp);
        if (ret || temp-size+1) {
            if (ret) {
                printf("conversion wasn't successful.\n");
            } else {
                printf("conversion wasn't successful. converted: \n");
              }
                free(out);
                out = NULL;
            } else {
                out = (unsigned char*)realloc(out,out_size+1);
                out[out_size]=0; /*null terminating out*/
            }
        } else {
            printf("no mem\n");
    }
    return (out);
}


void CMyUniverseDlg::OnBnClickedButtonPlay()
{
    g_StoryPage.bPlaying = 1;
}


void CMyUniverseDlg::OnBnClickedButtonPause()
{
    g_StoryPage.bPlaying = 0;
}

void CMyUniverseDlg::OnBnClickedButtonStep()
{
    g_StoryPage.nCurFrame += 1;
    g_StoryPage.nCurFrame %= g_StoryPage.nFrames;
}


void CMyUniverseDlg::OnEnChangeEditFrameRate()
{
    UpdateData(GET_DATA);
    g_StoryPage.frameRate = m_frame_rate;
}


void CMyUniverseDlg::OnBnClickedCheckAutoRot()
{
    UpdateData(GET_DATA);
    g_StoryPage.bRotating = !!m_rotating_UI;
}

LRESULT CMyUniverseDlg::OnGlbUpdateData(WPARAM wParam, LPARAM lParam)  
{  
    //UpdateData(wParam);  
    m_slider_rotx_ctrl.SetPos((int)(g_StorytellerEularAngle.m_1_Horz/360.0*100));
    m_slider_roty_ctrl.SetPos((int)(g_StorytellerEularAngle.m_2_Vert/360.0*100));
    m_slider_rotz_ctrl.SetPos((int)(g_StorytellerEularAngle.m_3_Axis/360.0*100));

    m_rotating_UI = g_StoryPage.bRotating;
    m_rotationRate_UI = g_StoryPage.rotationRate;
    if(g_StoryPage.bClockwise)
    {
        m_clockwise_UI.SetCheck(true);
        m_unclockwise_UI.SetCheck(false);
    }
    else
    {
        m_unclockwise_UI.SetCheck(true);
        m_clockwise_UI.SetCheck(false);
    }
    if(wParam)
    {
        UpdateData(PUT_DATA);
    }
    return 1;  
} 
LRESULT CMyUniverseDlg::OnGlbUdpReadOnePage(WPARAM wParam, LPARAM lParam)  
{
    CString chapter = m_chapter_value;
    CString page = m_page_value;

    UpdateData(PUT_DATA);

    ReadPageStruct();
    m_page_value = page;

    UpdateData(PUT_DATA);

    ReadOnePage(false);
    return 1;
}

void CMyUniverseDlg::OnEnChangemrotationrate()
{
    UpdateData(GET_DATA);
    if(m_rotationRate_UI > 30)
    {
        m_rotationRate_UI = 30;
    }
    else if(m_rotationRate_UI < 0)
    {
        m_rotationRate_UI = 0;
    }
    UpdateData(PUT_DATA);
    g_StoryPage.rotationRate = m_rotationRate_UI;
}


void CMyUniverseDlg::OnBnClickedUnclockwise()
{
    g_StoryPage.bClockwise = 0;
}


void CMyUniverseDlg::OnBnClickedClockwise()
{
    g_StoryPage.bClockwise = 1;
}


void CMyUniverseDlg::OnBnClickedRotReset()
{
	//EnterCriticalSection(&g_GlobeRotMat_CS);

 //   g_StorytellerEularAngle.m_1_Horz = (float)0;

 //   g_StorytellerEularAngle.m_2_Vert = (float)0;
 //
 //   g_StorytellerEularAngle.m_3_Axis = (float)0;

 //   StorytellerEular2GlobeEular(g_StorytellerEularAngle, g_OffsetEularAngle, g_GlobeEularAngle);
 //   glbEularAngle2Rotmat(g_GlobeEularAngle, r);
 //   LeaveCriticalSection(&g_GlobeRotMat_CS);
	m_slider_rotx_ctrl.SetPos(0);
	m_slider_roty_ctrl.SetPos(25);
	m_slider_rotz_ctrl.SetPos(0);
}



