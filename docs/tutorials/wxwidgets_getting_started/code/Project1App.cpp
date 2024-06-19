//---------------------------------------------------------------------------
//
// Name:        Project1App.cpp
// Author:      Jos de Jong
// Created:     4-5-2008 18:41:12
// Description: 
//
//---------------------------------------------------------------------------

#include "Project1App.h"
#include "Project1Dlg.h"

IMPLEMENT_APP(Project1DlgApp)

bool Project1DlgApp::OnInit()
{
	Project1Dlg* dialog = new Project1Dlg(NULL);
	SetTopWindow(dialog);
	dialog->Show(true);		
	return true;
}
 
int Project1DlgApp::OnExit()
{
	return 0;
}
