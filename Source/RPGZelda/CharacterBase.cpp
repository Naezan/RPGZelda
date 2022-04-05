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

	//AI��������
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//AI�� �ν��ϴ� ������Ʈ
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>
		(TEXT("AIPerceptionSource"));
	AIStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());

	//���Ʈ ����
	TakeDownAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDownAudioComponent"));
	TakeDownAudioComponent->bAutoActivate = false;
	TakeDownAudioComponent->SetupAttachment(RootComponent);

	DieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioComponent"));
	DieAudioComponent->bAutoActivate = false;
	DieAudioComponent->SetupAttachment(RootComponent);
	DieAudioComponent->bOverrideAttenuation = true;

	//������쿡 ȣ��Ǵ� �Լ��� ��������Ʈ�� �̸� �־����
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

	//�ǰ� �ִϸ��̼� ���
	if (HitAnim)
		PlayAnimMontage(HitAnim);

	//�ǰݻ��� ���
	if (TakeDownAudioComponent)
		TakeDownAudioComponent->Play();

	UE_LOG(LogTemp, Warning, TEXT("Hitted HP : %f"), CurrentHp);

	if (CurrentHp <= 0.001f)
	{
		if (!IsDead)
		{
			IsDead = true;

			//�״� ���� ȣ��
			OnDeadDelegate.Broadcast();

			//
			UE_LOG(LogTemp, Warning, TEXT("Die"));

			//������Ʈ ���� �̺�Ʈ
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

	//���װ� �������� �Դ� �̺�Ʈ
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
	//���� ����� ȣ��
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