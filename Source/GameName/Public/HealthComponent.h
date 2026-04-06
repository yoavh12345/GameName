// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GAMENAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	float StartHealth = 100; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	USoundBase* OnDamagedSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	USoundBase* OnHealedSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	USoundBase* OnDieSound;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Health")
	float Health;

	// returns did the character die
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool DoDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();
};
