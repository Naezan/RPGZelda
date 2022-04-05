// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

#include "AttributeComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//AI소유시점
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//AI가 인식하는 컴포넌트
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>
		(TEXT("AIPerceptionSource"));
	AIStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());

	//사운트 셋팅
	TakeDownAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDownAudioComponent"));
	TakeDownAudioComponent->bAutoActivate = false;
	TakeDownAudioComponent->SetupAttachment(RootComponent);

	DieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioComponent"));
	DieAudioComponent->bAutoActivate = false;
	DieAudioComponent->SetupAttachment(RootComponent);
	DieAudioComponent->bOverrideAttenuation = true;

	//죽을경우에 호출되는 함수를 델리게이트에 미리 넣어놓음
	OnDeadDelegate.AddUObject(this, &ACharacterBase::OnDead);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//AttributeComp->OnHealthChanged.AddDynamic(this, &ACharacterBase::OnHealthChanged);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AIStimuliSource->RegisterWithPerceptionSystem();

	// Sound
	if (TakeDownSoundCue->IsValidLowLevelFast())
		TakeDownAudioComponent->SetSound(TakeDownSoundCue);

	if (DieSoundCue->IsValidLowLevelFast())
		DieAudioComponent->SetSound(DieSoundCue);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHp -= DamageAmount;

	//피격 애니메이션 재생
	if (HitAnim)
		PlayAnimMontage(HitAnim);

	//피격사운드 재생
	if (TakeDownAudioComponent)
		TakeDownAudioComponent->Play();

	UE_LOG(LogTemp, Warning, TEXT("Hitted HP : %f"), CurrentHp);

	if (CurrentHp <= 0.001f)
	{
		if (!IsDead)
		{
			IsDead = true;

			//죽는 사운드 호출
			OnDeadDelegate.Broadcast();

			//
			UE_LOG(LogTemp, Warning, TEXT("Die"));

			//오브젝트 삭제 이벤트
			GetWorld()->GetTimerManager().SetTimer(
				DestoryActorTimer, FTimerDelegate::CreateLambda([&]()
			{
				//this->Destroy();
			}), 7.f, false);

			if (DamageCauser)
			{
				ACharacterBase* CharacterBase = Cast<ACharacterBase>(DamageCauser);
				if (CharacterBase)
				{
					//APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterBase);

					FString KillerName = CharacterBase->CharacterName;
					FString KilledName = CharacterName;

					//if (PlayerCharacter)
					//{
					//	if (PlayerCharacter->GetController()->IsPlayerController())
					//	{
					//		// Sound
					//		TakeDownAudioComponent->Play();
					//	}
					//}
				}
			}
		}

		return 0.f;
	}

	//안죽고 데미지만 입는 이벤트
	if (!GetController()->IsPlayerController() && DamageCauser)
	{
		UAISense_Damage::ReportDamageEvent(
			this, this, EventInstigator, FinalDamage,
			DamageCauser->GetActorLocation(), this->GetActorLocation());
	}

	return FinalDamage;
}

void ACharacterBase::OnDead()
{
	//데스 오디오 호출
	if (DieAudioComponent)
	{
		DieAudioComponent->Play();
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	DisableInput(PC);

	SetLifeSpan(5.0f);
}

void ACharacterBase::Attack()
{
}

void ACharacterBase::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

void ACharacterBase::ResetState()
{
	CurrentHp = MaxHp;
}

void ACharacterBase::SetDefaultHP(float PMaxHp)
{
	this->MaxHp = PMaxHp;
	this->CurrentHp = this->MaxHp;
}

void ACharacterBase::SetCharacterName(FString Name)
{
	CharacterName = Name;
}

bool ACharacterBase::IsAlive()
{
	return !IsDead;
}

void ACharacterBase::RagDollDead()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetSimulatePhysics(true);
	//GetCharacterMovement()->DisableMovement();
}