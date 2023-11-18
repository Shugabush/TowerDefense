// Fill out your copyright notice in the Description page of Project Settings.


#include "PDExit.h"

#include "PDPrisoner.h"
#include "PDPlayer.h"
#include "PDHUD.h"
#include "PDGameOverWidget.h"
#include "PrisonerDefenseGameModeBase.h"
#include "CustomUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APDExit::APDExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &APDExit::OnBeginOverlap);
}

// Called when the game starts or when spawned
void APDExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APDExit::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		// A prisoner made it to the exit, so we lose the game
		APDPlayer* Player = UCustomUtils::GetWorldPlayer(GetWorld(), 0);
		UPDGameOverWidget* GameOverWidget = nullptr;
		Player->GetHUD()->EnableWidget<UPDGameOverWidget>(UPDGameOverWidget::StaticClass(), GameOverWidget, true);
		GameOverWidget->SetResultsText(false);
		GetWorld()->GetWorldSettings()->SetTimeDilation(0);
		Player->GetGameMode()->Loss();
	}
}

