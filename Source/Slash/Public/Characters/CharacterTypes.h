#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "DeathPose1"),
	EDP_Death2 UMETA(DisplayName = "DeathPose2"),
	EDP_Death3 UMETA(DisplayName = "DeathPose3"),
	EDP_Death4 UMETA(DisplayName = "DeathPose4"),
	EDP_Death5 UMETA(DisplayName = "DeathPose5"),
	EDP_Death6 UMETA(DisplayName = "DeathPose6"),
};