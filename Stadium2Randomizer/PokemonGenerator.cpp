#include "PokemonGenerator.h"

#include "Constants.h"
#include "Tables.h"
#include "GlobalRandom.h"
#include "GeneratorUtil.h"


PokemonGenerator::PokemonGenerator()
{
	changeSpecies = true;
	changeEvsIvs = true;
	changeLevel = true;
	changeMoves = true;
	changeItem = true;
	changeHappiness = true;

	speciesFilter = nullptr;
	speciesFilterBuffer = nullptr;

	minLevel = 100;
	maxLevel = 100;
	levelDist = DiscreteDistribution(0, 100);

	itemFilter = nullptr;
	itemFilterBuffer = nullptr;
	includeTypeSpeciesSpecific = false;

	minOneMoveFilter = nullptr;
	minOneMoveFilterBuffer = nullptr;

	generalMoveFilter = nullptr;
	generalMoveFilterBuffer = nullptr;

	randEvs = true;
	randIvs = true;
	bstEvIvs = true;
	statsDist = DiscreteDistribution(0, 100);
}


PokemonGenerator::~PokemonGenerator()
{
}

DefPokemon PokemonGenerator::Generate() 
{
	DefPokemon mon;
	
	GenSpecies(mon);
	GenLevel(mon);
	GenEvsIvs(mon);
	GenHappiness(mon);
	GenMoves(mon);
	GenItem(mon);

	return mon;
}

DefPokemon PokemonGenerator::Generate(GameInfo::PokemonId species)
{
	DefPokemon mon;

	mon.species = species;
	GenLevel(mon);
	GenEvsIvs(mon);
	GenHappiness(mon);
	GenMoves(mon);
	if (changeItem)
		GenItem(mon);
	else
		mon.item = GameInfo::NO_ITEM;

	return mon;
}

DefPokemon PokemonGenerator::Generate(const DefPokemon & from)
{
	DefPokemon mon = from;

	if(changeSpecies)
		GenSpecies(mon);
	if(changeLevel)
		GenLevel(mon);
	if(changeEvsIvs)
		GenEvsIvs(mon);
	if(changeHappiness)
		GenHappiness(mon);
	if(changeMoves)
		GenMoves(mon);
	if (changeItem)
		GenItem(mon);

	return mon;
}

void PokemonGenerator::GenSpecies(DefPokemon & mon)
{
	const GameInfo::PokemonId* monList = nullptr;
	unsigned int monListN;
	if (speciesFilterBuffer) {
		monList = speciesFilterBuffer;
		monListN = speciesFilterBufferN;
	}
	else {
		monList = GameInfo::PokemonIds;
		monListN = _countof(GameInfo::PokemonIds);
	}

	if (speciesFilter) {
		unsigned int legalMonsN = 0;
		GameInfo::PokemonId legalMons[256];
		for (int i = 0; i < monListN; i++) {
			if (speciesFilter(monList[i])) {
				legalMons[legalMonsN++] = monList[i];
			}
		}
		if (legalMonsN > 0) {
			std::uniform_int_distribution<int> dist(0, (int)legalMonsN-1);
			mon.species = legalMons[dist(Random::Generator)];
			return;
		}
	}
	{
		std::uniform_int_distribution<int> dist(0, monListN-1);
		mon.species = monList[dist(Random::Generator)];
		return;
	}
	
}

void PokemonGenerator::GenLevel(DefPokemon& mon)
{
	if (minLevel == maxLevel) {
		mon.level = minLevel;
		return;
	}
	levelDist.SetMinMax(minLevel, maxLevel);
	mon.level = levelDist(Random::Generator);
}

void PokemonGenerator::GenEvsIvs(DefPokemon & mon)
{
	unsigned int minEv = 0;
	unsigned int maxEv = 0xFFFF;
	unsigned int minIv = 0;
	unsigned int maxIv = 15;

	if (bstEvIvs) {
		constexpr double maxEvRange = 0.6; //in percent from ffff range
		constexpr double maxIvRange = 0.5;

		SatUAr bst = GameInfo::Pokemons[mon.species - 1].CalcBST();
		SatUAr shiftedBst = std::clamp(bst.t, 200u, 680u) - 200; //from 0 to 480
		//200 bst should always be FFFF, 680bst should always be 0.
		if constexpr (false) {
			//round: 200 and 680 should have 0 derivation, while towards the middle (440) deviation should be high
			//EVs
			{
				SatUAr<unsigned int> max = (480u - shiftedBst) * 136.53125; //note: 136.53125 * 480 = 0xFFFF
				unsigned int deviation = (240 - std::abs(240 - (int)shiftedBst)) * (maxEvRange * 0xFFFF / 240.0);
				maxEv = max;
				minEv = max - deviation;
			}
			//IVs
			{
				SatUAr<unsigned int> max = (480u - shiftedBst) * 0.03125;
				unsigned int deviation = (240 - std::abs(240 - (int)(bst - 200u))) * (maxIvRange * 15 / 240.0);
				maxIv = max;
				minIv = max - deviation;
			}
		}
		else {
			//EVs
			{
				//create a parallelogram-shape on the bst/ev graph:
				//area size always N, but 0=[0xFFFF,0xFFFF+N] und 480=[0-N,0] clamped to [0,FFFF]
				constexpr double N = maxEvRange * 0xFFFF;
				//max line: b=0xFFFF+N, m = -(N/480) -(4369/32)
				constexpr double fMaxB = 0xFFFF + N;
				constexpr double fMaxM = -(N / 480) - (4369.0 / 32.0);
				maxEv = std::clamp<int>(fMaxM * shiftedBst + fMaxB, 0, 0xFFFF);
				minEv = std::clamp<int>(fMaxM * shiftedBst + fMaxB - N, 0, 0xFFFF);
			}
			//same for IVs, but 0xFFFF is just 0xF
			{
				constexpr double N = maxEvRange * 0xF;
				constexpr double fMaxB = 0xF + N;
				constexpr double fMaxM = -(N / 480) - (1.0 / 32.0);
				maxIv = std::clamp<int>(fMaxM * shiftedBst + fMaxB, 0, 0xF);
				minIv = std::clamp<int>(fMaxM * shiftedBst + fMaxB - N, 0, 0xF);
			}
		}

	}

	statsDist.SetMinMax(minEv, maxEv);
	mon.evHp = statsDist(Random::Generator);
	mon.evAtk = statsDist(Random::Generator);
	mon.evDef = statsDist(Random::Generator);
	mon.evSpd = statsDist(Random::Generator);
	mon.evSpc = statsDist(Random::Generator);

		
	
	//double nStandardDists = 2;
	statsDist.SetMinMax(minIv, maxIv);
	int dvs[4];
	for (int i = 0; i < 4; i++) dvs[i] = statsDist(Random::Generator);
	mon.dvs = dvs[0] | (dvs[1] << 4) | (dvs[2] << 8) | (dvs[3] << 12);
	
	
	
}

void PokemonGenerator::GenHappiness(DefPokemon& mon) 
{
	std::uniform_int_distribution<int> dist(0, 0xFF);
	mon.happiness = dist(Random::Generator);
}

void PokemonGenerator::GenMoves(DefPokemon & mon)
{
	static GameInfo::MoveId genMoveList[256];
	static unsigned int genMoveListN;
	genMoveListN = 0;

	if (generalMoveFilter) {
		for (int i = 1; i <= GameInfo::LAST_MOVE; i++) {
			if (generalMoveFilter((GameInfo::MoveId)i, mon.species)) {
				genMoveList[genMoveListN++] = (GameInfo::MoveId)i;
			}
		}
	}
	mon.move1 = mon.move2 = mon.move3 = mon.move4 = (GameInfo::MoveId)0;

	const GameInfo::MoveId* baseMoveList = nullptr;
	unsigned int baseMoveListN;
	if (generalMoveFilter) {
		baseMoveList = genMoveList;
		baseMoveListN = genMoveListN;
	}
	else if (generalMoveFilterBuffer) {
		baseMoveList = generalMoveFilterBuffer;
		baseMoveListN = generalMoveFilterBufferN;
	}
	else {
		baseMoveList = GameInfo::MoveIdList;
		baseMoveListN = _countof(GameInfo::MoveIdList);
	}

	std::uniform_int_distribution<int> dist(0, baseMoveListN-1);
	//generate first move specially if requested
	if (minOneMoveFilter) {
		static unsigned int legalMovesN;
		static GameInfo::MoveId legalMoves[256];
		legalMovesN = 0;
		for (unsigned int i = 0; i < baseMoveListN; i++) {
			GameInfo::MoveId move = baseMoveList[i];
			if (minOneMoveFilter(move, mon.species)) {
				legalMoves[legalMovesN++] = move;
			}
		}
		if (legalMovesN > 0) {
			std::uniform_int_distribution<int> dist(0, (int)legalMovesN-1);
			mon.move1 = legalMoves[dist(Random::Generator)];
		}
		
	}
	else if (minOneMoveFilterBuffer && minOneMoveFilterBufferN > 0) {
		std::uniform_int_distribution<int> dist(0, (int)minOneMoveFilterBufferN - 1);
		mon.move1 = minOneMoveFilterBuffer[dist(Random::Generator)];
	}
	if (mon.move1 == 0) {
		mon.move1 = baseMoveList[dist(Random::Generator)];
	}

	//prevent having the same move twice. note that this only works if every entry in basemovelist is unique
	if (dist.b() == 0) return;
	dist = std::uniform_int_distribution<int>(0, dist.b() - 1);
	int iMove2 = dist(Random::Generator);
	while(baseMoveList[iMove2] == mon.move1) iMove2++;
	mon.move2 = baseMoveList[iMove2];
	
	if (dist.b() == 0) return;
	dist = std::uniform_int_distribution<int>(0, dist.b() - 1);
	int iMove3 = dist(Random::Generator);
	while (baseMoveList[iMove3] == mon.move1 || baseMoveList[iMove3] == mon.move2) iMove3++;
	mon.move3 = baseMoveList[iMove3];

	if (dist.b() == 0) return;
	dist = std::uniform_int_distribution<int>(0, dist.b() - 1);
	int iMove4 = dist(Random::Generator);
	while (baseMoveList[iMove4] == mon.move1 || baseMoveList[iMove4] == mon.move2
		|| baseMoveList[iMove4] == mon.move3) iMove4++;
	mon.move4 = baseMoveList[iMove4];
}

void PokemonGenerator::GenItem(DefPokemon & mon)
{

	union {
		struct {
			GameInfo::ItemId speciesItem;
			GameInfo::ItemId typeItem1;
			GameInfo::ItemId typeItem2;
		};
		GameInfo::ItemId items[3];
	} extraItems;
	extraItems.speciesItem = GameInfo::SpeciesBattleItemMap[mon.species];
	extraItems.typeItem1 = GameInfo::TypeBattleItemMap[GameInfo::Pokemons[mon.species - 1].type1];
	extraItems.typeItem2 = GameInfo::TypeBattleItemMap[GameInfo::Pokemons[mon.species - 1].type2];
	
	int includeItemsIgnore = 0; //bitfield, bit 1 ,2 ,3 for species, type1, type2
	if (extraItems.speciesItem == GameInfo::NO_ITEM) includeItemsIgnore |= 1;
	if (extraItems.typeItem1 == GameInfo::NO_ITEM) includeItemsIgnore |= 2;
	if (extraItems.typeItem2 == GameInfo::NO_ITEM) includeItemsIgnore |= 4;
	if (extraItems.typeItem1 == extraItems.typeItem2) includeItemsIgnore |= 4; //in case of mono type pokemon

	unsigned int legalItemsN = 0;
	static GameInfo::ItemId legalItems[256];
	const GameInfo::ItemId* itemList;
	unsigned int itemListN;

	FilterBufferGenerateOrDefault(&itemList, &itemListN,
						legalItems, legalItemsN,
						GameInfo::ExistingItemMap, _countof(GameInfo::ExistingItemMap),
						itemFilterBuffer, itemFilterBufferN,
						&[&](GameInfo::ItemId iId) {
		if (extraItems.speciesItem == iId) includeItemsIgnore |= 1;
		if (extraItems.typeItem1 == iId) includeItemsIgnore |= 2;
		if (extraItems.typeItem2 == iId) includeItemsIgnore |= 4;
		if (itemFilter) return itemFilter(iId, mon.species); 
		return false;
	});


	if (includeTypeSpeciesSpecific) {
		//try drawing type or species specific item
		unsigned int addN = 3 - 
			((includeItemsIgnore & 1) + ((includeItemsIgnore & 2) >> 1) + ((includeItemsIgnore & 4) >> 2));
		if (addN > 0) {
			std::uniform_int_distribution<int> dist(0, (int)itemListN - 1 + addN);
			int rand = dist(Random::Generator);
			if (rand > (int)itemListN - 1) {
				unsigned int useAddN = rand - (int)itemListN - 1;
				for (int i = 0; i < 3; i++) {
					if (((1 << i) & includeItemsIgnore) && useAddN-- == 0) {
						mon.item = extraItems.items[i];
						return;
					}
				}
			}
		}
		
	}
	
	std::uniform_int_distribution<int> dist(0, (int)itemListN - 1);
	mon.item = itemList[dist(Random::Generator)];
	return;
	

	/*
	if (itemFilter) {
		for (int i = 0; i <= _countof(GameInfo::ExistingItemMap); i++) {
			if (itemFilter(GameInfo::ExistingItemMap[i], mon.species)) {
				legalItems[legalItemsN++] = (GameInfo::ItemId)i;
			}
		}
		if (legalItemsN > 0) {
			std::uniform_int_distribution<int> dist(0, (int)legalItemsN-1);
			mon.item = (GameInfo::ItemId)dist(Random::Generator);
			return;
		}
		
	}
	if (itemFilterBuffer) {
		std::uniform_int_distribution<int> dist(0, (int)itemFilterBufferN-1);
		mon.item = itemFilterBuffer[dist(Random::Generator)];
		return;
	}
	{
		std::uniform_int_distribution<int> dist(0, (int)_countof(GameInfo::ExistingItemMap)-1);
		mon.item = GameInfo::ExistingItemMap[dist(Random::Generator)];
		return;
	}*/
}
