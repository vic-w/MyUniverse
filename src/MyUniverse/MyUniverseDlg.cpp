
// MyUniverseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyUniverse.h"
#include "MyUniverseDlg.h"
#include "afxdialogex.h"
#include "GlobeThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CStoryPage g_StoryPage;

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
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
END_MESSAGE_MAP()


// CMyUniverseDlg 消息处理程序

BOOL CMyUniverseDlg::OnInitDialog()
{
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

    //读取ini文件中的配置
    char lpStoryPath[512];
    GetPrivateProfileString("MyUniverseCfg", "StoryPath", "", lpStoryPath, 512, ".\\config.ini");
    m_story_path = lpStoryPath;
    UpdateData(0);
    ReadChapterStruct();

    CreateThread(0, 0, GlobeThread, 0,0,0);//启动OpenGL显示线程

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
        CDialogEx::OnCancel();
    }
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(GET_DATA);
    m_edit_rotx = (int)(m_slider_rotx/100.0*360);
    UpdateData(PUT_DATA);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(GET_DATA);
    m_edit_roty = (int)(m_slider_roty/100.0*360);
    UpdateData(PUT_DATA);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(GET_DATA);
    m_edit_rotz = (int)(m_slider_rotz/100.0*360);
    UpdateData(PUT_DATA);
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
		//AfxMessageBox ("Invalid file handle.\n");
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
            //AfxMessageBox("ERROR_NO_MORE_FILES");
		}
	}
    m_chapter_select.SetCurSel(0);
    ReadPageStruct();
}

void CMyUniverseDlg::ReadPageStruct()//次处支持：folder，dds，jpg，avi，wma
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    m_page_select.ResetContent();

    UpdateData(1);
    m_page_struct_path = m_story_path;
    int pathLength = m_page_struct_path.GetLength();
    char lastChar = *(m_page_struct_path.GetBuffer()+pathLength-1);
    if(lastChar != '\\')
    m_page_struct_path +="\\";
    m_page_struct_path += m_chapter_value;
    m_page_struct_path += "\\";

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
                      || _stricmp(suffix,".dds") ==0
                      || _stricmp(suffix,".avi") ==0
                      || _stricmp(suffix,".wma") ==0
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
            //AfxMessageBox("ERROR_NO_MORE_FILES");
		}
	}
    m_page_select.SetCurSel(0);
    ReadOnePage();
}

void CMyUniverseDlg::ReadOnePage()
{
    g_StoryPage.bEmpty = 1;
    UpdateData(1);
    CString pagePath = m_page_struct_path + m_page_value;
    //AfxMessageBox(pagePath);
    char* suffix = pagePath.GetBuffer() + pagePath.GetLength() - 4;
    if( _stricmp(suffix,".jpg") == 0)
    {
        //AfxMessageBox("this is a jpg file");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = JPG;
    }
    else if( _stricmp(suffix,".png") == 0)
    {
        //AfxMessageBox("this is a png file");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = PNG;
    }
    else if( _stricmp(suffix,".dds") == 0)
    {
        //AfxMessageBox("this is a dds file");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = DDS;
    }
    else if( _stricmp(suffix,".avi") == 0)
    {
        AfxMessageBox("this is a avi file. 暂不支持此格式");
    }
    else if( _stricmp(suffix,".wma") == 0)
    {
        AfxMessageBox("this is a wma file. 暂不支持此格式");
    }
    else
    {
        AfxMessageBox("this is a folder. 暂不支持此格式");
    }
}

void CMyUniverseDlg::OnCbnSelchangeComboChapter()
{
    ReadPageStruct();
}


void CMyUniverseDlg::OnCbnSelchangeComboPage()
{
    ReadOnePage();
}
