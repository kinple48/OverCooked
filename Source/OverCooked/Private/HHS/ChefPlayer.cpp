// Fill out your copyright notice in the Description page of Project Settings.

#include "OverCooked/Public/HHS/ChefPlayer.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/ExtinguisherActor.h"
#include "LJW/Cucumber.h"
#include "LJW/Pot.h"
#include "LJW/Rice.h"
#include "LJW/DirtyDish.h"
#include "LJW/Fish.h"
#include "LYW/DishActor.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "HHS/Knife.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AChefPlayer::AChefPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/cat/SKM_BlackCat.SKM_BlackCat'"));
	if (MeshTemp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTemp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, 0.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(2.5f));
	}
	HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMesh"));
	HatMesh->SetupAttachment(GetMesh(), TEXT("headSocket"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HatAsset(TEXT("/Script/Engine.StaticMesh'/Game/Asset/WhiteHat.WhiteHat'"));  // 메시 경로
	if (HatAsset.Succeeded())
	{
		HatMesh->SetStaticMesh(HatAsset.Object);
		HatMesh->SetRelativeLocation(FVector::ZeroVector);  // 소켓에서의 위치 조정
		HatMesh->SetRelativeRotation(FRotator::ZeroRotator);  // 회전 조정 필요 시
		HatMesh->SetRelativeScale3D(FVector(1.0f));  // 크기 조정
	}

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	GetCharacterMovement()->bEnablePhysicsInteraction = true;
	GetCharacterMovement()->PushForceFactor = 1.0f;
	GetCharacterMovement()->bPushForceUsingZOffset = false;
	GetCharacterMovement()->bPushForceUsingZOffset = true;

	bReplicates = true;
	SetReplicateMovement(true);
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	// RVO 설정
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;

	GetCharacterMovement()->SetIsReplicated(true);
	
	CircleUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("CircleUI"));
	CircleUI->SetupAttachment(RootComponent);
	CircleUI->SetCastShadow(false);

	NetUpdateFrequency = 250.0f;
}

// Called when the game starts or when spawned
void AChefPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AChefPlayer::OnBeginOverlap);
	
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}

	}

	if (UUserWidget* Widget = Cast<UUserWidget>(CircleUI->GetUserWidgetObject()))
	{
		if (IsLocallyControlled())
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


// Called to bind functionality to input
void AChefPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AChefPlayer::Move);
		playerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &AChefPlayer::Dash);
		playerInput->BindAction(IA_GraborDrop, ETriggerEvent::Started, this, &AChefPlayer::GraborDrop);
		playerInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &AChefPlayer::OnInteractPressed);
		playerInput->BindAction(IA_Interact, ETriggerEvent::Completed, this, &AChefPlayer::OnInteractReleased);
	}
}



#pragma region 이동
void AChefPlayer::Move(const struct FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();
	Direction.X = value.X;
	Direction.Y = value.Y;

	if (!Direction.IsNearlyZero())
	{
		LastInputDirection = Direction;
		FVector LookDir = FVector(LastInputDirection.X, LastInputDirection.Y, 0.f).GetSafeNormal();
		Server_SetLookDirection(LookDir);
	}
}

void AChefPlayer::Dash()
{
	if (!HasAuthority())
    {
        ServerRPC_Dash();
        return;
    }

    if (!bCanDash || bIsDashing)
    {
        return;
    }

    bIsDashing = true;
    bCanDash = false;

    FVector DashDirection = GetActorForwardVector();
    LaunchCharacter(DashDirection * 2500.0f, true, true);

    // 대시 종료 타이머
    GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AChefPlayer::StopDash, DashDuration, false);
    // 쿨타임 타이머
    GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AChefPlayer::ResetDash, DashCooldown, false);
}

void AChefPlayer::ServerRPC_Dash_Implementation()
{
	Dash();
}

void AChefPlayer::StopDash()
{
	if (HasAuthority())
    {
        bIsDashing = false;
        GetCharacterMovement()->StopMovementImmediately();
        MulticastRPC_StopDash();
    }
}	

void AChefPlayer::MulticastRPC_StopDash_Implementation()
{
	bIsDashing = false;
    GetCharacterMovement()->StopMovementImmediately();
}

void AChefPlayer::ResetDash()
{
	 if (HasAuthority())
    {
        ServerRPC_ResetDash();
    }
}

void AChefPlayer::ServerRPC_ResetDash_Implementation()
{
	bCanDash = true;
}

#pragma endregion



// Called every frame
void AChefPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		for (TActorIterator<AChefPlayer> It(GetWorld()); It; ++It)
		{
			AChefPlayer* Other = *It;
			if (Other != this && GetCapsuleComponent()->IsOverlappingComponent(Other->GetCapsuleComponent()))
			{
				FVector MyLocation = GetActorLocation();
				FVector OtherLocation = Other->GetActorLocation();
				FVector PushDir = (OtherLocation - MyLocation).GetSafeNormal();

				if (bIsDashing)
				{
					// 내가 밀어내기
					Other->LaunchCharacter(PushDir * 600.0f, true, false);
				}
			}
		}
	}

	
	if (bIsThrowing && IsHoldingActor())
	{
		FVector Offset = FVector(80.0f, 0.0f, 0.0f);
		FVector TargetLocation = GetActorLocation() + GetActorRotation().RotateVector(Offset);
		HoldingActor->SetActorLocation(TargetLocation);
		HoldingActor->SetActorRotation(FRotator::ZeroRotator);
		return;
	}

	if (!bIsUsingExtinguisher)
	{
		AddMovementInput(Direction);
	}
	Direction = FVector::ZeroVector;
	//============================================Chop============================================
	if (bIsChopping && NearBoard)
	{
		if (bCutting)
		{
			float Distance = FVector::Dist(GetActorLocation(), NearBoard->GetActorLocation());
			float MaxChopDistance = 200.f;

			if (Distance > MaxChopDistance)
			{
				Multicast_ChopFinished();
				bIsChopping = false;
			}

			ChopTimer += DeltaTime;
			float ChopDelay = 0.5f;

			if (ChopTimer >= ChopDelay)
			{
				ChopTimer = 0.f;
				ChopCount++;

				if (ChopCount >= MaxChopCount)
				{
					bIsChopping = false;
					bCutting = false;
					ChopCount = 0;

					if (HoldingActor)
					{
						HoldingActor->Tags.AddUnique(FName("Chopped"));
					}

					if (CuttingBoard)
					{
						if (CuttingBoard->fish && CuttingBoard->fish->bCooked == false)
						{
							CuttingBoard->fish->ServerRPC_ChopFish();
						}

						if (CuttingBoard->cucumber && CuttingBoard->cucumber->bCooked == false)
						{
							CuttingBoard->cucumber->ServerRPC_ChopCucumber();
						}
					}

					Multicast_ChopFinished();
					ChopCount = 0;
					return;
				}
			}
		}
	}
	//============================================Sink============================================
	if (bIsWashing && NearSink)
	{
		float Distance = FVector::Dist(GetActorLocation(), NearSink->GetActorLocation());
		float MaxWashDistance = 200.f;

		if (Distance > MaxWashDistance)
		{
			Multicast_WashFinished();
			bIsWashing = false;
			return;
		}

		WashTimer += DeltaTime;
		float WashDelay = 0.5f;

		if (WashTimer >= WashDelay)
		{
			WashTimer = 0.f;
			WashCount++;

			if (WashCount >= MaxWashCount)
			{
				bIsWashing = false;
				WashCount = 0;

				if (HoldingActor)
				{
					HoldingActor->Tags.AddUnique(FName("Washed"));
				}

				if (NearSink && dirtydish)
				{
					dirtydish->Destroy();
					NearSink->MakeDish();
				}

				Multicast_WashFinished();
				NearSink = nullptr;
			}
		}
	}
}


#pragma region Grab or Drop
void AChefPlayer::GraborDrop()
{
	if (IsHoldingActor())
	{
		DropObject();
	}
	else
	{
		GrabObject();
	}
}

void AChefPlayer::DropObject()
{
	if (!HoldingActor) return;

	ServerRPC_Dropobject();

}

void AChefPlayer::GrabObject()
{
	if (IsHoldingActor()) return;

	ServerRPC_GrabObject();
}

#pragma endregion 


#pragma region Interact
void AChefPlayer::Chop()
{
	if (!HasAuthority())
	{
		ServerRPC_Chop();
		return;
	}

	if (!bIsChopping && NearBoard)
	{
		bIsChopping = true;
		ChopTimer = 0.f;
		if (!bCutting && Knife)
		{
			bCutting = true;
			holdKnife();
		}
		MulticastRPC_PlayChopMontage();
	}
}

void AChefPlayer::Throw()
{
	if (!HoldingActor) return;

	FVector ThrowDirection = FVector(LastInputDirection.X, LastInputDirection.Y, 0.f).GetSafeNormal();
	if (ThrowDirection.IsNearlyZero())
	{
		ThrowDirection = GetActorForwardVector();
	}

	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client: Throw - Actor=%s, ThrowDirection=%s"), *HoldingActor->GetName(), *ThrowDirection.ToString());
		UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
		if (BoxComp)
		{
			HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HoldingActor->SetActorEnableCollision(true);
			BoxComp->SetSimulatePhysics(true);
			BoxComp->SetEnableGravity(true);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
			BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			BoxComp->AddImpulse(ThrowDirection * 300.f, NAME_None, true);
		}
	}

	Server_Throw(ThrowDirection);
}

void AChefPlayer::FireExtinguisher()
{
	if (!HoldingActor) return;

	AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor);
	if (Extinguisher)
	{
		Server_FireExtinguisher();
	}
}

void AChefPlayer::StopExtinguisher()
{
	Server_StopExtinguisher();
}

void AChefPlayer::OnInteractPressed()
{
	FVector StartPoint = GetActorLocation() - FVector(0.0f, 0.0f, 50.0f);
	FVector EndPoint = StartPoint + GetActorForwardVector() * 50;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, StartPoint, EndPoint, ECC_Visibility, params);
	//DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Blue, false, 2.f);
	// 도마 감지
	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->ActorHasTag("CuttingBoard"))
	{
		CuttingBoard = Cast<ACuttingBoard>(hitInfo.GetActor());
		if (CuttingBoard)
		{
			NearBoard = CuttingBoard;
			if (!HasAuthority())
			{
				ServerRPC_SetCuttingBoard(CuttingBoard);
				ServerRPC_AttachKnifeFromBoard(CuttingBoard);
			}
			else
			{
				// 서버일 경우 직접 칼 부착
				AKnife* KnifeFromBoard = Cast<AKnife>(CuttingBoard->KnifeOnBoard);
				if (KnifeFromBoard)
				{
					KnifeFromBoard->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("KnifeSocket"));
					Multicast_AttachKnife(KnifeFromBoard); // 클라에 칼 부착 알림
					CuttingBoard->KnifeOnBoard = nullptr;
					Knife = KnifeFromBoard;
					bCutting = true;
				}
			}
		}
	}
	// 싱크대 감지
	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->ActorHasTag("Sink"))
	{
		Sink = Cast<ASink>(hitInfo.GetActor());
		if (Sink)
		{
			NearSink = Sink;
			if (!HasAuthority())
            {
                ServerRPC_SetNearSink(Sink); // 싱크대 동기화 추가
            }
		}
	}

	if (NearSink && !IsHoldingActor())
	{
		Wash();
		return;
	}
	if (IsHoldingExtinguisher())
	{
		FireExtinguisher();
		return;
	}
	if (IsHoldingActor())
	{
		bIsThrowing = true;
		return;
	}
	if (NearBoard)
	{
		Chop();
		return;
	}
	if (!IsHoldingActor())
	{
		GraborDrop();
	}
}

void AChefPlayer::OnInteractReleased()
{
	if (IsHoldingExtinguisher())
	{
		StopExtinguisher();
	}
	else if (bIsThrowing && IsHoldingActor())
	{
		Throw();
		bIsThrowing = false;
	}
}

bool AChefPlayer::IsHoldingExtinguisher() const
{
	return HoldingActor && HoldingActor->ActorHasTag("Extinguisher");
}

bool AChefPlayer::IsHoldingActor() const
{
	return HoldingActor != nullptr;
}

bool AChefPlayer::IsChoppingBoard() const
{
	return true;
}

void AChefPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

}
#pragma endregion
void AChefPlayer::Wash()
{
	if (!HasAuthority())
	{
		ServerRPC_Wash();
		return;
	}

	if (!bIsWashing && NearSink)
	{
		// 이미 설거지 완료된 접시인지 확인
		if (HoldingActor && HoldingActor->Tags.Contains(FName("Washed")))
		{
			return;
		}

		bIsWashing = true;
		WashTimer = 0.f;
		bSink = true;
		MulticastRPC_PlayWashMontage();
	}
}

void AChefPlayer::holdKnife()
{
	if (Knife)
	{
		UE_LOG(LogTemp, Log, TEXT("Knife 장착 - Knife=%s"), *Knife->GetName());
		Knife->SetActorHiddenInGame(false);
		Knife->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("KnifeSocket"));
	}
}

void AChefPlayer::UnholdKnife()
{
	if (Knife && NearBoard)
	{
		Knife->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Knife->AttachToComponent(NearBoard->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("KnifeLocationSocket"));
		Knife->SetActorLocationAndRotation(Knife->GetActorLocation() + FVector(0, 0, 4), FRotator(0, 180, 0));
		Knife->SetActorRelativeScale3D(FVector(1, 1, 1));
		NearBoard->KnifeOnBoard = Knife;
		Knife = nullptr;
	}
}

void AChefPlayer::Death()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// 입력 차단
		DisableInput(PC);
	}

	// 충돌 비활성화
	SetActorEnableCollision(false);

	// 이동 비활성화
	GetCharacterMovement()->DisableMovement(); // 이동 막기
	GetCharacterMovement()->StopMovementImmediately(); // 남아있는 속도 제거

	// 회전 끄기
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 죽음 애니메이션
	if (DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}

	// 리스폰 예약
	RespawnLocation = FVector(535.279487f, -384.511120f, 55.0f);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AChefPlayer::Respawn, 5.0f, false);
}


void AChefPlayer::Respawn()
{
	// 애니메이션 멈춤
	if (DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.2f);
		}
	}

	// 위치 리셋
	SetActorLocation(RespawnLocation);
	SetActorEnableCollision(true);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// 입력 복구
		EnableInput(PC);
	}

	// 이동 복구
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->StopMovementImmediately(); // 속도 초기화
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}



/*
void AChefPlayer::Death()
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		pc->DisableInput(pc);
	}
	SetActorEnableCollision(false);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	if (DeathMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
	RespawnLocation = FVector(535.279487, -384.511120, 55.0);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AChefPlayer::Respawn, 5.0f, false);
}

void AChefPlayer::Respawn()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Stop(0.2f, DeathMontage);
	}
	SetActorLocation(RespawnLocation);
	SetActorEnableCollision(true);
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
		if (pc)
		{
			pc->EnableInput(pc);
			pc->SetIgnoreLookInput(false);
		}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}*/
void AChefPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AChefPlayer* OtherPlayer = Cast<AChefPlayer>(OtherActor);
	if (!OtherPlayer || OtherPlayer == this)
		return;

	FVector PushDir = (OtherPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	if (bIsDashing)
	{
		// 대시 중이면 강하게 밀어냄
		OtherPlayer->LaunchCharacter(PushDir * 600.0f, true, false);
	}
	else
	{
		// 대시 중이 아니어도 부드럽게 밀어냄
		OtherPlayer->AddMovementInput(PushDir, 0.5f);
	}
}

#pragma region 네트워크

void AChefPlayer::ServerRPC_GrabObject_Implementation()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 200.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit && HitResult.GetActor())
	{
		FoodBox = Cast<AFoodBox>(HitResult.GetActor());
		if (FoodBox)
		{
			if (FoodBox->SnapActor)
			{
				HoldingActor = FoodBox->SnapActor;
				FoodBox->UnSnappedActor();

				// 모든 PrimitiveComponent에 대해 충돌 비활성화
				TArray<UPrimitiveComponent*> PrimComps;
				HoldingActor->GetComponents<UPrimitiveComponent>(PrimComps);
				for (UPrimitiveComponent* Comp : PrimComps)
				{
					Comp->SetSimulatePhysics(false);
					Comp->SetEnableGravity(false);
					Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					Comp->SetCollisionProfileName(TEXT("NoCollision"));
				}
				HoldingActor->SetActorEnableCollision(false);

				// 네트워크 동기화 호출 (FoodBox도 전달)
				NetMulticast_GrabObject(HoldingActor, FoodBox);
				return;
			}
			else
			{
				// 네트워크 복제 보장하여 Cucumber 생성
				ACucumber* Cucumber = FoodBox->MakeCucumber();
				if (Cucumber)
				{
					Cucumber->SetReplicates(true); // 반드시 복제 활성화
					HoldingActor = Cucumber;

					TArray<UPrimitiveComponent*> PrimComps;
					Cucumber->GetComponents<UPrimitiveComponent>(PrimComps);
					for (UPrimitiveComponent* Comp : PrimComps)
					{
						Comp->SetSimulatePhysics(false);
						Comp->SetEnableGravity(false);
						Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						Comp->SetCollisionProfileName(TEXT("NoCollision"));
					}
					Cucumber->SetActorEnableCollision(false);

					// 네트워크 동기화 호출 (FoodBox도 전달)
					NetMulticast_GrabObject(HoldingActor, FoodBox);
				}
				return;
			}
		}
	}

	// Sweep로도 못잡았을 때
	if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(GrabRadius), Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// FoodBox 연관관계 해제
			for (AActor* Actor : GetWorld()->GetCurrentLevel()->Actors)
			{
				FoodBox = Cast<AFoodBox>(Actor);
				if (FoodBox && FoodBox->SnapActor == HitActor)
				{
					FoodBox->UnSnappedActor();
					break;
				}
			}

			HoldingActor = HitActor;

			TArray<UPrimitiveComponent*> PrimComps;
			HoldingActor->GetComponents<UPrimitiveComponent>(PrimComps);
			for (UPrimitiveComponent* Comp : PrimComps)
			{
				Comp->SetSimulatePhysics(false);
				Comp->SetEnableGravity(false);
				Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Comp->SetCollisionProfileName(TEXT("NoCollision"));
			}
			HoldingActor->SetActorEnableCollision(false);

			// 네트워크 동기화 호출 (FoodBox도 전달)
			NetMulticast_GrabObject(HoldingActor, FoodBox);
		}
	}
}

void AChefPlayer::NetMulticast_GrabObject_Implementation(AActor* TargetActor, AFoodBox* SourceFoodBox)
{
	if (TargetActor)
	{
		// 모든 PrimitiveComponent에 대해 충돌 비활성화 (클라이언트 동기화)
		TArray<UPrimitiveComponent*> PrimComps;
		TargetActor->GetComponents<UPrimitiveComponent>(PrimComps);
		for (UPrimitiveComponent* Comp : PrimComps)
		{
			Comp->SetSimulatePhysics(false);
			Comp->SetEnableGravity(false);
			Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Comp->SetCollisionProfileName(TEXT("NoCollision"));
		}
		TargetActor->SetActorEnableCollision(false);

		// FoodBox 관련 처리 (필요한 경우)
		if (SourceFoodBox)
		{
			SourceFoodBox->UnSnappedActor(); // 상태 동기화
		}

		TargetActor->AttachToComponent(
			RootComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale
		);
		TargetActor->SetActorRelativeLocation(FVector(80.f, 0.f, 0.f));
		TargetActor->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
		TargetActor->SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	}
}

void AChefPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChefPlayer, HoldingActor);
	DOREPLIFETIME(AChefPlayer, bIsChopping);
	DOREPLIFETIME(AChefPlayer, NearBoard);
	DOREPLIFETIME(AChefPlayer, Knife);
	DOREPLIFETIME(AChefPlayer, bCutting);
	DOREPLIFETIME(AChefPlayer, CuttingBoard);
	DOREPLIFETIME(AChefPlayer, bIsWashing);
	DOREPLIFETIME(AChefPlayer, NearSink);
	DOREPLIFETIME(AChefPlayer, bSink);
	DOREPLIFETIME(AChefPlayer, bIsDashing);
	DOREPLIFETIME(AChefPlayer, bCanDash);
}

void AChefPlayer::ServerRPC_Dropobject_Implementation()
{
	if (!HoldingActor)
	{
		return;
	}

	ARice* LocalRice = Cast<ARice>(HoldingActor);
	dirtydish = Cast<ADirtyDish>(HoldingActor);
	ADishActor* LocalDish = Cast<ADishActor>(HoldingActor);
	AFish* LocalFish = Cast<AFish>(HoldingActor);
	ACucumber* LocalCucumber = Cast<ACucumber>(HoldingActor);

	FVector StartPoint = GetActorLocation() - FVector(0.0f, 0.0f, 50.0f);
	FVector EndPoint = StartPoint + GetActorForwardVector() * 50;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	AActor* SnapTarget = nullptr;
	FVector SnapLocation = FVector::ZeroVector;
	FRotator SnapRotation = FRotator::ZeroRotator;

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, StartPoint, EndPoint, ECC_Visibility, params);

	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->Tags.Contains(FName("Snappable")))
	{
		AActor* HitActor = hitInfo.GetActor();

		ACounterTop* LocalCounterTop = Cast<ACounterTop>(HitActor);
		APot* LocalPot = Cast<APot>(HitActor);
		AFoodBox* LocalFoodBox = Cast<AFoodBox>(HitActor);
		ACuttingBoard* LocalCuttingBoard = Cast<ACuttingBoard>(HitActor);
		ASink* LocalSink = Cast<ASink>(HitActor);

		if (LocalCounterTop && LocalCounterTop->SnapPoint && LocalCounterTop->bSnap)
		{
			if (LocalCounterTop->OnDish && LocalRice && !LocalRice->bCooked) return;
			SnapTarget = HitActor;
			SnapLocation = LocalCounterTop->SnapPoint->GetComponentLocation();
			SnapRotation = FRotator::ZeroRotator;
			if (!LocalDish && !LocalCounterTop->OnDish) LocalCounterTop->bSnap = false;
			if (LocalDish) LocalCounterTop->OnDish = true;
		}
		else if (LocalPot && LocalPot->SnapPoint && LocalPot->bSnap && LocalRice)
		{
			SnapTarget = HitActor;
			SnapLocation = LocalPot->SnapPoint->GetComponentLocation();
			SnapRotation = FRotator::ZeroRotator;
			LocalPot->bSnap = false;
		}
		else if (LocalCuttingBoard && LocalCuttingBoard->SnapPoint && LocalCuttingBoard->bSnap)
		{
			SnapTarget = HitActor;
			SnapLocation = LocalCuttingBoard->SnapPoint->GetComponentLocation();
			SnapRotation = FRotator::ZeroRotator;
			//if (LocalFish || LocalCucumber) bCutting = true;
			//if (!LocalDish && !LocalCuttingBoard->OnDish) LocalCuttingBoard->bSnap = false;
			//if (LocalDish) LocalCuttingBoard->OnDish = true;
			//-------------------------------------------------------------------------------
			if (LocalFish)
			{
				LocalCuttingBoard->fish = LocalFish;
				bCutting = true;
			}
			else if (LocalCucumber)
			{
				LocalCuttingBoard->cucumber = LocalCucumber;
				bCutting = true;
			}
			if (!LocalDish && !LocalCuttingBoard->OnDish) LocalCuttingBoard->bSnap = false;
			if (LocalDish) LocalCuttingBoard->OnDish = true;
			//-------------------------------------------------------------------------------
		}
		else if (LocalFoodBox && LocalFoodBox->SnapPoint && LocalFoodBox->bSnap)
		{
			SnapTarget = HitActor;
			SnapLocation = LocalFoodBox->SnapPoint->GetComponentLocation();
			SnapRotation = FRotator::ZeroRotator;
			if (!LocalDish && !LocalFoodBox->OnDish) LocalFoodBox->bSnap = false;
			if (LocalDish) LocalFoodBox->OnDish = true;
		}
		else if (LocalSink && LocalSink->SnapPoint && LocalSink->bSnap && dirtydish)
		{
			SnapTarget = HitActor;
			SnapLocation = LocalSink->SnapPoint->GetComponentLocation();
			SnapRotation = FRotator(0.f, 0.f, 60.f);
			LocalSink->bSnap = false;
			bSink = true;
		}
	}
	//--------------------------------------------------------------------------------------
	if (SnapTarget)
	{
		FTimerHandle SnapResetTimer;
		AActor* CapturedSnapTarget = SnapTarget;
		GetWorld()->GetTimerManager().SetTimer(SnapResetTimer, [CapturedSnapTarget]()
			{
				if (ACounterTop* CT = Cast<ACounterTop>(CapturedSnapTarget))
				{
					CT->bSnap = true;
				}
				else if (APot* Pot = Cast<APot>(CapturedSnapTarget))
				{
					Pot->bSnap = true;
				}
				else if (AFoodBox* FB = Cast<AFoodBox>(CapturedSnapTarget))
				{
					FB->bSnap = true;
				}
				else if (ACuttingBoard* CB = Cast<ACuttingBoard>(CapturedSnapTarget))
				{
					CB->bSnap = true;
				}
				else if (ASink* Sink = Cast<ASink>(CapturedSnapTarget))
				{
					Sink->bSnap = true;
				}
			}, 0.3f, false);
	}
	//--------------------------------------------------------------------------------------
	if (SnapTarget)
	{
		NetMulticast_Dropobject(HoldingActor, SnapLocation, SnapRotation, SnapTarget);
	}
	else
	{
		NetMulticast_Dropobject(HoldingActor, HoldingActor->GetActorLocation(), HoldingActor->GetActorRotation(), nullptr);
	}

	HoldingActor = nullptr;
}

void AChefPlayer::NetMulticast_Dropobject_Implementation(AActor* DroppedActor, FVector Location, FRotator Rotation, AActor* SnapTarget)
{
	if (!DroppedActor) return;

	DroppedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DroppedActor->SetActorEnableCollision(true);

	UBoxComponent* BoxComp = DroppedActor->FindComponentByClass<UBoxComponent>();
	if (BoxComp)
	{
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetEnableGravity(false);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//추가
		BoxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
		//-----------------------------------------------------------------------------
		BoxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	}

	UStaticMeshComponent* MeshComp = DroppedActor->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComp)
	{
		MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		MeshComp->SetVisibility(true);
	}

	DroppedActor->SetActorLocation(Location);
	DroppedActor->SetActorRotation(Rotation);

	if (SnapTarget)
	{
		AFoodBox* LocalFoodBox = Cast<AFoodBox>(SnapTarget);
		if (LocalFoodBox)
		{
			LocalFoodBox->SnappedActor(DroppedActor);
		}
	}
	else
	{
		if (BoxComp)
		{
			BoxComp->SetSimulatePhysics(true);
			BoxComp->SetEnableGravity(true);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//-----------------------------------------------------------------
			BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
			//BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}
	}
}

void AChefPlayer::Server_FireExtinguisher_Implementation()
{
	NetMulticast_ActivateExtinguisher();
}

void AChefPlayer::Server_StopExtinguisher_Implementation()
{
	NetMulticast_DeactivateExtinguisher();
}

void AChefPlayer::NetMulticast_ActivateExtinguisher_Implementation()
{
	bIsUsingExtinguisher = true;

	if (AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor))
	{
		Extinguisher->ActivateExtinguisher();
	}
}

void AChefPlayer::NetMulticast_DeactivateExtinguisher_Implementation()
{
	bIsUsingExtinguisher = false;

	if (AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor))
	{
		Extinguisher->DeactivateExtinguisher();
	}
}

void AChefPlayer::Server_SetLookDirection_Implementation(const FVector& LookDirection)
{
	if (!LookDirection.IsNearlyZero())
	{
		FRotator NewRotation = LookDirection.Rotation();
		SetActorRotation(NewRotation);

		AController* MyController = GetController();
		if (MyController)
		{
			MyController->SetControlRotation(NewRotation);
		}
		NetMulticast_SetLookDirection(NewRotation);
	}
}

void AChefPlayer::NetMulticast_SetLookDirection_Implementation(const FRotator& Rotation)
{
	SetActorRotation(Rotation);
	AController* MyController = GetController();
	if (MyController)
	{
		MyController->SetControlRotation(Rotation);
	}
}

void AChefPlayer::Server_Throw_Implementation(const FVector& ThrowDirection)
{
	if (!HoldingActor) return;

	UE_LOG(LogTemp, Warning, TEXT("Server: Throw - Actor=%s, ThrowDirection=%s"), *HoldingActor->GetName(), *ThrowDirection.ToString());

	// 서버에서 던지기 처리
	UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
	if (BoxComp)
	{
		HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingActor->SetActorEnableCollision(true);
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetEnableGravity(true);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
		BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		BoxComp->AddImpulse(ThrowDirection * 300.f, NAME_None, true);
	}

	// HoldingActor 복제 변수 업데이트
	AActor* ThrownActor = HoldingActor;
	HoldingActor = nullptr;

	// 모든 클라이언트에 동기화
	NetMulticast_Throw(ThrownActor, ThrowDirection);
}

void AChefPlayer::NetMulticast_Throw_Implementation(AActor* ThrownActor, const FVector& ThrowDirection)
{
	if (!ThrownActor) return;

	UE_LOG(LogTemp, Warning, TEXT("NetMulticast: Throw - Actor=%s, ThrowDirection=%s, IsLocallyControlled=%d"),
		*ThrownActor->GetName(), *ThrowDirection.ToString(), IsLocallyControlled());

	// 로컬 클라이언트가 이미 예측한 경우 중복 처리 방지
	if (!IsLocallyControlled())
	{
		UBoxComponent* BoxComp = ThrownActor->FindComponentByClass<UBoxComponent>();
		if (BoxComp)
		{
			ThrownActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ThrownActor->SetActorEnableCollision(true);
			BoxComp->SetSimulatePhysics(true);
			BoxComp->SetEnableGravity(true);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
			BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			BoxComp->AddImpulse(ThrowDirection * 300.f, NAME_None, true);
		}
	}
}

void AChefPlayer::ServerRPC_Chop_Implementation()
{
	Chop();
}

void AChefPlayer::MulticastRPC_PlayChopMontage_Implementation()
{
	if (ChopMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ChopMontage);
		UGameplayStatics::PlaySound2D(GetWorld(), ChopSound);
	}
}

void AChefPlayer::ServerRPC_AttachKnifeFromBoard_Implementation(ACuttingBoard* Board)
{
	if (!Board) return;

	AKnife* KnifeFromBoard = Cast<AKnife>(Board->KnifeOnBoard);
	if (KnifeFromBoard)
	{
		KnifeFromBoard->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("KnifeLocationSocket"));
		Knife = KnifeFromBoard;
		Board->KnifeOnBoard = nullptr;
		NearBoard = Board;
		bCutting = true;
		Multicast_AttachKnife(KnifeFromBoard);
	}
}

void AChefPlayer::Multicast_AttachKnife_Implementation(AKnife* KnifeToAttach)
{
	if (!KnifeToAttach || !GetMesh()) return;

	KnifeToAttach->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("KnifeSocket"));
}

void AChefPlayer::Multicast_ChopFinished_Implementation()
{
	bIsChopping = false;
	bCutting = false;

	if (Knife && NearBoard)
	{
		Knife->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Knife->AttachToComponent(NearBoard->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("KnifeLocationSocket"));
		Knife->SetActorLocationAndRotation(Knife->GetActorLocation() + FVector(0, 0, 4), FRotator(0, 180, 0));
		Knife->SetActorRelativeScale3D(FVector(1, 1, 1));
		NearBoard->KnifeOnBoard = Knife;
		Knife = nullptr;
	}

	if (ChopMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, ChopMontage);
	}
}	

void AChefPlayer::ServerRPC_SetCuttingBoard_Implementation(ACuttingBoard* Board)
{
	if (Board)
	{
		CuttingBoard = Board;
		NearBoard = Board;
		ForceNetUpdate(); // 즉시 복제
	}
}

void AChefPlayer::ServerRPC_SetNearSink_Implementation(ASink* DetectedSink)
{
	if (DetectedSink)
    {
        NearSink = DetectedSink;
        ForceNetUpdate(); // 즉시 복제
    }
}

void AChefPlayer::ServerRPC_Wash_Implementation()
{
	Wash();
}

void AChefPlayer::MulticastRPC_PlayWashMontage_Implementation()
{
	if (WashMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(WashMontage);
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), MySoundCue, 1.0f, 1.0f, 0.0f, nullptr, true);
		if (AudioComp)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [AudioComp]()
			{
				if (AudioComp) AudioComp->Stop();
			}, 2.2f, false);
		}
	}
}

void AChefPlayer::Multicast_WashFinished_Implementation()
{
	bIsWashing = false;
	bSink = false;

	if (WashMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, WashMontage);
	}
}

#pragma endregion 