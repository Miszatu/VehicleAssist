// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "SpawnAuto.generated.h"

UCLASS()
class VEHICLEASSIST_API ASpawnAuto : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnAuto();

	virtual void OnConstruction(const FTransform& Transform) override;

	/* declare overlap begin function */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* SplineMesh Array */
	UPROPERTY()
	TArray <class USplineMeshComponent*> SplineMeshArr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** The class to spawn*/
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AAutoCube> WhatToSpawn;

	/** Timer */
	FTimerHandle SpawnTimer;

	/** Minimum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeLow;

	/** Maximum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeHigh;

	/** Specified Max Speed of spawn autos*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MinV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxV;

	/* Box Component Start and End Road */
	class UBoxComponent* BoxStart;
	class UBoxComponent* BoxEnd;

	/* Spline Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Setup")
	class UStaticMesh* Mesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Setup")
	float Divisor = 500.0f;
	
	/* Spline*/
	UPROPERTY(VisibleAnywhere, Category = "Spline Setup")
	class USplineComponent* Spline;
	
	/* SplineMesh variables*/
	float DistanceA;
	float DistanceB;

	/* Scene Components*/
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	class USceneComponent* WhereToSpawn;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	class USceneComponent* EndRoad;

public:	
	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Spawn Autos */
	void SpawnFunction();

private:
	/* Spawn Delay */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float SpawnDelay;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	int32 Speed;
};
