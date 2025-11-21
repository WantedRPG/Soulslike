// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestObjectiveDefinition.h" // moved enum here
#include "QuestObjectiveActor.generated.h"

class USceneComponent;
class UBoxComponent;
class UPrimitiveComponent;
class UWidgetComponent;
class UUserWidget;
class UQuest;
struct FHitResult;

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
	void InitializeFromDefinition(class UQuestObjectiveDefinition* Definition);

	UFUNCTION(BlueprintCallable, Category = "Objective")
	bool IsCompleted() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	// EventManagerSubsystem에서 호출될 핸들러 (AddDynamic에 필요)
	UFUNCTION()
	void HandleOnItemUsed(FName ItemID);

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

	// 이제 타입은 정의 에셋에서 가져오되, 액터에도 복사/개별 설정을 허용
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

	// Objective 완료 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objective")
	bool bIsCompleted = false;

	// GetItem 목표의 경우 요구되는 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FName RequiredItemID;

private:
	// 구독한 Subsystem 포인터(EndPlay 시 안전한 제거용)
	UPROPERTY()
	TObjectPtr<class UEventManagerSubsystem> CachedEventSubsystem;
};
