// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayer.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayer::AMyPlayer()
{
	// 빙의 시, PlayerState의 ASC와 중복 방지하기 위함.
	ASC = nullptr;
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
		ASC = MyPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(MyPS/*owner*/, this/*빙의 대상*/);

		// ASC에 GE 적용
		for (const auto& SE : StatEffect) 
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(SE.Key/*GE*/, SE.Value/*Level*/, EffectContextHandle);
			
			if (EffectSpecHandle.IsValid())
			{
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
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
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
	}
}

void AMyPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 0/*input id*/);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 0/*input id*/);

			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 1);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyPlayer::GASInputReleased, 1);

			// 한 번 발동되면, 키를 떼도 취소 불가.
			EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AMyPlayer::GASInputPressed, 2);
		}
	}
}

void AMyPlayer::GASInputPressed(int32 InputId)
{
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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 입력 전달
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
