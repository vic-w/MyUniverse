// FlashPlayer_vc6Dlg.h : header file
//

#if !defined(AFX_FLASHPLAYER_VC6DLG_H__8C413D2B_E8FB_4F5F_89B9_EAA38C02EEF5__INCLUDED_)
#define AFX_FLASHPLAYER_VC6DLG_H__8C413D2B_E8FB_4F5F_89B9_EAA38C02EEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFlashPlayer_vc6Dlg dialog

class CFlashPlayer_vc6Dlg : public CDialog
{
// Construction
public:
	CFlashPlayer_vc6Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashPlayer_vc6Dlg)
	enum { IDD = IDD_FLASHPLAYER_VC6_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashPlayer_vc6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFlashPlayer_vc6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHPLAYER_VC6DLG_H__8C413D2B_E8FB_4F5F_89B9_EAA38C02EEF5__INCLUDED_)
