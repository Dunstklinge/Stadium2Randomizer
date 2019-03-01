// CMainDialog.cpp : implementation file
//

#include "CMainDialog.h"
#include "afxdialogex.h"
#include "resource.h"

#include <iostream>

#include "CMain.h"
#include "DefRoster.h"
#include "DefText.h"
#include "PokemonGenerator.h"
#include "TrainerGenerator.h"
#include "Tables.h"
#include "Filters.h"
#include "GlobalRandom.h"
#include "Crc.h"
#include "Randomizer.h"

// CMainDialog dialog

IMPLEMENT_DYNAMIC(CMainDialog, CDialogEx)

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, battleItems(FALSE)
	, trainerRandLevels(FALSE)
	, trainerRandMonNames(FALSE)
	, trainerRandName(FALSE)
	, trainerRandSpecies(FALSE)
	, randLevels(FALSE)
	, randLevelsUniformDist(FALSE)
	, randEvIv(FALSE)
	, randEvIvUniformDist(FALSE)
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
{

}

CMainDialog::~CMainDialog()
{
}


BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDLEVELS))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDMONNAMES))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDNAME))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CB_TRAINER_RANDSPECIES))->SetCheck(1);

	((CButton*)this->GetDlgItem(IDC_CBRANDCHOOSEITEMS))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBRANDLEVEL))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBRANDLEVEL_UNIFORMDIST))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBRANDOMEVIV))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBREVIV_UNIFORMDIST2))->SetCheck(0);

	((CButton*)this->GetDlgItem(IDC_CBBATTLEITEMS))->SetCheck(1);

	((CEdit*)this->GetDlgItem(IDC_EDCHANGEITEMN))->SetWindowText(TEXT("8"));
	((CButton*)this->GetDlgItem(IDC_RANDPRIMELEVELS))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CBMULTIPLEGLCRENTALS))->SetCheck(0);
	((CEdit*)this->GetDlgItem(IDC_EDGLCLEVEL))->SetWindowText(TEXT("75"));
	((CButton*)this->GetDlgItem(IDC_CBSPECIESEVIV))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBBOSSSTAYCLOSETOBST))->SetCheck(1);
	

	AddTooltip(IDC_STATIC, TEXT("Changes from this part apply to both rentals and trainer pokemon."));
	
	
		
	AddTooltip(IDC_CBREVIV_UNIFORMDIST2, TEXT("If checked, each ev/iv has equal probability. If unchecked, ")
		TEXT("a normal distribution is used instead."));

	AddTooltip(IDC_CBRANDCHOOSEITEMS, TEXT("Replaces the 6 berries that are always available as items ")
	TEXT("with 6 random ones"));
	AddTooltip(IDC_CBMONSPECIFICITEMS, TEXT("If unchecked, only general battle-items such as berries and ")
	TEXT("quick claw are used. If checked, type-boosting items such as magnet and pokemon-specific items ")
	TEXT("such as thick club are used as well."));
	AddTooltip(IDC_CBRANDLEVEL_UNIFORMDIST, TEXT("If checked, each level has equal probability. If unchecked, ")
		TEXT("a binomal distribution is used instead."));
	
	AddTooltip(IDC_CBBATTLEITEMS, TEXT("If checked, pokemon may have any item, including useless ones such as ")
		TEXT("TMs or Repels. If unchecked, only useful items are generated."));
	AddTooltip(IDC_CBMULTIPLEGLCRENTALS, TEXT("Adds a new set of rentals for the gym leader castle. Originally, the GLC ")
		TEXT("used PokeCup rentals, which may be a bad thing combined with PokeCup rentals random levels. ")
		TEXT("This option adds seperate Sets for Falkner-Chuck, Jasmine-Lance and Kanto"));
	AddTooltip(IDC_CBSPECIESEVIV, TEXT("If checked, Random Ivs/Evs are limited to a certain Range which depends on ")
		TEXT("the pokemons BST: weak pokemon will have high Ivs/Evs, strong Pokemon lower ones."));
	AddTooltip(IDC_CBSTAYCLOSETOBST, TEXT("Prevents regular trainers from having Mewtwos."));
	AddTooltip(IDC_CBBOSSSTAYCLOSETOBST, TEXT("Prevents Rival from having a weedle and a magikarp"));

	std::random_device dev;
	TCHAR seedStr[256];
	_ui64tot_s(dev(), seedStr, _countof(seedStr), 10);
	edSeed.SetWindowText(seedStr);
	((CButton*)this->GetDlgItem(IDC_EDPRIMECUPLEVEL))->SetWindowText(TEXT("95"));

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

	::SendMessage(wndTooltip, TTM_SETDELAYTIME, TTDT_AUTOPOP, _tcslen(tooltip) * 100);

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
	DDX_Check(pDX, IDC_CBRANDLEVEL_UNIFORMDIST, randLevelsUniformDist);
	DDX_Check(pDX, IDC_CBRANDOMEVIV, randEvIv);
	DDX_Check(pDX, IDC_CBREVIV_UNIFORMDIST2, randEvIvUniformDist);
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
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTNBROWSE, &CMainDialog::OnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTNGENERATE, &CMainDialog::OnClickedBtnGenerate)
	ON_EN_CHANGE(IDC_EDSEED, &CMainDialog::OnChangeEdSeed)
	ON_EN_CHANGE(IDC_EDPATH, &CMainDialog::OnChangeEdPath)
END_MESSAGE_MAP()


// CMainDialog message handlers


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

	CString param;
	romPath.GetWindowText(param);
	Randomizer::Randomize(param, this);
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
				char m_romRegion = *((char*)buffer + 0x3E);
				if (strcmp(gameName, "POKEMON STADIUM 2   ") == 0 && (m_romRegion == 'E' || m_romRegion == 'P')) {
					valid = true;
				}
			}
		}
	}

	if (valid) {
		static const TCHAR* valid = "valid";
		stStatus.SetWindowText(valid);
	}
	else {
		static const TCHAR* invalid = "invalid!";
		stStatus.SetWindowText(invalid);
	}
}

