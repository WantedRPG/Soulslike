// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"
#include "QuestDefinition.h"
#include "QuestObjectiveDefinition.h"
#include "QuestObjectiveActor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "QuestComponent.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Quest/QuestSnapshot.h"
#include "Misc/DateTime.h"

UQuest::UQuest()
	: Super()
	, CurrentObjectiveIndex(0)
{
}

bool UQuest::SpawnObjectiveActorsFromDefinition(UQuestDefinition* QuestDef, AActor* OwnerActor)
{
	if (!QuestDef)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuest::SpawnObjectivesFromDefinition - QuestDef is null"));
		return false;
	}

	UWorld* World = OwnerActor ? OwnerActor->GetWorld() : nullptr;
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuest::SpawnObjectivesFromDefinition - World is null"));
		return false;
	}

	// 기존에 있던 Objective 액터들을 정리
	for (TObjectPtr<AQuestObjectiveActor>& Obj : QuestObjectiveActors)
	{
		if (Obj)
		{
			if (AActor* Actor = Cast<AActor>(Obj.Get()))
			{
				Actor->Destroy();
			}
		}
	}
	QuestObjectiveActors.Empty();
	CurrentObjectiveIndex = 0;

	// 사용할 액터 클래스는 QuestDef->ObjectiveActorClass
	TSubclassOf<AQuestObjectiveActor> ObjClass = QuestDef->ObjectiveActorClass;
	if (!ObjClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuest::SpawnObjectivesFromDefinition - ObjectiveActorClass not set on QuestDef %s"), *GetNameSafe(QuestDef));
		return false;
	}

	// 각 ObjectiveDefinition의 SpawnTransform을 사용해 스폰
	for (TObjectPtr<UQuestObjectiveDefinition> ObjDefPtr : QuestDef->ObjectiveDefinitions)
	{
		if (!ObjDefPtr)
			continue;

		const FTransform& SpawnTransform = ObjDefPtr->SpawnTransform;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerActor;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AQuestObjectiveActor* Spawned = World->SpawnActor<AQuestObjectiveActor>(ObjClass, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnParams);
		if (Spawned)
		{
			// Owning Quest 정보 설정
			Spawned->SetOwningQuest(this);

			// Definition으로부터 추가 초기화 (ObjectiveName, SpawnTransform 등)
			Spawned->InitializeFromDefinition(ObjDefPtr);

			QuestObjectiveActors.Add(Spawned);
			UE_LOG(LogTemp, Log, TEXT("UQuest::SpawnObjectivesFromDefinition - Spawned objective actor: %s"), *Spawned->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UQuest::SpawnObjectivesFromDefinition - Failed to spawn objective actor"));
		}
	}

	// 스폰 후 현재 Objective에 해당하는 Trigger만 활성화
	UpdateObjectiveActors();

	return QuestObjectiveActors.Num() > 0;
}

void UQuest::UpdateObjectiveActors()
{
	// 먼저 전체를 안전하게 비활성화 및 Widget 숨김
	for (int32 i = 0; i < QuestObjectiveActors.Num(); ++i)
	{
		AQuestObjectiveActor* ObjActor = QuestObjectiveActors[i].Get();
		if (!ObjActor)
			continue;

		if (ObjActor->TriggerBox)
		{
			ObjActor->TriggerBox->SetGenerateOverlapEvents(false);
			ObjActor->TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// WidgetComponent가 있으면 숨김 처리
		if (ObjActor->WidgetComponent)
		{
			ObjActor->WidgetComponent->SetVisibility(false);
			ObjActor->WidgetComponent->SetHiddenInGame(true);
		}
	}

	// CurrentObjectiveIndex가 유효하면 해당 인덱스의 Trigger만 활성화
	if (QuestObjectiveActors.IsValidIndex(CurrentObjectiveIndex))
	{
		AQuestObjectiveActor* ActiveActor = QuestObjectiveActors[CurrentObjectiveIndex].Get();
		if (ActiveActor && ActiveActor->TriggerBox)
		{
			ActiveActor->TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			ActiveActor->TriggerBox->SetGenerateOverlapEvents(true);
		}

		// 활성 Objective의 Widget만 보이도록 설정
		if (ActiveActor->WidgetComponent)
		{
			ActiveActor->WidgetComponent->SetHiddenInGame(false);
			ActiveActor->WidgetComponent->SetVisibility(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("UQuest::UpdateObjectiveActors - CurrentObjectiveIndex %d out of range"), CurrentObjectiveIndex);
	}

	// Objective 상태가 바뀌었음을 알림 (UI 갱신 등)
	OnQuestProgressUpdated.Broadcast(this);
}

void UQuest::ActivateQuest()
{
	State = EQuestState::Active;
	UpdateObjectiveActors();
}	

void UQuest::ProgressQuest()
{
	// 다음 목표로 이동
	if (QuestObjectiveActors.Num() == 0)
		return;

	// 인덱스 증가 (끝 전까지)
	if (CurrentObjectiveIndex < QuestObjectiveActors.Num() - 1)
	{
		++CurrentObjectiveIndex;
		UpdateObjectiveActors();
	}
	else
	{
		// 마지막 목표를 완료했음 → 퀘스트 완료 처리
		CompleteQuest();
	}
}

void UQuest::CompleteQuest()
{
	// Broadcast 먼저 (UI가 반응하도록)
	State = EQuestState::Completed;
	OnQuestCompletedDelegate.Broadcast(this);

	// Owner가 UQuestComponent인지 확인하고 알림
	if (UQuestComponent* OwnerComp = Cast<UQuestComponent>(GetOuter()))
	{
		OwnerComp->OnQuestCompleted(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuest::CompleteQuest - Outer is not UQuestComponent; cannot notify owner"));
	}
}

void UQuest::CleanupObjectives()
{
	// Owner component와 World 얻기
	UQuestComponent* OwnerComp = Cast<UQuestComponent>(GetOuter());
	UWorld* World = OwnerComp ? OwnerComp->GetWorld() : nullptr;
	FTimerManager* TimerMgr = World ? &World->GetTimerManager() : nullptr;

	// 각 액터에 대해 안전한 비활성화 + 지연 파괴 스케쥴링
	for (TObjectPtr<AQuestObjectiveActor> ObjPtr : QuestObjectiveActors)
	{
		AQuestObjectiveActor* Actor = ObjPtr.Get();
		if (!Actor)
			continue;

		// 물리/충돌 비활성화 및 숨김
		if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
		{
			RootPrim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		Actor->SetActorHiddenInGame(true);

		// 타이머로 지연 파괴
		if (TimerMgr)
		{
			TWeakObjectPtr<AActor> WeakActor = Actor;
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([WeakActor]() {
				if (WeakActor.IsValid())
				{
					WeakActor->Destroy();
				}
			});
			FTimerHandle TmpHandle;
			TimerMgr->SetTimer(TmpHandle, TimerDel, 0.01f, false);
		}
		else
		{
			// World가 없으면 즉시 파괴
			Actor->Destroy();
		}
	}

	// 내부 배열 비우기
	QuestObjectiveActors.Empty();
}

FQuestSnapshot UQuest::CreateSnapshot() const
{
	FQuestSnapshot Snapshot;
	Snapshot.ID = ID;
	// 제목으로 Summary를 사용 (필요 시 다른 필드로 변경)
	Snapshot.Name = Summary;
	Snapshot.Summary = Summary;
	Snapshot.Description = Description;
	Snapshot.CompletedAt = FDateTime::Now();
	Snapshot.State = State;

	// Objective 스냅샷들 수집 (이름 + 완료 여부)
	Snapshot.Objectives.Empty();
	for (const TObjectPtr<AQuestObjectiveActor>& ObjPtr : QuestObjectiveActors)
	{
		if (ObjPtr)
		{
			FQuestObjectiveSnapshot ObjSnap;
			ObjSnap.Name = ObjPtr->GetObjectiveName();
			ObjSnap.bCompleted = ObjPtr->IsCompleted();
			Snapshot.Objectives.Add(ObjSnap);
		}
	}

	return Snapshot;
}