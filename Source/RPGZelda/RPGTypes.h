#pragma once

#include "UObject/PrimaryAssetId.h"
#include "RPGTypes.generated.h"

class URPGItem;

/** Struct representing a slot for an item, shown in the UI */
USTRUCT(BlueprintType)
struct RPGZELDA_API FRPGItemInfo
{
	GENERATED_BODY()

		/** Constructor, -1 means an invalid slot */
		FRPGItemInfo()
		: IdxNumber(-1)
	{}

	FRPGItemInfo(const FPrimaryAssetType& InItemType, int32 InIdxNumber)
		: ItemType(InItemType)
		, IdxNumber(InIdxNumber)
	{}

	/** The type of items that can go in this slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FPrimaryAssetType ItemType;

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 IdxNumber;

	/** Equality operators */
	bool operator==(const FRPGItemInfo& Other) const
	{
		return ItemType == Other.ItemType && IdxNumber == Other.IdxNumber;
	}
	bool operator!=(const FRPGItemInfo& Other) const
	{
		return !(*this == Other);
	}

	/** Implemented so it can be used in Maps/Sets */
	friend inline uint32 GetTypeHash(const FRPGItemInfo& Key)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
		Hash = HashCombine(Hash, (uint32)Key.IdxNumber);
		return Hash;
	}

	/** Returns true if slot is valid */
	bool IsValid() const
	{
		return ItemType.IsValid() && IdxNumber >= 0;
	}
};