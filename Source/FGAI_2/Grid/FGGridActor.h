#pragma once

#include "GameFramework/Actor.h"
#include "FGGridActor.generated.h"

USTRUCT(BlueprintType)
struct FFGTileInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Tile")
	bool bBlock = false;
};

class UStaticMeshComponent;
class UStaticMesh;
class UStaticMeshDescription;

UCLASS()
class FGAI_2_API AFGGridActor : public AActor
{
	GENERATED_BODY()
public:
	AFGGridActor();

	virtual void BeginPlay() override;

	/*
	* Called whenever placed in the editor or world, having its transform changed etc.
	* Responsible for eventually calling the infamous ConstructionScript in blueprint.
	*/
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY()
	UStaticMeshComponent* BlockStaticMeshComponent = nullptr;

	UPROPERTY()
	UStaticMesh* GridMesh = nullptr;

	UPROPERTY()
	UStaticMesh* BlockMesh = nullptr;

	UPROPERTY()
	UStaticMeshDescription* MeshDescription = nullptr;

	UPROPERTY()
	UStaticMeshDescription* BlockMeshDescription = nullptr;

	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector GetWorldLocationFromXY(int32 TileX, int32 TileY) const;

	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector GetWorldLocationFromIndex(int32 Index) const;

	UFUNCTION(BlueprintPure, Category = "Grid")
	bool GetXYFromWorldLocation(const FVector& WorldLocation, int32& TileX, int32& TileY) const;

	UFUNCTION(BlueprintPure, Category = "Grid")
	int32 GetTileIndexFromWorldLocation(const FVector& WorldLocation) const;

	UFUNCTION(BlueprintPure, Category = "Grid")
	bool TransformWorldLocationToTileLocation(const FVector& InWorldLocation, FVector& OutTileWorldLocation) const;

	/*
	* Returns a list of indices correlating to the location of a tile within the TileList
	*/
	void GetOverlappingTiles(const FVector& Origin, const FVector& Extent, TArray<int32>& OutOverlappingTiles) const;

	void DrawBlocks();

	void UpdateBlockingTiles();

	void GenerateGrid();

	bool IsWorldLocationInsideGrid(const FVector& WorldLocation) const;

	UFUNCTION(BlueprintPure, Category = "Grid")
	int32 GetTileIndexFromXY(int32 TileX, int32 TileY) const;
	bool IsTileIndexValid(int32 TileIndex) const;

	TArray<int32> GetCardinalNeighbours(int32 index) const;
	TArray<int32> GetDiagonalNeighbours(int32 index) const;
	
#if WITH_EDITOR
	/*
	* This is called whenever a property, on this Actor, is edited in the editor.
	* Only available in the editor. If you forget WITH_EDITOR you will get a compile error when compiling the non-editor build
	*/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetTileSizeHalf() const { return TileSize * 0.5f; }
	UFUNCTION(BlueprintPure, Category = "Grid")
	int32 GetNumTiles() const { return Width * Height; }
	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetGridSize() const { return GetNumTiles() * TileSize; }

	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetHalfWidth() const { return static_cast<float>(Width) * 0.5f; }
	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetHalfHeight() const { return static_cast<float>(Height) * 0.5f; }

	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetWidthSize() const { return (static_cast<float>(Width) * GetTileSizeHalf()) + BorderSize; }
	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetHeightSize() const { return (static_cast<float>(Height) * GetTileSizeHalf()) + BorderSize; }

	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector GetWidthExtends() const { return FVector(BorderSize, GetHeightSize(), BorderSize); }
	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector GetHeightExtends() const { return FVector(GetWidthSize(), BorderSize, BorderSize); }

	/*
	* Initializes to the size of the number of tiles in the grid. 
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
	TArray<FFGTileInfo> TileList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (ClampMin = 1))
	int Width = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (ClampMin = 1))
	int Height = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (ClampMin = 0.1))
	float BorderSize = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	float TileSize = 500.0f;

	private:
	const FVector2D CardinalDirection[4] = { FVector2D(0, 1),  FVector2D(-1, 0), FVector2D(1, 0), FVector2D(0, -1) };
	const FVector2D DiagonalDirection[4] = { FVector2D(-1, 1),  FVector2D(1, 1), FVector2D(-1, -1), FVector2D(1, -1) };

	bool IsValidXDirection(int32 Index, int32 X) const;
};
