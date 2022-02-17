#include "GFEGameFeatureAction_SpawnActors.h"

#include "DVEDataValidator.h"

#include <Engine/AssetManager.h>
#include <GameFeaturesSubsystemSettings.h>
#include <GameFramework/GameModeBase.h>
#include <Kismet/KismetSystemLibrary.h>

FGFESpawningActorEntry::FGFESpawningActorEntry() :
    bSpawnOnServer( true ),
    bSpawnOnClients( false )
{
}

void UGFEGameFeatureAction_SpawnActors::OnGameFeatureActivating()
{
    Super::OnGameFeatureActivating();

    GameModeInitializedEventDelegateHandle = FGameModeEvents::GameModeInitializedEvent.AddUObject( this, &ThisClass::OnGameModeInitialized );
}

void UGFEGameFeatureAction_SpawnActors::OnGameFeatureDeactivating( FGameFeatureDeactivatingContext & context )
{
    Super::OnGameFeatureDeactivating( context );

    Reset();
}

#if WITH_EDITORONLY_DATA
void UGFEGameFeatureAction_SpawnActors::AddAdditionalAssetBundleData( FAssetBundleData & asset_bundle_data )
{
    if ( UAssetManager::IsValid() )
    {
        for ( const auto & entry : ActorsList )
        {
            for ( const auto & actor_entry : entry.Actors )
            {
                if ( actor_entry.bSpawnOnClients )
                {
                    asset_bundle_data.AddBundleAsset( UGameFeaturesSubsystemSettings::LoadStateClient, actor_entry.ActorType->GetPathName() );
                }

                if ( actor_entry.bSpawnOnServer )
                {
                    asset_bundle_data.AddBundleAsset( UGameFeaturesSubsystemSettings::LoadStateServer, actor_entry.ActorType->GetPathName() );
                }
            }
        }
    }
}
#endif

#if WITH_EDITOR
EDataValidationResult UGFEGameFeatureAction_SpawnActors::IsDataValid( TArray< FText > & validation_errors )
{
    return FDVEDataValidator( validation_errors )
        .CustomValidation< TArray< FGFESpawningWorldActorsEntry > >( ActorsList, []( TArray< FText > & errors, TArray< FGFESpawningWorldActorsEntry > actors_list ) {
            int32 entry_index = 0;
            for ( const auto & Entry : actors_list )
            {
                int32 actor_index = 0;
                for ( const auto & ActorEntry : Entry.Actors )
                {
                    if ( ActorEntry.ActorType == nullptr )
                    {
                        errors.Emplace( FText::FromString( FString::Printf( TEXT( "Null ActorType for actor #%i at index %i in ActorsList." ), actor_index, entry_index ) ) );
                    }
                    ++actor_index;
                }
                ++entry_index;
            }
        } )
        .Result();
}

#endif

void UGFEGameFeatureAction_SpawnActors::OnGameModeInitialized( AGameModeBase * game_mode )
{
    UWorld * world = game_mode->GetWorld();
    const auto is_standalone = UKismetSystemLibrary::IsStandalone( world );
    auto is_server = IsRunningDedicatedServer();

#if WITH_EDITOR
    checkSlow( GameInstance->GetWorldContext() );
    is_server |= world->GetGameInstance()->GetWorldContext()->RunAsDedicated;
#endif

    const auto is_client = !is_server;

    for ( const auto & entry : ActorsList )
    {
        if ( !entry.TargetWorld.IsNull() )
        {
            UWorld * target_world = entry.TargetWorld.Get();
            if ( target_world != world )
            {
                continue;
            }
        }

        for ( const auto & actor_entry : entry.Actors )
        {
            const bool should_spawn_actor = is_standalone || is_server && actor_entry.bSpawnOnServer || is_client && actor_entry.bSpawnOnClients;

            if ( !should_spawn_actor )
            {
                continue;
            }

            AActor * new_actor = world->SpawnActor< AActor >( actor_entry.ActorType, actor_entry.SpawnTransform );
            SpawnedActors.Add( new_actor );
        }
    }
}

void UGFEGameFeatureAction_SpawnActors::Reset()
{
    for ( auto & actor_ptr : SpawnedActors )
    {
        if ( actor_ptr.IsValid() )
        {
            actor_ptr->Destroy();
        }
    }

    SpawnedActors.Reset();
}
