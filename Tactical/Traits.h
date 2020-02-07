#ifndef __TRAITS_H
#define __TRAITS_H

// SANDRO - replaced this list to represent the old traits only
typedef enum
{
	NO_SKILLTRAIT_OT = 0,
	LOCKPICKING_OT,
	HANDTOHAND_OT,
	ELECTRONICS_OT,
	NIGHTOPS_OT,
	THROWING_OT,
	TEACHING_OT,
	HEAVY_WEAPS_OT,
	AUTO_WEAPS_OT,
	STEALTHY_OT,
	AMBIDEXT_OT,
	THIEF_OT,
	MARTIALARTS_OT,
	KNIFING_OT,
	PROF_SNIPER_OT,
	CAMOUFLAGED_OT,
	// SANDRO - I dared to remove these for it is really useless,
	//   with the camo removing/repainting feature you only need the basic camouflage trait anyway
	//CAMOUFLAGED_URBAN,
	//CAMOUFLAGED_DESERT,
	//CAMOUFLAGED_SNOW,
	EXPERT,
	NUM_SKILLTRAITS_OT
} OTrait;

//////////////////////////////////////////////////////////////////////
// SANDRO - the list for new traits
typedef enum
{
	NO_SKILLTRAIT_NT = 0,
	// MAJOR TRAITS
	AUTO_WEAPONS_NT = 1,
	HEAVY_WEAPONS_NT = 2,
	SNIPER_NT = 3,
	RANGER_NT = 4,
	GUNSLINGER_NT = 5,
	MARTIAL_ARTS_NT = 6,
	SQUADLEADER_NT = 7,
	TECHNICIAN_NT = 8,
	DOCTOR_NT = 9,
	// MINOR TRAITS
	AMBIDEXTROUS_NT = 10,
	MELEE_NT = 11,
	THROWING_NT = 12,
	NIGHT_OPS_NT = 13,
	STEALTHY_NT = 14,
	ATHLETICS_NT = 15,
	BODYBUILDING_NT = 16,
	DEMOLITIONS_NT = 17,
	TEACHING_NT = 18,
	SCOUTING_NT = 19,

	// Flugente: new traits have to go here, even if they are major traits, as otherwise the existing traits in profiles get mixed up
	COVERT_NT = 20,	// a major trait
	RADIO_OPERATOR_NT = 21,		// a minor trait
	SNITCH_NT = 22,		// a minor trait 
	SURVIVAL_NT = 23,	// a minor trait

	NUM_SKILLTRAITS_NT
} NTrait;

#define NUM_MAJOR_TRAITS 10
#define NUM_MINOR_TRAITS 13

// save game relevant, so be careful when changing
#define MAX_NUM_TRAITS 30

#define NUM_ORIGINAL_MAJOR_TRAITS 9

// Flugente: I've had it with this hardcoding madness. Without this, adding or removing a new trait would crash anything related to a bubblehelp display of traits
// always check every use of these enums and every use of the skill-strings if you add a new trait
#define NEWTRAIT_MERCSKILL_EXPERTOFFSET	(NUM_MAJOR_TRAITS + NUM_MINOR_TRAITS)
#define NEWTRAIT_MERCSKILL_OFFSET_ALL	(NEWTRAIT_MERCSKILL_EXPERTOFFSET + NUM_MAJOR_TRAITS)

// Flugente: these aren't really traits, but it is convenient to pretend so
#define INTEL	(2 * NEWTRAIT_MERCSKILL_EXPERTOFFSET + 2)
#define VARIOUSSKILLS	(INTEL + 1)

class SOLDIERTYPE;
class MERCPROFILESTRUCT;

class SoldierTraits {
public:
	SoldierTraits( SOLDIERTYPE *pSoldier );

	// traits, non-accessors

	BOOLEAN HasMartialArtistSpecialAnimation( void );
	BOOLEAN HasThrowingSpecialAnimation( void );
	BOOLEAN HasRequiredNumberOfTraitsNeededForSurgery( void );
	FLOAT BandagingSpeedFactorInPercent( void );
	BOOLEAN HasFocus( void );
	UINT8 HandcuffBonus( void );
	FLOAT GetAttackAPMultiplier( OBJECTTYPE* pObj, UINT8 ubMode );

	// traits, properties

	UINT8 ubAWBonusCtHAssaultRifles( void );
	UINT8 ubAWBonusCtHSMGs( void );
	UINT8 ubAWBonusCtHLMGs( void );
	UINT8 ubAWAutoFirePenaltyReduction( void );
	UINT8 ubAWUnwantedBulletsReduction( void );
	UINT8 ubAWFiringSpeedBonusLMGs( void );
	UINT8 ubAWPercentReadyLMGReduction( void );
	UINT8 ubHWGrenadeLaunchersAPsReduction( void );
	UINT8 ubHWRocketLaunchersAPsReduction( void );
	UINT8 ubHWBonusCtHGrenadeLaunchers( void );
	UINT8 ubHWBonusCtHRocketLaunchers( void );
	UINT8 ubHWMortarAPsReduction( void );
	UINT8 ubHWMortarCtHPenaltyReduction( void );
	UINT8 ubHWDamageTanksBonusPercent( void );
	UINT8 ubHWDamageBonusPercentForHW( void );
	UINT8 ubSNBonusCtHRifles( void );
	UINT8 ubSNBonusCtHSniperRifles( void );
	UINT8 ubSNEffRangeToTargetReduction( void );
	UINT8 ubSNAimingBonusPerClick( void );
	UINT8 ubSNDamageBonusPerClick( void );
	UINT8 ubSNDamageBonusFromNumClicks( void );
	UINT8 ubSNChamberRoundAPsReduction( void );
	UINT8 ubSNAimClicksAdded( void );
	UINT8 ubSNFocusRadius( void );
	INT16 sSNFocusInterruptBonus( void );
	UINT8 ubRABonusCtHRifles( void );
	UINT8 ubRABonusCtHShotguns( void );
	UINT8 ubRAPumpShotgunsAPsReduction( void );
	UINT8 ubRAFiringSpeedBonusShotguns( void );
	UINT8 ubRAAimClicksAdded( void );
	UINT8 ubRAEffectiveRangeBonusShotguns( void );
	UINT8 ubRAReloadSpeedShotgunsManual( void );
	UINT8 ubGSFiringSpeedBonusPistols( void );
	UINT8 ubGSEffectiveRangeBonusPistols( void );
	UINT8 ubGSBonusCtHPistols( void );
	UINT8 ubGSBonusCtHMachinePistols( void );
	UINT8 ubGSCtHMPExcludeAuto( void );
	UINT8 ubGSAimingBonusPerClick( void );
	UINT8 ubGSPercentReadyPistolsReduction( void );
	UINT8 ubGSRealoadSpeedHandgunsBonus( void );
	UINT8 ubGSAimClicksAdded( void );
	UINT8 ubMAPunchAPsReduction( void );
	UINT8 ubMABonusCtHBareHands( void );
	UINT8 ubMABonusCtHBrassKnuckles( void );
	UINT8 ubMABonusDamageHandToHand( void );
	UINT8 ubMABonusBreathDamageHandToHand( void );
	UINT16 usMALostBreathRegainPenalty( void );
	UINT16 usMAAimedPunchDamageBonus( void );
	INT8 bMAAimedPunchCtHModifier( void );
	UINT8 ubMAChanceToDodgeHtH( void );
	UINT8 ubMAOnTopCTDHtHBareHanded( void );
	UINT8 ubMAOnTopCTDHtHBrassKnuckles( void );
	UINT8 ubMAChanceToDodgeMelee( void );
	UINT8 ubMAReducedAPsToSteal( void );
	UINT8 ubMAAPsChangeStanceReduction( void );
	UINT8 ubMAApsTurnAroundReduction( void );
	UINT8 ubMAAPsClimbOrJumpReduction( void );
	UINT8 ubMAReducedAPsRegisteredWhenMoving( void );
	UINT8 ubMAChanceToCkickDoors( void );
	BOOLEAN fPermitExtraAnimationsOnlyToMA( void );
	UINT16 usSLRadiusNormal( void );
	UINT16 usSLRadiusExtendedEar( void );
	UINT8 ubSLMaxBonuses( void );
	UINT8 ubSLBonusAPsPercent( void );
	UINT8 ubSLEffectiveLevelInRadius( void );
	UINT8 ubSLEffectiveLevelAsStandby( void );
	UINT8 ubSLCollectiveInterruptsBonus( void );
	UINT8 ubSLOverallSuppresionBonusPercent( void );
	UINT8 ubSLMoraleGainBonus( void );
	UINT8 ubSLMoraleLossReduction( void );
	UINT8 ubSLFearResistance( void );
	UINT8 ubSLDeathMoralelossMultiplier( void );
	UINT16 usTERepairSpeedBonus( void );
	UINT16 usTELockpickingBonus( void );
	UINT16 usTEDisarmElTrapBonus( void );
	UINT16 usTEAttachingItemsBonus( void );
	UINT8 ubTEUnjamGunBonus( void );
	UINT8 ubTERepairElectronicsPenaltyReduction( void );
	UINT8 ubTEChanceToDetectTrapsBonus( void );
	UINT8 ubTECtHControlledRobotBonus( void );
	UINT8 ubTETraitsNumToRepairRobot( void );
	UINT8 ubTERepairRobotPenaltyReduction( void );
	BOOLEAN fTETraitsCanRestoreItemThreshold( void );
	UINT8 ubDONumberTraitsNeededForSurgery( void );
	UINT8 ubDOSurgeryHealPercentBase( void );
	UINT8 ubDOSurgeryHealPercentOnTop( void );
	UINT8 ubDOSurgeryHealPercentBloodbag( void );
	UINT16 usDOSurgeryMedBagConsumption( void );
	UINT16 usDOSurgeryMaxBreathLoss( void );
	UINT16 usDORepairStatsRateBasic( void );
	UINT16 usDORepairStatsRateOnTop( void );
	UINT8 ubDORepStPenaltyIfAlsoHealing( void );
	UINT8 ubDOHealingPenaltyIfAlsoStatRepair( void );
	BOOLEAN fDORepStShouldThrowMessage( void );
	UINT16 usDODoctorAssignmentBonus( void );
	UINT8 ubDOBandagingSpeedPercent( void );
	UINT8 ubDONaturalRegenBonus( void );
	UINT8 ubDOMaxRegenBonuses( void );
	UINT8 ubAMPenaltyDoubleReduction( void );
	UINT8 ubAMReloadSpeedMagazines( void );
	UINT8 ubAMReloadSpeedLoose( void );
	UINT8 ubAMPickItemsAPsReduction( void );
	UINT8 ubAMWorkBackpackAPsReduction( void );
	UINT8 ubAMHandleDoorsAPsReduction( void );
	UINT8 ubAMHandleBombsAPsReduction( void );
	UINT8 ubAMAttachingItemsAPsReduction( void );
	UINT8 ubMEBladesAPsReduction( void );
	UINT8 ubMECtHBladesBonus( void );
	UINT8 ubMECtHBluntBonus( void );
	UINT8 ubMEDamageBonusBlades( void );
	UINT8 ubMEDamageBonusBlunt( void );
	UINT16 usMEAimedMeleeAttackDamageBonus( void );
	UINT8 ubMEDodgeBladesBonus( void );
	UINT8 ubMECtDBladesOnTopWithBladeInHands( void );
	UINT8 ubMEDodgeBluntBonus( void );
	UINT8 ubMECtDBluntOnTopWithBladeInHands( void );
	UINT8 ubTHBladesAPsReduction( void );
	UINT8 ubTHBladesMaxRange( void );
	UINT8 ubTHBladesCtHBonus( void );
	UINT8 ubTHBladesCtHBonusPerClick( void );
	UINT8 ubTHBladesDamageBonus( void );
	UINT8 ubTHBladesDamageBonusPerClick( void );
	UINT8 ubTHBladesSilentCriticalHitChance( void );
	UINT8 ubTHBladesCriticalHitMultiplierBonus( void );
	UINT8 ubTHBladesAimClicksAdded( void );
	UINT8 ubNOeSightRangeBonusInDark( void );
	UINT8 ubNOHearingRangeBonus( void );
	UINT8 ubNOHearingRangeBonusInDark( void );
	UINT8 ubNOIterruptsBonusInDark( void );
	UINT8 ubNONeedForSleepReduction( void );
	UINT8 ubSTStealthModeSpeedBonus( void );
	UINT8 ubSTBonusToMoveQuietly( void );
	UINT8 ubSTStealthBonus( void );
	UINT8 ubSTReducedAPsRegistered( void );
	UINT8 ubSTStealthPenaltyForMovingReduction( void );
	UINT8 ubATAPsMovementReduction( void );
	UINT8 ubATBPsMovementReduction( void );
	UINT8 ubBBDamageResistance( void );
	UINT8 ubBBCarryWeightBonus( void );
	UINT8 ubBBBreathLossForHtHImpactReduction( void );
	UINT16 usBBIncreasedNeededDamageToFallDown( void );
	UINT8 ubDEAPsNeededToThrowGrenadesReduction( void );
	UINT8 ubDEMaxRangeToThrowGrenades( void );
	UINT8 ubDECtHWhenThrowingGrenades( void );
	UINT8 ubDEDamageOfBombsAndMines( void );
	UINT8 ubDEAttachDetonatorCheckBonus( void );
	UINT8 ubDEPlantAndRemoveBombCheckBonus( void );
	UINT8 ubDEPlacedBombLevelBonus( void );
	UINT8 ubDEShapedChargeDamageMultiplier( void );
	UINT8 ubTGBonusToTrainMilitia( void );
	UINT8 ubTGEffectiveLDRToTrainMilitia( void );
	UINT8 ubTGBonusToTeachOtherMercs( void );
	UINT8 ubTGEffectiveSkillValueForTeaching( void );
	UINT8 ubTGBonusOnPractising( void );
	UINT8 ubSCSightRangebonusWithScopes( void );
	UINT16 usSCSightRangebonusWithBinoculars( void );
	UINT8 ubSCTunnelVisionReducedWithBinoculars( void );
	BOOLEAN fSCCanDetectEnemyPresenseAround( void );
	BOOLEAN fSCCanDetermineEnemyNumbersAround( void );
	BOOLEAN fSCDetectionInDiagonalSectors( void );
	BOOLEAN fSCPreventsTheEnemyToAmbushMercs( void );
	BOOLEAN fSCPreventsBloodcatsAmbushes( void );
	BOOLEAN fSCThrowMessageIfAmbushPrevented( void );
	INT16 sCOMeleeCTHBonus( void );
	INT16 sCoMeleeInstakillBonus( void );
	INT16 sCODisguiseAPReduction( void );
	INT16 sCOCloseDetectionRange( void );
	INT16 sCOCloseDetectionRangeSoldierCorpse( void );
	UINT16 usCOEliteUncoverRadius( void );
	BOOLEAN fCODetectIfBleeding( void );
	BOOLEAN fCOStripIfUncovered( void );
	BOOLEAN fROAllowArtillery( void );
	BOOLEAN fROArtilleryDistributedOverTurns( void );
	UINT8 ubSNTBaseChance( void );
	BOOLEAN fSNTMercOpinionAboutSnitchBonusModifier( void );
	BOOLEAN fSNTSnitchOpinionAboutMercBonusModifier( void );
	BOOLEAN fSNTSnitchLeadershipBonusModifer( void );
	INT8 bSNTSociableMercBonus( void );
	INT8 bSNTLonerMercBonus( void );
	INT8 bSNTSameAssignmentBonus( void );
	INT8 bSNTMercOpinionAboutMercTreshold( void );
	UINT8 ubSNTPassiveReputationGain( void );
	UINT8 ubSNTHearingRangeBonus( void );
	BOOLEAN fSNTPrisonSnitchInterrogationMultiplier( void );
	BOOLEAN fSNTPrisonSnitchGuardStrengthMultiplier( void );
	UINT8 ubSVGroupTimeSpentForTravellingFoot( void );
	UINT8 ubSVGroupTimeSpentForTravellingVehicle( void );
	UINT8 ubSVMaxBonusesToTravelSpeed( void );
	UINT8 ubSVBreathForTravellingReduction( void );
	FLOAT dSVWeatherPenaltiesReduction( void );
	UINT8 ubSVCamoWornountSpeedReduction( void );
	UINT16 usSVTrackerMaxRange( void );
	UINT16 usSVTrackerAbility( void );
	UINT16 usSVDiseaseResistance( void );
	INT16 sSVFoodConsumption( void );
	INT16 sSVDrinkConsumption( void );
	UINT16 usSVSnakeDefense( void );

	INT8 StealthLevels();
	INT8 HearingBonus();
	INT8 ExtraHearingBonus();

// TODO can't make this private yet, would need to remove magic HAS_SKILL_TRAIT and NUM_SKILL_TRAITS first
	INT8 SoldierNTraitLevel( NTrait trait );
	INT8 SoldierOTraitLevel( OTrait trait );

private:
	SOLDIERTYPE* pSoldier; // extension methods base object
};

class ProfileTraits {
public:
	ProfileTraits( MERCPROFILESTRUCT* pProfile );

	void FillHelpText( CHAR16 sString[] );

	INT8 ProfileNTraitLevel( NTrait trait );
	INT8 ProfileOTraitLevel( OTrait trait );

private:
	MERCPROFILESTRUCT* pProfile; // extension methods base object
};

BOOLEAN HasMajorNTrait( NTrait trait );
BOOLEAN HasMajorOTrait( OTrait trait );

BOOLEAN TwoStagedOldImpTrait( UINT8 uiImpSkillTraitNumber );

// TODO get rid of magic methods that can't leverage from compiler static analysis
// TODO get rid of trait based checking!
// instead of checking a trait, a stat should be checked
// => dont check if there is a docter, check is there is someone with bandaging skills
BOOLEAN HAS_SKILL_TRAIT( SOLDIERTYPE* pSoldier, UINT8 uiSkillTraitNumber );
INT8 NUM_SKILL_TRAITS( SOLDIERTYPE* pSoldier, UINT8 uiSkillTraitNumber );


#endif