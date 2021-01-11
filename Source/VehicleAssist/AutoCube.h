// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoCube.generated.h"

UCLASS()
class VEHICLEASSIST_API AAutoCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoCube();
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Current Speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeCarSetup")
	float CurrentSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeCarSetup")
	int32 CurrentSpeed_int;

	/* Max speed of CubeCar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeCarSetup")
	int32 MaxSpeed_int;

	/* Distance between two cars*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeCarSetup")
	float Distance = 0.0f;
	
	/* Safe distance length*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeCarSetup")
	float SafeDistance = 200.0f;
	
	/* Linetrace length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeCarSetup")
	float Trace = 250;

	/* Actor found with trace*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CubeCarSetup")
	AActor* ActorTrace;

	/* AutoCube Static Mesh Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeCarSetup")
	class UStaticMeshComponent* AutoCubeMesh;

	/* Functions */
	UFUNCTION(BlueprintCallable)
	float CalcSafeRatio(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void IncreaseSpeed(float Ratio);
	
	UFUNCTION(BlueprintCallable)
	void DecreaseSpeed(float Ratio);
	
	UFUNCTION(BlueprintCallable)
	void SetSpeedKMH();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeCarSetup")
	class UArrowComponent* Arrow;

	float Ratio;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMaxSpeed(float MaxV);
};
