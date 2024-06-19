//---------------------------------------------------------------------------
//
// Name:        Project1App.h
// Author:      Jos de Jong
// Created:     4-5-2008 18:41:12
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __PROJECT1DLGApp_h__
#define __PROJECT1DLGApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class Project1DlgApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
