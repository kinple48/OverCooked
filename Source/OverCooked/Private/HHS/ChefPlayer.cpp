// Fill out your copyright notice in the Description page of Project Settings.

#include "OverCooked/Public/HHS/ChefPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/ExtinguisherActor.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/Cucumber.h"
#include "LJW/Pot.h"
#include "LJW/Rice.h"
#include "LJW/DirtyDish.h"
#include "LJW/Fish.h"
#include "LYW/DishActor.h"

// Sets default values
AChefPlayer::AChefPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/cat/SKM_BlackCat.SKM_BlackCat'"));
	if (MeshTemp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTemp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-88.0f),FRotator(0.0f,0.0f,0.0f));
		GetMesh()->SetRelativeScale3D(FVector(2.5f));
	}
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AChefPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}

		ACameraActor* Camera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
		if (Camera)
		{
			pc->SetViewTargetWithBlend(Camera, 0.5f, VTBlend_Linear);
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
	Direction.Y	= value.Y;
	
	// 8방향 저장
	if (!Direction.IsNearlyZero())
	{
		LastInputDirection = Direction;
	}
}

void AChefPlayer::Dash()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Emerald,TEXT("Dash"),true);
	if (!bCanDash || bIsDashing)
		return;

	bIsDashing = true;
	bCanDash = false;

	FVector DashDirection = GetActorForwardVector();
	LaunchCharacter(DashDirection * 2500.0f, true, true);
	
	// 대시 종료
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AChefPlayer::StopDash, DashDuration, false);
	// 쿨타임 
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AChefPlayer::ResetDash, DashCooldown, false);
}

void AChefPlayer::StopDash()
{
	bIsDashing = false;
	GetCharacterMovement()->StopMovementImmediately();
}	

void AChefPlayer::ResetDash()
{
	bCanDash = true;
}

#pragma endregion



// Called every frame
void AChefPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if ( bIsThrowing && IsHoldingActor() )
	{	
		FVector Offset = FVector(80.0f, 0.0f, 0.0f);
		FVector TargetLocation = GetActorLocation() + GetActorRotation().RotateVector(Offset);
		HoldingActor->SetActorLocation(TargetLocation);
		HoldingActor->SetActorRotation(FRotator::ZeroRotator);

		// 8방향
		if (!Direction.IsNearlyZero())
		{
			FVector Forward = FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal();
			SetActorRotation(Forward.Rotation());
		}
		return; // 이동하지 않음
	}
	if (!bIsUsingExtinguisher)
	{
		AddMovementInput(Direction);
	}
	else
	{
		if (!Direction.IsNearlyZero())
		{
			FVector Forward = FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal();
			SetActorRotation(Forward.Rotation()	);
		}
	}
	Direction = FVector::ZeroVector;
//============================================Chop============================================
	if (bIsChopping && NearBoard )
	{
		if (bCutting)
		{
			float Distance = FVector::Dist(GetActorLocation(), NearBoard->GetActorLocation());
			float MaxChopDistance = 200.f;
	
			if (Distance > MaxChopDistance)
			{
				// 거리 벗어나면 다지기 중단
				bIsChopping = false;
				UnholdKnife();
				UAnimMontage* MontageToStop = ChopMontage;
				if (GetMesh()->GetAnimInstance() && MontageToStop)
				{
					GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, MontageToStop);
				}
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("거리 멀음"));
				return;
			}
			ChopTimer += DeltaTime;
			float ChopDelay = 0.5f; // 0.5초에 한 번 카운트
	
			if (ChopTimer >= ChopDelay)
			{
				ChopTimer = 0.f;
				ChopCount++;
	
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("다지기 횟수: %d / %d"), ChopCount, MaxChopCount));
	
				if (ChopCount >= MaxChopCount)
				{
					bIsChopping = false;
					ChopCount = 0;
					UnholdKnife();
					
					if (HoldingActor)
					{
						HoldingActor->Tags.AddUnique(FName("Chopped"));
					}
					NearBoard = nullptr;
					
					UAnimMontage* MontageToStop = ChopMontage;
					if (GetMesh()->GetAnimInstance() && MontageToStop)
					{
						GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, MontageToStop);
						FTimerHandle GameQuitTimerHandle;
						GetWorld()->GetTimerManager().SetTimer(GameQuitTimerHandle,[this]() {UnholdKnife();},5.f, false);
					}
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("다지기 끝 ^ㅁ^"));

					if (CuttingBoard->fish)
					{
						CuttingBoard->fish->bCooked = true;
						UStaticMesh* Mesh1 = LoadObject<UStaticMesh>( nullptr , TEXT( "/Script/Engine.StaticMesh'/Game/Asset/Fish/SM_Sliced_Fish.SM_Sliced_Fish'" ) );
						CuttingBoard->fish->meshcomp->SetStaticMesh(Mesh1);
						CuttingBoard->fish->boxcomp->SetRelativeScale3D(FVector(0.2f));
						CuttingBoard->fish->boxcomp->SetRelativeRotation(FRotator(30.0f,180.0f,0.0f));
					}
					else if (CuttingBoard->cucumber)
					{
						CuttingBoard->cucumber->bCooked = true;
						UStaticMesh* Mesh2 = LoadObject<UStaticMesh>( nullptr , TEXT( "/Script/Engine.StaticMesh'/Game/HHS/assets/Food/SM_Cucumber_Sliced.SM_Cucumber_Sliced'" ) );
						CuttingBoard->cucumber->meshcomp->SetStaticMesh(Mesh2);
						CuttingBoard->cucumber->meshcomp->SetRelativeScale3D(FVector(1.0f));
					}
					bCutting = false;
				}
			}
		}
	}
//============================================Sink============================================
	if (bIsWashing && NearSink )
	{
		float Distance = FVector::Dist(GetActorLocation(), NearSink->GetActorLocation());
		float MaxWashDistance = 200.f;

		if (Distance > MaxWashDistance)
		{
			// 거리 벗어나면 설거지 중단
			bIsWashing = false;
			UAnimMontage* MontageToStop = WashMontage;
            if (GetMesh()->GetAnimInstance() && MontageToStop)
            {
            	GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, MontageToStop);
            }
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("거리 멀음"));
			return;
		}
		WashTimer += DeltaTime;
		float WashDelay = 0.5f; 

		if (WashTimer >= WashDelay)
		{
			WashTimer = 0.f;
			WashCount++;

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("설거지 횟수: %d / %d"), WashCount, MaxWashCount));

			if (WashCount >= MaxWashCount)
			{
				bIsWashing = false;
				WashCount = 0;
				
				if (HoldingActor)
				{
					HoldingActor->Tags.AddUnique(FName("Washed"));
				}
				
				UAnimMontage* MontageToStop = WashMontage;
				if (GetMesh()->GetAnimInstance() && MontageToStop)
				{
					GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, MontageToStop);
				}
				bSink = false;
				dirtydish->Destroy();
				NearSink->MakeDish();
				NearSink = nullptr;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("설거지 끝 ^ㅁ^"));

			}
		}
	}
}


#pragma region Grab or Drop
void AChefPlayer::GraborDrop()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, TEXT("GraborDrop 호출됨"));
	if (IsHoldingActor())
	//if (HoldingActor)
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
	auto rice = Cast<ARice>(HoldingActor);
	dirtydish = Cast<ADirtyDish>(HoldingActor);
	auto dish = Cast<ADishActor>(HoldingActor);
	auto fish = Cast<AFish>(HoldingActor);
	auto cucumber = Cast<ACucumber>(HoldingActor);
	FVector StartPoint = GetActorLocation() - FVector(0.0f, 0.0f, 50.0f);
	FVector EndPoint = StartPoint + GetActorForwardVector() * 50;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, StartPoint, EndPoint, ECC_Visibility, params);
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Blue, false, 5.f);

	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->Tags.Contains(FName("Snappable")))
	{
		// Snappable 태그가 있는 액터라면
		AActor* HitActor = hitInfo.GetActor();
		CounterTop = Cast<ACounterTop>(HitActor);
		Pot = Cast<APot>(HitActor);
		FoodBox = Cast<AFoodBox>(HitActor);
		CuttingBoard = Cast<ACuttingBoard>(HitActor);
		Sink = Cast<ASink>(HitActor);

		if (CounterTop && CounterTop->SnapPoint && CounterTop->bSnap)
		{
			if (CounterTop->OnDish && rice && !rice->bCooked)
			{
				return;
			}
			HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HoldingActor->SetActorEnableCollision(true);

			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);
				BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				BoxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
				UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
				if (MeshComp)
				{
					MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
					MeshComp->SetVisibility(true);
				}
			}

			// SnapPoint로 스냅
			FVector SnapLocation = CounterTop->SnapPoint->GetComponentLocation();
			HoldingActor->SetActorLocation(SnapLocation);
			HoldingActor->SetActorRotation(FRotator::ZeroRotator); // 회전 초기화
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%s에 스냅"), *HitActor->GetName()));

			FoodBox = Cast<AFoodBox>(HitActor);
			if (FoodBox)
			{
				FoodBox->SnappedActor(HoldingActor);
			}

			if (!dish)
			{
				if (!CounterTop->OnDish)
				{
					CounterTop->bSnap = false;
				}
			}
			else
			{
				CounterTop->OnDish = true;
			}

			HoldingActor = nullptr;
		}
		else if (Pot && Pot->SnapPoint && Pot->bSnap && rice)
		{

			HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HoldingActor->SetActorEnableCollision(true);

			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);
				BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
				if (MeshComp)
				{
					MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
					MeshComp->SetVisibility(true);
				}
			}

			// SnapPoint로 스냅
			FVector SnapLocation = Pot->SnapPoint->GetComponentLocation();
			HoldingActor->SetActorLocation(SnapLocation);
			HoldingActor->SetActorRotation(FRotator::ZeroRotator); // 회전 초기화
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%s에 스냅"), *HitActor->GetName()));

			FoodBox = Cast<AFoodBox>(HitActor);
			if (FoodBox)
			{
				FoodBox->SnappedActor(HoldingActor);
			}



			Pot->bSnap = false;
			HoldingActor = nullptr;
		}
		else if (CuttingBoard && CuttingBoard->SnapPoint && CuttingBoard->bSnap)
		{
			HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HoldingActor->SetActorEnableCollision(true);

			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);
				BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				BoxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
				UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
				if (MeshComp)
				{
					MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
					MeshComp->SetVisibility(true);
				}
			}

			// SnapPoint로 스냅
			FVector SnapLocation = CuttingBoard->SnapPoint->GetComponentLocation();
			HoldingActor->SetActorLocation(SnapLocation);
			HoldingActor->SetActorRotation(FRotator::ZeroRotator); // 회전 초기화
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%s에 스냅"), *HitActor->GetName()));

			FoodBox = Cast<AFoodBox>(HitActor);
			if (FoodBox)
			{
				FoodBox->SnappedActor(HoldingActor);
			}

			if (fish || cucumber )
			{
				bCutting = true;
			}
				
			if (!dish)
			{
				if (!CuttingBoard->OnDish)
				{
					CuttingBoard->bSnap = false;
				}
			}
			else
			{
				CuttingBoard->OnDish = true;
			}

			HoldingActor = nullptr;
		}
		else if (FoodBox && FoodBox->SnapPoint && FoodBox->bSnap)
		{
			HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HoldingActor->SetActorEnableCollision(true);

			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);
				BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				BoxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
				UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
				if (MeshComp)
				{
					MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
					MeshComp->SetVisibility(true);
				}
			}

			// SnapPoint로 스냅
			FVector SnapLocation = FoodBox->SnapPoint->GetComponentLocation();
			HoldingActor->SetActorLocation(SnapLocation);
			HoldingActor->SetActorRotation(FRotator::ZeroRotator); // 회전 초기화
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%s에 스냅"), *HitActor->GetName()));
			FoodBox->SnappedActor(HoldingActor);

			if (!dish)
			{
				if (!FoodBox->OnDish)
				{
					FoodBox->bSnap = false;
				}
			}
			else
			{
				FoodBox->OnDish = true;
			}

			HoldingActor = nullptr;
		}
		else if (Sink && Sink->SnapPoint && Sink->bSnap && dirtydish)
		{
			HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HoldingActor->SetActorEnableCollision(true);

			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);
				BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
				if (MeshComp)
				{
					MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
					MeshComp->SetVisibility(true);
				}
			}

			// SnapPoint로 스냅
			FVector SnapLocation = Sink->SnapPoint->GetComponentLocation();
			HoldingActor->SetActorLocation(SnapLocation);
			HoldingActor->SetActorRotation(FRotator(0.f, 0.f, 60.f)); // 회전 초기화
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%s에 스냅"), *HitActor->GetName()));

			FoodBox = Cast<AFoodBox>(HitActor);
			if (FoodBox)
			{
				FoodBox->SnappedActor(HoldingActor);
			}
			
			Sink->bSnap = false;
			bSink = true;
			HoldingActor = nullptr;
		}

	}

	else
	{
		HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingActor->SetActorEnableCollision(true);

		UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
		if (BoxComp)
		{
			BoxComp->SetSimulatePhysics(false);
			BoxComp->SetEnableGravity(false);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
			if (MeshComp)
			{
				MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
				MeshComp->SetVisibility(true);
			}
		}
		// Snappable가 없으면 물리 적용
		if (BoxComp)
		{
			BoxComp->SetSimulatePhysics(true);
			BoxComp->SetEnableGravity(true);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		HoldingActor = nullptr;
	}
}



void AChefPlayer::GrabObject()
{
	if ( IsHoldingActor() ) return;
	
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 200.f;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
	DrawDebugSphere(GetWorld(), End, GrabRadius, 12, FColor::Green, false, 1.5f);
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit && HitResult.GetActor())
	{
		FoodBox = Cast<AFoodBox>(HitResult.GetActor());
		if (FoodBox)
		{

			// 푸드박스 위에 스냅된 액터가 있는지 확인
			if (FoodBox->SnapActor)
			{
				// 스냅된 액터를 집음
				HoldingActor = FoodBox->SnapActor;
				FoodBox->UnSnappedActor(); // 스냅 해제

				UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
				if (BoxComp)
				{
					BoxComp->SetSimulatePhysics(false);
					BoxComp->SetEnableGravity(false);
					BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					BoxComp->SetCollisionProfileName(TEXT("NoCollision"));

					HoldingActor->SetActorEnableCollision(false);
					HoldingActor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					HoldingActor->SetActorRelativeLocation(FVector(80.f, 0.f, 0.f));
					HoldingActor->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
					HoldingActor->SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));

					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, 
						FString::Printf(TEXT("스냅된 오브젝트 집음: %s"), *HoldingActor->GetName()));
				}
				return;
			}
			else
			{
					ACucumber* Cucumber = FoodBox->MakeCucumber(); 
					if (Cucumber)
					{
						HoldingActor = Cucumber;
						UBoxComponent* BoxComp = Cucumber->FindComponentByClass<UBoxComponent>();
						if (BoxComp)
						{	
							BoxComp->SetSimulatePhysics(false);
							BoxComp->SetEnableGravity(false);
							BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
							BoxComp->SetCollisionProfileName(TEXT("NoCollision"));
	
							Cucumber->SetActorEnableCollision(false);
							Cucumber->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
							Cucumber->SetActorRelativeLocation(FVector(80.f, 0.f, 0.f));
							Cucumber->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
	
							GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("오이 획득"));
						}
					}
				return;
			}
		}
	}
	
	if ( GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(GrabRadius),Params))
	{
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor)
		{
			// 스냅된 액터를 집으면 푸드박스에서 스냅 해제
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("잡은 오브젝트: %s"), *HitActor->GetName()), true);
			
			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);	
				BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				BoxComp->SetCollisionProfileName(TEXT("NoCollision"));
				
				HoldingActor->SetActorEnableCollision(false);
				HoldingActor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				
				HoldingActor->SetActorRelativeLocation(FVector(80.f, 0.f, 0.f));
				HoldingActor->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
				
				HoldingActor->SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f)); 
			}
		}
	}
}

#pragma endregion 


#pragma region Interact
void AChefPlayer::Chop()
{
	if (bCutting)
	{
		if (!bIsChopping && NearBoard )
		{
			// 이미 다진 재료인지 확인
			if (HoldingActor && HoldingActor->Tags.Contains(FName("Chopped")))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("이미 다진 재료"));
				return;
			}
			bIsChopping = true;
			ChopTimer = 0.f;
			holdKnife();
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange,TEXT("다지기"),true);
	
			if (ChopMontage && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(ChopMontage);
			}
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange,TEXT("다지기애니메이션 재생"),true);
		}
		//else if (bIsChopping)
		//{
		//	ChopCount++;
	//
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,FString::Printf(TEXT("다지기 횟수: %d / %d"), ChopCount, MaxChopCount));
	//
		//	if (ChopCount >= MaxChopCount)
		//	{
		//		bIsChopping = false;
		//		ChopCount = 0;
		//		UnholdKnife();
	//
		//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("다지기 끝^^"));
		//	}
		//}
	}
}

void AChefPlayer::Throw()
{
	if (!HoldingActor) return;
	
	UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
	if (BoxComp)
	{
		HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingActor->SetActorEnableCollision(true);
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetEnableGravity(true); //추가
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
		BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		
		// 8방향
		FVector ThrowDirection = FVector(LastInputDirection.X, LastInputDirection.Y, 0.f).GetSafeNormal();
		if (ThrowDirection.IsNearlyZero())
		{
			ThrowDirection = GetActorForwardVector();
		}
		BoxComp->AddImpulse(ThrowDirection * 700.f, NAME_None, true);
	}
	HoldingActor = nullptr;
}

void AChefPlayer::FireExtinguisher()
{
	if (!HoldingActor) return;

	AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor);
	if (Extinguisher)
	{
		bIsUsingExtinguisher = true;
		Extinguisher->ActivateExtinguisher();

		// 8방향
		if (!LastInputDirection.IsNearlyZero())
		{
			FVector LookDir = FVector(LastInputDirection.X, LastInputDirection.Y, 0.f).GetSafeNormal();
			SetActorRotation(LookDir.Rotation());
		}
		
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("소화기 사용"),true);
	}
}

void AChefPlayer::StopExtinguisher()
{
	bIsUsingExtinguisher = false;
	if (HoldingActor)
	{
		AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor);
		if (Extinguisher)
		{
			Extinguisher->DeactivateExtinguisher();
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("소화기 중지"),true);

		}
	}
}

void AChefPlayer::OnInteractPressed()
{
	FVector StartPoint = GetActorLocation()-FVector(0.0f,0.0f,50.0f);
	FVector EndPoint = StartPoint + GetActorForwardVector() * 50;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, StartPoint, EndPoint, ECC_Visibility, params);
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Blue, false, 2.f);
	// 도마 감지
	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->ActorHasTag("CuttingBoard"))
	{
		CuttingBoard = Cast<ACuttingBoard>(hitInfo.GetActor());
		if (CuttingBoard)
		{
			NearBoard = CuttingBoard;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("도마 감지"));
			// 도마 위 칼 가져오기
			Knife = CuttingBoard->KnifeOnBoard;
			if (Knife)
			{
				Knife->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale, FName("KnifeSocket"));
				CuttingBoard->KnifeOnBoard = nullptr;
				
			}
		}
	}
	// 싱크대 감지
	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->ActorHasTag("Sink"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("네"));
		Sink = Cast<ASink>(hitInfo.GetActor());
		if (Sink)
		{
			NearSink = Sink;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("싱크대 감지"));
		}
	}

	if ( NearSink && !IsHoldingActor() )
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("싱크대2222222222222222"));
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


void AChefPlayer::ChopOrThrowOrExtinguish()
{
	if (IsHoldingExtinguisher())	// 1. 소화기 들고 있을 경우 -> 소화기 작동 ( 8방향 Tick )
	{
		FireExtinguisher();
	}
	else if (IsHoldingActor())	// 2. actor 들고 있을 경우 -> 던지기 ( 8방향 )
	{
		Throw();
	}
	else if (NearBoard)		// 3. 아무것도 안 들고 있음 -> 다지기 (애니메이션, 범위확인) / 도마 앞에서만 ( 도마 근처 판단 로직 )
	{
		Chop();
	}
	else if ( NearSink )
	{
		Wash();
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



void AChefPlayer::OnChopCountNotify()
{
//	if (bIsChopping)
//	{
//		ChopCount++;
//		ChopTimer = 0.f;
//
//		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
//			FString::Printf(TEXT("다지기 횟수: %d / %d"), ChopCount, MaxChopCount));
//
//		if (ChopCount >= MaxChopCount)
//		{
//			bIsChopping = false;
//			ChopCount = 0;
//			UnholdKnife();
//
//			if (HoldingActor)
//			{
//				HoldingActor->Tags.AddUnique(FName("Chopped"));
//			}
//			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("다지기 완료"));
//		}
//	}
}

void AChefPlayer::Wash()
{
	if (bSink)
	{
		if (!bIsWashing && NearSink )
		{
			// 이미 설거지 끝낸 접시 인지 확인
			if (HoldingActor && HoldingActor->Tags.Contains(FName("Washed")))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("설거지 끝난 접시"));
				return;
			}
			bIsWashing = true;
			WashTimer = 0.f;
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange,TEXT("설거지"),true);
	
			if (WashMontage && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(WashMontage);
			}
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange,TEXT("설거지 애니메이션 재생"),true);
		}
		//else if (bIsWashing)
		//{
		//	WashCount++;
	//
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,FString::Printf(TEXT("설거지 횟수: %d / %d"), WashCount, MaxWashCount));
	//
		//	if (WashCount >= MaxWashCount)
		//	{
		//		bIsWashing = false;
		//		WashCount = 0;
	//
		//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("설거지 끝^^"));
		//		bSink = false;
		//	}
		//}
	}
}

void AChefPlayer::OnWashCountNotify()
{
	if (bIsWashing)
	{
		WashCount++;
		WashTimer = 0.f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
			FString::Printf(TEXT("설거지 횟수: %d / %d"), WashCount, MaxWashCount));
		if (WashCount >= MaxWashCount)
		{
			bIsWashing = false;
			WashCount = 0;
			if (HoldingActor)
			{
				HoldingActor->Tags.AddUnique(FName("Washed"));
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("설거지 완료"));
		}
	}
}

void AChefPlayer::holdKnife()
{
	if (Knife)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("칼 있음!"));

		Knife->SetActorHiddenInGame(false);
		Knife->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("KnifeSocket"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Knife가 null임!"));
	}
}

void AChefPlayer::UnholdKnife()
{
	if (Knife && NearBoard)
	{
		Knife->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Knife->AttachToComponent(NearBoard->GetRootComponent(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("KnifeLocationSocket"));
		Knife->SetActorLocationAndRotation(Knife->GetActorLocation() + FVector(0, 0, 4), FRotator(0,180,0));
		Knife->SetActorRelativeScale3D(FVector(1,1,1));
		NearBoard->KnifeOnBoard = Knife;
		Knife = nullptr;
	}
}

void AChefPlayer::Death()
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc) pc->DisableInput(pc);
	SetActorEnableCollision(false);
	if (DeathMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
	RespawnLocation = FVector(470, -360, 21);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AChefPlayer::Respawn, 5.0f, false);
}

void AChefPlayer::Respawn()
{
	SetActorLocation(RespawnLocation);
	SetActorEnableCollision(true);
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc) pc->DisableInput(pc);
}
