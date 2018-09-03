#include "ItemTable.h"

namespace GameInfo {

	ItemId ExistingItemMap[] = {
		MASTER_BALL,
		ULTRA_BALL,
		BRIGHTPOWDER,
		GREAT_BALL,
		POKE_BALL,

		BICYCLE,
		MOON_STONE,
		ANTIDOTE,
		BURN_HEAL,
		ICE_HEAL,
		AWAKENING,
		PARLYZ_HEAL,
		FULL_RESTORE,
		MAX_POTION,
		HYPER_POTION,
		SUPER_POTION,
		POTION,
		ESCAPE_ROPE,
		REPEL,
		MAX_ELIXER,
		FIRE_STONE,
		THUNDERSTONE,
		WATER_STONE,
		HP_UP,
		PROTEIN,
		IRON,
		CARBOS,
		LUCKY_PUNCH,
		CALCIUM,
		RARE_CANDY,
		X_ACCURACY,
		LEAF_STONE,
		METAL_POWDER,
		NUGGET,
		POKE_DOLL,
		FULL_HEAL,
		REVIVE,
		MAX_REVIVE,
		GUARD_SPEC,
		SUPER_REPEL,
		MAX_REPEL,
		DIRE_HIT,
		FRESH_WATER,
		SODA_POP,
		LEMONADE,
		X_ATTACK,
		X_DEFEND,
		X_SPEED,
		X_SPECIAL,
		COIN_CASE,
		ITEMFINDER,
		EXP_SHARE,
		OLD_ROD,
		GOOD_ROD,
		SILVER_LEAF,
		SUPER_ROD,
		PP_UP,
		ETHER,
		MAX_ETHER,
		ELIXER,
		RED_SCALE,
		SECRETPOTION,
		S_S_TICKET,
		MYSTERY_EGG,
		CLEAR_BELL,
		SILVER_WING,
		MOOMOO_MILK,
		QUICK_CLAW,
		PSNCUREBERRY,
		GOLD_LEAF,
		SOFT_SAND,
		SHARP_BEAK,
		PRZCUREBERRY,
		BURNT_BERRY,
		ICE_BERRY,
		POISON_BARB,
		KINGS_ROCK,
		BITTER_BERRY,
		MINT_BERRY,
		RED_APRICORN,
		TINYMUSHROOM,
		BIG_MUSHROOM,
		SILVERPOWDER,
		BLU_APRICORN,
		AMULET_COIN,
		YLW_APRICORN,
		GRN_APRICORN,
		CLEANSE_TAG,
		MYSTIC_WATER,
		TWISTEDSPOON,
		WHT_APRICORN,
		BLACKBELT,
		BLK_APRICORN,
		PNK_APRICORN,
		BLACKGLASSES,
		SLOWPOKETAIL,
		PINK_BOW,
		STICK,
		SMOKE_BALL,
		NEVERMELTICE,
		MAGNET,
		MIRACLEBERRY,
		PEARL,
		BIG_PEARL,
		EVERSTONE,
		SPELL_TAG,
		RAGECANDYBAR,
		GS_BALL,
		BLUE_CARD,
		MIRACLE_SEED,
		THICK_CLUB,
		FOCUS_BAND,
		ENERGYPOWDER,
		ENERGY_ROOT,
		HEAL_POWDER,
		REVIVAL_HERB,
		HARD_STONE,
		LUCKY_EGG,
		CARD_KEY,
		MACHINE_PART,
		EGG_TICKET,
		STARDUST,
		STAR_PIECE,
		BASEMENT_KEY,
		PASS,
		CHARCOAL,
		BERRY_JUICE,
		SCOPE_LENS,
		METAL_COAT,
		DRAGON_FANG,
		LEFTOVERS,
		MYSTERYBERRY,
		DRAGON_SCALE,
		BERSERK_GENE,
		SACRED_ASH,
		HEAVY_BALL,
		FLOWER_MAIL,
		LEVEL_BALL,
		LURE_BALL,
		FAST_BALL,
		LIGHT_BALL,
		FRIEND_BALL,
		MOON_BALL,
		LOVE_BALL,
		NORMAL_BOX,
		GORGEOUS_BOX,
		SUN_STONE,
		POLKADOT_BOW,
		UP_GRADE,
		BERRY,
		GOLD_BERRY,
		SQUIRTBOTTLE,
		PARK_BALL,
		RAINBOW_WING,
		BRICK_PIECE,
		SURF_MAIL,
		LITEBLUEMAIL,
		PORTRAITMAIL,
		LOVELY_MAIL,
		EON_MAIL,
		MORPH_MAIL,
		BLUESKY_MAIL,
		MUSIC_MAIL,
		MIRAGE_MAIL,
		TM_01,
		TM_02,
		TM_03,
		TM_04,
		TM_05,
		TM_06,
		TM_07,
		TM_08,
		TM_09,
		TM_10,
		TM_11,
		TM_12,
		TM_13,
		TM_14,
		TM_15,
		TM_16,
		TM_17,
		TM_18,
		TM_19,
		TM_20,
		TM_21,
		TM_22,
		TM_23,
		TM_24,
		TM_25,
		TM_26,
		TM_27,
		TM_28,
		TM_29,
		TM_30,
		TM_31,
		TM_32,
		TM_33,
		TM_34,
		TM_35,
		TM_36,
		TM_37,
		TM_38,
		TM_39,
		TM_40,
		TM_41,
		TM_42,
		TM_43,
		TM_44,
		TM_45,
		TM_46,
		TM_47,
		TM_48,
		TM_49,
		TM_50,
		HM_01,
		HM_02,
		HM_03,
		HM_04,
		HM_05,
		HM_06,
		HM_07,
		HM_08
	};


	ItemId BattleItemMap[38] = {
		LUCKY_PUNCH,
		METAL_POWDER,
		QUICK_CLAW,
		PSNCUREBERRY,
		SOFT_SAND,
		SHARP_BEAK,
		PRZCUREBERRY,
		BURNT_BERRY,
		ICE_BERRY,
		POISON_BARB,
		KINGS_ROCK,
		BITTER_BERRY,
		MINT_BERRY,
		SILVERPOWDER,
		MYSTIC_WATER,
		TWISTEDSPOON,
		BLACKBELT,
		BLACKGLASSES,
		PINK_BOW,
		STICK,
		NEVERMELTICE,
		MAGNET,
		MIRACLEBERRY,
		MIRACLE_SEED,
		THICK_CLUB,
		FOCUS_BAND,
		HARD_STONE,
		CHARCOAL,
		BERRY_JUICE,
		SCOPE_LENS,
		DRAGON_FANG,
		LEFTOVERS,
		MYSTERYBERRY,
		BERSERK_GENE,
		LIGHT_BALL,
		POLKADOT_BOW,
		BERRY,
		GOLD_BERRY,

	};

	ItemId NonPokemonBattleItemMap[33]{
		QUICK_CLAW,
		PSNCUREBERRY,
		SOFT_SAND,
		SHARP_BEAK,
		PRZCUREBERRY,
		BURNT_BERRY,
		ICE_BERRY,
		POISON_BARB,
		KINGS_ROCK,
		BITTER_BERRY,
		MINT_BERRY,
		SILVERPOWDER,
		MYSTIC_WATER,
		TWISTEDSPOON,
		BLACKBELT,
		BLACKGLASSES,
		PINK_BOW,
		NEVERMELTICE,
		MAGNET,
		MIRACLEBERRY,
		MIRACLE_SEED,
		FOCUS_BAND,
		HARD_STONE,
		CHARCOAL,
		BERRY_JUICE,
		SCOPE_LENS,
		DRAGON_FANG,
		LEFTOVERS,
		MYSTERYBERRY,
		BERSERK_GENE,
		POLKADOT_BOW,
		BERRY,
		GOLD_BERRY,
	};
	ItemId GeneralBattleItemMap[17]{
		QUICK_CLAW,
		PSNCUREBERRY,
		PRZCUREBERRY,
		BURNT_BERRY,
		ICE_BERRY,
		KINGS_ROCK,
		BITTER_BERRY,
		MINT_BERRY,
		MIRACLEBERRY,
		FOCUS_BAND,
		BERRY_JUICE,
		SCOPE_LENS,
		LEFTOVERS,
		MYSTERYBERRY,
		BERSERK_GENE,
		BERRY,
		GOLD_BERRY,
	};

	ItemId SpeciesBattleItemMap[256]{
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		LIGHT_BALL,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		STICK,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		THICK_CLUB,
		THICK_CLUB,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		LUCKY_PUNCH,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		METAL_POWDER,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
		NO_ITEM,
	};
}
