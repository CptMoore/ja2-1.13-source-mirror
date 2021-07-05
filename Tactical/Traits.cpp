#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include "builddefines.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "wcheck.h"
#include "Render Fun.h"
#include "stdlib.h"
#include "debug.h"
#include "MemMan.h"
#include "Overhead Types.h"
#include "soldier profile type.h"
#include "Soldier Control.h"
#include "Interface.h"
#include "Text.h"
#include "personnel.h"
#endif
#include <Laptop\IMP Skill Trait.h>
#include <Soldier Profile.h>
#include <Animation Data.h>

SoldierTraits::SoldierTraits( SOLDIERTYPE* pSoldier ) : pSoldier( pSoldier )
{
}

// traits, non-accessors

BOOLEAN SoldierTraits::HasMartialArtistSpecialAnimation()
{
	if ( pSoldier->ubProfile == NO_PROFILE || pSoldier->ubBodyType != REGMALE )  // SANDRO - added check for body type
		return FALSE;

	return gGameOptions.fNewTraitSystem
		? ( SoldierNTraitLevel( MARTIAL_ARTS_NT ) >= ( ( gSkillTraitValues.fPermitExtraAnimationsOnlyToMA ) ? 2 : 1 ) )
		: SoldierOTraitLevel( MARTIALARTS_OT ) > 0;
}

BOOLEAN SoldierTraits::HasThrowingSpecialAnimation()
{
	return gGameOptions.fNewTraitSystem ? SoldierNTraitLevel( THROWING_NT ) > 0 : SoldierOTraitLevel( THROWING_OT ) > 0;
}

BOOLEAN SoldierTraits::HasRequiredNumberOfTraitsNeededForSurgery()
{
	return gGameOptions.fNewTraitSystem && ( SoldierNTraitLevel( DOCTOR_NT ) >= gSkillTraitValues.ubDONumberTraitsNeededForSurgery );
}

FLOAT SoldierTraits::BandagingSpeedFactorInPercent()
{
	return gGameOptions.fNewTraitSystem ?
		( 100 - gSkillTraitValues.bSpeedModifierBandaging )
		* gSkillTraitValues.ubDOBandagingSpeedPercent
		* SoldierNTraitLevel( DOCTOR_NT )
		: 100;
}

BOOLEAN SoldierTraits::HasFocus()
{
	return gGameOptions.fNewTraitSystem
		&& (
			HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT )
			|| HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT )
			|| HAS_SKILL_TRAIT( pSoldier, SNIPER_NT )
			|| HAS_SKILL_TRAIT( pSoldier, RANGER_NT )
			|| HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT )
			);
}

UINT8 SoldierTraits::HandcuffBonus( void )
{
	if ( gGameOptions.fNewTraitSystem )
	{
		return 25 * SoldierNTraitLevel( MARTIAL_ARTS_NT ) + 10 * SoldierNTraitLevel( MELEE_NT );
	}
	else
	{
		return 25 * SoldierOTraitLevel( MARTIALARTS_OT ) + 25 * SoldierOTraitLevel( HANDTOHAND_OT ) + 10 * SoldierOTraitLevel( KNIFING_OT );
	}
}

FLOAT SoldierTraits::GetAttackAPMultiplier( OBJECTTYPE* pObj, UINT8 ubMode )
{
	UINT8 percent = 0;

	if ( !pSoldier || !pObj || !gGameOptions.fNewTraitSystem )
	{
		// we shouldn't be here...
	}
	else if ( ubMode == WM_NORMAL || ubMode == WM_BURST || ubMode == WM_AUTOFIRE )
	{
		WEAPONTYPE* pWeaponType = &Weapon[pObj->usItem];
		INVTYPE* pInvType = &Item[pObj->usItem];

		if ( ( ubMode == WM_BURST || ubMode == WM_AUTOFIRE ) && pWeaponType->ubWeaponType == GUN_LMG )
		{
			percent = ubAWFiringSpeedBonusLMGs();
		}
		else if ( pWeaponType->ubWeaponType == GUN_PISTOL )
		{
			percent = ubGSFiringSpeedBonusPistols();
		}
		else if ( pWeaponType->ubWeaponType == GUN_SHOTGUN )
		{
			percent = ubRAFiringSpeedBonusShotguns();
		}
		else if ( pInvType->usItemClass == IC_BLADE )
		{
			percent = ubMEBladesAPsReduction();
		}
		else if ( pInvType->usItemClass == IC_PUNCH && pInvType->brassknuckles )
		{
			percent = ubMAPunchAPsReduction();
		}
		else if ( pInvType->usItemClass == IC_THROWING_KNIFE )
		{
			percent = ubTHBladesAPsReduction();
		}
		// grenade launchers
		else if ( ( pInvType->usItemClass == IC_LAUNCHER || pInvType->grenadelauncher ) && !( pInvType->rocketlauncher ) && !( pInvType->mortar ) )
		{
			percent = ubHWGrenadeLaunchersAPsReduction();
		}
		// rocket launchers
		else if ( ( pInvType->rocketlauncher || pInvType->singleshotrocketlauncher ) && !( pInvType->mortar ) )
		{
			percent = ubHWRocketLaunchersAPsReduction();
		}
		// mortar
		else if ( pInvType->mortar )
		{
			percent = ubHWMortarAPsReduction() + pSoldier->GetBackgroundValue( BG_ARTILLERY );
		}
	}
	else if ( ubMode == WM_ATTACHED_GL || ubMode == WM_ATTACHED_GL_BURST || ubMode == WM_ATTACHED_GL_AUTO )
	{
		percent = ubHWGrenadeLaunchersAPsReduction();
	}

	return ( 100 - percent ) / 100.0f;
}

// traits, properties

UINT8 SoldierTraits::ubAWBonusCtHAssaultRifles()
{
	return gSkillTraitValues.ubAWBonusCtHAssaultRifles * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubAWBonusCtHSMGs()
{
	return gSkillTraitValues.ubAWBonusCtHSMGs * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubAWBonusCtHLMGs()
{
	return gSkillTraitValues.ubAWBonusCtHLMGs * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubAWAutoFirePenaltyReduction()
{
	return gSkillTraitValues.ubAWAutoFirePenaltyReduction * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubAWUnwantedBulletsReduction()
{
	return gSkillTraitValues.ubAWUnwantedBulletsReduction * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubAWFiringSpeedBonusLMGs()
{
	return gSkillTraitValues.ubAWFiringSpeedBonusLMGs * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubAWPercentReadyLMGReduction()
{
	return gSkillTraitValues.ubAWPercentReadyLMGReduction * SoldierNTraitLevel( AUTO_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWGrenadeLaunchersAPsReduction()
{
	return gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWRocketLaunchersAPsReduction()
{
	return gSkillTraitValues.ubHWRocketLaunchersAPsReduction * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWBonusCtHGrenadeLaunchers()
{
	return gSkillTraitValues.ubHWBonusCtHGrenadeLaunchers * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWBonusCtHRocketLaunchers()
{
	return gSkillTraitValues.ubHWBonusCtHRocketLaunchers * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWMortarAPsReduction()
{
	return gSkillTraitValues.ubHWMortarAPsReduction * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWMortarCtHPenaltyReduction()
{
	return gSkillTraitValues.ubHWMortarCtHPenaltyReduction * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWDamageTanksBonusPercent()
{
	return gSkillTraitValues.ubHWDamageTanksBonusPercent * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubHWDamageBonusPercentForHW()
{
	return gSkillTraitValues.ubHWDamageBonusPercentForHW * SoldierNTraitLevel( HEAVY_WEAPONS_NT );
}

UINT8 SoldierTraits::ubSNBonusCtHRifles()
{
	return gSkillTraitValues.ubSNBonusCtHRifles * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNBonusCtHSniperRifles()
{
	return gSkillTraitValues.ubSNBonusCtHSniperRifles * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNEffRangeToTargetReduction()
{
	return gSkillTraitValues.ubSNEffRangeToTargetReduction * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNAimingBonusPerClick()
{
	return gSkillTraitValues.ubSNAimingBonusPerClick * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNDamageBonusPerClick()
{
	return gSkillTraitValues.ubSNDamageBonusPerClick * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNDamageBonusFromNumClicks()
{
	return gSkillTraitValues.ubSNDamageBonusFromNumClicks * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNChamberRoundAPsReduction()
{
	return gSkillTraitValues.ubSNChamberRoundAPsReduction * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNAimClicksAdded()
{
	return gSkillTraitValues.ubSNAimClicksAdded * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubSNFocusRadius()
{
	return gSkillTraitValues.ubSNFocusRadius * SoldierNTraitLevel( SNIPER_NT );
}

INT16 SoldierTraits::sSNFocusInterruptBonus()
{
	return gSkillTraitValues.sSNFocusInterruptBonus * SoldierNTraitLevel( SNIPER_NT );
}

UINT8 SoldierTraits::ubRABonusCtHRifles()
{
	return gSkillTraitValues.ubRABonusCtHRifles * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubRABonusCtHShotguns()
{
	return gSkillTraitValues.ubRABonusCtHShotguns * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubRAPumpShotgunsAPsReduction()
{
	return gSkillTraitValues.ubRAPumpShotgunsAPsReduction * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubRAFiringSpeedBonusShotguns()
{
	return gSkillTraitValues.ubRAFiringSpeedBonusShotguns * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubRAAimClicksAdded()
{
	return gSkillTraitValues.ubRAAimClicksAdded * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubRAEffectiveRangeBonusShotguns()
{
	return gSkillTraitValues.ubRAEffectiveRangeBonusShotguns * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubRAReloadSpeedShotgunsManual()
{
	return gSkillTraitValues.ubRAReloadSpeedShotgunsManual * SoldierNTraitLevel( RANGER_NT );
}

UINT8 SoldierTraits::ubGSFiringSpeedBonusPistols()
{
	return gSkillTraitValues.ubGSFiringSpeedBonusPistols * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSEffectiveRangeBonusPistols()
{
	return gSkillTraitValues.ubGSEffectiveRangeBonusPistols * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSBonusCtHPistols()
{
	return gSkillTraitValues.ubGSBonusCtHPistols * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSBonusCtHMachinePistols()
{
	return gSkillTraitValues.ubGSBonusCtHMachinePistols * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSCtHMPExcludeAuto()
{
	return gSkillTraitValues.ubGSCtHMPExcludeAuto * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSAimingBonusPerClick()
{
	return gSkillTraitValues.ubGSAimingBonusPerClick * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSPercentReadyPistolsReduction()
{
	return gSkillTraitValues.ubGSPercentReadyPistolsReduction * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSRealoadSpeedHandgunsBonus()
{
	return gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubGSAimClicksAdded()
{
	return gSkillTraitValues.ubGSAimClicksAdded * SoldierNTraitLevel( GUNSLINGER_NT );
}

UINT8 SoldierTraits::ubMAPunchAPsReduction()
{
	return gSkillTraitValues.ubMAPunchAPsReduction * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMABonusCtHBareHands()
{
	return gSkillTraitValues.ubMABonusCtHBareHands * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMABonusCtHBrassKnuckles()
{
	return gSkillTraitValues.ubMABonusCtHBrassKnuckles * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMABonusDamageHandToHand()
{
	return gSkillTraitValues.ubMABonusDamageHandToHand * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMABonusBreathDamageHandToHand()
{
	return gSkillTraitValues.ubMABonusBreathDamageHandToHand * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT16 SoldierTraits::usMALostBreathRegainPenalty()
{
	return gSkillTraitValues.usMALostBreathRegainPenalty * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT16 SoldierTraits::usMAAimedPunchDamageBonus()
{
	return gSkillTraitValues.usMAAimedPunchDamageBonus * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

INT8 SoldierTraits::bMAAimedPunchCtHModifier()
{
	return gSkillTraitValues.bMAAimedPunchCtHModifier * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAChanceToDodgeHtH()
{
	return gSkillTraitValues.ubMAChanceToDodgeHtH * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAOnTopCTDHtHBareHanded()
{
	return gSkillTraitValues.ubMAOnTopCTDHtHBareHanded * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAOnTopCTDHtHBrassKnuckles()
{
	return gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAChanceToDodgeMelee()
{
	return gSkillTraitValues.ubMAChanceToDodgeMelee * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAReducedAPsToSteal()
{
	return gSkillTraitValues.ubMAReducedAPsToSteal * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAAPsChangeStanceReduction()
{
	return gSkillTraitValues.ubMAAPsChangeStanceReduction * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAApsTurnAroundReduction()
{
	return gSkillTraitValues.ubMAApsTurnAroundReduction * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAAPsClimbOrJumpReduction()
{
	return gSkillTraitValues.ubMAAPsClimbOrJumpReduction * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAReducedAPsRegisteredWhenMoving()
{
	return gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT8 SoldierTraits::ubMAChanceToCkickDoors()
{
	return gSkillTraitValues.ubMAChanceToCkickDoors * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

BOOLEAN SoldierTraits::fPermitExtraAnimationsOnlyToMA()
{
	return gSkillTraitValues.fPermitExtraAnimationsOnlyToMA * SoldierNTraitLevel( MARTIAL_ARTS_NT );
}

UINT16 SoldierTraits::usSLRadiusNormal()
{
	return gSkillTraitValues.usSLRadiusNormal * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT16 SoldierTraits::usSLRadiusExtendedEar()
{
	return gSkillTraitValues.usSLRadiusExtendedEar * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLMaxBonuses()
{
	return gSkillTraitValues.ubSLMaxBonuses * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLBonusAPsPercent()
{
	return gSkillTraitValues.ubSLBonusAPsPercent * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLEffectiveLevelInRadius()
{
	return gSkillTraitValues.ubSLEffectiveLevelInRadius * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLEffectiveLevelAsStandby()
{
	return gSkillTraitValues.ubSLEffectiveLevelAsStandby * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLCollectiveInterruptsBonus()
{
	return gSkillTraitValues.ubSLCollectiveInterruptsBonus * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLOverallSuppresionBonusPercent()
{
	return gSkillTraitValues.ubSLOverallSuppresionBonusPercent * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLMoraleGainBonus()
{
	return gSkillTraitValues.ubSLMoraleGainBonus * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLMoraleLossReduction()
{
	return gSkillTraitValues.ubSLMoraleLossReduction * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLFearResistance()
{
	return gSkillTraitValues.ubSLFearResistance * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT8 SoldierTraits::ubSLDeathMoralelossMultiplier()
{
	return gSkillTraitValues.ubSLDeathMoralelossMultiplier * SoldierNTraitLevel( SQUADLEADER_NT );
}

UINT16 SoldierTraits::usTERepairSpeedBonus()
{
	return gSkillTraitValues.usTERepairSpeedBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT16 SoldierTraits::usTELockpickingBonus()
{
	return gSkillTraitValues.usTELockpickingBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT16 SoldierTraits::usTEDisarmElTrapBonus()
{
	return gSkillTraitValues.usTEDisarmElTrapBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT16 SoldierTraits::usTEAttachingItemsBonus()
{
	return gSkillTraitValues.usTEAttachingItemsBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubTEUnjamGunBonus()
{
	return gSkillTraitValues.ubTEUnjamGunBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubTERepairElectronicsPenaltyReduction()
{
	return gSkillTraitValues.ubTERepairElectronicsPenaltyReduction * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubTEChanceToDetectTrapsBonus()
{
	return gSkillTraitValues.ubTEChanceToDetectTrapsBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubTECtHControlledRobotBonus()
{
	return gSkillTraitValues.ubTECtHControlledRobotBonus * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubTETraitsNumToRepairRobot()
{
	return gSkillTraitValues.ubTETraitsNumToRepairRobot * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubTERepairRobotPenaltyReduction()
{
	return gSkillTraitValues.ubTERepairRobotPenaltyReduction * SoldierNTraitLevel( TECHNICIAN_NT );
}

BOOLEAN SoldierTraits::fTETraitsCanRestoreItemThreshold()
{
	return gSkillTraitValues.fTETraitsCanRestoreItemThreshold * SoldierNTraitLevel( TECHNICIAN_NT );
}

UINT8 SoldierTraits::ubDONumberTraitsNeededForSurgery()
{
	return gSkillTraitValues.ubDONumberTraitsNeededForSurgery * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDOSurgeryHealPercentBase()
{
	return gSkillTraitValues.ubDOSurgeryHealPercentBase * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDOSurgeryHealPercentOnTop()
{
	return gSkillTraitValues.ubDOSurgeryHealPercentOnTop * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDOSurgeryHealPercentBloodbag()
{
	return gSkillTraitValues.ubDOSurgeryHealPercentBloodbag * SoldierNTraitLevel( DOCTOR_NT );
}

UINT16 SoldierTraits::usDOSurgeryMedBagConsumption()
{
	return gSkillTraitValues.usDOSurgeryMedBagConsumption * SoldierNTraitLevel( DOCTOR_NT );
}

UINT16 SoldierTraits::usDOSurgeryMaxBreathLoss()
{
	return gSkillTraitValues.usDOSurgeryMaxBreathLoss * SoldierNTraitLevel( DOCTOR_NT );
}

UINT16 SoldierTraits::usDORepairStatsRateBasic()
{
	return gSkillTraitValues.usDORepairStatsRateBasic * SoldierNTraitLevel( DOCTOR_NT );
}

UINT16 SoldierTraits::usDORepairStatsRateOnTop()
{
	return gSkillTraitValues.usDORepairStatsRateOnTop * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDORepStPenaltyIfAlsoHealing()
{
	return gSkillTraitValues.ubDORepStPenaltyIfAlsoHealing * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDOHealingPenaltyIfAlsoStatRepair()
{
	return gSkillTraitValues.ubDOHealingPenaltyIfAlsoStatRepair * SoldierNTraitLevel( DOCTOR_NT );
}

BOOLEAN SoldierTraits::fDORepStShouldThrowMessage()
{
	return gSkillTraitValues.fDORepStShouldThrowMessage * SoldierNTraitLevel( DOCTOR_NT );
}

UINT16 SoldierTraits::usDODoctorAssignmentBonus()
{
	return gSkillTraitValues.usDODoctorAssignmentBonus * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDOBandagingSpeedPercent()
{
	return gSkillTraitValues.ubDOBandagingSpeedPercent * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDONaturalRegenBonus()
{
	return gSkillTraitValues.ubDONaturalRegenBonus * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubDOMaxRegenBonuses()
{
	return gSkillTraitValues.ubDOMaxRegenBonuses * SoldierNTraitLevel( DOCTOR_NT );
}

UINT8 SoldierTraits::ubAMPenaltyDoubleReduction()
{
	return gSkillTraitValues.ubAMPenaltyDoubleReduction * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMReloadSpeedMagazines()
{
	return gSkillTraitValues.ubAMReloadSpeedMagazines * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMReloadSpeedLoose()
{
	return gSkillTraitValues.ubAMReloadSpeedLoose * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMPickItemsAPsReduction()
{
	return gSkillTraitValues.ubAMPickItemsAPsReduction * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMWorkBackpackAPsReduction()
{
	return gSkillTraitValues.ubAMWorkBackpackAPsReduction * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMHandleDoorsAPsReduction()
{
	return gSkillTraitValues.ubAMHandleDoorsAPsReduction * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMHandleBombsAPsReduction()
{
	return gSkillTraitValues.ubAMHandleBombsAPsReduction * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubAMAttachingItemsAPsReduction()
{
	return gSkillTraitValues.ubAMAttachingItemsAPsReduction * SoldierNTraitLevel( AMBIDEXTROUS_NT );
}

UINT8 SoldierTraits::ubMEBladesAPsReduction()
{
	return gSkillTraitValues.ubMEBladesAPsReduction * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMECtHBladesBonus()
{
	return gSkillTraitValues.ubMECtHBladesBonus * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMECtHBluntBonus()
{
	return gSkillTraitValues.ubMECtHBluntBonus * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMEDamageBonusBlades()
{
	return gSkillTraitValues.ubMEDamageBonusBlades * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMEDamageBonusBlunt()
{
	return gSkillTraitValues.ubMEDamageBonusBlunt * SoldierNTraitLevel( MELEE_NT );
}

UINT16 SoldierTraits::usMEAimedMeleeAttackDamageBonus()
{
	return gSkillTraitValues.usMEAimedMeleeAttackDamageBonus * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMEDodgeBladesBonus()
{
	return gSkillTraitValues.ubMEDodgeBladesBonus * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMECtDBladesOnTopWithBladeInHands()
{
	return gSkillTraitValues.ubMECtDBladesOnTopWithBladeInHands * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMEDodgeBluntBonus()
{
	return gSkillTraitValues.ubMEDodgeBluntBonus * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubMECtDBluntOnTopWithBladeInHands()
{
	return gSkillTraitValues.ubMECtDBluntOnTopWithBladeInHands * SoldierNTraitLevel( MELEE_NT );
}

UINT8 SoldierTraits::ubTHBladesAPsReduction()
{
	return gSkillTraitValues.ubTHBladesAPsReduction * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesMaxRange()
{
	return gSkillTraitValues.ubTHBladesMaxRange * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesCtHBonus()
{
	return gSkillTraitValues.ubTHBladesCtHBonus * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesCtHBonusPerClick()
{
	return gSkillTraitValues.ubTHBladesCtHBonusPerClick * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesDamageBonus()
{
	return gSkillTraitValues.ubTHBladesDamageBonus * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesDamageBonusPerClick()
{
	return gSkillTraitValues.ubTHBladesDamageBonusPerClick * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesSilentCriticalHitChance()
{
	return gSkillTraitValues.ubTHBladesSilentCriticalHitChance * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesCriticalHitMultiplierBonus()
{
	return gSkillTraitValues.ubTHBladesCriticalHitMultiplierBonus * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubTHBladesAimClicksAdded()
{
	return gSkillTraitValues.ubTHBladesAimClicksAdded * SoldierNTraitLevel( THROWING_NT );
}

UINT8 SoldierTraits::ubNOeSightRangeBonusInDark()
{
	return gSkillTraitValues.ubNOeSightRangeBonusInDark * SoldierNTraitLevel( NIGHT_OPS_NT );
}

UINT8 SoldierTraits::ubNOHearingRangeBonus()
{
	return gSkillTraitValues.ubNOHearingRangeBonus * SoldierNTraitLevel( NIGHT_OPS_NT );
}

UINT8 SoldierTraits::ubNOHearingRangeBonusInDark()
{
	return gSkillTraitValues.ubNOHearingRangeBonusInDark * SoldierNTraitLevel( NIGHT_OPS_NT );
}

UINT8 SoldierTraits::ubNOIterruptsBonusInDark()
{
	return gSkillTraitValues.ubNOIterruptsBonusInDark * SoldierNTraitLevel( NIGHT_OPS_NT );
}

UINT8 SoldierTraits::ubNONeedForSleepReduction()
{
	return gSkillTraitValues.ubNONeedForSleepReduction * SoldierNTraitLevel( NIGHT_OPS_NT );
}

UINT8 SoldierTraits::ubSTStealthModeSpeedBonus()
{
	return gSkillTraitValues.ubSTStealthModeSpeedBonus * SoldierNTraitLevel( STEALTHY_NT );
}

UINT8 SoldierTraits::ubSTBonusToMoveQuietly()
{
	return gSkillTraitValues.ubSTBonusToMoveQuietly * SoldierNTraitLevel( STEALTHY_NT );
}

UINT8 SoldierTraits::ubSTStealthBonus()
{
	return gSkillTraitValues.ubSTStealthBonus * SoldierNTraitLevel( STEALTHY_NT );
}

UINT8 SoldierTraits::ubSTReducedAPsRegistered()
{
	return gSkillTraitValues.ubSTReducedAPsRegistered * SoldierNTraitLevel( STEALTHY_NT );
}

UINT8 SoldierTraits::ubSTStealthPenaltyForMovingReduction()
{
	return gSkillTraitValues.ubSTStealthPenaltyForMovingReduction * SoldierNTraitLevel( STEALTHY_NT );
}

UINT8 SoldierTraits::ubATAPsMovementReduction()
{
	return gSkillTraitValues.ubATAPsMovementReduction * SoldierNTraitLevel( ATHLETICS_NT );
}

UINT8 SoldierTraits::ubATBPsMovementReduction()
{
	return gSkillTraitValues.ubATBPsMovementReduction * SoldierNTraitLevel( ATHLETICS_NT );
}

UINT8 SoldierTraits::ubBBDamageResistance()
{
	return gSkillTraitValues.ubBBDamageResistance * SoldierNTraitLevel( BODYBUILDING_NT );
}

UINT8 SoldierTraits::ubBBCarryWeightBonus()
{
	return gSkillTraitValues.ubBBCarryWeightBonus * SoldierNTraitLevel( BODYBUILDING_NT );
}

UINT8 SoldierTraits::ubBBBreathLossForHtHImpactReduction()
{
	return gSkillTraitValues.ubBBBreathLossForHtHImpactReduction * SoldierNTraitLevel( BODYBUILDING_NT );
}

UINT16 SoldierTraits::usBBIncreasedNeededDamageToFallDown()
{
	return gSkillTraitValues.usBBIncreasedNeededDamageToFallDown * SoldierNTraitLevel( BODYBUILDING_NT );
}

UINT8 SoldierTraits::ubDEAPsNeededToThrowGrenadesReduction()
{
	return gSkillTraitValues.ubDEAPsNeededToThrowGrenadesReduction * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDEMaxRangeToThrowGrenades()
{
	return gSkillTraitValues.ubDEMaxRangeToThrowGrenades * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDECtHWhenThrowingGrenades()
{
	return gSkillTraitValues.ubDECtHWhenThrowingGrenades * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDEDamageOfBombsAndMines()
{
	return gSkillTraitValues.ubDEDamageOfBombsAndMines * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDEAttachDetonatorCheckBonus()
{
	return gSkillTraitValues.ubDEAttachDetonatorCheckBonus * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDEPlantAndRemoveBombCheckBonus()
{
	return gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDEPlacedBombLevelBonus()
{
	return gSkillTraitValues.ubDEPlacedBombLevelBonus * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubDEShapedChargeDamageMultiplier()
{
	return gSkillTraitValues.ubDEShapedChargeDamageMultiplier * SoldierNTraitLevel( DEMOLITIONS_NT );
}

UINT8 SoldierTraits::ubTGBonusToTrainMilitia()
{
	return gSkillTraitValues.ubTGBonusToTrainMilitia * SoldierNTraitLevel( TEACHING_NT );
}

UINT8 SoldierTraits::ubTGEffectiveLDRToTrainMilitia()
{
	return gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia * SoldierNTraitLevel( TEACHING_NT );
}

UINT8 SoldierTraits::ubTGBonusToTeachOtherMercs()
{
	return gSkillTraitValues.ubTGBonusToTeachOtherMercs * SoldierNTraitLevel( TEACHING_NT );
}

UINT8 SoldierTraits::ubTGEffectiveSkillValueForTeaching()
{
	return gSkillTraitValues.ubTGEffectiveSkillValueForTeaching * SoldierNTraitLevel( TEACHING_NT );
}

UINT8 SoldierTraits::ubTGBonusOnPractising()
{
	return gSkillTraitValues.ubTGBonusOnPractising * SoldierNTraitLevel( TEACHING_NT );
}

UINT8 SoldierTraits::ubSCSightRangebonusWithScopes()
{
	return gSkillTraitValues.ubSCSightRangebonusWithScopes * SoldierNTraitLevel( SCOUTING_NT );
}

UINT16 SoldierTraits::usSCSightRangebonusWithBinoculars()
{
	return gSkillTraitValues.usSCSightRangebonusWithBinoculars * SoldierNTraitLevel( SCOUTING_NT );
}

UINT8 SoldierTraits::ubSCTunnelVisionReducedWithBinoculars()
{
	return gSkillTraitValues.ubSCTunnelVisionReducedWithBinoculars * SoldierNTraitLevel( SCOUTING_NT );
}

BOOLEAN SoldierTraits::fSCCanDetectEnemyPresenseAround()
{
	return gSkillTraitValues.fSCCanDetectEnemyPresenseAround * SoldierNTraitLevel( SCOUTING_NT );
}

BOOLEAN SoldierTraits::fSCCanDetermineEnemyNumbersAround()
{
	return gSkillTraitValues.fSCCanDetermineEnemyNumbersAround * SoldierNTraitLevel( SCOUTING_NT );
}

BOOLEAN SoldierTraits::fSCDetectionInDiagonalSectors()
{
	return gSkillTraitValues.fSCDetectionInDiagonalSectors * SoldierNTraitLevel( SCOUTING_NT );
}

BOOLEAN SoldierTraits::fSCPreventsTheEnemyToAmbushMercs()
{
	return gSkillTraitValues.fSCPreventsTheEnemyToAmbushMercs * SoldierNTraitLevel( SCOUTING_NT );
}

BOOLEAN SoldierTraits::fSCPreventsBloodcatsAmbushes()
{
	return gSkillTraitValues.fSCPreventsBloodcatsAmbushes * SoldierNTraitLevel( SCOUTING_NT );
}

BOOLEAN SoldierTraits::fSCThrowMessageIfAmbushPrevented()
{
	return gSkillTraitValues.fSCThrowMessageIfAmbushPrevented * SoldierNTraitLevel( SCOUTING_NT );
}

INT16 SoldierTraits::sCOMeleeCTHBonus()
{
	return gSkillTraitValues.sCOMeleeCTHBonus * SoldierNTraitLevel( COVERT_NT );
}

INT16 SoldierTraits::sCoMeleeInstakillBonus()
{
	return gSkillTraitValues.sCoMeleeInstakillBonus * SoldierNTraitLevel( COVERT_NT );
}

INT16 SoldierTraits::sCODisguiseAPReduction()
{
	return gSkillTraitValues.sCODisguiseAPReduction * SoldierNTraitLevel( COVERT_NT );
}

INT16 SoldierTraits::sCOCloseDetectionRange()
{
	return gSkillTraitValues.sCOCloseDetectionRange * SoldierNTraitLevel( COVERT_NT );
}

INT16 SoldierTraits::sCOCloseDetectionRangeSoldierCorpse()
{
	return gSkillTraitValues.sCOCloseDetectionRangeSoldierCorpse * SoldierNTraitLevel( COVERT_NT );
}

UINT16 SoldierTraits::usCOEliteUncoverRadius()
{
	return gSkillTraitValues.usCOEliteUncoverRadius * SoldierNTraitLevel( COVERT_NT );
}

BOOLEAN SoldierTraits::fCODetectIfBleeding()
{
	return gSkillTraitValues.fCODetectIfBleeding * SoldierNTraitLevel( COVERT_NT );
}

BOOLEAN SoldierTraits::fCOStripIfUncovered()
{
	return gSkillTraitValues.fCOStripIfUncovered * SoldierNTraitLevel( COVERT_NT );
}

BOOLEAN SoldierTraits::fROAllowArtillery()
{
	return gSkillTraitValues.fROAllowArtillery * SoldierNTraitLevel( RADIO_OPERATOR_NT );
}

BOOLEAN SoldierTraits::fROArtilleryDistributedOverTurns()
{
	return gSkillTraitValues.fROArtilleryDistributedOverTurns * SoldierNTraitLevel( RADIO_OPERATOR_NT );
}

UINT8 SoldierTraits::ubSNTBaseChance()
{
	return gSkillTraitValues.ubSNTBaseChance * SoldierNTraitLevel( SNITCH_NT );
}

BOOLEAN SoldierTraits::fSNTMercOpinionAboutSnitchBonusModifier()
{
	return gSkillTraitValues.fSNTMercOpinionAboutSnitchBonusModifier * SoldierNTraitLevel( SNITCH_NT );
}

BOOLEAN SoldierTraits::fSNTSnitchOpinionAboutMercBonusModifier()
{
	return gSkillTraitValues.fSNTSnitchOpinionAboutMercBonusModifier * SoldierNTraitLevel( SNITCH_NT );
}

BOOLEAN SoldierTraits::fSNTSnitchLeadershipBonusModifer()
{
	return gSkillTraitValues.fSNTSnitchLeadershipBonusModifer * SoldierNTraitLevel( SNITCH_NT );
}

INT8 SoldierTraits::bSNTSociableMercBonus()
{
	return gSkillTraitValues.bSNTSociableMercBonus * SoldierNTraitLevel( SNITCH_NT );
}

INT8 SoldierTraits::bSNTLonerMercBonus()
{
	return gSkillTraitValues.bSNTLonerMercBonus * SoldierNTraitLevel( SNITCH_NT );
}

INT8 SoldierTraits::bSNTSameAssignmentBonus()
{
	return gSkillTraitValues.bSNTSameAssignmentBonus * SoldierNTraitLevel( SNITCH_NT );
}

INT8 SoldierTraits::bSNTMercOpinionAboutMercTreshold()
{
	return gSkillTraitValues.bSNTMercOpinionAboutMercTreshold * SoldierNTraitLevel( SNITCH_NT );
}

UINT8 SoldierTraits::ubSNTPassiveReputationGain()
{
	return gSkillTraitValues.ubSNTPassiveReputationGain * SoldierNTraitLevel( SNITCH_NT );
}

UINT8 SoldierTraits::ubSNTHearingRangeBonus()
{
	return gSkillTraitValues.ubSNTHearingRangeBonus * SoldierNTraitLevel( SNITCH_NT );
}

BOOLEAN SoldierTraits::fSNTPrisonSnitchInterrogationMultiplier()
{
	return gSkillTraitValues.fSNTPrisonSnitchInterrogationMultiplier * SoldierNTraitLevel( SNITCH_NT );
}

BOOLEAN SoldierTraits::fSNTPrisonSnitchGuardStrengthMultiplier()
{
	return gSkillTraitValues.fSNTPrisonSnitchGuardStrengthMultiplier * SoldierNTraitLevel( SNITCH_NT );
}

UINT8 SoldierTraits::ubSVGroupTimeSpentForTravellingFoot()
{
	return gSkillTraitValues.ubSVGroupTimeSpentForTravellingFoot * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT8 SoldierTraits::ubSVGroupTimeSpentForTravellingVehicle()
{
	return gSkillTraitValues.ubSVGroupTimeSpentForTravellingVehicle * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT8 SoldierTraits::ubSVMaxBonusesToTravelSpeed()
{
	return gSkillTraitValues.ubSVMaxBonusesToTravelSpeed * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT8 SoldierTraits::ubSVBreathForTravellingReduction()
{
	return gSkillTraitValues.ubSVBreathForTravellingReduction * SoldierNTraitLevel( SURVIVAL_NT );
}

FLOAT SoldierTraits::dSVWeatherPenaltiesReduction()
{
	return gSkillTraitValues.dSVWeatherPenaltiesReduction * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT8 SoldierTraits::ubSVCamoWornountSpeedReduction()
{
	return gSkillTraitValues.ubSVCamoWornountSpeedReduction * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT16 SoldierTraits::usSVTrackerMaxRange()
{
	return gSkillTraitValues.usSVTrackerMaxRange * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT16 SoldierTraits::usSVTrackerAbility()
{
	return gSkillTraitValues.usSVTrackerAbility * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT16 SoldierTraits::usSVDiseaseResistance()
{
	return gSkillTraitValues.usSVDiseaseResistance * SoldierNTraitLevel( SURVIVAL_NT );
}

INT16 SoldierTraits::sSVFoodConsumption()
{
	return gSkillTraitValues.sSVFoodConsumption * SoldierNTraitLevel( SURVIVAL_NT );
}

INT16 SoldierTraits::sSVDrinkConsumption()
{
	return gSkillTraitValues.sSVDrinkConsumption * SoldierNTraitLevel( SURVIVAL_NT );
}

UINT16 SoldierTraits::usSVSnakeDefense()
{
	return gSkillTraitValues.usSVSnakeDefense * SoldierNTraitLevel( SURVIVAL_NT );
}

INT8 SoldierTraits::StealthLevels()
{
	if ( gGameOptions.fNewTraitSystem )
	{
		return NUM_SKILL_TRAITS( pSoldier, STEALTHY_NT );
	}
	else
	{
		return NUM_SKILL_TRAITS( pSoldier, STEALTHY_OT );
	}
}

INT8 SoldierTraits::HearingBonus()
{
	INT8 bHearing = 0;
	if ( gGameOptions.fNewTraitSystem )
	{
		bHearing += pSoldier->traits.ubNOHearingRangeBonus();
		bHearing += pSoldier->traits.ubSNTHearingRangeBonus();
	}
	else
	{
		if ( HAS_SKILL_TRAIT( pSoldier, NIGHTOPS_OT ) )
			bHearing += 1 * NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT );
	}
	return bHearing;
}

INT8 SoldierTraits::ExtraHearingBonus()
{
	INT8 bHearing = 0;
	// yet another bonus for nighttime
	// old/new traits check - SANDRO
	if ( gGameOptions.fNewTraitSystem )
	{
		if ( HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ) )
			bHearing += gSkillTraitValues.ubNOHearingRangeBonusInDark;
	}
	else
	{
		if ( HAS_SKILL_TRAIT( pSoldier, NIGHTOPS_OT ) )
			bHearing += 1 * NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT );
	}
	return bHearing;
}

// traits, utils

INT8 SoldierTraits::SoldierNTraitLevel( NTrait trait )
{
	if ( !gGameOptions.fNewTraitSystem )
		return 0;

	INT8 bNumberOfTraits = 0;
	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	for ( INT8 bCnt = 0; bCnt < min( MAX_NUM_TRAITS, bMaxTraits ); ++bCnt )
	{
		NTrait otherTrait = (NTrait) pSoldier->stats.ubSkillTraits[bCnt];
		if ( otherTrait == trait )
		{
			++bNumberOfTraits;
		}

		if ( HasMajorNTrait( otherTrait ) )
			++bNumMajorTraitsCounted;

		// if we exceeded the allowed number of major traits, ignore the rest of them
		if ( bNumMajorTraitsCounted > min( NUM_MAJOR_TRAITS, bMaxMajorTraits ) )
			break;
	}

	// cannot have more than one same minor trait
	if ( !HasMajorNTrait( trait ) )
		return ( min( 1, bNumberOfTraits ) );

	return ( min( 2, bNumberOfTraits ) );
}

INT8 SoldierTraits::SoldierOTraitLevel( OTrait trait )
{
	if ( gGameOptions.fNewTraitSystem )
		return 0;

	INT8 bNumberOfTraits = 0;
	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	if ( trait == (OTrait) pSoldier->stats.ubSkillTraits[0] )
	{
		++bNumberOfTraits;
	}

	if ( trait == (OTrait) pSoldier->stats.ubSkillTraits[1] )
	{
		++bNumberOfTraits;
	}

	if ( !HasMajorOTrait( trait ) )
		return ( min( 1, bNumberOfTraits ) );

	return ( bNumberOfTraits );
}

ProfileTraits::ProfileTraits( MERCPROFILESTRUCT* pProfile ) : pProfile( pProfile )
{
}

void ProfileTraits::FillHelpText( CHAR16 sString[] )
{
	CHAR16 sTemp[20];

	if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits check
	{
		UINT8 ubTempSkillArray[MAX_NUM_TRAITS];
		INT8 bNumSkillTraits = 0;

		// lets rearrange our skills to a temp array
		// we also get the number of lines (skills) to be displayed 
		for ( UINT8 ubCnt = 1; ubCnt < NUM_SKILLTRAITS_NT; ubCnt++ )
		{
			if ( pProfile->traits.ProfileNTraitLevel( (NTrait)ubCnt ) == 2 )
			{
				ubTempSkillArray[bNumSkillTraits] = ( ubCnt + NEWTRAIT_MERCSKILL_EXPERTOFFSET );
				bNumSkillTraits++;
			}
			else if ( pProfile->traits.ProfileNTraitLevel( (NTrait)ubCnt ) == 1 )
			{
				ubTempSkillArray[bNumSkillTraits] = ubCnt;
				bNumSkillTraits++;
			}
		}

		if ( bNumSkillTraits == 0 )
		{
			swprintf( sString, L"%s", pPersonnelScreenStrings[PRSNL_TXT_NOSKILLS] );
		}
		else
		{
			for ( UINT8 ubCnt = 0; ubCnt < bNumSkillTraits; ubCnt++ )
			{
				swprintf( sTemp, L"%s\n", gzMercSkillTextNew[ubTempSkillArray[ubCnt]] );
				wcscat( sString, sTemp );
			}
		}
	}
	else
	{
		INT8 bSkill1 = 0, bSkill2 = 0;
		bSkill1 = pProfile->bSkillTraits[0];
		bSkill2 = pProfile->bSkillTraits[1];

		if ( bSkill1 == 0 && bSkill2 == 0 )
		{
			swprintf( sString, L"%s", pPersonnelScreenStrings[PRSNL_TXT_NOSKILLS] );
		}
		else
		{
			//if the 2 skills are the same, add the '(expert)' at the end
			if ( bSkill1 == bSkill2 )
			{
				swprintf( sString, L"%s %s", gzMercSkillText[bSkill1], gzMercSkillText[EXPERT] );
			}
			else
			{
				//Display the first skill
				if ( bSkill1 != 0 )
				{
					swprintf( sString, L"%s\n", gzMercSkillText[bSkill1] );
				}
				if ( bSkill2 != 0 )
				{
					swprintf( sTemp, L"%s", gzMercSkillText[bSkill2] );
					wcscat( sString, sTemp );
				}
			}
		}
	}
}

INT8 ProfileTraits::ProfileNTraitLevel( NTrait trait )
{
	if ( !gGameOptions.fNewTraitSystem )
		return 0;

	INT8 bNumTraits = 0;
	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	for ( INT8 bCnt = 0; bCnt < bMaxTraits; ++bCnt )
	{
		NTrait otherTrait = (NTrait)pProfile->bSkillTraits[bCnt];
		if ( otherTrait == trait )
			++bNumTraits;

		if ( HasMajorNTrait( otherTrait ) )
			++bNumMajorTraitsCounted;

		// if we exceeded the allowed number of major traits, ignore the rest of them
		if ( bNumMajorTraitsCounted > bMaxMajorTraits )
			break;
	}

	if ( HasMajorNTrait( trait ) )
		return ( min( 2, bNumTraits ) );

	return ( min( 1, bNumTraits ) );
}


INT8 ProfileTraits::ProfileOTraitLevel( OTrait trait )
{
	if ( gGameOptions.fNewTraitSystem )
		return 0;

	INT8 bNumTraits = 0;
	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	if ( pProfile->bSkillTraits[0] == trait )
		++bNumTraits;

	if ( pProfile->bSkillTraits[1] == trait )
		++bNumTraits;

	if ( HasMajorOTrait( trait ) )
		return ( min( 1, bNumTraits ) );

	return ( bNumTraits );
}

BOOLEAN HasMajorNTrait( NTrait trait )
{
	if ( trait == NO_SKILLTRAIT_NT )
		return FALSE;

	// traits below DOCTOR_NT are all major
	if ( trait <= DOCTOR_NT )
		return TRUE;

	// covert ops is a major trait that is in a different location
	if ( trait == COVERT_NT )
		return TRUE;

	return FALSE;
}

BOOLEAN HasMajorOTrait( OTrait trait )
{
	if ( trait == NO_SKILLTRAIT_OT )
		return FALSE;

	if ( trait == ELECTRONICS_OT ||
		trait == AMBIDEXT_OT ||
		trait == CAMOUFLAGED_OT )
		return FALSE;
	return TRUE;
}

BOOLEAN TwoStagedOldImpTrait( UINT8 uiSkillTraitNumber )
{
	if ( uiSkillTraitNumber == IMP_SKILL_TRAITS__ELECTRONICS ||
		uiSkillTraitNumber == IMP_SKILL_TRAITS__AMBIDEXTROUS ||
		uiSkillTraitNumber == IMP_SKILL_TRAITS__CAMO )
		return FALSE;
	return TRUE;
}

// deprecated due to magic not allowing static analysis
BOOLEAN HAS_SKILL_TRAIT( SOLDIERTYPE* pSoldier, UINT8 uiSkillTraitNumber )
{
	if ( pSoldier == NULL || uiSkillTraitNumber == NO_SKILLTRAIT_NT )
		return FALSE;

	// Flugente: compatibility with skills
	if ( uiSkillTraitNumber == INTEL || uiSkillTraitNumber == VARIOUSSKILLS )
		return TRUE;

	// check old/new traits
	if ( gGameOptions.fNewTraitSystem )
	{
		return pSoldier->traits.SoldierNTraitLevel( (NTrait)uiSkillTraitNumber ) > 0;
	}
	else
	{
		return pSoldier->traits.SoldierOTraitLevel( (OTrait)uiSkillTraitNumber ) > 0;
	}

	return( FALSE );
}

// deprecated due to magic not allowing static analysis
INT8 NUM_SKILL_TRAITS( SOLDIERTYPE* pSoldier, UINT8 uiSkillTraitNumber )
{
	if ( pSoldier == NULL || uiSkillTraitNumber == NO_SKILLTRAIT_NT )
		return 0;

	// check old/new traits
	if ( gGameOptions.fNewTraitSystem )
	{
		return pSoldier->traits.SoldierNTraitLevel( (NTrait)uiSkillTraitNumber );
	}
	else
	{
		return pSoldier->traits.SoldierOTraitLevel( (OTrait)uiSkillTraitNumber );
	}
}