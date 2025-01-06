#pragma once
#include <afxwin.h>
class CMyStatic : public CStatic
{
    DECLARE_DYNAMIC(CMyStatic)

public:
    CMyStatic();
    virtual ~CMyStatic();

protected:
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon1;
    HICON m_hIcon2;
    HCURSOR m_hCursor;
    HCURSOR m_hOldCursor;

    HWND m_hWndPre;
    HWND m_hParentWnd;

    //设置标记，检测鼠标状态
    BOOL m_bFlag;
protected:
    //绘制窗口
    HDC DrawWndSPy(HWND hWnd);

    //获取窗口句柄后，将相关信息显示
    void GetWndInfo(HWND hWnd);

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    //获取父窗口句柄
    void SetParentHwnd(HWND hParentWnd);
    HWND m_ChosenWindow;
};

