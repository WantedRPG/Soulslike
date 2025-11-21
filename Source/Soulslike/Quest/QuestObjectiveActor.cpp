// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestObjectiveActor.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Quest/Quest.h"
#include "Quest/QuestObjectiveDefinition.h"
#include "Management/EventManagerSubsystem.h"
#include "Engine/GameInstance.h"

// Sets default values
AQuestObjectiveActor::AQuestObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트/컴포넌트 초기화
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
	RootSceneComponent->SetMobility(EComponentMobility::Static);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetDrawSize(FVector2D(200.f, 100.f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
	WidgetComponent->SetTwoSided(true);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Quest/UI/WBP_QuestObjectiveMarker.WBP_QuestObjectiveMarker_C"));

	if (WidgetClassFinder.Class)
	{
		WidgetComponent->SetWidgetClass(WidgetClassFinder.Class);
		UE_LOG(LogTemp, Log, TEXT("WidgetClassFinder :  %s"), *WidgetClassFinder.GetReferencerName());
	}
}

// Called when the game starts or when spawned
void AQuestObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	// TriggerBox extent 적용
	if (TriggerBox && !TriggerBoxExtent.IsZero())
	{
		TriggerBox->SetBoxExtent(TriggerBoxExtent);
	}

	// Overlap 바인딩
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AQuestObjectiveActor::OnTriggerBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AQuestObjectiveActor::OnTriggerEndOverlap);
	}

	// Objective 타입이 GetItem이면 EventManagerSubsystem의 OnItemUsed를 구독
	if (ObjectiveType == EQuestObjectiveType::GetItem)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UEventManagerSubsystem* EMS = GI->GetSubsystem<UEventManagerSubsystem>())
				{
					EMS->OnItemUsed.AddDynamic(this, &AQuestObjectiveActor::HandleOnItemUsed);
					CachedEventSubsystem = EMS;
				}
			}
		}
	}
}

void AQuestObjectiveActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 구독 해제
	if (CachedEventSubsystem)
	{
		CachedEventSubsystem->OnItemUsed.RemoveDynamic(this, &AQuestObjectiveActor::HandleOnItemUsed);
		CachedEventSubsystem = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AQuestObjectiveActor::StartObjective()
{
	// Objective 시작 처리
}

void AQuestObjectiveActor::FinishObjective()
{
	// 완료 상태 설정
	bIsCompleted = true;

	// 후처리 콜백
	OnQuestObjectiveFinish();

	if (GetOwningQuest())
	{
		GetOwningQuest()->ProgressQuest();
	}
}

void AQuestObjectiveActor::OnQuestObjectiveStart()
{
}

void AQuestObjectiveActor::OnQuestObjectiveFinish()
{
	// Objective 완료 처리: 위젯 숨김 등
	if (WidgetComponent)
	{
		WidgetComponent->SetHiddenInGame(true);
		WidgetComponent->SetVisibility(false);
	}
	UE_LOG(LogTemp, Log, TEXT("AQuestObjectiveActor::OnQuestObjectiveFinish - %s completed"), *GetName());
}

// Called every frame
void AQuestObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestObjectiveActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("AQuestObjectiveActor::OnTriggerBeginOverlap - %s overlapped with %s"), *GetName(), *OtherActor->GetName());
	}

	FinishObjective();
}

void AQuestObjectiveActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("AQuestObjectiveActor::OnTriggerEndOverlap - %s ended overlap with %s"), *GetName(), *OtherActor->GetName());
	}
}

void AQuestObjectiveActor::SetOwningQuest(UQuest* InQuest)
{
	OwningQuest = InQuest;
	UE_LOG(LogTemp, Verbose, TEXT("AQuestObjectiveActor::SetOwningQuest - %s now owned by quest %s"), *GetName(), OwningQuest ? *OwningQuest->ID.ToString() : TEXT("None"));
}

UQuest* AQuestObjectiveActor::GetOwningQuest() const
{
	return OwningQuest.Get();
}

FText AQuestObjectiveActor::GetObjectiveName() const
{
	return ObjectiveName;
}

void AQuestObjectiveActor::InitializeFromDefinition(UQuestObjectiveDefinition* Definition)
{
	if (!Definition)
		return;

	// 이름/기타 필드 초기화
	ObjectiveName = Definition->ObjectiveName;

	// 초기화 시 완료 상태 초기화
	bIsCompleted = false;

	// 필요 시 QuestObjectiveDefinition에서 RequiredItemID 같은 값을 읽어 초기화하도록 확장 가능
}

bool AQuestObjectiveActor::IsCompleted() const
{
	return bIsCompleted;
}

void AQuestObjectiveActor::HandleOnItemUsed(FName ItemID)
{
	// 이미 완료되었거나 타입이 다르면 무시
	if (bIsCompleted || ObjectiveType != EQuestObjectiveType::GetItem)
		return;

	// RequiredItemID가 비어있으면 모든 아이템 사용으로 완료 처리할지 여부를 결정할 수 있음.
	// 여기서는 RequiredItemID가 비어있으면 아무 아이템으로 완료 처리하지 않음(명시된 아이템과 비교).
	if (RequiredItemID.IsNone())
	{
		UE_LOG(LogTemp, Verbose, TEXT("AQuestObjectiveActor::HandleOnItemUsed - No RequiredItemID set for %s"), *GetName());
		return;
	}

	if (ItemID == RequiredItemID)
	{
		UE_LOG(LogTemp, Log, TEXT("AQuestObjectiveActor::HandleOnItemUsed - Required item used (%s) for %s"), *ItemID.ToString(), *GetName());
		FinishObjective();
	}
}

