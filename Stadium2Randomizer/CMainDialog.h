#pragma once

#include <array>
#include <functional>
#include <vector>
#include <fstream>
#include <stdint.h>

#include "Constants.h"
#include "CustomRosterInfo.h"
#include "DiscreteDistribution.h"

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

	template<typename... Args>
	void DistSelect(DiscreteDistribution& dist, Args... p);
public:

	afx_msg void OnClickedBtnBrowse();
	afx_msg void OnClickedBtnGenerate();

	afx_msg void OnChangeEdSeed();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdPath();

	afx_msg void OnBnClickedCbmorerentaltables();
	afx_msg void OnBnClickedCbrandchooseitems();
	afx_msg void OnBnClickedCbshuffleboss();
	afx_msg void OnBnClickedCbshuffleregular();

	afx_msg void OnBnClickedCbshufflecustoms();
	afx_msg void OnBnClickedCbshufflecustomsintoregs();
	afx_msg void OnBnClickedRandprimelevels();
	afx_msg void OnBnClickedCbglcrentallevels();
	afx_msg void OnBnClickedCbchangeitemn();

public:
	afx_msg void OnBnClickedCbrandomrentals();
	afx_msg void OnBnClickedCbTrainerRandpoke();
	afx_msg void OnBnClickedCbTrainerRandom();
	afx_msg void OnBnClickedCbrandomeviv();
	afx_msg void OnBnClickedCbrandlevel();
	afx_msg void OnBnClickedCbTrainerRanditems();
	afx_msg void OnBnClickedCbTrainerRandeviv();

	afx_msg void OnBnClickedCbrandlevelBtndist();
	afx_msg void OnBnClickedCbrandomevivBtndist();
	afx_msg void OnBnClickedCbTrainerRandevivBtndist();
	afx_msg void OnBnClickedBtnrpbstdist();
	afx_msg void OnBnClickedBtnrmbpdist();
	afx_msg void OnBnClickedBtnrmppdist();
	afx_msg void OnBnClickedBtnrmaccdist();
protected:
	BOOL uberMoves;
	BOOL uberMovesBalance;
	BOOL uberMovesAcc;
	DiscreteDistribution uberMovesAccDist;
protected:
	BOOL uberMovesBp;
	DiscreteDistribution uberMovesBpDist;
	BOOL uberMovesCloseBp;
	BOOL uberMovesPp;
	DiscreteDistribution uberMovesPpDist;
	BOOL uberMovesStatusMoves;
	BOOL uberMovesType;
	BOOL uberMovesSecEffectChance;
	DiscreteDistribution uberMovesSecEffectChanceDist;
	BOOL uberMovesSecEffect;
	BOOL uberSpeciesBst;
	DiscreteDistribution uberSpeciesBstDist;
	BOOL uberSpeciesTypes;
	BOOL uberSpeciesEvoBst;
	BOOL uberSpeciesEvoStats;
	int uberSpeciesBstButtons;


	DECLARE_MESSAGE_MAP()
	BOOL battleItems;
	BOOL trainerRandLevels;
	BOOL trainerRandMonNames;
	BOOL trainerRandName;
	BOOL trainerRandSpecies;

	BOOL randLevels;
//	BOOL randLevelsUniformDist;
	DiscreteDistribution randLevelsDist;
	BOOL randEvIv;
//	BOOL randEvIvUniformDist;
	DiscreteDistribution randEvIvDist;

	int min1Buttons;

	CEdit romPath;
	CStatic stStatus;

	BOOL randPrimecupLevels;
	CProgressCtrl progressBar;
	CEdit edSeed;

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

	BOOL randMoves;
	BOOL trainerRandMoves;
	BOOL trainerRandEvIv;
	BOOL trainerRandHappiness;
	BOOL trainerRandItems;
//	BOOL trainerRandIvEvUniform;
	DiscreteDistribution trainerRandIvEvDist;
	BOOL trainerRandPoke;
	BOOL randRentalHappiness;
	BOOL randRentals;
	BOOL trainerRand;
	
public:
	afx_msg void OnBnClickedBtnrmecdist();
protected:
	CString uberMovesSecAddPercent;
	CString uberMovesSecRemPercent;
	CString uberSpeciesAddTypePercent;
	CString uberSpeciesRemTypePercent;
public:
	afx_msg void OnBnClickedCbubertypes();
	afx_msg void OnBnClickedCbuberbst();
	afx_msg void OnBnClickedCbubermoves();
	afx_msg void OnBnClickedCbrmbp();
	afx_msg void OnBnClickedCbrmeffectchance();
protected:
	BOOL patchCic;
};
