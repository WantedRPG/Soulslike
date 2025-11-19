// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestObjectiveActor.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Quest/Quest.h"
#include "Quest/QuestObjectiveDefinition.h"

// Sets default values
AQuestObjectiveActor::AQuestObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 씬 컴포넌트 생성 및 설정
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
	RootSceneComponent->SetMobility(EComponentMobility::Static);

	// TriggerBox 생성 및 기본 설정
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// WidgetComponent 생성 및 기본 설정
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

	// 오버랩 이벤트 바인딩
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AQuestObjectiveActor::OnTriggerBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AQuestObjectiveActor::OnTriggerEndOverlap);
	}
}

void AQuestObjectiveActor::StartObjective()
{
	// Objective 시작 처리 (필요 시 확장)
}

void AQuestObjectiveActor::FinishObjective()
{
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
	// Objective 완료 처리
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
		// 필요 시 추가 처리
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

	// 이름/스폰 트랜스폼 등 초기화
	ObjectiveName = Definition->ObjectiveName;
}

