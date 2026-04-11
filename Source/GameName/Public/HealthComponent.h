// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GAMENAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	UNiagaraSystem* OnDamageEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	UNiagaraSystem* OnHealEffect;

	// returns did the character die
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool DoDamage(float DamageAmount, AActor* initiator);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float newHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();
};
