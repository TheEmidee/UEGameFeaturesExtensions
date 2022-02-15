#pragma once

#include "GFEGameFeatureAction_WorldActionBase.h"

#include <CoreMinimal.h>

#include "GFEGameFeatureAction_SpawnActors.generated.h"

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

UCLASS()
class GAMEFEATURESEXTENSIONS_API UGFEGameFeatureAction_SpawnActors final : public UGFEGameFeatureAction_WorldActionBase
{
    GENERATED_BODY()

public:
    void OnGameFeatureDeactivating( FGameFeatureDeactivatingContext & context ) override;

#if WITH_EDITORONLY_DATA
    void AddAdditionalAssetBundleData( FAssetBundleData & asset_bundle_data ) override;
#endif

#if WITH_EDITOR
    EDataValidationResult IsDataValid( TArray< FText > & validation_errors ) override;
#endif

private:
    void AddToWorld( const FWorldContext & world_context ) override;
    void Reset();

    UPROPERTY( EditAnywhere, Category = "Actor" )
    TArray< FGFESpawningWorldActorsEntry > ActorsList;

    TArray< TWeakObjectPtr< AActor > > SpawnedActors;
};