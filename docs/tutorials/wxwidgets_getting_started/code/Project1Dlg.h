//---------------------------------------------------------------------------
//
// Name:        Project1Dlg.h
// Author:      Jos de Jong
// Created:     4-5-2008 18:41:12
// Description: Project1Dlg class declaration
//
//---------------------------------------------------------------------------

#ifndef __PROJECT1DLG_h__
#define __PROJECT1DLG_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif


//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
////Header Include End

////Dialog Style Start
#undef Project1Dlg_STYLE
#define Project1Dlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class Project1Dlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		Project1Dlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Project1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = Project1Dlg_STYLE);
		virtual ~Project1Dlg();
		void btnClickClick(wxCommandEvent& event);
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *btnClick;
		wxTextCtrl *txtAge;
		wxTextCtrl *txtName;
		wxStaticText *lblAge;
		wxStaticText *WxStaticText1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_BTNCLICK = 1006,
			ID_TXTAGE = 1004,
			ID_TXTNAME = 1003,
			ID_LBLAGE = 1002,
			ID_WXSTATICTEXT1 = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
