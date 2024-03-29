#pragma once

#include "GFEGameFeatureAction_WorldActionBase.h"

#include <CoreMinimal.h>
#include <GameFeatureAction.h>

#include "GFEGameFeatureAction_SpawnActors.generated.h"

// Taken from the AncientOfTheValley UE5 template.
// Added properties to allow to choose to spawn the actors on the server and / or clients

USTRUCT()
struct FGFESpawningActorEntry
{
    GENERATED_BODY()

    FGFESpawningActorEntry();

    UPROPERTY( EditAnywhere, Category = "Actor" )
    TSubclassOf< AActor > ActorType;

    UPROPERTY( EditAnywhere, Category = "Actor|Transform" )
    FTransform SpawnTransform;

    UPROPERTY( EditAnywhere, Category = "Actor|Spawn" )
    uint8 bSpawnOnServer : 1;

    UPROPERTY( EditAnywhere, Category = "Actor|Spawn" )
    uint8 bSpawnOnClients : 1;
};

USTRUCT()
struct FGFESpawningWorldActorsEntry
{
    GENERATED_BODY()

    UPROPERTY( EditAnywhere, Category = "Feature Data" )
    TSoftObjectPtr< UWorld > TargetWorld;

    UPROPERTY( EditAnywhere, Category = "Feature Data" )
    TArray< FGFESpawningActorEntry > Actors;
};

// Currently works only for spawning actors on the server
UCLASS()
class GAMEFEATURESEXTENSIONS_API UGFEGameFeatureAction_SpawnActors final : public UGFEGameFeatureAction_WorldActionBase
{
    GENERATED_BODY()

public:
    void OnGameFeatureActivating( FGameFeatureActivatingContext & context ) override;
    void OnGameFeatureDeactivating( FGameFeatureDeactivatingContext & context ) override;

#if WITH_EDITORONLY_DATA
    void AddAdditionalAssetBundleData( FAssetBundleData & asset_bundle_data ) override;
#endif

#if WITH_EDITOR
    EDataValidationResult IsDataValid( TArray< FText > & validation_errors ) override;
#endif

private:
    void AddToWorld( const FWorldContext & world_context, const FGameFeatureStateChangeContext & change_context ) override;
    void Reset( TArray< TWeakObjectPtr< AActor > > & actors );

    UPROPERTY( EditAnywhere, Category = "Actor" )
    TArray< FGFESpawningWorldActorsEntry > ActorsList;

    TMap < FGameFeatureStateChangeContext, TArray< TWeakObjectPtr< AActor > > > SpawnedActorsMap;
};
