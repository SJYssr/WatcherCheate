
// WatcherCheaterDlg.h: 头文件
//

#pragma once

#include "MyStatic.h"

// CWatcherCheaterDlg 对话框
class CWatcherCheaterDlg : public CDialogEx
{
// 构造
public:
	CWatcherCheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WATCHERCHEATER_DIALOG };
#endif

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
	CMyStatic m_PictureCtrl;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedShow();
	BOOL ChangeAffinity(HWND Wnd, DWORD dwAffinity);
};
