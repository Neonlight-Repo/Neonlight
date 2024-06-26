// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameActor/Player/Weapon.h"
#include "PlayerCharacter.h"
#include "GreatSword.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WOS_API UGreatSword : public UWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Init(TObjectPtr<APlayerCharacter> PlayerCharacter);
	virtual void OnSwitchedFrom(TObjectPtr<UWeapon> Other);
	virtual void OnSwitchedTo(TObjectPtr<UWeapon> Other);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	virtual void LightAttackHandler(int Axis);
	virtual void HeavyAttackHandler(int Axis);
	virtual void Skill1Handler(int Axis);
	virtual void Skill2Handler(int Axis);

	float GetEnergy();
	bool IsOverflow();
	float GetOverflowRemainingTime();

private:
	UPROPERTY(EditAnywhere, Category = GreatSwordInfo)
		int MaxEnergy;
	UPROPERTY(EditAnywhere, Category = GreatSwordInfo)
		int EnergyRegenSpeed;

	UPROPERTY(EditAnywhere, Category = GreatSwordLightAttack)
		int LightAttackDamage;
	UPROPERTY(EditAnywhere, Category = GreatSwordLightAttack)
		float LightAttackAfterDelay;

	UPROPERTY(EditAnywhere, Category = GreatSwordHeavyAttack)
		int HeavyAttackDamage;
	UPROPERTY(EditAnywhere, Category = GreatSwordHeavyAttack)
		float HeavyAttackAfterDelay;

	UPROPERTY(EditAnywhere, Category = GreatSwordVerticalSmash)
		int VerticalSmashDamage;
	UPROPERTY(EditAnywhere, Category = GreatSwordVerticalSmash)
		int VerticalSmashNeededEnergy;
	UPROPERTY(EditAnywhere, Category = GreatSwordVerticalSmash)
		float VerticalSmashCoolTime;
	UPROPERTY(EditAnywhere, Category = GreatSwordVerticalSmash)
		float VerticalSmashAfterDelay;

	UPROPERTY(EditAnywhere, Category = GreatSwordOverflow)
		int OverflowTime;
	UPROPERTY(EditAnywhere, Category = GreatSwordOverflow)
		int OverflowNeededEnergy;

	void VerticalSmashAnimationLogic();

	float Energy;
	float OverflowTimer;
	
	float LastVerticalSmashTime;
	float LastOverflowTime;
};
