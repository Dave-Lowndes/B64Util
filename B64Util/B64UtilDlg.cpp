// B64UtilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "B64Util.h"
#include "B64UtilDlg.h"
#include <atlenc.h>
#include <vector>
#include <string>
using std::string;
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CB64UtilDlg dialog

CB64UtilDlg::CB64UtilDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CB64UtilDlg::IDD, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CB64UtilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CB64UtilDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TO_B64, &CB64UtilDlg::OnBnClickedToB64)
	ON_BN_CLICKED(IDC_FROM_B64, &CB64UtilDlg::OnBnClickedFromB64)
	ON_EN_CHANGE(IDC_TEXT, &CB64UtilDlg::OnEnChangeText)
	ON_BN_CLICKED( IDC_AS_UNICODE, &CB64UtilDlg::OnBnClickedCharSet )
	ON_BN_CLICKED( IDC_AS_ANSI, &CB64UtilDlg::OnBnClickedCharSet )
END_MESSAGE_MAP()


// CB64UtilDlg message handlers

BOOL CB64UtilDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		const auto loaded = strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (loaded && !strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CheckRadioButton( IDC_AS_UNICODE, IDC_AS_ANSI, IDC_AS_UNICODE );

	/* Create a fixed pitch font - used when displaying binary data format */
	{
		/* Get the dialog font */
		CFont * pFont = GetFont();
	
		LOGFONT lf;
	
		/* Get the LOGFONT for the font */
		pFont->GetLogFont( &lf );

		/* Create a handle of the default font */
		m_hDefaultFont.CreateFontIndirect( &lf );
	
		/* Make it fixed pitch */
		lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
		/* Clear the face name as this would override the fixed pitch request */
		lf.lfFaceName[0] = _T('\0');
	
		m_hFixedFont.CreateFontIndirect( &lf );
	}

	// Initialise the 2 text fields byte lengths
	UpdateTextLengthDisplay();
	SetDlgItemInt( IDC_B64_TEXT_LENGTH, 0 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CB64UtilDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CB64UtilDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CB64UtilDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CB64UtilDlg::OnBnClickedToB64()
{
	/* Clear the destination field */
	SetDlgItemTextA( m_hWnd, IDC_B64TEXT, "" );

	/* Convert the text in the entry field to Base 64 and display it in the B64 field */
	CString sText;

	if ( GetDlgItemText( IDC_TEXT, sText ) != 0 )
	{
		const BYTE * pStringData;
		int StringLen;
		CStringA ANSIString;

		int TextType = GetCheckedRadioButton( IDC_AS_UNICODE, IDC_AS_ANSI );

		if ( TextType == IDC_AS_UNICODE )
		{
			/* Interpret the Unicode string as raw data */
			pStringData = reinterpret_cast<const BYTE *>(static_cast<LPCTSTR>( sText ));
			StringLen = sText.GetLength()*sizeof(TCHAR);
		}
		else
		{
			/* Treat the text as ANSI/MBCS */
			/* Copying to an ANSI string will perform a Unicode->ANSI/MBCS conversion */
			ANSIString = sText;
			pStringData = reinterpret_cast<const BYTE *>(static_cast<LPCSTR>( ANSIString ) );
			StringLen = ANSIString.GetLength();
		}

		/* Convert to base 64 */
		int Len = Base64EncodeGetRequiredLength( StringLen, ATL_BASE64_FLAG_NOCRLF );

		string sB64Text;
		/* Allocate the space needed */
		sB64Text.reserve( Len );
		/* Set its string size */
		sB64Text.resize( Len );

		BOOL bRv = Base64Encode( pStringData, StringLen, &sB64Text[0], &Len, ATL_BASE64_FLAG_NOCRLF );
		_ASSERT( bRv );

		/* Stick it in the Base64 control */
		SetDlgItemTextA( m_hWnd, IDC_B64TEXT, sB64Text.c_str() );

		// Update the Base64 text length
		SetDlgItemInt( IDC_B64_TEXT_LENGTH, Len );
	}
}

void CB64UtilDlg::OnBnClickedFromB64()
{
	/* Clear the destination field */
	SetDlgItemTextA( m_hWnd, IDC_TEXT, "" );

	/* Convert the text in the entry field to Base 64 and display it in the B64 field */
	const int cchB64 = GetDlgItem( IDC_B64TEXT )->GetWindowTextLength();

	if ( cchB64 != 0 )
	{
		CStringA sB64Text;
		LPSTR psB64 = sB64Text.GetBufferSetLength( cchB64+1 );

		if ( GetDlgItemTextA( m_hWnd, IDC_B64TEXT, psB64, cchB64+1 ) != 0 )
		{
			sB64Text.ReleaseBuffer();

			/* Convert from base 64 */
			int Len = Base64DecodeGetRequiredLength( sB64Text.GetLength() );

			vector<BYTE> vDecoded( Len );

			BOOL bRV = Base64Decode( sB64Text, sB64Text.GetLength(), &vDecoded[0], &Len );
			_ASSERT( bRV );

			bool bDispBinary = IsDlgButtonChecked( IDC_DISPLAY_AS_BINARY ) == BST_CHECKED;
			bool bUnicode = false;

			/* Use the Windows guessing function to guess if the converted data is a Unicode string or something else */
			int UniFlags = /*IS_TEXT_UNICODE_STATISTICS*/IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_NULL_BYTES;
			if ( IsTextUnicode( &vDecoded[0], Len, &UniFlags ) )
			{
				if ( ( UniFlags & IS_TEXT_UNICODE_UNICODE_MASK ) == 0 )
				{
					bDispBinary = true;
				}
				else
				{
					bUnicode = true;
				}
			}
			else
			{
				/* Think it's ANSI/MBCS */
			}

			if ( bDispBinary )
			{
				/* Convert to a hex pair representation */
				CString sHex( _T("Binary data:\r\n") );

				/* Format in a traditional hex view representation:
				* xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx cccccccccccccccccc
				*/
				for ( int indx = 0; indx < Len;  )
				{
					CString sLine;
					int col;

					for ( col = 0; (col < 16 ) && (indx+col < Len); ++col )
					{
						CString sTmp;
						sTmp.Format( L"%02x ", static_cast<BYTE>( vDecoded[indx + col] ) );
						sLine += sTmp;
					}

					/* A complete row of hex chars will take 3 chars per column.
					* Calculate the number of characters that remain in the
					* final incomplete row and build up the string to align it.
					*/
					int CharsRemaining = 3*(16 - col);
					for ( ; CharsRemaining != 0; --CharsRemaining )
					{
						sLine += _T(' ');
					}

					/* Now output the character form */
					for ( col = 0; (col < 16 ) && (indx < Len); ++col, ++indx )
					{
						char ch = static_cast<char>( vDecoded[indx] );
						/* Can't display a null, CR, LF & tab messes up the formatting, so replace it with something we can */
						if ( ( ch == '\0' ) || ( ch == '\r') || ( ch == '\n' ) || ( ch == '\t' ) )
						{
							ch = '.';
						}
						sLine += ch;
					}
					sLine += _T("\r\n");
					sHex += sLine;
				}
#if 0
				for ( int indx = 0; indx < Len; ++indx )
				{
					CString sTmp;
					sTmp.Format( L"%02x", static_cast<BYTE>( vDecoded[indx] ) );

					if ( indx < Len-1 )
					{
						sTmp += L' ';
					}

					sHex += sTmp;
				}
#endif
				/* Set the edit field to use the fixed pitch font */
				CWnd * pEdit = GetDlgItem( IDC_TEXT );
				pEdit->SetFont( &m_hFixedFont );

				SetDlgItemText( IDC_TEXT, sHex );

				/* Uncheck the radio buttons */
				CheckDlgButton( IDC_AS_ANSI, BST_UNCHECKED );
				CheckDlgButton( IDC_AS_UNICODE, BST_UNCHECKED );
			}
			else
			{
				/* Set the edit field to use the default font */
				CWnd * pEdit = GetDlgItem( IDC_TEXT );
				pEdit->SetFont( &m_hDefaultFont );

				if ( bUnicode )
				{
					CString sText( reinterpret_cast<LPCTSTR>( &vDecoded[0] ), Len/sizeof(TCHAR) );

					/* Stick it in the other control */
					SetDlgItemText( IDC_TEXT, sText );

					CheckRadioButton( IDC_AS_UNICODE, IDC_AS_ANSI, IDC_AS_UNICODE );
				}
				else
				{
					/* It's an ANSI/MBCS string */
					CStringA sText( reinterpret_cast<LPCSTR>( &vDecoded[0] ), Len );

					/* Stick it in the other control */
					SetDlgItemTextA( m_hWnd, IDC_TEXT, sText );

					CheckRadioButton( IDC_AS_UNICODE, IDC_AS_ANSI, IDC_AS_ANSI );
				}
			}

			///* Use the Windows guessing function to guess if the converted data is a Unicode string or something else */
			//int UniFlags = /*IS_TEXT_UNICODE_STATISTICS*/IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_NULL_BYTES;
			//if ( IsTextUnicode( &vDecoded[0], Len, &UniFlags ) )
			//{
			//	if ( ( UniFlags & IS_TEXT_UNICODE_UNICODE_MASK ) == 0 )
			//	{
			//		bDispBinary = true;
			//	}
			//	else
			//	{
			//	}

			//	CheckRadioButton( IDC_AS_UNICODE, IDC_AS_ANSI, IDC_AS_UNICODE );
			//}
			//else
			//{
			//}
		}
	}
}

void CB64UtilDlg::OnEnChangeText()
{
	/* The user has entered something into the text field - change the font to the default */
	CWnd * pEdit = GetDlgItem( IDC_TEXT );
	pEdit->SetFont( &m_hDefaultFont );

	UpdateTextLengthDisplay();
}

void CB64UtilDlg::UpdateTextLengthDisplay()
{
	CWnd * pEdit = GetDlgItem( IDC_TEXT );

	// Update the byte count display
	int Len = pEdit->GetWindowTextLength();

	// Assuming ANSI or Unicode text?
	int TextType = GetCheckedRadioButton( IDC_AS_UNICODE, IDC_AS_ANSI );

	if ( TextType == IDC_AS_UNICODE )
	{
		Len *= 2;
	}

	SetDlgItemInt( IDC_TEXT_LENGTH, Len );
}

void CB64UtilDlg::OnBnClickedCharSet()
{
	UpdateTextLengthDisplay();
}
