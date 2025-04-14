// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPSPlayerAnimInstance)
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsChopping;


	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
