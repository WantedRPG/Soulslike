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
#include "Components/ArrowComponent.h"

AMyPlayer::AMyPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<USLAttributeSet>(TEXT("Player_AttributeSet"));
	Level = 1;

	// 인벤토리
	InventoryComponent = CreateDefaultSubobject<USLInventoryComponent>(TEXT("InventoryComponent"));

	// 체력바
	HpBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("HpBarWidget"));
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

	// 스테미나바
	StaminaBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("StaminaBarWidget"));
	StaminaBar->SetupAttachment(GetMesh());
	StaminaBar->SetRelativeLocation(FVector(0.0f, 0.0f, 170.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> StaminaBarWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KMS/Asset/UI/WBP_PlayerStaminaBar.WBP_PlayerStaminaBar_C'"));
	if (StaminaBarWidgetRef.Class)
	{
		StaminaBar->SetWidgetClass(StaminaBarWidgetRef.Class);
		StaminaBar->SetWidgetSpace(EWidgetSpace::Screen);
		StaminaBar->SetDrawSize(FVector2D(150.0f, 15.f));
		StaminaBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		ASC->InitAbilityActorInfo(MyPS, this);

		// 능력 부여
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& InputAbility : InputAbilities)
		{
			FGameplayAbilitySpec StartSpec(InputAbility.InputGA);
			StartSpec.InputID = InputAbility.InputId;
			ASC->GiveAbility(StartSpec);
		}

		// GE 적용
		FGameplayEffectContextHandle GEContextHandle = ASC->MakeEffectContext();
		GEContextHandle.AddSourceObject(this);

		for (const auto& StatEffect : StatEffects)
		{
			FGameplayEffectSpecHandle GESpecHandle = ASC->MakeOutgoingSpec(StatEffect, Level, GEContextHandle);
			if (GESpecHandle.IsValid())
			{
				// 본인에게 Setting하는 GE
				ASC->BP_ApplyGameplayEffectSpecToSelf(GESpecHandle);
			}
		}

		// 입력 바인딩
		SetupGASInputComponent();
	}
}

#pragma region 입력 바인딩
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
			for (const auto& InputAbility : InputAbilities) 
			{
				EnhancedInputComponent->BindAction(InputAbility.IA, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, InputAbility.InputId);
				EnhancedInputComponent->BindAction(InputAbility.IA, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, InputAbility.InputId);
			}
		}
	}
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
#pragma endregion

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
