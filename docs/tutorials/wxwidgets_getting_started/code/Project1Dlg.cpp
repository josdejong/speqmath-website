//---------------------------------------------------------------------------
//
// Name:        Project1Dlg.cpp
// Author:      Jos de Jong
// Created:     4-5-2008 18:41:12
// Description: Project1Dlg class implementation
//
//---------------------------------------------------------------------------

#include "Project1Dlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// Project1Dlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(Project1Dlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(Project1Dlg::OnClose)
	EVT_BUTTON(ID_BTNCLICK,Project1Dlg::btnClickClick)
END_EVENT_TABLE()
////Event Table End

Project1Dlg::Project1Dlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

Project1Dlg::~Project1Dlg()
{
} 

void Project1Dlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetTitle(wxT("Project1"));
	SetIcon(wxNullIcon);
	SetSize(8,8,320,334);
	Center();
	

	btnClick = new wxButton(this, ID_BTNCLICK, wxT("Click here"), wxPoint(141,85), wxSize(125,25), 0, wxDefaultValidator, wxT("btnClick"));
	btnClick->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	txtAge = new wxTextCtrl(this, ID_TXTAGE, wxT("<enter your age here>"), wxPoint(65,48), wxSize(200,20), 0, wxDefaultValidator, wxT("txtAge"));
	txtAge->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	txtName = new wxTextCtrl(this, ID_TXTNAME, wxT("<enter your name here>"), wxPoint(65,18), wxSize(200,20), 0, wxDefaultValidator, wxT("txtName"));
	txtName->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	lblAge = new wxStaticText(this, ID_LBLAGE, wxT("Age"), wxPoint(16,49), wxDefaultSize, 0, wxT("lblAge"));
	lblAge->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Name"), wxPoint(16,20), wxDefaultSize, 0, wxT("WxStaticText1"));
	WxStaticText1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	////GUI Items Creation End
}

void Project1Dlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

/*
 * btnClickClick
 */
void Project1Dlg::btnClickClick(wxCommandEvent& event)
{
    // get the text from the two editboxes
    wxString nameValue = txtName->GetValue();
    wxString ageValue = txtAge->GetValue();
    
    // convert the string with the age to a long value
    long age = 0;
    ageValue.ToLong(&age, 10);

    // calculate the year of birth from the given age and current year, 
    // then convert it to a string
    int yearnow = wxDateTime::GetCurrentYear();
    wxString yearofbirth = wxString::Format("%i", yearnow - age); 
    
    // open a message box.
    wxString info = "Hello " + nameValue + ", welcome!" + "\n" + "\n"
                    "So you are born in " + yearofbirth + "...";
	wxMessageBox(info, "Welcome!");
}
