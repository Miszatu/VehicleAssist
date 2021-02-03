// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoCube.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
void AAutoCube::OnConstruction(const FTransform& Transform)
{
FVector Origin, Extent;
float Radius;
UKismetSystemLibrary::GetComponentBounds(AutoCubeMesh, Origin, Extent, Radius);

/* Sensor position */
Arrow->SetRelativeLocation(FVector(Extent.X, 0.0f, 0.0f));
}
AAutoCube::AAutoCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Static Mesh Component to represent the spawn AutoCube
	AutoCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AutoCube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UStaticMesh* SM_Asset = MeshAsset.Object;
	AutoCubeMesh->SetStaticMesh(SM_Asset);

	/* At the beginning of the object */
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->AttachToComponent(AutoCubeMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AAutoCube::BeginPlay()
{
	Super::BeginPlay();
	CurrentSpeed = 0;
	PrevLocation = GetActorLocation();
}

/* Set MaxSpeed_int during spawn in SpawnAuto Actor*/
void AAutoCube::SetMaxSpeed(float MaxV)
{
	MaxSpeed_int = MaxV;
}

/* Calculate SaveDistance between two actors*/
float AAutoCube::CalcSafeRatio(float Delta)
{
	Ratio = ((Distance - SafeDistance) * Delta) - (CurrentSpeed_int * Delta);
	return Ratio;
}

/*	Increase speed */
void AAutoCube::IncreaseSpeed(float R)
{
	if (CurrentSpeed < MaxSpeed_int)
	{
		CurrentSpeed += R;
		if (CurrentSpeed > MaxSpeed_int)
		{
			CurrentSpeed = MaxSpeed_int;
		}
		CurrentSpeed_int = FMath::Floor(CurrentSpeed);
	}
}

/*	Decrease speed */
void AAutoCube::DecreaseSpeed(float R)
{
	if (CurrentSpeed > 0)
	{
		CurrentSpeed += R;

		if (CurrentSpeed < 0)
		{
			CurrentSpeed = 0;
		}
		CurrentSpeed_int = FMath::Floor(CurrentSpeed);
	}
}

/* Set Speed in KMH */
void AAutoCube::SetSpeedKMH(float DeltaTime)
{
	FVector Location;
	Location = (GetActorLocation() + (GetActorForwardVector() * CurrentSpeed_int) * DeltaTime);
	SetActorLocation(Location);
}

float AAutoCube::CalcCurrentSpeed()
{
	return UKismetMathLibrary::VSize(GetActorLocation() - PrevLocation);
}


// Called every frame
void AAutoCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Velocity = RootComponent->GetComponentVelocity();

	CurrentSpeed = CalcCurrentSpeed() / DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), CurrentSpeed);
	PrevLocation = GetActorLocation();

		
	//if (CurrentSpeed < MaxSpeed_int)
	//{
	//	if (CurrentSpeed_int < MaxSpeed_int)
	//	{
	//		CurrentSpeed_int += 1;
	//		SetSpeedKMH(DeltaTime);
	//	}
	//	else
	//	{
	//		SetSpeedKMH(DeltaTime);
	//	}
	//}
	//if (CurrentSpeed > MaxSpeed_int)
	//{
	//	if (CurrentSpeed_int > MaxSpeed_int)
	//	{
	//		CurrentSpeed_int -= 1;
	//		SetSpeedKMH(DeltaTime);
	//	}
	//	else
	//	{
	//		SetSpeedKMH(DeltaTime);
	//	}
	//}


	Ratio = CalcSafeRatio(DeltaTime);

	/* LineTrace */
	const FVector Start = Arrow->GetComponentLocation();
	const FVector End = ((Arrow->GetForwardVector() * Trace) + Start);
	ActorTrace = nullptr;
	FHitResult HitResult;
	const TArray<AActor*> ActorsToIgnore;
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::White,
		0.1f);
	if (bHit)
	{
//		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("TraceHit: %f, Name: %s, Ratio: %f"), HitResult.Distance, *HitResult.Actor->GetName(), Ratio));
		ActorTrace = HitResult.Actor.Get();
		Distance = HitResult.Distance;
		/* Change Speed */
		if (Ratio > 0)
		{
			IncreaseSpeed(Ratio);
			SetSpeedKMH(DeltaTime);
		}
		else
		{
			DecreaseSpeed(Ratio);
			SetSpeedKMH(DeltaTime);
		}
	}
	/* if bHit is False then Increased Speed to Max Speed value*/
	else
	{
		IncreaseSpeed(1.0f);
		SetSpeedKMH(DeltaTime);
	}
}





