#include "PokemonTable.h"
#include "MoveTable.h"

#include <algorithm>
#include <string.h>

namespace GameInfo {
	Pokemon Pokemons[256] = { 0 };					//1 based (for some reason, its like that in the rom)
	LevelupMoves PokemonLevelupMoveEntries[251];	//0 based (its like that in the rom)
	PokemonGen1TMsEntry PokemonGen1Tms[151];		//0 based
	PokemonLegalMovesEntry PokemonLegalMoves[251];	//0 based

	std::vector<MoveId> PokemonLegalMoves_BaseList;

	namespace {
		void InitPokemonLegalMoves() {
			for (int i = 0; i < 251; i++) {
				int thisMonStart = PokemonLegalMoves_BaseList.size();
				auto contains = [&](MoveId move) {
					return std::find(PokemonLegalMoves_BaseList.begin() + thisMonStart,
									 PokemonLegalMoves_BaseList.end(),
									 move)
						!= PokemonLegalMoves_BaseList.end();
				};
				auto add = [&](MoveId move) {
					if (!contains(move)) PokemonLegalMoves_BaseList.push_back(move);
				};
				//start with glc tms
				for (int tm = 0; tm < sizeof(GameInfo::TmsGlc); tm++) {
					if ((Pokemons[i + 1].tms[tm / 8] >> (tm % 8)) & 1) {
						PokemonLegalMoves_BaseList.push_back(TmsGlc[tm]);
					}
				}
				//add rb tms
				if(i < 151) for (int tm = 0; tm < sizeof(GameInfo::TmsRby); tm++) {
					if (((PokemonGen1Tms[i].tms[tm / 8] >> (tm % 8)) & 1) 
						&& !contains(TmsRby[tm]))
					{
						add(TmsRby[tm]);
					}
				}
				//add y tms; note that those are hardcoded in stadium as well, so we will do this here too
				if ((i + 1) == PokemonId::CHARIZARD)		add(MoveId::FLY);	//06	06	Charizard(got fly)
				else if ((i + 1) == PokemonId::BUTTERFREE)	add(MoveId::FLASH);	//0C	12	Butterfree(got flash)
				else if ((i + 1) == PokemonId::VENOMOTH)	add(MoveId::FLASH);	//31	49	Venomoth(got flash)
				else if ((i + 1) == PokemonId::DIGLETT)		add(MoveId::CUT);	//32	50	Diglett(got cut)
				else if ((i + 1) == PokemonId::DUGTRIO)		add(MoveId::CUT);	//33	51	Dugtrio(got cut)
				else if ((i + 1) == PokemonId::KABUTOPS)	add(MoveId::CUT);	//8D	141	Kaputops(got cut)
				//lastly, levelup non-egg moves
				for (auto& it : PokemonLevelupMoveEntries[i]) {
					if (!it.flags.egg && !it.flags.eggC) {
						add(it.move);
					}
				}

				//now, egg moves
				int eggStart = PokemonLegalMoves_BaseList.size();
				for (auto& it : PokemonLevelupMoveEntries[i]) {
					if (it.flags.egg || it.flags.eggC) {
						add(it.move);
					}
				}

				//build entry out of these
				PokemonLegalMovesEntry entry;
				entry.beginIt = thisMonStart;
				entry.normal.beginIt = thisMonStart;
				entry.normal.endIt = eggStart;
				entry.egg.beginIt = eggStart;
				entry.egg.endIt = PokemonLegalMoves_BaseList.size();
				entry.endIt = PokemonLegalMoves_BaseList.size();
				
				PokemonLegalMoves[i] = entry;
			}
		}
	}

	void InitPokemonTable()
	{
		//TODO: maybe read from config file

		//note that bulbasaur appears to be 1 so 0 must hold a special meaning
		memcpy(Pokemons+1, PokemonRawData, sizeof(PokemonRawData));


		//levelup move list
		int it = 0;
		for (int i = 0; i < 251; i++) {
			PokemonLevelupMoveEntries[i] = { 0,0 };
			while (true) {
				PokemonId mon = (PokemonId)(i + 1);
				LevelupMoveEntry* t = (LevelupMoveEntry*)(PokemonLevelupMovesRawData + it);
				if(!PokemonLevelupMoveEntries[i].beginIt) PokemonLevelupMoveEntries[i].beginIt = t;
				it += 3;
				if (!t->level) {
					PokemonLevelupMoveEntries[i].endIt = (LevelupMoveEntry*)(PokemonLevelupMovesRawData + it - 3);
					while (it % 4) it++;
					break;
				}
			}
		}

		//gen 1 tm list
		memcpy(PokemonGen1Tms, PokemonGen1TmsRawData, sizeof(PokemonGen1TMsEntry) * 151);

		//build legal moves list from this info
		InitPokemonLegalMoves();
	}


	const PokemonId PokemonIds[]{
		BULBASAUR,
		IVYSAUR,
		VENUSAUR,
		CHARMANDER,
		CHARMELEON,
		CHARIZARD,
		SQUIRTLE,
		WARTORTLE,
		BLASTOISE,
		CATERPIE,
		METAPOD,
		BUTTERFREE,
		WEEDLE,
		KAKUNA,
		BEEDRILL,
		PIDGEY,
		PIDGEOTTO,
		PIDGEOT,
		RATTATA,
		RATICATE,
		SPEAROW,
		FEAROW,
		EKANS,
		ARBOK,
		PIKACHU,
		RAICHU,
		SANDSHREW,
		SANDSLASH,
		NIDORAN_F,
		NIDORINA,
		NIDOQUEEN,
		NIDORAN_M,
		NIDORINO,
		NIDOKING,
		CLEFAIRY,
		CLEFABLE,
		VULPIX,
		NINETALES,
		JIGGLYPUFF,
		WIGGLYTUFF,
		ZUBAT,
		GOLBAT,
		ODDISH,
		GLOOM,
		VILEPLUME,
		PARAS,
		PARASECT,
		VENONAT,
		VENOMOTH,
		DIGLETT,
		DUGTRIO,
		MEOWTH,
		PERSIAN,
		PSYDUCK,
		GOLDUCK,
		MANKEY,
		PRIMEAPE,
		GROWLITHE,
		ARCANINE,
		POLIWAG,
		POLIWHIRL,
		POLIWRATH,
		ABRA,
		KADABRA,
		ALAKAZAM,
		MACHOP,
		MACHOKE,
		MACHAMP,
		BELLSPROUT,
		WEEPINBELL,
		VICTREEBEL,
		TENTACOOL,
		TENTACRUEL,
		GEODUDE,
		GRAVELER,
		GOLEM,
		PONYTA,
		RAPIDASH,
		SLOWPOKE,
		SLOWBRO,
		MAGNEMITE,
		MAGNETON,
		FARFETCH_D,
		DODUO,
		DODRIO,
		SEEL,
		DEWGONG,
		GRIMER,
		MUK,
		SHELLDER,
		CLOYSTER,
		GASTLY,
		HAUNTER,
		GENGAR,
		ONIX,
		DROWZEE,
		HYPNO,
		KRABBY,
		KINGLER,
		VOLTORB,
		ELECTRODE,
		EXEGGCUTE,
		EXEGGUTOR,
		CUBONE,
		MAROWAK,
		HITMONLEE,
		HITMONCHAN,
		LICKITUNG,
		KOFFING,
		WEEZING,
		RHYHORN,
		RHYDON,
		CHANSEY,
		TANGELA,
		KANGASKHAN,
		HORSEA,
		SEADRA,
		GOLDEEN,
		SEAKING,
		STARYU,
		STARMIE,
		MR_MIME,
		SCYTHER,
		JYNX,
		ELECTABUZZ,
		MAGMAR,
		PINSIR,
		TAUROS,
		MAGIKARP,
		GYARADOS,
		LAPRAS,
		DITTO,
		EEVEE,
		VAPOREON,
		JOLTEON,
		FLAREON,
		PORYGON,
		OMANYTE,
		OMASTAR,
		KABUTO,
		KABUTOPS,
		AERODACTYL,
		SNORLAX,
		ARTICUNO,
		ZAPDOS,
		MOLTRES,
		DRATINI,
		DRAGONAIR,
		DRAGONITE,
		MEWTWO,
		MEW,
		CHIKORITA,
		BAYLEEF,
		MEGANIUM,
		CYNDAQUIL,
		QUILAVA,
		TYPHLOSION,
		TOTODILE,
		CROCONAW,
		FERALIGATR,
		SENTRET,
		FURRET,
		HOOTHOOT,
		NOCTOWL,
		LEDYBA,
		LEDIAN,
		SPINARAK,
		ARIADOS,
		CROBAT,
		CHINCHOU,
		LANTURN,
		PICHU,
		CLEFFA,
		IGGLYBUFF,
		TOGEPI,
		TOGETIC,
		NATU,
		XATU,
		MAREEP,
		FLAAFFY,
		AMPHAROS,
		BELLOSSOM,
		MARILL,
		AZUMARILL,
		SUDOWOODO,
		POLITOED,
		HOPPIP,
		SKIPLOOM,
		JUMPLUFF,
		AIPOM,
		SUNKERN,
		SUNFLORA,
		YANMA,
		WOOPER,
		QUAGSIRE,
		ESPEON,
		UMBREON,
		MURKROW,
		SLOWKING,
		MISDREAVUS,
		UNOWN,
		WOBBUFFET,
		GIRAFARIG,
		PINECO,
		FORRETRESS,
		DUNSPARCE,
		GLIGAR,
		STEELIX,
		SNUBBULL,
		GRANBULL,
		QWILFISH,
		SCIZOR,
		SHUCKLE,
		HERACROSS,
		SNEASEL,
		TEDDIURSA,
		URSARING,
		SLUGMA,
		MAGCARGO,
		SWINUB,
		PILOSWINE,
		CORSOLA,
		REMORAID,
		OCTILLERY,
		DELIBIRD,
		MANTINE,
		SKARMORY,
		HOUNDOUR,
		HOUNDOOM,
		KINGDRA,
		PHANPY,
		DONPHAN,
		PORYGON2,
		STANTLER,
		SMEARGLE,
		TYROGUE,
		HITMONTOP,
		SMOOCHUM,
		ELEKID,
		MAGBY,
		MILTANK,
		BLISSEY,
		RAIKOU,
		ENTEI,
		SUICUNE,
		LARVITAR,
		PUPITAR,
		TYRANITAR,
		LUGIA,
		HO_OH,
		CELEBI,
	};

	//generated from a website with a python script, hope its right
	PokemonId PokemonPrevEvo[256] {
		NO_POKEMON,
		NO_POKEMON,
		BULBASAUR,
		IVYSAUR,
		NO_POKEMON,
		CHARMANDER,
		CHARMELEON,
		NO_POKEMON,
		SQUIRTLE,
		WARTORTLE,
		NO_POKEMON,
		CATERPIE,
		METAPOD,
		NO_POKEMON,
		WEEDLE,
		KAKUNA,
		NO_POKEMON,
		PIDGEY,
		PIDGEOTTO,
		NO_POKEMON,
		RATTATA,
		NO_POKEMON,
		SPEAROW,
		NO_POKEMON,
		EKANS,
		PICHU,
		NO_POKEMON,
		NO_POKEMON,
		SANDSHREW,
		NO_POKEMON,
		NIDORAN_F,
		NIDORINA,
		NO_POKEMON,
		NIDORAN_M,
		NIDORINO,
		CLEFFA,
		CLEFAIRY,
		NO_POKEMON,
		VULPIX,
		IGGLYBUFF,
		JIGGLYPUFF,
		NO_POKEMON,
		ZUBAT,
		NO_POKEMON,
		ODDISH,
		GLOOM,
		NO_POKEMON,
		PARAS,
		NO_POKEMON,
		VENONAT,
		NO_POKEMON,
		DIGLETT,
		NO_POKEMON,
		MEOWTH,
		NO_POKEMON,
		PSYDUCK,
		NO_POKEMON,
		MANKEY,
		NO_POKEMON,
		GROWLITHE,
		NO_POKEMON,
		POLIWAG,
		POLIWHIRL,
		NO_POKEMON,
		ABRA,
		KADABRA,
		NO_POKEMON,
		MACHOP,
		MACHOKE,
		NO_POKEMON,
		BELLSPROUT,
		WEEPINBELL,
		NO_POKEMON,
		TENTACOOL,
		NO_POKEMON,
		GEODUDE,
		GRAVELER,
		NO_POKEMON,
		PONYTA,
		NO_POKEMON,
		SLOWPOKE,
		NO_POKEMON,
		MAGNEMITE,
		NO_POKEMON,
		NO_POKEMON,
		DODUO,
		NO_POKEMON,
		SEEL,
		NO_POKEMON,
		GRIMER,
		NO_POKEMON,
		SHELLDER,
		NO_POKEMON,
		GASTLY,
		HAUNTER,
		NO_POKEMON,
		NO_POKEMON,
		DROWZEE,
		NO_POKEMON,
		KRABBY,
		NO_POKEMON,
		VOLTORB,
		NO_POKEMON,
		EXEGGCUTE,
		NO_POKEMON,
		CUBONE,
		TYROGUE,
		TYROGUE,
		NO_POKEMON,
		NO_POKEMON,
		KOFFING,
		NO_POKEMON,
		RHYHORN,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		HORSEA,
		NO_POKEMON,
		GOLDEEN,
		NO_POKEMON,
		STARYU,
		NO_POKEMON,
		NO_POKEMON,
		SMOOCHUM,
		ELEKID,
		MAGBY,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		MAGIKARP,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		EEVEE,
		EEVEE,
		EEVEE,
		NO_POKEMON,
		NO_POKEMON,
		OMANYTE,
		NO_POKEMON,
		KABUTO,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		DRATINI,
		DRAGONAIR,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		CHIKORITA,
		BAYLEEF,
		NO_POKEMON,
		CYNDAQUIL,
		QUILAVA,
		NO_POKEMON,
		TOTODILE,
		CROCONAW,
		NO_POKEMON,
		SENTRET,
		NO_POKEMON,
		HOOTHOOT,
		NO_POKEMON,
		LEDYBA,
		NO_POKEMON,
		SPINARAK,
		GOLBAT,
		NO_POKEMON,
		CHINCHOU,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		TOGEPI,
		NO_POKEMON,
		NATU,
		NO_POKEMON,
		MAREEP,
		FLAAFFY,
		GLOOM,
		NO_POKEMON,
		MARILL,
		NO_POKEMON,
		POLIWHIRL,
		NO_POKEMON,
		HOPPIP,
		SKIPLOOM,
		NO_POKEMON,
		NO_POKEMON,
		SUNKERN,
		NO_POKEMON,
		NO_POKEMON,
		WOOPER,
		EEVEE,
		EEVEE,
		NO_POKEMON,
		SLOWPOKE,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		PINECO,
		NO_POKEMON,
		NO_POKEMON,
		ONIX,
		NO_POKEMON,
		SNUBBULL,
		NO_POKEMON,
		SCYTHER,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		TEDDIURSA,
		NO_POKEMON,
		SLUGMA,
		NO_POKEMON,
		SWINUB,
		NO_POKEMON,
		NO_POKEMON,
		REMORAID,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		HOUNDOUR,
		SEADRA,
		NO_POKEMON,
		PHANPY,
		PORYGON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		TYROGUE,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		CHANSEY,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		LARVITAR,
		PUPITAR,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON,
		NO_POKEMON
	};
};