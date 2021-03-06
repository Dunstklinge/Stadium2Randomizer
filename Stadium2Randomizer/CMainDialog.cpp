// CMainDialog.cpp : implementation file
//

#include "CMainDialog.h"
#include "afxdialogex.h"
#include "resource.h"

#include <iostream>

#include "CMain.h"
#include "CDistDialog.h"
#include "DefRoster.h"
#include "DefText.h"
#include "PokemonGenerator.h"
#include "TrainerGenerator.h"
#include "Tables.h"
#include "Filters.h"
#include "GlobalRandom.h"
#include "Crc.h"
#include "Randomizer.h"
#include "GlobalConfigs.h"


// CMainDialog dialog

IMPLEMENT_DYNAMIC(CMainDialog, CDialogEx)


constexpr unsigned int CMainDialog::WM_WORKERENDED;

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, battleItems(FALSE)
	, trainerRandLevels(FALSE)
	, trainerRandMonNames(FALSE)
	, trainerRandName(FALSE)
	, trainerRandSpecies(FALSE)
	, randLevels(FALSE)
	, randEvIv(FALSE)
	, min1Buttons(0)
	, randPrimecupLevels(FALSE)
	, randChooseItems(FALSE)
	, strPrimeCupLevel(_T(""))
	, randIncludeMonspecificItems(FALSE)
	, changeGlcRentalLevel(FALSE)
	, seperateGlcRentalsLevel(_T(""))
	, rentalSpeciesEvIv(FALSE)
	, trainerMin1Atk(FALSE)
	, stayCloseToBST(FALSE)
	, bossStayCloseToBST(FALSE)
	, changeItemN(FALSE)
	, itemsPerRentalSet(FALSE)
	, moreRentalTables(FALSE)
	, multipleGlcRentals(FALSE)
	, multiplePokecupRentals(FALSE)
	, multipleR2Rentals(FALSE)
	, changeItemNText(_T(""))
	, legalMovesOnly(FALSE)
	, shuffleBosses(FALSE)
	, shuffleCross(FALSE)
	, shuffleRegulars(FALSE)
	, mixCustomsInBosses(FALSE)
	, mixCustomsInTrainers(FALSE)
	, strCustomTrainerN(_T(""))
	, randMoves(FALSE)
	, trainerRandMoves(FALSE)
	, trainerRandEvIv(FALSE)
	, trainerRandHappiness(FALSE)
	, trainerRandItems(FALSE)
	, trainerRandPoke(FALSE)
	, randRentalHappiness(FALSE)
	, randRentals(FALSE)
	, trainerRand(FALSE)
	, uberMoves(FALSE)
	, uberMovesBalance(FALSE)
	, uberMovesAcc(FALSE)
	, uberMovesBp(FALSE)
	, uberMovesCloseBp(FALSE)
	, uberMovesPp(FALSE)
	, uberMovesStatusMoves(FALSE)
	, uberMovesType(FALSE)
	, uberMovesSecEffectChance(FALSE)
	, uberMovesSecEffect(FALSE)
	, uberSpeciesBst(FALSE)
	, uberSpeciesTypes(FALSE)
	, uberSpeciesEvoBst(FALSE)
	, uberSpeciesEvoStats(FALSE)
	, uberSpeciesBstButtons(0)
	, uberMovesSecAddPercent(_T(""))
	, uberMovesSecRemPercent(_T(""))
	, uberSpeciesAddTypePercent(_T(""))
	, uberSpeciesRemTypePercent(_T(""))
	, patchCic(FALSE)
{

	uberMovesAccDist = DiscreteDistribution(155, 255, DiscreteDistribution::triangleParams{34, 56});
	uberMovesBpDist = DiscreteDistribution(50, 120);
	uberMovesPpDist = DiscreteDistribution(5, 40);
	uberMovesSecEffectChanceDist = DiscreteDistribution(5, 30);
	uberSpeciesBstDist = DiscreteDistribution(150, 700);
	randLevelsDist = DiscreteDistribution(50, 55);
	randEvIvDist = DiscreteDistribution(1, 100, DiscreteDistribution::normalParams{ 5, 0 });
	trainerRandIvEvDist = DiscreteDistribution(1, 100, DiscreteDistribution::normalParams{ 5, 0});
}

CMainDialog::~CMainDialog()
{
	delete currRandomizer;
}


BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDLEVELS))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDMONNAMES))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDNAME))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDSPECIES))->SetCheck(1);

	((CButton*)this->GetDlgItem(IDC_CBRANDCHOOSEITEMS))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBRANDMOVES))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBRANDLEVEL))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CBRANDOMEVIV))->SetCheck(1);

	((CButton*)this->GetDlgItem(IDC_CBBATTLEITEMS))->SetCheck(1);

	((CEdit*)this->GetDlgItem(IDC_EDCHANGEITEMN))->SetWindowText(TEXT("8"));
	((CButton*)this->GetDlgItem(IDC_RANDPRIMELEVELS))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CBMULTIPLEGLCRENTALS))->SetCheck(0);
	((CEdit*)this->GetDlgItem(IDC_EDGLCLEVEL))->SetWindowText(TEXT("75"));
	((CButton*)this->GetDlgItem(IDC_CBSPECIESEVIV))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBBOSSSTAYCLOSETOBST))->SetCheck(1);

	((CButton*)this->GetDlgItem(IDC_CBRANDOMRENTALS))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDOM))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDPOKE))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDMOVES))->SetCheck(1);
	
	((CComboBox*)this->GetDlgItem(IDC_CBSPECIAL));
	CString customTrainerN;
	customTrainerN.Format(TEXT("%u"), GlobalConfig::CustomTrainers.customTrainers.size());
	((CEdit*)this->GetDlgItem(IDC_EDCUSTOMTRAINERN))->SetWindowText(customTrainerN);

	((CButton*)this->GetDlgItem(IDC_BTNGENERATE))->EnableWindow(FALSE);

	((CSpinButtonCtrl*)this->GetDlgItem(IDC_SPIN1))->SetRange32(0, 100);
	((CSpinButtonCtrl*)this->GetDlgItem(IDC_SPIN2))->SetRange32(0, 100);
	((CSpinButtonCtrl*)this->GetDlgItem(IDC_SPIN3))->SetRange32(0, 255);
	((CSpinButtonCtrl*)this->GetDlgItem(IDC_SPIN4))->SetRange32(0, 255);


	std::random_device dev;
	TCHAR seedStr[256];
	_ui64tot_s(dev(), seedStr, _countof(seedStr), 10);
	edSeed.SetWindowText(seedStr);
	((CButton*)this->GetDlgItem(IDC_EDPRIMECUPLEVEL))->SetWindowText(TEXT("95"));
	((CButton*)this->GetDlgItem(IDC_EDRPTADDPERCENT))->SetWindowText(TEXT("50"));
	((CButton*)this->GetDlgItem(IDC_EDRPTREMPERCENT))->SetWindowText(TEXT("50"));
	((CButton*)this->GetDlgItem(IDC_EDRMSECADDPERCENT))->SetWindowText(TEXT("50"));
	((CButton*)this->GetDlgItem(IDC_EDRMSECREMPERCENT))->SetWindowText(TEXT("50"));

	this->OnBnClickedCbrandomrentals();
	this->OnBnClickedCbTrainerRandom();
	this->OnBnClickedCbTrainerRandpoke();
	this->OnBnClickedCbubertypes();
	this->OnBnClickedCbuberbst();
	this->OnBnClickedCbubermoves();

	AddTooltip(IDC_STATIC, TEXT("Changes from this part apply to both rentals and trainer pokemon."));
	
	AddTooltip(IDC_RD1NOTHING, TEXT("Does not put a restriction on the first move."));
	AddTooltip(IDC_RD1ATK, TEXT("At least one move of each pokemon has a base power >= 1."));
	AddTooltip(IDC_RD1STAB, TEXT("At least one move of each pokemon has a base power >= 1 and is stab."));
	AddTooltip(IDC_RD1STRONGATK, TEXT("At least one move of each pokemon has a base power >= 75."));
	AddTooltip(IDC_RD1STRONGSTAB, TEXT("At least one move of each pokemon has a base power >= 75 and is stab."));
	AddTooltip(IDC_RDYOLONOME, TEXT("All Moves of all Pokemon are Metronome."));

	AddTooltip(IDC_CBLEGALMOVESONLY, TEXT("All Moves of all Pokemon must be legal, except if Metronome was selected. ")
		TEXT("Above restrictions still apply as long as at least one such move exists."));
	AddTooltip(IDC_RANDPRIMELEVELS, TEXT("Levels of both rentals and enemy pokemon in prime cup will be between this level and 100. ")
		TEXT(" (Trainers only if \"Random Levels\" is checked in the Trainer category."));

	AddTooltip(IDC_CB_TRAINER_RANDOM, TEXT("Enables or Disables the Randomization of Trainer Properties (expect their Pokemon)"));
	AddTooltip(IDC_CB_TRAINER_RANDNAME, TEXT("Trainers will be given random names taken from the appropriate config file."));
	AddTooltip(IDC_CBSHUFFLEBOSS, TEXT("Gym Leaders, Elite 4 and Rivals will be shuffled around so that e.g a new set of 4 ")
		TEXT("bosses will make up the elite 4. Both Round 1 and Round 2 will be shuffled independently. ")
		TEXT("This only shuffles their text and faces, but not their pokemon."));
	AddTooltip(IDC_CBSHUFFLEREGULAR, TEXT("Non-Bosses will be shuffled around like the bosses. ")
		TEXT("This only shuffles their text and faces, but not their pokemon."));
	AddTooltip(IDC_CBSHUFFLECROSS, TEXT("If active, random trainers can take the place of bosses and vise-versa."));
	AddTooltip(IDC_CBSHUFFLECUSTOMS, TEXT("Custom Trainers from config/trainers may replace random bosses. This ")
		TEXT("includes all of their appearances in both rounds. Only affects text and faces, but not their pokemon. "));
	AddTooltip(IDC_CBSHUFFLECUSTOMSINTOREGS, TEXT("Custom Trainers from config/trainers may replace random regular non-boss trainers. This ")
		TEXT("includes all of their appearances in both rounds. Only affects text and faces, but not their pokemon. "));
	
	AddTooltip(IDC_CBRANDOMRENTALS, TEXT("Enables or Disables Rental Modification."));
	AddTooltip(IDC_CBRANDMOVES, TEXT("Rentals have random moves."));
	AddTooltip(IDC_CBRANDLEVEL, TEXT("Rentals for the Pokecup have a random level between 50 and 55 instead of always being 50."));
	AddTooltip(IDC_CBRANDHAPPINESS, TEXT("Rentals have random happiness."));
	AddTooltip(IDC_CBRANDOMEVIV, TEXT("Rental Pokemon have randomized gene strength."));
	AddTooltip(IDC_CBSPECIESEVIV, TEXT("Random Ivs/Evs are limited to a certain Range which depends on ")
		TEXT("the pokemons BST: weak pokemon will have high Ivs/Evs, strong Pokemon lower ones."));

	AddTooltip(IDC_CBMORERENTALTABLES, TEXT("Uses assembly hacks to add additional rental sets that the original game does not have. ")
		TEXT("\r\nNOTE: Rental registration does not follow these bounds and may be used to cheese."));
	AddTooltip(IDC_CBPOKECUPRENTALS, TEXT("The 4 Poke-Cups (Pokeball to Masterball) have their own rental sets. "));
	AddTooltip(IDC_CBR2RENTALS, TEXT("All rental sets are different in round 2. This includes rental sets added ")
		TEXT("by this hack such as seperate PokeCup or GLC rentals."));
	AddTooltip(IDC_CBMULTIPLEGLCRENTALS, TEXT("Adds a new set of rentals for the gym leader castle. Originally, the GLC ")
		TEXT("used PokeCup rentals, which may be a bad thing combined with PokeCup rentals random levels. ")
		TEXT("This option adds seperate Sets for Falkner-Chuck, Jasmine-Lance and Kanto"));
	AddTooltip(IDC_CBGLCRENTALLEVELS, TEXT("Sets level of GLC rentals as well as the Rivals Pokemon. ")
		TEXT("\r\nNOTE: these include ubers. This can not be turned off; if you dont want to use ubers, please ignore them."));
	
	AddTooltip(IDC_CBRANDCHOOSEITEMS, TEXT("Uses assembly hacks to change the items choosable in the rental screen and/or ")
		TEXT("add additional sets of items for the rental selection."));
	AddTooltip(IDC_CBMONSPECIFICITEMS, TEXT("If unchecked, only general battle-items such as berries and ")
		TEXT("quick claw are used. If checked, type-boosting items such as magnet and pokemon-specific items ")
		TEXT("such as thick club are used as well."));
	AddTooltip(IDC_CBCHANGEITEMN, TEXT("Changes the amount of items avialable per individual item set. The original game has 6."));
	AddTooltip(IDC_CBITEMSPERRENTAL, TEXT("Every rental set, including ones generated by this hack with the options above, have ")
		TEXT("their own item set."));
	
	AddTooltip(IDC_CB_TRAINER_RANDPOKE, TEXT("Trainers Teams will be modified."));
	AddTooltip(IDC_CB_TRAINER_RANDSPECIES, TEXT("Trainers have random Pokemon species (within the bounds of the options below)."));
	AddTooltip(IDC_CB_TRAINER_RANDLEVELS, TEXT("Trainers in PokeCup have random levels bewteen 50 and 55 for their pokemon. ")
		TEXT("Trainers in PrimeCup have random levels if this was selected in the All-Pokemon category."));
	AddTooltip(IDC_CB_TRAINER_RANDMOVES, TEXT("Trainers Pokemon have random moves."));
	AddTooltip(IDC_CB_TRAINER_RANDHAPPINESS, TEXT("Trainers Pokemon have random happiness."));
	AddTooltip(IDC_CB_TRAINER_RANDITEMS, TEXT("Trainers Pokemon have random items."));
	AddTooltip(IDC_CBBATTLEITEMS, TEXT("If checked, pokemon may have any item, including useless ones such as ")
		TEXT("TMs or Repels. If unchecked, only useful items are generated."));
	AddTooltip(IDC_CB_TRAINER_RANDEVIV, TEXT("Trainers Pokemon have Evs and Ivs."));
	AddTooltip(IDC_CB_TRAINER_RANDMONNAMES, TEXT("Trainers have random nicknames for their Pokemon taken from the appropriate config file."));
	AddTooltip(IDC_CB_TRAINERPOKE_MIN1ATK, TEXT("Every trainer pokemon has at least one move with base power > 0. ")
		TEXT("\r\nNOTE: consider that you can pick the best 6 from 250 random pokemon, but trainers can only pick 6 from 6."));
	AddTooltip(IDC_CBSTAYCLOSETOBST, TEXT("Prevents regular trainers from having Mewtwos."));
	AddTooltip(IDC_CBBOSSSTAYCLOSETOBST, TEXT("Prevents Rival from having a weedle and a magikarp"));

	AddTooltip(IDC_CBUBERTYPES, TEXT("Changes Types of pokemon Species to random types, excluding the ??? type"));
	AddTooltip(IDC_CBUBERBST, TEXT("Randomize species stats"));
	AddTooltip(IDC_CBRPBSTEVOBST, TEXT("Prevents e.g Kadabra from having lower BST than Abra"));
	AddTooltip(IDC_CBRPBSTEVOSTATS, TEXT("Prevents e.g Blastoise from having lower defense than Squirtle.")
		TEXT("\r\nNOTE: this also applies to pokemon like Scizor and Beedrill which usually do not follow these rules."));
	AddTooltip(IDC_RDRPBSTKEEPBST, TEXT("Species keep their BST, but their stat distribution gets changed."));
	AddTooltip(IDC_RDRPBSTCLOSEBST, TEXT("BST will only get changed += 10%"));
	AddTooltip(IDC_RDRPBSTRANDBST, TEXT("BST is completely random"));
	
	AddTooltip(IDC_CBUBERMOVES, TEXT("Randomize move effects. Could e.g turn tackle into a multi-hit poison type move."));
	AddTooltip(IDC_CBRMBALANCE, TEXT("Tries to keep moves equally strong according to a point system."));
	AddTooltip(IDC_CBRMTYPE, TEXT("Random move type, both for attack and status moves."));
	AddTooltip(IDC_CBRMBP, TEXT("Random BP for moves that have a BP"));
	AddTooltip(IDC_CBRMCLOSEBP, TEXT("BP gets only changed += 10% from the original BP."));
	AddTooltip(IDC_CBRMSEC, TEXT("Attacking moves (i.e moves that have a BP) get a random secondary effects. The following restrictions apply:\r\n")
		TEXT("If no random bp was selected, damage moves that ignore BP (like OHKO moves or Counter) are not randomised.\r\n")
		TEXT("If no random effect chance was selected, moves with chance-effects (e.g 10% burn) are only given other chance effects ")
		TEXT("and vice versa"));
	AddTooltip(IDC_CBRMEFFECTCHANCE, TEXT("Random chance for secondary chance-effects."));
	AddTooltip(IDC_CBRMRANDSTATUS, TEXT("Status moves (i.e moves with no BP) get a random effect."));
	AddTooltip(IDC_CBRMPP, TEXT("Random power point maximum."));
	AddTooltip(IDC_CBRMACC, TEXT("Random Accuracy for all moves (except self-targeted status moves)."));
		
	AddTooltip(IDC_PATCHCIC, TEXT("When ticked, the ROMS CRC are not recalculated, but the bootcode is modified to skip the check instead."));
	

	progressBar.SetRange(0, 1000);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDialog::AddTooltip(int dlgId, const TCHAR* tooltip)
{
	HWND wnd = this->GetDlgItem(dlgId)->m_hWnd;

	HWND wndTooltip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		this->m_hWnd, NULL, program.m_hInstance, NULL);

	::SendMessage(wndTooltip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 0x7FFF);//_tcslen(tooltip) * 100);
	::SendMessage(wndTooltip, TTM_SETMAXTIPWIDTH, 0, 500);

	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = this->m_hWnd;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)wnd;
	toolInfo.lpszText = (TCHAR*)tooltip;
	::SendMessage(wndTooltip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CBBATTLEITEMS, battleItems);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDLEVELS, trainerRandLevels);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDMONNAMES, trainerRandMonNames);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDNAME, trainerRandName);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDSPECIES, trainerRandSpecies);
	DDX_Check(pDX, IDC_CBRANDLEVEL, randLevels);
	DDX_Check(pDX, IDC_CBRANDOMEVIV, randEvIv);
	DDX_Control(pDX, IDC_EDPATH, romPath);
	DDX_Control(pDX, IDC_STSTATUS, stStatus);
	DDX_Radio(pDX, IDC_RD1NOTHING, min1Buttons);
	DDX_Check(pDX, IDC_RANDPRIMELEVELS, randPrimecupLevels);
	DDX_Control(pDX, IDC_PROGRESS, progressBar);
	DDX_Control(pDX, IDC_EDSEED, edSeed);
	DDX_Check(pDX, IDC_CBRANDCHOOSEITEMS, randChooseItems);
	DDX_Text(pDX, IDC_EDPRIMECUPLEVEL, strPrimeCupLevel);
	DDX_Check(pDX, IDC_CBMONSPECIFICITEMS, randIncludeMonspecificItems);
	DDX_Check(pDX, IDC_CBGLCRENTALLEVELS, changeGlcRentalLevel);
	DDX_Text(pDX, IDC_EDGLCLEVEL, seperateGlcRentalsLevel);
	DDX_Check(pDX, IDC_CBSPECIESEVIV, rentalSpeciesEvIv);
	DDX_Check(pDX, IDC_CB_TRAINERPOKE_MIN1ATK, trainerMin1Atk);
	DDX_Check(pDX, IDC_CBSTAYCLOSETOBST, stayCloseToBST);
	DDX_Check(pDX, IDC_CBBOSSSTAYCLOSETOBST, bossStayCloseToBST);
	DDX_Check(pDX, IDC_CBCHANGEITEMN, changeItemN);
	DDX_Check(pDX, IDC_CBITEMSPERRENTAL, itemsPerRentalSet);
	DDX_Check(pDX, IDC_CBMORERENTALTABLES, moreRentalTables);
	DDX_Check(pDX, IDC_CBMULTIPLEGLCRENTALS, multipleGlcRentals);
	DDX_Check(pDX, IDC_CBPOKECUPRENTALS, multiplePokecupRentals);
	DDX_Check(pDX, IDC_CBR2RENTALS, multipleR2Rentals);
	DDX_Text(pDX, IDC_EDCHANGEITEMN, changeItemNText);

	DDX_Check(pDX, IDC_CBLEGALMOVESONLY, legalMovesOnly);
	DDX_Check(pDX, IDC_CBSHUFFLEBOSS, shuffleBosses);
	DDX_Check(pDX, IDC_CBSHUFFLECROSS, shuffleCross);
	DDX_Check(pDX, IDC_CBSHUFFLEREGULAR, shuffleRegulars);
	DDX_Check(pDX, IDC_CBSHUFFLECUSTOMS, mixCustomsInBosses);
	DDX_Check(pDX, IDC_CBSHUFFLECUSTOMSINTOREGS, mixCustomsInTrainers);
	DDX_Text(pDX, IDC_EDCUSTOMTRAINERN, strCustomTrainerN);
	DDX_Check(pDX, IDC_CBRANDMOVES, randMoves);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDMOVES, trainerRandMoves);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDEVIV, trainerRandEvIv);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDHAPPINESS, trainerRandHappiness);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDITEMS, trainerRandItems);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDPOKE, trainerRandPoke);
	DDX_Check(pDX, IDC_CBRANDHAPPINESS, randRentalHappiness);
	//  DDX_Check(pDX, IDC_CBRANDRENTALITEMS, randRentalItems);
	DDX_Check(pDX, IDC_CBRANDOMRENTALS, randRentals);
	DDX_Check(pDX, IDC_CB_TRAINER_RANDOM, trainerRand);
	//  DDX_Check(pDX, IDC_CBUBERBST, speciesBst);
	//  DDX_Check(pDX, IDC_CBUBERTYPES, speciesTypes);
	DDX_Check(pDX, IDC_CBUBERMOVES, uberMoves);
	//  DDX_Control(pDX, IDC_CBRMACC, uberMovesAcc);
	DDX_Check(pDX, IDC_CBRMBALANCE, uberMovesBalance);
	DDX_Check(pDX, IDC_CBRMACC, uberMovesAcc);
	DDX_Check(pDX, IDC_CBRMBP, uberMovesBp);
	DDX_Check(pDX, IDC_CBRMCLOSEBP, uberMovesCloseBp);
	DDX_Check(pDX, IDC_CBRMPP, uberMovesPp);
	//  DDX_Check(pDX, IDC_CBRMRANDEFFECT, uberMovesSecEffect);
	DDX_Check(pDX, IDC_CBRMRANDSTATUS, uberMovesStatusMoves);
	DDX_Check(pDX, IDC_CBRMTYPE, uberMovesType);
	DDX_Check(pDX, IDC_CBRMEFFECTCHANCE, uberMovesSecEffectChance);
	DDX_Check(pDX, IDC_CBRMSEC, uberMovesSecEffect);
	DDX_Check(pDX, IDC_CBUBERBST, uberSpeciesBst);
	DDX_Check(pDX, IDC_CBUBERTYPES, uberSpeciesTypes);
	DDX_Check(pDX, IDC_CBRPBSTEVOBST, uberSpeciesEvoBst);
	DDX_Check(pDX, IDC_CBRPBSTEVOSTATS, uberSpeciesEvoStats);
	DDX_Radio(pDX, IDC_RDRPBSTKEEPBST, uberSpeciesBstButtons);
	DDX_Text(pDX, IDC_EDRMSECADDPERCENT, uberMovesSecAddPercent);
	DDX_Text(pDX, IDC_EDRMSECREMPERCENT, uberMovesSecRemPercent);
	DDX_Text(pDX, IDC_EDRPTADDPERCENT, uberSpeciesAddTypePercent);
	DDX_Text(pDX, IDC_EDRPTREMPERCENT, uberSpeciesRemTypePercent);
	DDX_Check(pDX, IDC_PATCHCIC, patchCic);
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTNBROWSE, &CMainDialog::OnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTNGENERATE, &CMainDialog::OnClickedBtnGenerate)
	ON_EN_CHANGE(IDC_EDSEED, &CMainDialog::OnChangeEdSeed)
	ON_EN_CHANGE(IDC_EDPATH, &CMainDialog::OnChangeEdPath)
	ON_BN_CLICKED(IDC_CBMORERENTALTABLES, &CMainDialog::OnBnClickedCbmorerentaltables)
	ON_BN_CLICKED(IDC_CBRANDCHOOSEITEMS, &CMainDialog::OnBnClickedCbrandchooseitems)
	ON_MESSAGE(CMainDialog::WM_WORKERENDED, &CMainDialog::OnRandomizeEnd)
	ON_BN_CLICKED(IDC_CBSHUFFLEBOSS, &CMainDialog::OnBnClickedCbshuffleboss)
	ON_BN_CLICKED(IDC_CBSHUFFLEREGULAR, &CMainDialog::OnBnClickedCbshuffleregular)
	ON_BN_CLICKED(IDC_CBSHUFFLECUSTOMS, &CMainDialog::OnBnClickedCbshufflecustoms)
	ON_BN_CLICKED(IDC_CBSHUFFLECUSTOMSINTOREGS, &CMainDialog::OnBnClickedCbshufflecustomsintoregs)
	ON_BN_CLICKED(IDC_RANDPRIMELEVELS, &CMainDialog::OnBnClickedRandprimelevels)
	ON_BN_CLICKED(IDC_CBGLCRENTALLEVELS, &CMainDialog::OnBnClickedCbglcrentallevels)
	ON_BN_CLICKED(IDC_CBCHANGEITEMN, &CMainDialog::OnBnClickedCbchangeitemn)
	ON_BN_CLICKED(IDC_CBRANDOMRENTALS, &CMainDialog::OnBnClickedCbrandomrentals)
	ON_BN_CLICKED(IDC_CB_TRAINER_RANDPOKE, &CMainDialog::OnBnClickedCbTrainerRandpoke)
	ON_BN_CLICKED(IDC_CB_TRAINER_RANDOM, &CMainDialog::OnBnClickedCbTrainerRandom)
	ON_BN_CLICKED(IDC_CBRANDOMEVIV, &CMainDialog::OnBnClickedCbrandomeviv)
	ON_BN_CLICKED(IDC_CBRANDLEVEL, &CMainDialog::OnBnClickedCbrandlevel)
	ON_BN_CLICKED(IDC_CB_TRAINER_RANDITEMS, &CMainDialog::OnBnClickedCbTrainerRanditems)
	ON_BN_CLICKED(IDC_CB_TRAINER_RANDEVIV, &CMainDialog::OnBnClickedCbTrainerRandeviv)
ON_BN_CLICKED(IDC_CBRANDLEVEL_BTNDIST, &CMainDialog::OnBnClickedCbrandlevelBtndist)
ON_BN_CLICKED(IDC_CBRANDOMEVIV_BTNDIST, &CMainDialog::OnBnClickedCbrandomevivBtndist)
ON_BN_CLICKED(IDC_CB_TRAINER_RANDEVIV_BTNDIST, &CMainDialog::OnBnClickedCbTrainerRandevivBtndist)
ON_BN_CLICKED(IDC_BTNRPBSTDIST, &CMainDialog::OnBnClickedBtnrpbstdist)
ON_BN_CLICKED(IDC_BTNRMBPDIST, &CMainDialog::OnBnClickedBtnrmbpdist)
ON_BN_CLICKED(IDC_BTNRMPPDIST, &CMainDialog::OnBnClickedBtnrmppdist)
ON_BN_CLICKED(IDC_BTNRMACCDIST, &CMainDialog::OnBnClickedBtnrmaccdist)
ON_BN_CLICKED(IDC_BTNRMECDIST, &CMainDialog::OnBnClickedBtnrmecdist)
ON_BN_CLICKED(IDC_CBUBERTYPES, &CMainDialog::OnBnClickedCbubertypes)
ON_BN_CLICKED(IDC_CBUBERBST, &CMainDialog::OnBnClickedCbuberbst)
ON_BN_CLICKED(IDC_CBUBERMOVES, &CMainDialog::OnBnClickedCbubermoves)
ON_BN_CLICKED(IDC_CBRMBP, &CMainDialog::OnBnClickedCbrmbp)
ON_BN_CLICKED(IDC_CBRMEFFECTCHANCE, &CMainDialog::OnBnClickedCbrmeffectchance)
END_MESSAGE_MAP()


// CMainDialog message handlers
#include "DefFaces.h"

void CMainDialog::OnClickedBtnBrowse()
{
	const TCHAR filters[] = _T("n64 rom files|*.n64;*.z64;*.v64||");
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY, filters, this);
	if (dlg.DoModal() == IDOK) {
		romPath.SetWindowText(dlg.GetPathName());
	}

}
void CMainDialog::OnClickedBtnGenerate()
{
	UpdateData(true);

	DisableAll();

	delete currRandomizer;
	currRandomizer = new Randomizer(this);
	currRandomizerThread = AfxBeginThread(RandomizeThreadProc, this);
}

UINT CMainDialog::RandomizeThreadProc(LPVOID param) {
	CMainDialog* settings = (CMainDialog*)param;
	CString path;
	settings->romPath.GetWindowText(path);
	settings->currRandomizer->RandomizeRom(path);
	settings->SendMessage(WM_WORKERENDED);
	return 0;
}

LRESULT CMainDialog::OnRandomizeEnd(WPARAM wparam, LPARAM lparam)
{
	RestoreAll();
	return 0;
}

void CMainDialog::DisableAll()
{
	controlEnableState.clear();
	::EnumChildWindows(this->m_hWnd, DisableAllChildProc, (LPARAM)this);
}

BOOL CALLBACK CMainDialog::DisableAllChildProc(HWND hwnd, LPARAM we) {
	CMainDialog* ptr = (CMainDialog*)we;
	TCHAR name[128];
	*name = 0;
	::GetClassName(hwnd, name, 128);
	if (_tcsicmp(name, "BUTTON") == 0 || _tcsicmp(name, "EDIT")) {
		ptr->controlEnableState.emplace_back(hwnd, ::IsWindowEnabled(hwnd));
		::EnableWindow(hwnd, FALSE);
	}
	return TRUE;
}

void CMainDialog::RestoreAll() 
{
	for (auto& pair : controlEnableState) {
		::EnableWindow(pair.first, pair.second);
	}
}


void CMainDialog::OnChangeEdSeed()
{
	CString strSeed;
	edSeed.GetWindowText(strSeed);
	unsigned long seed = _tcstoul(strSeed, NULL, 10);
	Random::Generator.seed(seed);
}




void CMainDialog::OnChangeEdPath()
{
	bool valid = false;
	CString path;
	romPath.GetWindowText(path);

	DWORD attr = GetFileAttributes(path);

	char romRegion = '?';
	if (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
		std::ifstream in(path, std::ios::binary);
		uint8_t buffer[0x40];
		in.read((char*)buffer, 0x40);
		in.seekg(0, in.end);
		unsigned int size = (unsigned int)in.tellg();
		if (size == 0x4000000) {
			uint8_t firstByte = buffer[0];
			bool m_normal = firstByte == 0x80, m_byteswapped = firstByte == 0x37, m_wordswapped = firstByte == 0x40;
			if (m_normal || m_byteswapped || m_wordswapped) {
				auto DoSwaps = [&](void* buffer, size_t size) {
					uint8_t* bBuffer = (uint8_t*)buffer;
					if (m_normal) return;
					else if (m_byteswapped) for (size_t i = 0; i < size; i += 2) SwitchEndianness(*((uint16_t*)(bBuffer + i)));
					else if (m_wordswapped) for (size_t i = 0; i < size; i += 4) SwitchEndianness(*((uint32_t*)(bBuffer + i)));
				};
				DoSwaps(buffer, 0x40);
				char* gameName = (char*)buffer + 0x20;
				romRegion = *((char*)buffer + 0x3E);
				if (strcmp(gameName, "POKEMON STADIUM 2   ") == 0 && (romRegion == 'E' /*|| romRegion == 'P'*/)) {
					valid = true;
				}
			}
		}
	}

	if (valid) {
		static TCHAR valid[] = "valid (X)";
		valid[7] = romRegion;
		stStatus.SetWindowText(valid);
		((CButton*)this->GetDlgItem(IDC_BTNGENERATE))->EnableWindow(TRUE);
	}
	else {
		static const TCHAR* invalid = "invalid!";
		stStatus.SetWindowText(invalid);
		((CButton*)this->GetDlgItem(IDC_BTNGENERATE))->EnableWindow(FALSE);
	}
}



void CMainDialog::OnBnClickedCbmorerentaltables()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBMORERENTALTABLES))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_CBPOKECUPRENTALS))->EnableWindow(checked);
	((CButton*)this->GetDlgItem(IDC_CBR2RENTALS))->EnableWindow(checked);
	((CButton*)this->GetDlgItem(IDC_CBMULTIPLEGLCRENTALS))->EnableWindow(checked);
	((CButton*)this->GetDlgItem(IDC_CBGLCRENTALLEVELS))->EnableWindow(checked);
	((CEdit*)this->GetDlgItem(IDC_EDGLCLEVEL))->EnableWindow(checked);

	if (checked) OnBnClickedCbglcrentallevels();
}


void CMainDialog::OnBnClickedCbrandchooseitems()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBRANDCHOOSEITEMS))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_CBMONSPECIFICITEMS))->EnableWindow(checked);
	((CButton*)this->GetDlgItem(IDC_CBCHANGEITEMN))->EnableWindow(checked);
	((CButton*)this->GetDlgItem(IDC_CBITEMSPERRENTAL))->EnableWindow(checked);
	((CEdit*)this->GetDlgItem(IDC_EDCHANGEITEMN))->EnableWindow(checked);

	if(checked) OnBnClickedCbchangeitemn();
}


void CMainDialog::OnBnClickedCbshuffleboss()
{
	BOOL bossChecked = ((CButton*)this->GetDlgItem(IDC_CBSHUFFLEBOSS))->GetCheck() == BST_CHECKED;
	BOOL checked = bossChecked && 
					((CButton*)this->GetDlgItem(IDC_CBSHUFFLEREGULAR))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_CBSHUFFLECROSS))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbshuffleregular()
{
	OnBnClickedCbshuffleboss();
}


void CMainDialog::OnBnClickedCbshufflecustoms()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBSHUFFLECUSTOMSINTOREGS))->GetCheck() == BST_CHECKED ||
					((CButton*)this->GetDlgItem(IDC_CBSHUFFLECUSTOMS))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_EDCUSTOMTRAINERN))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbshufflecustomsintoregs()
{
	OnBnClickedCbshufflecustoms();
}


void CMainDialog::OnBnClickedRandprimelevels()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_RANDPRIMELEVELS))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_EDPRIMECUPLEVEL))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbglcrentallevels()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBGLCRENTALLEVELS))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_EDGLCLEVEL))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbchangeitemn()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBCHANGEITEMN))->GetCheck() == BST_CHECKED;

	((CButton*)this->GetDlgItem(IDC_EDCHANGEITEMN))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbrandomrentals()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBRANDOMRENTALS))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CBRANDHAPPINESS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRANDCHOOSEITEMS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRANDLEVEL))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRANDOMEVIV))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRANDMOVES))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSPECIESEVIV))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBMORERENTALTABLES))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBPOKECUPRENTALS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBR2RENTALS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBMULTIPLEGLCRENTALS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBGLCRENTALLEVELS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDGLCLEVEL))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRANDCHOOSEITEMS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBMONSPECIFICITEMS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBCHANGEITEMN))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDCHANGEITEMN))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBITEMSPERRENTAL))->EnableWindow(checked);

	if (checked) {
		OnBnClickedCbmorerentaltables();
		OnBnClickedCbrandchooseitems();
		OnBnClickedCbglcrentallevels();
		OnBnClickedCbrandomeviv();
		OnBnClickedCbchangeitemn();
		OnBnClickedCbrandomeviv();
	}
}


void CMainDialog::OnBnClickedCbTrainerRandpoke()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDPOKE))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDSPECIES))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDMOVES))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDLEVELS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDITEMS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBBATTLEITEMS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDEVIV))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDHAPPINESS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDMONNAMES))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CB_TRAINERPOKE_MIN1ATK))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSTAYCLOSETOBST))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBBOSSSTAYCLOSETOBST))->EnableWindow(checked);

	if (checked) {
		OnBnClickedCbTrainerRanditems();
		OnBnClickedCbTrainerRandeviv();
	}
}


void CMainDialog::OnBnClickedCbTrainerRandom()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDOM))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CB_TRAINER_RANDNAME))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSHUFFLEBOSS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSHUFFLEREGULAR))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSHUFFLECROSS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSHUFFLECUSTOMS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBSHUFFLECUSTOMSINTOREGS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDCUSTOMTRAINERN))->EnableWindow(checked);

	if (checked) {
		OnBnClickedCbshuffleboss();
		OnBnClickedCbshufflecustoms();
	}
}


void CMainDialog::OnBnClickedCbrandomeviv()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBRANDOMEVIV))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CBSPECIESEVIV))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbrandlevel()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBRANDLEVEL))->GetCheck() == BST_CHECKED;

}


void CMainDialog::OnBnClickedCbTrainerRanditems()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDITEMS))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CBBATTLEITEMS))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbTrainerRandeviv()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDEVIV))->GetCheck() == BST_CHECKED;

}


void CMainDialog::OnBnClickedCbubertypes()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBUBERTYPES))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_EDRPTADDPERCENT))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDRPTREMPERCENT))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_SPIN1))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_SPIN2))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbuberbst()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBUBERBST))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CBRPBSTEVOBST))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRPBSTEVOSTATS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_RDRPBSTKEEPBST))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_RDRPBSTCLOSEBST))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_RDRPBSTRANDBST))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbubermoves()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBUBERMOVES))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CBRMBALANCE))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMTYPE))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMBP))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMCLOSEBP))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMSEC))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMEFFECTCHANCE))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDRMSECADDPERCENT))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDRMSECREMPERCENT))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_SPIN3))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_SPIN4))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMRANDSTATUS))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMPP))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_CBRMACC))->EnableWindow(checked);

	OnBnClickedCbrmbp();
	OnBnClickedCbrmeffectchance();
}


void CMainDialog::OnBnClickedCbrmbp()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBRMBP))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_CBRMCLOSEBP))->EnableWindow(checked);
}


void CMainDialog::OnBnClickedCbrmeffectchance()
{
	BOOL checked = ((CButton*)this->GetDlgItem(IDC_CBRMEFFECTCHANCE))->GetCheck() == BST_CHECKED;

	((CWnd*)this->GetDlgItem(IDC_EDRMSECADDPERCENT))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_EDRMSECREMPERCENT))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_SPIN3))->EnableWindow(checked);
	((CWnd*)this->GetDlgItem(IDC_SPIN4))->EnableWindow(checked);
}

template<typename... Args>
void CMainDialog::DistSelect(DiscreteDistribution& dist, Args... p) {
	CDistDialog d(this, dist, p...);
	if (d.DoModal() == IDOK) {
		dist = d.dist;
	}
}

void CMainDialog::OnBnClickedCbrandlevelBtndist()
{
	DistSelect(randLevelsDist, 50, 55);
}


void CMainDialog::OnBnClickedCbrandomevivBtndist()
{
	DistSelect(randEvIvDist);
}


void CMainDialog::OnBnClickedCbTrainerRandevivBtndist()
{
	DistSelect(trainerRandIvEvDist);
}


void CMainDialog::OnBnClickedBtnrpbstdist()
{
	DistSelect(uberSpeciesBstDist, 50, 1000);
}


void CMainDialog::OnBnClickedBtnrmbpdist()
{
	DistSelect(uberMovesBpDist, 10, 200);
}


void CMainDialog::OnBnClickedBtnrmppdist()
{
	DistSelect(uberMovesPpDist, 1, 63);
}


void CMainDialog::OnBnClickedBtnrmaccdist()
{
	DistSelect(uberMovesAccDist, 1, 255);
}


void CMainDialog::OnBnClickedBtnrmecdist()
{
	DistSelect(uberMovesSecEffectChanceDist, 1, 255);
}


