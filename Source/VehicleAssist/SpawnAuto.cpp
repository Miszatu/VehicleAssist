// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnAuto.h"
#include "Kismet/KismetMathLibrary.h"
#include "AutoCube.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnAuto::ASpawnAuto()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Create objects */
	WhereToSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("WhereToSpawn"));
	BoxStart = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxStart"));
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	EndRoad = CreateDefaultSubobject<USceneComponent>(TEXT("EndRoad"));
	BoxEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxEnd"));

	RootComponent = WhereToSpawn;
	WhereToSpawn->SetWorldLocation(FVector(250.0f, -250.0f, 50.0f));

	Spline->AttachToComponent(WhereToSpawn, FAttachmentTransformRules::KeepRelativeTransform);
	Spline->SetRelativeLocation(FVector(-100.0f, 250.0f, -50.0f));

	/* Set dimensions of two BoxComponent */
	BoxStart->InitBoxExtent(FVector(250.0f, 250.0f, 250.0f));
	BoxEnd->InitBoxExtent(FVector(250.0f, 250.0f, 250.0f));

	/* BoxComponents attach to SceneComponents */
	BoxStart->AttachToComponent(WhereToSpawn, FAttachmentTransformRules::KeepRelativeTransform);
	BoxEnd->AttachToComponent(EndRoad, FAttachmentTransformRules::KeepRelativeTransform);

	/* If Cube Auto Overlap BoxEnd (End of Road) then will be destroyed */
	BoxEnd->OnComponentBeginOverlap.AddDynamic(this, &ASpawnAuto::OnOverlapBegin);

	/* Set SplineMesh Road*/
	Mesh = CreateDefaultSubobject<UStaticMesh>(TEXT("Mesh"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Meshes/SM_Env_Road_Lines_03.SM_Env_Road_Lines_03'"));
	//UStaticMesh* SM_Asset = MeshAsset.Object;
	//Mesh = SM_Asset;

	/* Set the spawn delay range */
	SpawnDelayRangeLow = 10.0f;
	SpawnDelayRangeHigh = 15.0f;

	/* Set default random speed of cars */
	MinV = 20;
	MaxV = 50;
}

/* Implement Spline in Construction Script */
void ASpawnAuto::OnConstruction(const FTransform& Transform)
{
	float Reminder;
	int32 Count = UKismetMathLibrary::FMod(Spline->GetSplineLength(),Divisor, Reminder);
	for (int SplineCount = 0; SplineCount < Count; SplineCount++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this,USplineMeshComponent::StaticClass());
		SplineMeshArr.Add(SplineMeshComponent);
		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DistanceA = Divisor * SplineCount;
		DistanceB = (Divisor * SplineCount) + Divisor;

		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetStartAndEnd(
			FVector(Spline->GetLocationAtDistanceAlongSpline(DistanceA, ESplineCoordinateSpace::Local)),
			FVector(Spline->GetDirectionAtDistanceAlongSpline(DistanceA, ESplineCoordinateSpace::Local)),
			FVector(Spline->GetLocationAtDistanceAlongSpline(DistanceB, ESplineCoordinateSpace::Local)),
			FVector(Spline->GetDirectionAtDistanceAlongSpline(DistanceB, ESplineCoordinateSpace::Local)),
			true
		);
	}
	EndRoad->SetRelativeLocation(FVector(Spline->GetSplineLength(), 0.0f, 0.0f));
}

/* Destroy anything that comes into BoxComponent */
void ASpawnAuto::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		OtherActor->Destroy();	
}

// Called when the game starts or when spawned
void ASpawnAuto::BeginPlay()
{
	Super::BeginPlay();
	/* call first time "SpawnFunction" */
	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnAuto::SpawnFunction, 1.0f, false);
}

// Called every frame
void ASpawnAuto::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnAuto::SpawnFunction()
{
	/* If we have set something to spawn */
	if (WhatToSpawn != NULL)
	{
		/* Check for a valid World: */ 
		UWorld* const World = GetWorld();
		if (World)
		{
			/* Set the spawn parameters */
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			FTransform TransformActor = this->GetActorTransform();
			/* Set speed for spawn autos */
			Speed = UKismetMathLibrary::RandomIntegerInRange(MinV, MaxV);
			AAutoCube* SpawnActor = World->SpawnActor<AAutoCube>(WhatToSpawn, TransformActor, SpawnParams);
			SpawnActor->SetMaxSpeed(Speed);

			/* Calculate new delay and calling "SpawnFunction" */
			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnAuto::SpawnFunction, SpawnDelay, false);
		}
	}
}

