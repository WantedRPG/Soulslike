// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestObjectiveActor.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Quest/Quest.h"

// Sets default values
AQuestObjectiveActor::AQuestObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 씬 컴포넌트 생성 및 설정하여 에디터에서 Transform을 노출
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
	RootSceneComponent->SetMobility(EComponentMobility::Static);

	// TriggerBox 생성 및 루트에 부착
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	// 기본 박스 크기(에디터에서 TriggerBoxExtent로 수정 가능)
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// WidgetComponent 생성 및 루트에 부착
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetDrawSize(FVector2D(200.f, 100.f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
	WidgetComponent->SetTwoSided(true);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Quest/UI/WBP_QuestObjectiveMarker.WBP_QuestObjectiveMarker_C"));

	// If a widget class is set on the CDO, apply it here (will also be changeable in editor/blueprint instances)
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

	// 에디터에서 지정한 Extent가 있으면 적용
	if (TriggerBox && !TriggerBoxExtent.IsZero())
	{
		TriggerBox->SetBoxExtent(TriggerBoxExtent);
	}

	// 오버랩 델리게이트 바인딩
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AQuestObjectiveActor::OnTriggerBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AQuestObjectiveActor::OnTriggerEndOverlap);
	}
}

void AQuestObjectiveActor::StartObjective()
{
	// Objective 시작 시 로직 구현

}

void AQuestObjectiveActor::FinishObjective()
{
	GetOwningQuest()->ProgressQuest();
}

void AQuestObjectiveActor::OnQuestObjectiveStart()
{
}

void AQuestObjectiveActor::OnQuestObjectiveFinish()
{
	// Objective 완료 시 로직 구현
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
		// 필요하면 블루프린트 이벤트 호출 또는 추가 로직 여기에 삽입
	}

	FinishObjective();
}

void AQuestObjectiveActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("AQuestObjectiveActor::OnTriggerEndOverlap - %s ended overlap with %s"), *GetName(), *OtherActor->GetName());
		// 필요하면 블루프린트 이벤트 호출 또는 추가 로직 여기에 삽입
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

