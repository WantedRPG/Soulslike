// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AttributeSet/SLAttributeSet.h"
#include "MyPlayerController.h"
#include "Inventory/SLInventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Item/SLItemPickupActor.h"
#include "Character/UI/MyWidgetComponent.h"
#include "Character/UI/MyUserWidget.h"

AMyPlayer::AMyPlayer()
{
	// 빙의 시, PlayerState의 ASC와 중복 방지하기 위함.
	//ASC = nullptr;
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<USLAttributeSet>(TEXT("Player_AttributeSet"));
	InventoryComponent = CreateDefaultSubobject<USLInventoryComponent>(TEXT("InventoryComponent"));

	Level = 1;

	//
	HpBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KMS/Asset/UI/WBP_PlayerHpBar.WBP_PlayerHpBar_C'"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMyPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ScanItem();
}

UAbilitySystemComponent* AMyPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AMyPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMyPlayerState* MyPS = GetPlayerState<AMyPlayerState>();

	if (MyPS)
	{
		// ASC = MyPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(MyPS, this);

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		for (const auto& StatEffect : StatEffects) 
		{
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(StatEffect, Level, EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				// 본인에게 Setting하는 GE
				ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			}
		}

		// GameplayAbiltySpec 등록
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		// 입력 바인딩
		SetupGASInputComponent();

		// 디버그 모드
		if (AMyPlayerController* PlayerController = CastChecked<AMyPlayerController>(NewController))
		{
			PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
		}
		
	}
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMyPlayer::MouseLook);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AMyPlayer::ToggleInventory);
	}
}

void AMyPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 1);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 1);

			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 2);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 2);

			EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 3);

			// Equip Toggle
			EnhancedInputComponent->BindAction(GrabWeaponAction, ETriggerEvent::Triggered, this, &AMyPlayer::PickupItem, 4);
			// EnhancedInputComponent->BindAction(GrabWeaponAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 4);

			EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 5);
			// EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 5);

			// Attack
			// ComboAttack
			EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 6);
			EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 6);

			// Acid
			EnhancedInputComponent->BindAction(AcidAttackAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 7);
			EnhancedInputComponent->BindAction(AcidAttackAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 7);

			// Flame
			EnhancedInputComponent->BindAction(FireAttackAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 8);
			EnhancedInputComponent->BindAction(FireAttackAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 8);

			// Electricity
			EnhancedInputComponent->BindAction(ElectricityAttackAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 9);
			EnhancedInputComponent->BindAction(ElectricityAttackAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 9);

			// Impact
			EnhancedInputComponent->BindAction(ImpactAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 10);
			EnhancedInputComponent->BindAction(ImpactAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 10);
		}
	}
}

void AMyPlayer::ScanItem()
{
	UWorld* World = GetWorld();
	if (!World) return;

	//트레이스 시작점과 끝점 계산
	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + GetControlRotation().Vector() * 500.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	//Todo : 줍기 전에는 무기가 ItemType이었다가 주운 후 무기타입으로 변경
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	bool bHit = World->LineTraceSingleByObjectType(
		HitResult,
		StartLocation,
		EndLocation,
		ObjectTypes, 
		Params
	);

	//FColor LineColor = FColor::Red;
	//float DrawDuration = 2.0f;     
	
	// 5. 결과 처리
	if (bHit)
	{
		//LineColor = FColor::Green;
		ItemActor = HitResult.GetActor();

		if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
		{
			PC->ShowItemText();
		}
	}
	else
	{
		ItemActor = nullptr;

		if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
		{
			PC->HiddenItemText();
		}
	}
	/*
	DrawDebugLine(
		World,
		StartLocation,
		EndLocation,
		LineColor,
		false,              
		DrawDuration 
	);
	*/
}

void AMyPlayer::PickupItem(int32 InputId)
{
	if (nullptr == ItemActor || IsDead())
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (nullptr == Spec) return;
	Spec->InputPressed = true;
	if (Spec->IsActive())
	{
		// GA가 활성화 상태면 입력 신호 전달
		ASC->AbilitySpecInputPressed(*Spec);
		return;
	}
	

	//아이템방향으로 회전
	FVector ItemLocation = ItemActor->GetActorLocation();
	ItemLocation.Z = 0;
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = 0;
	FVector Direction = ItemLocation - CurrentLocation;
	SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	
	//아이템 획득
	if (InventoryComponent)
	{
		if (ASLItemPickupActor* NewItem = Cast<ASLItemPickupActor>(ItemActor))
		{
			InventoryComponent->AddItem(NewItem->GetItemID(), NewItem->GetStackCount());
			if (InventoryComponent->OnItemAcquired.IsBound())
				InventoryComponent->OnItemAcquired.Broadcast(NewItem->GetItemID());
			NewItem->Destroy();
		}
	}

	//무기 획득 및 줍기 애니메이션 동작
	// GA가 비활성화 상태면 활성화 시도
	ASC->TryActivateAbility(Spec->Handle);
}

void AMyPlayer::GASInputPressed(int32 InputId)
{
	if (IsDead())
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = true;

		
		if (Spec->IsActive())
		{
			// GA가 활성화 상태면 입력 신호 전달
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			// GA가 비활성화 상태면 활성화 시도
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AMyPlayer::GASInputReleased(int32 InputId)
{
	if (IsDead())
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			// GA에게 입력이 마무리됐다는 신호 전달
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AMyPlayer::Move(const FInputActionValue& Value)
{
	if (IsDead())
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	// 카메라의 수평 고려
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw/*수평 회전*/, 0);

	// Yaw 기준으로 좌표계의 x/y 축 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 축에 맞춰, 입력 전달
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMyPlayer::MouseLook(const FInputActionValue& Value)
{
	if (IsDead())
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 입력 전달
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMyPlayer::ToggleInventory()
{
	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
	{
		PC->ToggleInventory();
	}
}

void AMyPlayer::StopSprint(const FGameplayEffectContextHandle& Context)
{
	FGameplayEventData EventData;
	EventData.Target = this;
	EventData.Instigator = Context.GetInstigator();
	EventData.ContextHandle = Context;
	FGameplayTag StopSprintTag = FGameplayTag::RequestGameplayTag(FName("Character.State.StopSprint"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, StopSprintTag, EventData);
}

void AMyPlayer::KnockBack(const FGameplayEffectContextHandle& Context, float InMagnitude)
{
	// 넉백 이벤트
	FGameplayEventData EventData;
	EventData.Target = this;
	EventData.Instigator = Context.GetInstigator();
	EventData.ContextHandle = Context;

	EventData.EventMagnitude = InMagnitude;

	FGameplayTag HitTag = FGameplayTag::RequestGameplayTag(FName("Character.KnockBack"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitTag, EventData);
}

void AMyPlayer::Death()
{
	if (IsDead())
	{
		return;
	}

	SetPlayerMode(EPlayerState::Dead);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
	}
}
