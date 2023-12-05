// B64UtilDlg.h : header file
//

#pragma once


// CB64UtilDlg dialog
class CB64UtilDlg : public CDialog
{
// Construction
public:
	CB64UtilDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_B64UTIL_DIALOG };

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
	afx_msg void OnBnClickedToB64();
	afx_msg void OnBnClickedFromB64();
private:
	CFont m_hFixedFont;
	CFont m_hDefaultFont;
private:
	afx_msg void OnEnChangeText();
	void UpdateTextLengthDisplay();
	afx_msg void OnBnClickedCharSet();
};
