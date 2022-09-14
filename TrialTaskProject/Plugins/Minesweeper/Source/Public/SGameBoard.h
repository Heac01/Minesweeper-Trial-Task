// Mathieu Jacq - Trial Task

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "Widgets/Layout/SUniformGridPanel.h"

struct FMinesweeperSettings;
class STile;

struct FTilePosition
{
	int32 Height = 0;
	int32 Width = 0;

	FTilePosition() {}
	FTilePosition(const int32 lHeight, const int32 lWidth) { Height = lHeight; Width = lWidth; }

	bool operator==(const FTilePosition& Other) const
	{
		return (Height == Other.Height && Width == Other.Width);
	}
	bool operator!=(const FTilePosition& Other) const
	{
		return (Height != Other.Height || Width != Other.Width);
	}
};

struct FTileWithPosition
{
	FTilePosition Position;
	TSharedPtr<STile> Tile;

	FTileWithPosition() {}
	FTileWithPosition(const FTilePosition lPosition, const TSharedPtr<STile> lTile) { Position = lPosition; Tile = lTile; }
};

enum EGameState { Playing, Loss, Victory};

class MINESWEEPER_API SGameBoard : public SUniformGridPanel
{
public:
	
	SLATE_BEGIN_ARGS(SGameBoard)
	{}
	SLATE_ARGUMENT(TSharedPtr<SMinesweeperWindow>, MainWindow)
		SLATE_END_ARGS()

			SGameBoard() {}

		void Construct(const FArguments& InArgs);

		void GenerateGrid();

		//	Will place the mines when the user interacts with a tile for the first time
		//	Ensures the first move is done on an empty tile
		void TryToPlaceMines(const FTilePosition InitiatorPosition);

		TArray<FTilePosition> GetNeighborPositions(const FTilePosition TilePosition) const;
		STile* GetTileFromPosition(const FTilePosition TilePosition) const;

		void SetCurrentGameSettings(const FMinesweeperSettings lCurrentGameSettings);
		void SetGameState(const EGameState NewGameState);
		EGameState GetGameState() const { return GameState; }

		//	Tiles left to reveal before Victory
		int32 NumberOfTilesToReveal = 0;

protected:

	//	Called before generating a new grid
	void ResetBoard();

	bool IsValidTilePosition(const FTilePosition TilePosition) const;

	TSharedPtr<SMinesweeperWindow> MainWindow;
	FMinesweeperSettings CurrentGameSettings;
	TArray<FTileWithPosition> Tiles;
	EGameState GameState = Playing;
	bool bAreMinesPlaced = false;
};
