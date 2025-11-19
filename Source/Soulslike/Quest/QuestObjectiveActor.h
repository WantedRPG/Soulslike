// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestObjectiveActor.generated.h"

class USceneComponent;
class UBoxComponent;
class UPrimitiveComponent;
class UWidgetComponent;
class UUserWidget;
class UQuest;
class UQuestObjectiveDefinition;
struct FHitResult;

UENUM()
enum class EQuestObjectiveType : uint8
{
	GetItem UMETA(DisplayName = "GetItem"),
	Arrival UMETA(DisplayName = "Arrival")
};

UCLASS()
class SOULSLIKE_API AQuestObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestObjectiveActor();

	UFUNCTION()
	void StartObjective();

	// 초기화: QuestObjectiveDefinition으로부터 이름, 스폰 트랜스폼 등 멤버 설정
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void InitializeFromDefinition(UQuestObjectiveDefinition* Definition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void FinishObjective();

	UFUNCTION()
	void OnQuestObjectiveStart();

	UFUNCTION()
	void OnQuestObjectiveFinish();

	// Overlap handlers
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 루트 씬 컴포넌트 - 에디터에서 Transform을 편집할 수 있도록 노출
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objective", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootSceneComponent;

	// Trigger box component for detecting overlaps with other actors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objective", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> TriggerBox;

	// 트리거 박스의 기본 Extent를 에디터에서 조정 가능하도록 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FVector TriggerBoxExtent = FVector(100.f, 100.f, 100.f);

	// Widget component to show UI for this objective
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;

	// Owning Quest (이 액터를 스폰한 UQuest 인스턴스)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Objective")
	TObjectPtr<UQuest> OwningQuest;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
	EQuestObjectiveType ObjectiveType;

public:

	// OwningQuest 설정/조회 함수
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void SetOwningQuest(UQuest* InQuest);

	UFUNCTION(BlueprintCallable, Category = "Objective")
	UQuest* GetOwningQuest() const;

	// Objective 이름 (QuestObjectiveDefinition에서 초기화)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FText ObjectiveName;

	// ObjectiveName getter
	UFUNCTION(BlueprintCallable, Category = "Objective")
	FText GetObjectiveName() const;
};
