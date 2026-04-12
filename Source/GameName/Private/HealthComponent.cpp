// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CodeEventComponent.h"
#include "CodeRuleSubsystem.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Health = this->StartHealth;

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHealthComponent::DoDamage(float DamageAmount, AActor* initiator)
{
	if (auto* codeEventComp = Cast<UCodeEventComponent>(GetOwner()->GetComponentByClass(UCodeEventComponent::StaticClass())))
	{
		FCodeEventPayload payload;
		payload.OtherActor  = initiator;
		payload.SourceActor = GetOwner();
		payload.Initiator = GetOwner();
		codeEventComp->RaiseEvent(ECodeEvent::OnHit, payload);
		if (!(codeEventComp->GetCodeSubsystem()->GetMatchingRules(ECodeEvent::OnHit, GetOwner()).Num() > 0))
			SetHealth(FMath::Clamp(Health - DamageAmount, 0, MaxHealth));
	}
	else {
		SetHealth(FMath::Clamp(Health - DamageAmount, 0, MaxHealth));
	}
	return false;
}

void UHealthComponent::Heal(float HealAmount)
{
	SetHealth(FMath::Clamp(Health + HealAmount, 0, MaxHealth));	
	UE_LOG(LogTemp, Warning, TEXT("Healed by: %f \ncurrent health: %f"), HealAmount, Health);

}

void UHealthComponent::SetHealth(float newHealth)
{
	if (newHealth < Health)
	{
		if (OnDamagedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OnDamagedSound, GetOwner()->GetActorLocation());
		}
		if (OnDamageEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnDamageEffect, GetOwner()->GetActorLocation());
		}
	}
	else 
	{
		if (OnHealedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OnHealedSound, GetOwner()->GetActorLocation());
		}
		if (OnHealEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnHealEffect, GetOwner()->GetActorLocation());
		}
	}
	Health = newHealth;
	
	if (Health <= 0)
	{
		Die();
		return;
	}
}

void UHealthComponent::Die()
{
	OnDie.Broadcast();
	if (OnDieSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OnDieSound, GetOwner()->GetActorLocation());
	}
	GetOwner()->Destroy();
}

