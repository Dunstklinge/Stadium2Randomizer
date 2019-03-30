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

	static constexpr unsigned int WM_WORKERENDED = WM_USER + 1;

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
	BOOL legalMovesOnly;

	BOOL shuffleBosses;
	BOOL shuffleCross;
	BOOL shuffleRegulars;
	BOOL mixCustomsInBosses;
	BOOL mixCustomsInTrainers;
	CString strCustomTrainerN;

private:
	void AddTooltip(int dlgId, const TCHAR* tooltip);


	friend class Randomizer;
	Randomizer* currRandomizer;
	CWinThread* currRandomizerThread;
	static UINT RandomizeThreadProc(LPVOID param);
	LRESULT OnRandomizeEnd(WPARAM wparam, LPARAM lparam);
	
	std::vector<std::pair<HWND,bool>> controlEnableState;
	void DisableAll();
	void RestoreAll();
	static BOOL CALLBACK DisableAllChildProc(HWND hwnd, LPARAM we);

public:
	afx_msg void OnBnClickedCbmorerentaltables();
	afx_msg void OnBnClickedCbrandchooseitems();

	
	afx_msg void OnBnClickedCbshuffleboss();
	afx_msg void OnBnClickedCbshuffleregular();

	afx_msg void OnBnClickedCbshufflecustoms();
	afx_msg void OnBnClickedCbshufflecustomsintoregs();
	afx_msg void OnBnClickedRandprimelevels();
	afx_msg void OnBnClickedCbglcrentallevels();
	afx_msg void OnBnClickedCbchangeitemn();
};
