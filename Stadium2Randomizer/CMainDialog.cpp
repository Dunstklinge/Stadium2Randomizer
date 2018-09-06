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
	, seperateGlcRentals(FALSE)
	, seperateGlcRentalsLevel(_T(""))
	, rentalSpeciesEvIv(FALSE)
	, trainerMin1Atk(FALSE)
	, stayCloseToBST(FALSE)
	, bossStayCloseToBST(FALSE)
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

	((CButton*)this->GetDlgItem(IDC_CBRANDLEVEL))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CBRANDLEVEL_UNIFORMDIST))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBRANDOMEVIV))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_CBREVIV_UNIFORMDIST2))->SetCheck(0);

	((CButton*)this->GetDlgItem(IDC_CBBATTLEITEMS))->SetCheck(1);

	((CButton*)this->GetDlgItem(IDC_RANDPRIMELEVELS))->SetCheck(0);
	((CButton*)this->GetDlgItem(IDC_CBGLCRENTALS))->SetCheck(0);
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
	AddTooltip(IDC_CBGLCRENTALS, TEXT("Adds a new set of rentals for the gym leader castle. Originally, the GLC ")
		TEXT("used PokeCup rentals, which may be a bad thing combined with PokeCup rentals random levels. ")
		TEXT("Note that weird trickery had to be used to fit this onto the rom. Bugs may happen, though i havnt seen any."));
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
	DDX_Check(pDX, IDC_CBGLCRENTALS, seperateGlcRentals);
	DDX_Text(pDX, IDC_EDGLCLEVEL, seperateGlcRentalsLevel);
	DDX_Check(pDX, IDC_CBSPECIESEVIV, rentalSpeciesEvIv);
	DDX_Check(pDX, IDC_CB_TRAINERPOKE_MIN1ATK, trainerMin1Atk);
	DDX_Check(pDX, IDC_CBSTAYCLOSETOBST, stayCloseToBST);
	DDX_Check(pDX, IDC_CBBOSSSTAYCLOSETOBST, bossStayCloseToBST);
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
	std::ofstream genLog("genLog.txt");

	UpdateData(true);

	int primecupLevel = _tstoi(strPrimeCupLevel);

	int progressBarMin, progressBarMax;
	progressBar.GetRange(progressBarMin, progressBarMax);
	auto SetProgress = [&](double percent) {
		progressBar.SetPos((int)((progressBarMax - progressBarMin) * percent + progressBarMin));
	};
	
	CString param;
	romPath.GetWindowText(param);

	std::ifstream in(param, std::ifstream::binary);

	//there seem to be 3 formats: normal, byteswapped (read in as 16 bit little endian) and wordswapped
	//(confusingly just called little endian, which makes 0 sense because the rom has no idea about the semantics
	// of values)
	//the first 4 bytes of the header are 80 37 12 40. therefor:
	//if the first byte is 0x80, we are normal
	//if the first byte is 0x37, we are byteswapped
	//if the first byte is 0x40, its wordswapped
	uint8_t firstByte;
	in.read((char*)&firstByte, 1);
	
	bool normal = firstByte == 0x80, byteswapped = firstByte == 0x37, wordswapped = firstByte == 0x40;
	auto DoSwaps = [&](void* buffer, size_t size) {
		uint8_t* bBuffer = (uint8_t*)buffer;
		if (normal) return;
		else if (byteswapped) for (size_t i = 0; i < size; i += 2) SwitchEndianness(*((uint16_t*)bBuffer + i));
		else if (wordswapped) for (size_t i = 0; i < size; i += 4) SwitchEndianness(*((uint32_t*)bBuffer + i));
	};

	DefRoster* roster = DefRoster::FromFileStream(in);
	DefText* text = DefText::FromFileStream(in);

	DoSwaps(roster, DefRoster::segSize);
	DoSwaps(text, DefText::segSize);
	roster->Curate(true);
	text->Curate(true);
	
	//rules for cups
	enum CupRulesId {
		POKECUP = 0,
		LITTLECUP = 1,
		PRIMECUP = 2,
		CHALLENGECUP_1 = 3, CHALLENGECUP_2 = 4, CHALLENGECUP_3 = 5, CHALLENGECUP_4 = 6
	};
	struct CupRules {
		int minLevel;
		int maxLevel;
		int levelSum;

		std::function<bool(GameInfo::MoveId, GameInfo::PokemonId)> legalMoveFilter = nullptr;
		GameInfo::PokemonId* legalMonList = nullptr;
		unsigned int legalMonListN = 0;

	} cupRules[] = { 
		{50, 55, 155, nullptr, PokecupLegalMons, _countof(PokecupLegalMons)},
		{5, 5, 999, &FilterOutLittlecupMoves, LittlecupLegalMons, _countof(LittlecupLegalMons)},
		{randPrimecupLevels ? primecupLevel : 100, 100, 999},  //poke, little, prime/gym

		{30,30,999, nullptr, ChallengecupLegalMonsPokeball, _countof(ChallengecupLegalMonsPokeball)}, //challenge cup 1,2,3,4
		{45,45,999, nullptr, ChallengecupLegalMonsGreatball, _countof(ChallengecupLegalMonsGreatball)},
		{60,60,999, nullptr, ChallengecupLegalMonsUltraball, _countof(ChallengecupLegalMonsUltraball)},
		{75,75,999, nullptr, ChallengecupLegalMonsMasterball, _countof(ChallengecupLegalMonsMasterball)}, 
	}; 

	//
	//randomize rentals
	//
	/*0: Little Cup rentals, offset 490 (#1168)
	1: Prime Cup/Gym leaders castle/Free battle rentals, offset CB0 (#3248)
	2: Prime Cup round 2 (same but with celeby), offset 23D0 (#9168)
	3: Poke Cup, offset 3B20 (#15136)
	4: Poke Cup again (same exact table, referenced twice - maybe this was supposed to be Poke Cup r2?)*/
	auto rentalIt = roster->rentalBegin();
	PokemonGenerator pokeGen;
	pokeGen.changeSpecies = false;
	pokeGen.changeEvsIvs = true;
	pokeGen.changeLevel = true;
	pokeGen.changeMoves = true;
	pokeGen.changeItem = false;

	pokeGen.randEvs = randEvIv;
	pokeGen.randIvs = randEvIv;
	pokeGen.bstEvIvs = rentalSpeciesEvIv;
	pokeGen.statsUniformDistribution = randEvIvUniformDist;
	pokeGen.levelUniformDistribution = randLevelsUniformDist;

	{
		using namespace std::placeholders;
		if (min1Buttons == 1) pokeGen.minOneMoveFilter = std::bind(&FilterMoveByBP, _1, 1, 999);
		else if (min1Buttons == 2) pokeGen.minOneMoveFilter = std::bind(&FilterMoveByBP, _1, 75, 999);
		else if (min1Buttons == 3) pokeGen.minOneMoveFilter = &FilterMoveByStab;
		else if (min1Buttons == 4) pokeGen.minOneMoveFilter =
			std::bind(std::logical_and<bool>(), std::bind(&FilterMoveByBP, _1, 75, 999), std::bind(&FilterMoveByStab, _1, _2));
		else pokeGen.minOneMoveFilter = nullptr;
	}
	auto randomizeRentals = [&](int cupIndex) {
		for (int i = 0; i < rentalIt[cupIndex].nPokemon; i++) {
			DefPokemon newMon = pokeGen.Generate(rentalIt[cupIndex].pokemon[i]);
			rentalIt[cupIndex].pokemon[i] = newMon;
			newMon.Print(genLog);
			genLog << "\n";
		}
	};
	genLog << "Randomizing rentals...\n";
	
	genLog << "Littlecup:\n";
	//little cup
	pokeGen.minLevel = 5;
	pokeGen.maxLevel = 5;
	pokeGen.generalMoveFilter = cupRules[LITTLECUP].legalMoveFilter;
	pokeGen.speciesFilterBuffer = cupRules[LITTLECUP].legalMonList;
	pokeGen.speciesFilterBufferN = cupRules[LITTLECUP].legalMonListN;
	randomizeRentals(0);

	genLog << "Prime Cup:\n";
	//prime cup
	pokeGen.ClearAllFilters();
	pokeGen.minLevel = randPrimecupLevels ? primecupLevel : 100;
	pokeGen.maxLevel = 100;
	randomizeRentals(1);

	//prime cup r2
	genLog << "Prime Cup R2:\n";
	randomizeRentals(2);

	//poke cup
	genLog << "Poke Cup:\n";
	pokeGen.ClearAllFilters();
	pokeGen.speciesFilterBuffer = cupRules[POKECUP].legalMonList;
	pokeGen.speciesFilterBufferN = cupRules[POKECUP].legalMonListN;
	pokeGen.minLevel = 50;
	pokeGen.maxLevel = randLevels ? 55 : 50;
	randomizeRentals(3);

	DefPokemon* testRentals = nullptr;
	if (seperateGlcRentals) {
		//EXPERIMENTAL: try to make a new set of rentals for GLC
		//we will put it here: 
			//; empty space
			//.incbin "baseromgs.n64", 0x2589400, 0x258d000 - 0x2589400
		genLog << "Test\n";
		int glcLevel = _tstoi(seperateGlcRentalsLevel);
		pokeGen.ClearAllFilters();
		pokeGen.minLevel = glcLevel;
		pokeGen.maxLevel = glcLevel;
		testRentals = new DefPokemon[251];
		memset(testRentals, 0, sizeof DefPokemon * 251);
		for (int i = 0; i < 251; i++) {
			testRentals[i].species = (GameInfo::PokemonId)(GameInfo::BULBASAUR + i);
			DefPokemon newMon = pokeGen.Generate(testRentals[i]);
			testRentals[i] = newMon;
			newMon.Print(genLog);
			genLog << "\n";
			testRentals[i].Curate(false);
		}
		roster->tables.subtableInfos[4].tableOffset = 0x2589400 - DefRoster::offStart;
		roster->tables.subtableInfos[4].tableSize = sizeof DefPokemon * 251 + 4;
	}
	

	SetProgress(0.05);

	genLog << "... generating rentals done!\n";
	//
	//randomize trainers
	//

	genLog << "Randomizing Trainers...:\n";
	TrainerGenerator tgen;
	{
		using namespace std::placeholders;
		if (min1Buttons == 1) tgen.gen.minOneMoveFilter = std::bind(&FilterMoveByBP, _1, 1, 999);
		else if (min1Buttons == 2) tgen.gen.minOneMoveFilter = std::bind(&FilterMoveByBP, _1, 75, 999);
		else if (min1Buttons == 3) tgen.gen.minOneMoveFilter = &FilterMoveByStab;
		else if (min1Buttons == 4) tgen.gen.minOneMoveFilter =
			std::bind(std::logical_and<bool>(), std::bind(&FilterMoveByBP, _1, 75, 999), std::bind(&FilterMoveByStab, _1, _2));
		else if (trainerMin1Atk) tgen.gen.minOneMoveFilter = std::bind(&FilterMoveByBP, _1, 1, 999);
		else tgen.gen.minOneMoveFilter = nullptr;
	}
	tgen.usefulItem = battleItems;
	tgen.gen.changeLevel = trainerRandLevels;
	tgen.gen.changeSpecies = trainerRandSpecies;
	tgen.changeName = trainerRandName;
	tgen.changePokemonNicknames = trainerRandMonNames;
	tgen.gen.randEvs = randEvIv;
	tgen.gen.randIvs = randEvIv;
	tgen.gen.statsUniformDistribution = randEvIvUniformDist;
	tgen.gen.levelUniformDistribution = randLevelsUniformDist;
	tgen.stayCloseToBST = stayCloseToBST;
	tgen.stayCloseToBSTThreshold = 30;
	
	
	//rule map; see look FormatNotes
	const CupRules* ruleMap[62];
	for (int i = 2; i <= 5; i++) ruleMap[i - 2] = cupRules;
	for (int i = 6; i <= 6; i++) ruleMap[i - 2] = cupRules+1;
	for (int i = 7; i <= 28; i++) ruleMap[i - 2] = cupRules+2;

	for (int i = 29; i <= 32; i++) ruleMap[i - 2] = cupRules;
	for (int i = 33; i <= 33; i++) ruleMap[i - 2] = cupRules + 1;
	for (int i = 34; i <= 55; i++) ruleMap[i - 2] = cupRules + 2;

	for (int i = 34; i <= 55; i++) ruleMap[i - 2] = cupRules + 2;
	for (int i = 34; i <= 55; i++) ruleMap[i - 2] = cupRules + 2;
	for (int i = 34; i <= 55; i++) ruleMap[i - 2] = cupRules + 2;

	for (int i = 56; i <= 56; i++) ruleMap[i - 2] = cupRules + 3;
	for (int i = 57; i <= 57; i++) ruleMap[i - 2] = cupRules + 4;
	for (int i = 58; i <= 58; i++) ruleMap[i - 2] = cupRules + 5;
	for (int i = 59; i <= 59; i++) ruleMap[i - 2] = cupRules + 6;

	for (int i = 60; i <= 60; i++) ruleMap[i - 2] = cupRules + 3;
	for (int i = 61; i <= 61; i++) ruleMap[i - 2] = cupRules + 4;
	for (int i = 62; i <= 62; i++) ruleMap[i - 2] = cupRules + 5;
	for (int i = 63; i <= 63; i++) ruleMap[i - 2] = cupRules + 6;

	SetProgress(0.08);

	

	auto trainerIt = roster->trainerBegin() + 2;
	for (int i = 2; i <= 63; i++, ++trainerIt) {
		tgen.minLevel = ruleMap[i - 2]->minLevel;
		tgen.maxLevel = ruleMap[i - 2]->maxLevel;
		tgen.levelSum = ruleMap[i - 2]->levelSum;
		tgen.gen.generalMoveFilter = ruleMap[i - 2]->legalMoveFilter;
		tgen.gen.speciesFilterBuffer = ruleMap[i - 2]->legalMonList;
		tgen.gen.speciesFilterBufferN = ruleMap[i - 2]->legalMonListN;
		auto oldOneMoveFilter = tgen.gen.minOneMoveFilter;
		int nTrainers = trainerIt->nTrainers;
		for (int j = 0; j < nTrainers; j++) {
			bool isBoss = j == nTrainers - 1;
			if(!isBoss) switch (trainerIt->trainers[j].trainerCat) {
			case GameInfo::GYM_LEADER:
			case GameInfo::CHAMPION:
			case GameInfo::RIVAL:
			case GameInfo::RIVAL17:
				isBoss = true;
				break;
			case GameInfo::POKEMON_TRAINER:
				isBoss = trainerIt->trainers[j].trainerId - 1 == TableInfo::RED;
				break;
			default:
				break;
			}

			if (isBoss) {
				tgen.stayCloseToBST = bossStayCloseToBST;
				using namespace std::placeholders;
				if (tgen.gen.minOneMoveFilter == nullptr) {
					tgen.gen.minOneMoveFilter = std::bind(FilterMoveByBP, _1, 60, 999);
				}
				else {
					tgen.gen.minOneMoveFilter = std::bind(std::logical_and<bool>(),
						std::bind(tgen.gen.minOneMoveFilter, _1, _2),
						std::bind(&FilterMoveByBP, _1, 60, 999));
				}
			}
			DefTrainer newDef = tgen.Generate(trainerIt->trainers[j]);
			trainerIt->trainers[j] = newDef;
			if (isBoss) {
				tgen.stayCloseToBST = stayCloseToBST;
				tgen.gen.minOneMoveFilter = oldOneMoveFilter;
			}
			newDef.Print(text, genLog);
			genLog << "\n";
			//SetProgress(0.08 + (i - 2) * 0.0132 + 0.0132 * (j+1)/(float)nTrainers);
		}
		
		SetProgress(0.08 + (i - 1) * (0.82/62));
	}

	genLog << "... generating trainers done!\n";

	//apply text changes too
	DefText* newText = (DefText*)new uint8_t[DefText::segSize];
	tgen.textChanges.Apply(text, newText);
	memcpy(text, newText, DefText::segSize);
	delete[]((uint8_t*)newText);

	SetProgress(0.93);
	
	DoSwaps(roster, DefRoster::segSize);
	DoSwaps(text, DefText::segSize);
	text->Curate(false);
	roster->Curate(false);

	//
	// rental chooseable items
	//

	uint8_t* checksumPartBuffer = nullptr;
	
	if (randChooseItems) {
		genLog << "Generating 6 new default items:\n";
		constexpr uint32_t rentalItemOffsets[]{ 0xD963B, 0xD9643, 0xD9653, 0xD9663, 0xD9673,0xD9683 };

		checksumPartBuffer = new uint8_t[CHECKSUM_END];
		in.seekg(0);
		in.read((char*)checksumPartBuffer, CHECKSUM_END);

		//no repetitions!
		GameInfo::ItemId battleItemMapCopy[_countof(GameInfo::BattleItemMap)];
		unsigned int battleItemMapCopySize;
		if (randIncludeMonspecificItems) {
			battleItemMapCopySize = _countof(GameInfo::BattleItemMap);
			memcpy(battleItemMapCopy, GameInfo::BattleItemMap, sizeof GameInfo::BattleItemMap);
		}
		else {
			battleItemMapCopySize = _countof(GameInfo::GeneralBattleItemMap);
			memcpy(battleItemMapCopy, GameInfo::GeneralBattleItemMap, sizeof GameInfo::GeneralBattleItemMap);
		}
		

		for (int i = 0; i < _countof(rentalItemOffsets); i++) {
			std::uniform_int_distribution<unsigned int> dist(0, battleItemMapCopySize - 1 - i);
			unsigned int index = dist(Random::Generator);
			GameInfo::ItemId item = battleItemMapCopy[index];
			genLog << GameInfo::ItemNames[item] << "\n";
			std::swap(battleItemMapCopy[index], battleItemMapCopy[battleItemMapCopySize - 1 - i]);
			checksumPartBuffer[rentalItemOffsets[i]] = item;
		}


		uint32_t crcs[2];
		bool crcSuc = CalculateCrcs(crcs, checksumPartBuffer);
		if (crcSuc) {
			SwitchEndianness(crcs[0]);
			SwitchEndianness(crcs[1]);
			//crcs are at 0x10 and 0x14
			*((uint32_t*)(checksumPartBuffer + 0x10)) = crcs[0];
			*((uint32_t*)(checksumPartBuffer + 0x14)) = crcs[1];
		}
	}
	SetProgress(1);

	const TCHAR filters[] = _T("n64 rom files|*.n64;*.z64;*.v64||");
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filters, this);
	int choice = dlg.DoModal();
	
	if (choice == IDOK) {
		BOOL suc = FALSE;
		if (param == dlg.GetPathName()) {
			//overwrite original
			in.close();
			suc = TRUE;
		}
		else {
			suc = CopyFileA(param, dlg.GetPathName(), FALSE);
		}
		
		if (suc) {
			std::ofstream out(dlg.GetPathName(), std::ofstream::in | std::ofstream::out | std::ofstream::binary);
			out.seekp(DefRoster::offStart);
			out.write((char*)roster, DefRoster::offEnd - DefRoster::offStart);

			out.seekp(DefText::offStart);
			out.write((char*)text, DefText::segSize);

			if (seperateGlcRentals) {
				out.seekp(0x2589400);
				uint8_t testrentalsData[4] = {};
				testrentalsData[0] = 251;
				DoSwaps(testrentalsData, 4);
				DoSwaps(testRentals, sizeof DefPokemon * 251);
				out.write((char*)testrentalsData, 4);
				out.write((char*)testRentals, sizeof DefPokemon * 251);
			}
			

			if (randChooseItems) {
				out.seekp(0);
				out.write((char*)checksumPartBuffer, CHECKSUM_END);
			}
			
		}
	}

	
	delete[] testRentals;
	delete[] checksumPartBuffer;
	delete[]((uint8_t*)text);
	delete[]((uint8_t*)roster);


	SetProgress(0);
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
		uint8_t buffer[0x35];
		in.read((char*)buffer, 0x35);
		in.seekg(0, in.end);
		unsigned int size = (unsigned int)in.tellg();
		if (size == 0x4000000) {
			uint8_t firstByte = buffer[0];
			bool normal = firstByte == 0x80, byteswapped = firstByte == 0x37, wordswapped = firstByte == 0x40;
			if (normal || byteswapped || wordswapped) {
				auto DoSwaps = [&](void* buffer, size_t size) {
					uint8_t* bBuffer = (uint8_t*)buffer;
					if (normal) return;
					else if (byteswapped) for (size_t i = 0; i < size; i += 2) SwitchEndianness(*((uint16_t*)bBuffer + i));
					else if (wordswapped) for (size_t i = 0; i < size; i += 4) SwitchEndianness(*((uint32_t*)bBuffer + i));
				};
				DoSwaps(buffer, 0x35);
				char* gameName = (char*)buffer + 0x20;
				if (strcmp(gameName, "POKEMON STADIUM 2   ") == 0) {
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
