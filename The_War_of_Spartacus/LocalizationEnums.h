#pragma once

enum Language
{
	ENGLISH,
	UKRAINIAN,
	RUSSIAN,
	LATIN
};

namespace Localized
{
	enum Message
	{
		MESSAGES_LOG,
		CONTINUE_GAME,
		RESUME_GAME,
		SAVE_GAME,
		LOAD_GAME,
		NEW_GAME,
		SETTINGS,
		SPECIALS,
		SPECIALS_TEXT,
		EXIT_TO_MENU,
		EXIT,
		CHARACTER_CREATION,
		NAME,
		AGE,
		UNNASSIGNED_ATTRIBUTES,
		STRENGTH,
		CONSTITUTION,
		DEXTERITY,
		INTELLIGENCE,
		WISDOM,
		CHARISMA,
		STRENGTH_GENITIVE,
		CONSTITUTION_GENITIVE,
		DEXTERITY_GENITIVE,
		INTELLIGENCE_GENITIVE,
		WISDOM_GENITIVE,
		CHARISMA_GENITIVE,
		BACK,
		NEXT,
		VIDEO_SETTINGS,
		AUDIO_SETTINGS,
		AUTOSAVE_SETTINGS,
		LANGUAGE_SETTINGS,
		GLADIATOR,
		CITY,
		ARENA,
		QUEST,
		MARKET,
		VIEW_CHARACTER,
		REST,
		MAP,
		TRAVEL,
		YOU_HAVE_TRAVELED,
		ENTER,
		MENU,
		FIGHT,
		BET,
		TRAIN,
		LANISTA,
		TAVERN,
		TALK_TO_PEOPLE,
		BUY_FREEDOM,
		TAKE_QUEST,
		ASK_FOR_PROMOTION,
		CHOOSE_OPPONENT,
		LEVEL,
		EXPERIENCE,
		DAMAGE,
		BLOCK_DEFENSE,
		BLOCK_CHANCE,
		EMPTY_HAND,
		HEALTH,
		FAME,
		STRENGTH_SCALE,
		DEXTERITY_SCALE,
		ARMOUR_DEFENSE,
		EVASION_CHANCE,
		STUN_RESISTANCE_CHANCE,
		NO_ARMOUR,
		RESET_CHANGES,
		APPLY_CHANGES,
		REST_REGEN,
		REST_REGEN_FULL,
		REST_FULL,
		HEALTH_POINTS_GENITIVE,
		HP,
		LVL,
		TRAIN_GAIN,
		TRAIN_NEED_REST,
		LEVELED_UP,
		EXPERIENCE_GENITIVE,
		HAVE_TO_CHOOSE_OPPONENT,
		OPPONENT_NOT_SELECTED,
		FIGHT_NEED_REST,
		FIGHT_INJURED,
		SPARE_OPPONENT,
		EXECUTE_OPPONENT,
		SURRENDER,
		CONTINUE_FIGHT,
		END_FIGHT,
		VERSUS,
		OPPONENT_ATTACKS_FIRST,
		PLAYER_ATTACKS_FIRST,
		YOU_HAVE_FOUGHT_WITH,
		YOU_HAVE_WON,
		YOU_HAVE_LOST,
		YOU_HAVE_GAINED,
		FAME_GENITIVE,
		YOU_HAVE_DEALT,
		YOU_HAVE_STUNNED,
		YOU_MISSED,
		OPPONENT_BLOCKED,
		OPPONENT_COUNTERATTACKED,
		OPPONENT_HAVE_DEALT,
		OPPONENT_HAVE_STUNNED,
		OPPONENT_MISSED,
		YOU_HAVE_BLOCKED,
		YOU_HAVE_COUNTERATTACKED,
		DAMAGE_GENITIVE,
		CRITICAL_DAMAGE_GENITIVE,
		WON_OPPONENT_DEAD,
		WON_OPPONENT_SURRENDERED,
		LOST_SURRENDER,
		LOST_DEAD,
		INVENTORY,
		RIGHT_HAND,
		LEFT_HAND,
		ARMOUR,
		UNEQUIP_ITEM,
		EQUIP_ITEM,
		INSPECT_ITEM,
		DESTROY_ITEM,
		VALUE,
		QUANTITY,
		TOTAL_VALUE,
		ITEM_TIER,
		SELL,
		BUY,
		CANT_SELL_ITEM,
		TRADER_DOESNT_HAVE_GOLD,
		CANT_BUY_ITEM,
		PLAYER_DOESNT_HAVE_GOLD,
		PLAYER_DOESNT_HAVE_STORAGE,
		GOLD_GENITIVE,
		CHARACTER,
		EQUIPMENT,
		CHOOSE_STARTING_EQUIPMENT,
		SET,
		SELECT,
		SELECTED,
		SELECT_EQUIPMENT,
		EQUIPMENT_UNSELECTED,
		SAVE_SUCCESFUL,
		CHOOSE_SAVE,
		SAVE_UNSELECTED,
		STORY_WELCOME_SCREEN_DATE,
		STORY_WELCOME_SCREEN_TEXT,
		STORY_START_GAME_TEXT,
		UNNASSIGNED_ATTRIBUTES_GENITIVE,
		PLAYER_IS_SLAVE,
		CANT_TRAVEL,
		STORY_FIRST_VICTORY_TEXT,
		STORY_STOLEN_MONEY_TEXT1,
		STORY_STOLEN_MONEY_TEXT2,
		STORY_STOLEN_MONEY_TEXT3,
		STORY_STOLEN_MONEY_ESCAPE,
		STORY_STOLEN_MONEY_STAY,
		ESCAPE,
		STAY,
		STORY_CENTURION_OFFER_TEXT1,
		STORY_CENTURION_OFFER_TEXT2,
		STORY_CENTURION_OFFER_ACCEPT,
		STORY_CENTURION_OFFER_DECLINE,
		ACCEPT_OFFER,
		DECLINE_OFFER,
		STORY_GAINED_FREEDOM_TEXT1,
		STORY_GAINED_FREEDOM_TEXT2,
		STORY_GAINED_FREEDOM_CHOSE_REBELS,
		STORY_GAINED_FREEDOM_CHOSE_LEGION,
		CHOOSE_REBELS,
		CHOOSE_LEGION,
		GAINED_FREEDOM,
		YOU_HAVE_BEEN_PROMOTED,
		NOT_ENOUGH_CHARISMA_FOR_PROMOTION,
		NOT_ENOUGH_CHARISMA_FOR_PROMOTION_AGRESSIVELY,
		ALREADY_PROMOTED,
		ALREADY_PROMOTED_AGRESSIVELY,
		NOT_ENOUGH_MONEY_FOR_TRAVEL,
		SPENT_MONEY_ON_TRAVEL,
		SHANDAR_STAT_INTRO,
		SHANDAR_BUT_INTRO,
		SHANDAR_STAT_INTRO2,
		SHANDAR_BUT_CHOICE1_QUESTION_WHO,
		SHANDAR_BUT_CHOICE1_QUESTION_WHY,
		SHANDAR_BUT_CHOICE1_QUESTION_WHERE,
		SHANDAR_BUT_CHOICE1_LEAVE,
		SHANDAR_STAT_CHOICE1_ANSWER_WHO,
		SHANDAR_STAT_CHOICE1_ANSWER_WHY,
		SHANDAR_STAT_CHOICE1_ANSWER_WHERE,
		SHANDAR_BUT_CHOICE2_TALK,
		SHANDAR_BUT_CHOICE2_LEAVE,
		SHANDAR_STAT_CHOICE2_ANSWER_TALK,
		SHANDAR_BUT_CHOICE3_FIGHT,
		SHANDAR_BUT_CHOICE3_STRENGTH,
		SHANDAR_BUT_CHOICE3_CHARISMA,
		SHANDAR_STAT_CHOICE3_STRENGTH_WIN,
		SHANDAR_STAT_CHOICE3_STRENGTH_LOSE,
		SHANDAR_STAT_CHOICE3_CHARISMA_WIN,
		SHANDAR_STAT_CHOICE3_CHARISMA_LOSE,
		SHANDAR_BUT_COMEBACK_NIGHT,
		SHANDAR_STAT_COMEBACK_NIGHT,
		SHANDAR_BUT_CHOICE4_NIGHT_ENTER,
		SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY,
		SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN,
		SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE,
		SHANDAR_STAT_ENTER_HOUSE,
		SHANDAR_BUT_ENTER_HOUSE,
		SHANDAR_STAT_ENTER_HOUSE2,
		SHANDAR_BUT_CHOICE5_FIGHT,
		SHANDAR_BUT_CHOICE5_CHARISMA,
		SHANDAR_STAT_CHOICE5_FIGHT_WIN,
		SHANDAR_STAT_CHOICE5_FIGHT_LOSE,
		SHANDAR_STAT_LOST_QUEST,
		SHANDAR_STAT_CHOICE5_CHARISMA_WIN,
		SHANDAR_STAT_CHOICE5_CHARISMA_LOSE,
		SHANDAR_BUT_CHOICE6_SPARE,
		SHANDAR_BUT_CHOICE6_EXECUTE,
		SHANDAR_BUT_CHOICE6_EXTORT,
		SHANDAR_BUT_CHOICE6_EXILE,
		SHANDAR_STAT_CHOICE6_EXECUTED,
		SHANDAR_STAT_CHOICE6_EXTORT,
		SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE,
		SHANDAR_STAT_CHOICE6_EXILE,
		SHANDAR_BUT_CHOICE6_WHERE_GIRL,
		SHANDAR_STAT_FOUND_GIRL,
		SHANDAR_BUT_GREET_GIRL,
		SHANDAR_STAT_GREET_GIRL,
		SHANDAR_BUT_LETS_LEAVE,
		SHANDAR_STAT_LEAVE_QUIET,
		SHANDAR_STAT_LEAVE_LOUD,
		SHANDAR_STAT_RETURNED_DAUGHTER,
		SHANDAR_BUT_ASK_REWARD,
		SHANDAR_STAT_REWARD,
		SHANDAR_BUT_GOODBYE,
		SHANDAR_STAT_GOODBYE,
		SHANDAR_BUT_NAME,
		SHANDAR_STAT_THANKS,
		SHANDAR_STAT_END,
		WALK_STREETS_NOTHING,
		MESSAGE_NUMBER
	};
}