#pragma once

#include <array>
#include <functional>
#include <vector>
#include <fstream>
#include <stdint.h>

#include "Constants.h"
#include "CustomRosterInfo.h"

// CMainDialog dialog

class CMainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	BOOL battleItems;
	BOOL trainerRandLevels;
	BOOL trainerRandMonNames;
	BOOL trainerRandName;
	BOOL trainerRandSpecies;

	BOOL randLevels;
	BOOL randLevelsUniformDist;
	BOOL randEvIv;
	BOOL randEvIvUniformDist;

	int min1Buttons;

	CEdit romPath;
	CStatic stStatus;

private:
	void AddTooltip(int dlgId, const TCHAR* tooltip);
	
protected:
	BOOL randPrimecupLevels;
	CProgressCtrl progressBar;
	CEdit edSeed;

	afx_msg void OnClickedBtnBrowse();
	afx_msg void OnClickedBtnGenerate();

	afx_msg void OnChangeEdSeed();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdPath();

	BOOL randChooseItems;
	CString strPrimeCupLevel;
	BOOL randIncludeMonspecificItems;
	BOOL changeGlcRentalLevel;
	CString seperateGlcRentalsLevel;
	BOOL rentalSpeciesEvIv;
	BOOL trainerMin1Atk;
	BOOL stayCloseToBST;
	BOOL bossStayCloseToBST;
	BOOL changeItemN;
	BOOL itemsPerRentalSet;
	BOOL moreRentalTables;
	BOOL multipleGlcRentals;
	BOOL multiplePokecupRentals;
	BOOL multipleR2Rentals;
	CString changeItemNText;


private:

	friend class Randomizer;
	
};
