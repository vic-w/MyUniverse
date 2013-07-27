
// MyUniverseDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "GlbRot.h"
#include "resource.h"
#include "afxcmn.h"

#define GET_DATA 1 
#define PUT_DATA 0

//自定义消息（更新slider_rotz),供TimingThread调用
#define WM_GLB_UPDATEDATA (WM_USER+100)
#define WM_GLB_UDPREADONEPAGE (WM_USER+101)

// CMyUniverseDlg 对话框
class CMyUniverseDlg : public CDialogEx
{
// 构造
public:
	CMyUniverseDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CMyUniverseDlg();
// 对话框数据
	enum { IDD = IDD_MYUNIVERSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
private:
    CString GetPageStructPath(CString StoryPath, CString ChapterName);
    void ReadOnePage(bool bUpdateDataFromUI);
    void ReadChapterStruct();
    void ReadPageStruct();
    void ReadFolderContent(CString folderPath, CString suffix);
    void GlobeRotate(int Horz, int Vert, int Axis, GlbRotmat &r);
    void ReadStoryConfigXML();
    CString FindXMLFilePath(CString pageStructPath);
    unsigned char* convert (unsigned char *in, char *encoding);
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    LRESULT OnGlbUpdateData(WPARAM wParam, LPARAM lParam);//自定义消息的响应函数
    LRESULT OnGlbUdpReadOnePage(WPARAM wParam, LPARAM lParam);//自定义消息的响应函数
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    int m_edit_rotx;
    int m_edit_roty;
    int m_edit_rotz;
    int m_slider_rotx;
    int m_slider_roty;
    int m_slider_rotz;
    afx_msg void OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditRotx();
    afx_msg void OnEnChangeEditRoty();
    afx_msg void OnEnChangeEditRotz();
    afx_msg void OnCbnSelchangeComboChapter();
    afx_msg void OnBnClickedButtonBrowse();
    afx_msg void OnCbnSelchangeComboPage();
    CComboBox m_chapter_select; //章节，下拉选择框
    CComboBox m_page_select;    //页面，下拉选择框
    CString m_story_path;       //章节的总目录名
    CString m_chapter_value;    //章节的子目录名
    CString m_page_struct_path; //页面文件的目录
    CString m_page_value;       //页面文件的文件名（或别名）
    afx_msg void OnBnClickedButtonStep();
    afx_msg void OnBnClickedButtonPlay();
    afx_msg void OnBnClickedButtonPause();
    int m_frame_rate;
    afx_msg void OnEnChangeEditFrameRate();
    afx_msg void OnBnClickedCheckAutoRot();
    BOOL m_rotating_UI;
    CSliderCtrl m_slider_rotz_ctrl;
    CEdit m_edit_rotz_ctrl;
    CSliderCtrl m_slider_rotx_ctrl;
    CSliderCtrl m_slider_roty_ctrl;
    int m_rotationRate_UI;
    afx_msg void OnEnChangemrotationrate();
    CButton m_unclockwise_UI;
    CButton m_clockwise_UI;
    afx_msg void OnBnClickedUnclockwise();
    afx_msg void OnBnClickedClockwise();
};
