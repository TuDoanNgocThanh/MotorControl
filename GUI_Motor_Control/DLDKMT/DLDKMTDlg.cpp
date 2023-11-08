
// DLDKMTDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DLDKMT.h"
#include "DLDKMTDlg.h"
#include "afxdialogex.h"
#include "stdio.h"
#include <wtypes.h>
#include <winnt.h>
#include <vector>
#include <array>
#include <winsmcrd.h>
#include <cstddef>
#include <windef.h>
#include <string>
#include <iostream>
#include "SerialCtrl.h"
//#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define EVENT_SEND_BY_TIMER 2
//STX
BYTE STX[] = { 0x02 };
//CMD
BYTE MOV[] = { 0x4D,0x4F,0x56,0x4C };
BYTE POS[] = { 0x47,0x50,0x4F,0x53 };
BYTE VEL[] = { 0x47,0x56,0x45,0x4C };
BYTE STT[] = { 0x47,0x53,0x54,0x54 };
//OPTION
BYTE OPT[] = { 0x0,0x0,0x00 };;
//DATA
BYTE DATA[8] = { 0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01 };
//SYNC/ACK
BYTE SYNC[] = { 0x16 };
BYTE ACK[] = { 0x06 };
//ETX
BYTE ETX[] = { 0x03 };

//RECEIVE
BYTE RECEIVE[18] = { };
BYTE PROCESSDATA[8] = {};

int fTime[200];
double cnt_draw = 0;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDLDKMTDlg dialog



CDLDKMTDlg::CDLDKMTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLDKMT_DIALOG, pParent)
	, m_rad_offline(1)
	, m_rad_online(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDLDKMTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboSN);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBR);
	DDX_Control(pDX, IDC_COMBO_DATASIZE, m_CbDataSize);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_comboParity);
	DDX_Control(pDX, IDC_COMBO_HandShake, m_comboHS);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_EDIT_Kp, Kp);
	DDX_Control(pDX, IDC_EDIT_Ki, Ki);
	DDX_Control(pDX, IDC_EDIT_Kd, Kd);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_LIST_RECV, m_listboxRead);
	DDX_Control(pDX, IDC_EDIT_Send, m_EditSend1);
	DDX_Control(pDX, IDC_LIST_Speed, m_Speed);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_Reset);
	DDX_Control(pDX, IDC_CHARTCTRL_VEL, pChartCtrlVel);
}

BEGIN_MESSAGE_MAP(CDLDKMTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CDLDKMTDlg::OnCbnDropdownComboPort)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDLDKMTDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_Send, &CDLDKMTDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_PID, &CDLDKMTDlg::OnBnClickedButtonPid)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDLDKMTDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDLDKMTDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_Clear_Graph, &CDLDKMTDlg::OnBnClickedButtonClearGraph)
END_MESSAGE_MAP()


// CDLDKMTDlg message handlers

BOOL CDLDKMTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	Kp.SetWindowText(L"2.2");
	Ki.SetWindowText(L"1");
	Kd.SetWindowText(L"0.003");
	m_EditSend1.SetWindowText(L"0");
	// TODO: Add extra initialization here

	m_comboBR.InsertString(0, L"4800");
	m_comboBR.InsertString(1, L"9600");
	m_comboBR.InsertString(2, L"19200");
	m_comboBR.InsertString(3, L"115200");
	m_comboBR.SetCurSel(3);

	//m_SerialTab.m_Serial_DataSize.ResetContent();
	m_CbDataSize.InsertString(0, (CString)"8");
	m_CbDataSize.InsertString(1, (CString)"7");
	m_CbDataSize.SetCurSel(0);

	//m_SerialTab.m_Serial_Parity.ResetContent();
	m_comboParity.InsertString(0, (CString)"Odd");
	m_comboParity.InsertString(1, (CString)"Mark");
	m_comboParity.InsertString(2, (CString)"Even");
	m_comboParity.InsertString(3, (CString)"None");
	m_comboParity.SetCurSel(3);

	//m_SerialTab.m_Serial_HandShake.ResetContent();

	m_comboHS.InsertString(0, (CString)"OFF");
	m_comboHS.InsertString(1, (CString)"Xon/Xoff");
	m_comboHS.InsertString(2, (CString)"RTS/CTS");
	m_comboHS.SetCurSel(0);

	CChartCtrl ref;

	ref.RemoveAllSeries();
	pChartCtrlVel.EnableRefresh(true);
	pBottomAxis = pChartCtrlVel.CreateStandardAxis(CChartCtrl::BottomAxis);
	pLeftAxis = pChartCtrlVel.CreateStandardAxis(CChartCtrl::LeftAxis);
	pBottomAxis->SetMinMax(0, 10);
	pLeftAxis->SetMinMax(-300, 300);
	//pLeftAxis->GetLabel()->SetText("Intensity");
	pBottomAxis->SetTickIncrement(true, 20.0);
	pBottomAxis->SetDiscrete(false);
	pBottomAxis->EnableScrollBar(true);
	pChartVelSeries = pChartCtrlVel.CreateLineSerie();
	pChartVelSeries->ClearSerie();
	pChartVelSeries->SetWidth(2); //line width
	pChartVelSeries->SetColor(RGB(0, 0, 255)); //color of line


	//m_SerialTab.m_Serial_Mode.ResetContent();
	m_comboMode.InsertString(0, (CString)"Data");
	m_comboMode.SetCurSel(0);
	return TRUE;  // return TRUE  unless you set the focus to a control


	for (int i = 0; i < 200; i++)
	{
		fTime[i] = i ;
	}

//	Kp.SetWindowText((LPCTSTR)'0.7');
}

void CDLDKMTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDLDKMTDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDLDKMTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDLDKMTDlg::OnEventOpen(BOOL bSuccess)
{
	CString str;
	if (bSuccess)
	{
		str = m_strPortName + L" open successfully";

		bPortOpened = TRUE;
		m_btnOpen.SetWindowText(L"Close");

	}
	else
	{
		str = m_strPortName + L" open failed";
	}
	m_staticInfo.SetWindowText(str);
}

void CDLDKMTDlg::OnEventClose(BOOL bSuccess)
{
	CString str;
	if (bSuccess)
	{
		str = m_strPortName + L" close successfully";
		bPortOpened = FALSE;
		m_btnOpen.SetWindowText(L"Open");

	}
	else
	{
		str = m_strPortName + L" close failed";
	}
	m_staticInfo.SetWindowText(str);
}

void CDLDKMTDlg::OnEventRead(char* inPacket, int inLength)
{
	union {
		float a;
		char b[4];
	} my_union_t;
	CString Cmd;
	float my_num;
	double speed;
	CString csInPacket;

	for (UINT i = 0; i < (UINT)inLength; i++) {
		csInPacket.AppendFormat(L"%02X ", inPacket[i]);
	}

	if (inPacket[15] == 'S')
	{
		for (int y = 0; y < 4; y++)
		{
			my_union_t.b[y] = inPacket[y + 11];
		}
		my_num = my_union_t.a;
		speed = (double)my_union_t.a;
		Cmd.Format(_T("%f"), my_num);
		m_Speed.InsertString(0, Cmd);

		CString str;
		str.Format(L"%d bytes read", inLength);

		m_staticInfo.SetWindowText(str);

		//if ((int)cnt_draw +1 % 5 == 0) 
		//{
		//	pBottomAxis->SetMinMax(0 + cnt_draw, 10 +cnt_draw);
		//}
			pChartVelSeries->AddPoint(cnt_draw,speed);
			cnt_draw += 0.04;


	}
	else if (inPacket[15] == 'R')
	{
		m_listboxRead.InsertString(0, csInPacket);
		csInPacket.Format(L"Receive: ");
		m_listboxRead.InsertString(0, csInPacket);
		CString str;
		str.Format(L"%d bytes read", inLength);

		m_staticInfo.SetWindowText(str);
	}
	//------------------------------------------------
	//union {
	//	float a;
	//	char b[4];
	//} my_union_t;
	//CString Cmd;
	//float my_num;

	//CString csInPacket;



	//for (UINT i = 0; i < (UINT)inLength; i++) {
	//	/*csInPacket.AppendFormat(L"%02X ", inPacket[i]);*/
	//	csInPacket.AppendFormat(L"%02X ", inPacket[i]);
	//}

	//if (inPacket[15] == 'S')
	//{
	//	for (int y = 0; y < 4; y++) {
	//		my_union_t.b[y] = inPacket[y + 11];
	//	}
	//	my_num = my_union_t.a;
	//	Cmd.Format(_T("%f"), my_num);
	//	m_Speed.InsertString(0, Cmd);

	//	/*m_listboxRead.InsertString(0, Cmd);*/
	//	ProcessData(inPacket, inLength);

	//	//m_listboxRead.InsertString(0, csInPacket);
	//	/*m_listboxRead.AddString(inPacket);*/

	//	CString str;
	//	str.Format(L"%d bytes read", inLength);

	//	m_staticInfo.SetWindowText(str);
	//}
	////---------------------------------------------------------------------
	//else if (inPacket[15] == 'R') {
	//	//csInPacket.Format(L"Receive: ");
	//	//m_listboxRead.InsertString(0, csInPacket);
	//	//csInPacket.Empty();

	//	//for (UINT i = 0; i < (UINT)inLength; i++) {
	//	//	csInPacket.AppendFormat(L"%02X ", inPacket[i]);
	//	//}
	//	m_listboxRead.InsertString(0, csInPacket);
	//	csInPacket.Format(L"Receive: ");
	//	m_listboxRead.InsertString(0, csInPacket);
	//	ProcessData(inPacket, inLength);

	//	//m_listboxRead.InsertString(0, csInPacket);
	//	/*m_listboxRead.AddString(inPacket);*/

	//	CString str;
	//	str.Format(L"%d bytes read", inLength);

	//	m_staticInfo.SetWindowText(str);
	//}

//		pChartVelSeries->ClearSerie();

}
void CDLDKMTDlg::OnEventWrite(int nWritten)
{
	if (nWritten > 0)
	{
		CString str;
		str.Format(L"%d bytes written", nWritten);
		m_staticInfo.SetWindowText(str);
	}
	else
	{
		m_staticInfo.SetWindowText(L"Write failed");
	}

}


//void CDLDKMTDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	BYTE FRAME[50] = {};
//	UINT index = 0;
//	CString cmd;
//
//	if (!GetPortActivateValue()) return;
//
//	switch (nIDEvent) {
//	case EVENT_SEND_BY_TIMER:
//		memcpy(FRAME + index, STX, sizeof(STX));
//		index += sizeof(STX);
//		memcpy(FRAME + index, STT, sizeof(STT));
//		index += sizeof(STT);
//		memcpy(FRAME + index, OPT, sizeof(OPT));
//		index += sizeof(OPT);
//		memcpy(FRAME + index, DATA, sizeof(DATA));
//		index += sizeof(DATA);
//		memcpy(FRAME + index, SYNC, sizeof(SYNC));
//		index += sizeof(SYNC);
//		memcpy(FRAME + index, ETX, sizeof(ETX));
//		index += sizeof(ETX);
//		/*std::copy(FRAME.begin(), FRAME.end(), res);*/
//		Write((char*)FRAME, index);
//		cmd.Format(L"%s", L"Timer STT CMD: ");
//		m_listboxRead.InsertString(0, cmd);
//		cmd.Empty();
//		for (UINT i = 0; i < index; i++) {
//
//			cmd.AppendFormat(L"%02X ", FRAME[i]);
//
//		}
//		m_listboxRead.InsertString(0, cmd);
//		break;
//	default:
//		break;
//	}

//	__super::OnTimer(nIDEvent);
//}




void CDLDKMTDlg::OnCbnDropdownComboPort()
{
	m_comboSN.ResetContent();

	TCHAR lpTargetPath[5000]; // buffer to store the path of the COMPORTS
	DWORD test;
	bool gotPort = 0; // in case the port is not found

	for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
	{
		CString str;
		str.Format(_T("%d"), i);
		CString ComName = CString("COM") + CString(str); // converting to COM0, COM1, COM2


		test = QueryDosDevice(ComName, (LPWSTR)lpTargetPath, 2000);

		// Test the return value and error if any
		if (test != 0) //QueryDosDevice returns zero if it didn't find an object
		{
			m_comboSN.AddString((CString)ComName); // add to the ComboBox
			gotPort = 1; // found port
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			lpTargetPath[2000]; // in case the buffer got filled, increase size of the buffer.
			continue;
		}

	}

	if (!gotPort) // if not port
		m_comboSN.AddString((CString)"No Active Ports Found"); // to display error message incase no ports found
}




void CDLDKMTDlg::OnBnClickedButtonOpen()
{

	if (bPortOpened == FALSE)
	{
		//DCB dcb;
		CString strPortName;
		CString strBaudRate;
		CString strDataSize;
		CString strParity;
		CString strHandShake;
		CString strMode;
		m_comboSN.GetLBText(m_comboSN.GetCurSel(), strPortName);
		m_comboBR.GetLBText(m_comboBR.GetCurSel(), strBaudRate);
		m_comboHS.GetLBText(m_comboHS.GetCurSel(), strHandShake);
		m_comboParity.GetLBText(m_comboParity.GetCurSel(), strParity);
		m_comboMode.GetLBText(m_comboMode.GetCurSel(), strMode);
		m_CbDataSize.GetLBText(m_CbDataSize.GetCurSel(), strDataSize);


		OpenPort(strPortName, strBaudRate);

	}
	else
	{
		ClosePort();

	}
	// TODO: Add your control notification handler code here
}



void CDLDKMTDlg::OnBnClickedButtonSend()
{
	union {
		float a;
		char b[4];
	} my_union_t;
	CString Cmd;
	float my_num;
	char send[17];
	send[0] = '1';
	send[1] = 'V';
	send[2] = 'M';
	send[15] = 'S';
	send[16] = '0';

	if (!GetPortActivateValue()) return;
	m_EditSend1.GetWindowText(Cmd);
	//m_listboxRead.InsertString(0, Cmd);
	my_num = std::stof((LPCTSTR)Cmd);
	my_union_t.a = my_num;
	send[11] = my_union_t.b[0];
	send[12] = my_union_t.b[1];
	send[13] = my_union_t.b[2];
	send[14] = my_union_t.b[3];
	Write(send, 17);
	// TODO: Add your control notification handler code here
}


void CDLDKMTDlg::OnBnClickedButtonPid()
{
	union {
		float a;
		char b[4];
	} my_union_t;
	CString Cmd;
	float my_num;
	char send[17];
	send[0] = '1';
	send[1] = 'S';
	send[2] = 'M';
	send[15] = 'S';
	send[16] = '0';

	//	m_listboxRead.InsertString(0, L"Kp Ki Kd");
	if (!GetPortActivateValue()) return;
	Kp.GetWindowText(Cmd);
	//	m_listboxRead.InsertString(0, Cmd);
	my_num = std::stof((LPCTSTR)Cmd);
	my_union_t.a = my_num;
	//	Write(my_union_t.b, 4);
	send[3] = my_union_t.b[0];
	send[4] = my_union_t.b[1];
	send[5] = my_union_t.b[2];
	send[6] = my_union_t.b[3];

	if (!GetPortActivateValue()) return;
	Ki.GetWindowText(Cmd);
	//	m_listboxRead.InsertString(0, Cmd);
	my_num = std::stof((LPCTSTR)Cmd);
	my_union_t.a = my_num;
	send[7] = my_union_t.b[0];
	send[8] = my_union_t.b[1];
	send[9] = my_union_t.b[2];
	send[10] = my_union_t.b[3];
	//Write(send + 3, 8);

	if (!GetPortActivateValue()) return;
	Kd.GetWindowText(Cmd);
	//	m_listboxRead.InsertString(0, Cmd);
	my_num = std::stof((LPCTSTR)Cmd);
	my_union_t.a = my_num;
	send[11] = my_union_t.b[0];
	send[12] = my_union_t.b[1];
	send[13] = my_union_t.b[2];
	send[14] = my_union_t.b[3];
	Write(send, 17);
	// TODO: Add your control notification handler code here
}


void CDLDKMTDlg::OnBnClickedButtonStop()
{
	union {
		float a;
		char b[4];
	} my_union_t;
	CString Cmd;
	float my_num;
	char send[17];
	send[0] = '1';
	send[1] = 'D';
	send[2] = 'M';
	send[3] = 'S';
	send[4] = 'T';
	send[5] = 'O';
	send[6] = 'P';

	send[15] = 'S';
	send[16] = '0';

	Write(send, 17);
	// TODO: Add your control notification handler code here
}


void CDLDKMTDlg::OnBnClickedButtonReset()
{
	union {
		float a;
		char b[4];
	} my_union_t;
	CString Cmd;
	float my_num;
	char send[17];
	send[0] = '1';
	send[1] = 'D';
	send[2] = 'M';

	send[3] = 'R';
	send[4] = 'E';
	send[5] = 'S';
	send[6] = 'E';
	send[7] = 'T';

	send[15] = 'S';
	send[16] = '0';

	Write(send, 17);
	// TODO: Add your control notification handler code here
}


void CDLDKMTDlg::OnBnClickedButtonClearGraph()
{
	CChartCtrl ref;

	ref.RemoveAllSeries();
	pChartCtrlVel.EnableRefresh(true);
	pBottomAxis = pChartCtrlVel.CreateStandardAxis(CChartCtrl::BottomAxis);
	pLeftAxis = pChartCtrlVel.CreateStandardAxis(CChartCtrl::LeftAxis);
	pBottomAxis->SetMinMax(0, 10);
	pLeftAxis->SetMinMax(-300, 300);
	//pLeftAxis->GetLabel()->SetText("Intensity");
	pBottomAxis->SetTickIncrement(true, 20.0);
	pBottomAxis->SetDiscrete(false);
	pBottomAxis->EnableScrollBar(true);
	pChartVelSeries = pChartCtrlVel.CreateLineSerie();
	pChartVelSeries->ClearSerie();
	pChartVelSeries->SetWidth(2); //line width
	pChartVelSeries->SetColor(RGB(0, 0, 255)); //color of line
	cnt_draw = 0;
	// TODO: Add your control notification handler code here
}

