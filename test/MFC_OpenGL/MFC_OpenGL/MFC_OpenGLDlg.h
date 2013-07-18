
// MFC_OpenGLDlg.h : 头文件
//

#pragma once
#include "OpenGL.h"

// CMFC_OpenGLDlg 对话框
class CMFC_OpenGLDlg : public CDialogEx
{
private:
      COpenGL m_OpenglWindow;
// 构造
public:
	CMFC_OpenGLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_OPENGL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
