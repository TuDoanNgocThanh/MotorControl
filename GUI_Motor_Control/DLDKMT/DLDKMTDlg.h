
// DLDKMTDlg.h : header file
//

#pragma once
#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartLabel.h"

#include "SerialCtrl.h"
#include "afxwin.h"

// CDLDKMTDlg dialog
class CDLDKMTDlg : public CDialogEx , public CSerialIO
{
// Construction
public:
	CDLDKMTDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLDKMT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnEventOpen(BOOL bSuccess);
	virtual void OnEventClose(BOOL bSuccess);
	virtual void OnEventRead(char* inPacket, int inLength);
	virtual void OnEventWrite(int nWritten);
	CComboBox m_comboSN;
	CComboBox m_comboBR;
	CButton m_btnOpen;
	BOOL bPortOpened;
	CStatic m_staticInfo;
	CListBox m_listboxRead;
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//	afx_msg void OnBnClickedRadioOffline();
	afx_msg void OnBnClickedButtonOpen();
protected:
	int m_rad_offline;
	int m_rad_online;
public:
	CComboBox m_CbDataSize;
	CComboBox m_comboParity;
	CComboBox m_comboHS;
	CComboBox m_comboMode;

	CButton m_Btn_POS;
	CButton m_Btn_VEL;
	CButton m_Btn_STT;
	CButton m_Btn_MOV;


	VOID ProcessData(char* data, int inLength);


	//void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD);
	//void CallBackTimer(HWND hwWin, UINT nUint, UINT_PTR uPtr, DWORD dw);
	VOID CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);


	CEdit Kp;
	CEdit Ki;
	CEdit Kd;
	CEdit m_EditSend1;
	CListBox m_Speed;
	afx_msg void OnCbnDropdownComboPort();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonPid();
	

	CChartStandardAxis* pBottomAxis;
	CChartStandardAxis* pLeftAxis;

	CButton m_Stop;
	afx_msg void OnBnClickedButtonStop();
	CButton m_Reset;
	afx_msg void OnBnClickedButtonReset();
	CChartCtrl pChartCtrlVel;
	CChartLineSerie* pChartVelSeries;
	afx_msg void OnBnClickedButtonClearGraph();
};
