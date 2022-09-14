// Mathieu Jacq - Trial Task


#include "SGameBoard.h"
#include "SMinesweeperWindow.h"
#include "STile.h"
#include "MinesweeperStyle.h"

void SGameBoard::Construct(const FArguments& InArgs)
{
	MainWindow = InArgs._MainWindow;
}

void SGameBoard::GenerateGrid()
{
	ResetBoard();

	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();
	float TileSize = StyleSet->GetFloat("MinesweeperStyle.DefaultTileSize") / float(CurrentGameSettings.GetHighestLine()) * 10.f;

	for (int32 HeightIndex = 0; HeightIndex < CurrentGameSettings.Height; ++HeightIndex)
	{
		for (int32 WidthIndex = 0; WidthIndex < CurrentGameSettings.Width; ++WidthIndex)
		{
			TSharedPtr<STile> NewTile;
			AddSlot(HeightIndex, WidthIndex)
			[
				SNew(SBox)
				.HeightOverride(TileSize)
				.WidthOverride(TileSize)
				[
					SAssignNew(NewTile, STile)
					.GameBoard(SharedThis(this))
					.Position(FTilePosition(HeightIndex, WidthIndex))
				]
			];
			Tiles.Add(FTileWithPosition(FTilePosition(HeightIndex, WidthIndex), NewTile));
		}
	}
	SetGameState(Playing);
}

void SGameBoard::TryToPlaceMines(const FTilePosition InitiatorPosition)
{
	if (bAreMinesPlaced) return;

	TArray<FTileWithPosition> MineCandidates;

	//	If the number of mines allows it, remove the adjacent tiles from the MineCandidates.
	//	If it doesn't, only remove the Initiator from the MineCandidates.
	if (CurrentGameSettings.GetNumberOfTiles() - 15 > CurrentGameSettings.NumberOfMines)
	{
		TArray<FTilePosition> NeighborPositions = GetNeighborPositions(InitiatorPosition);
		MineCandidates = Tiles.FilterByPredicate([&NeighborPositions, &InitiatorPosition](FTileWithPosition TileWithPosition)
			{
				return (!NeighborPositions.Contains(TileWithPosition.Position) && TileWithPosition.Position != InitiatorPosition);
			});
	}
	else
	{
		TArray<FTilePosition> NeighborPositions = GetNeighborPositions(InitiatorPosition);
		MineCandidates = Tiles.FilterByPredicate([&InitiatorPosition](FTileWithPosition TileWithPosition)
			{
				return (TileWithPosition.Position != InitiatorPosition);
			});
	}

	//	Place Mines in MineCandidates
	for (int32 MineCount = 0; MineCount < CurrentGameSettings.NumberOfMines; ++MineCount)
	{
		int32 RandIndex = FMath::RandRange(0, MineCandidates.Num() - 1);
		if (MineCandidates[RandIndex].Tile.IsValid())
		{
			MineCandidates[RandIndex].Tile.Get()->PlaceMine();
			MineCandidates.RemoveAt(RandIndex);
		}
	}
	bAreMinesPlaced = true;
}

void SGameBoard::ResetBoard()
{
	ClearChildren();
	Tiles.Empty();
	bAreMinesPlaced = false;
}

TArray<FTilePosition> SGameBoard::GetNeighborPositions(const FTilePosition TilePosition) const
{
	TArray<FTilePosition> NeighborPositions;
	for (int x = -1; x < 2; ++x)
	{
		for (int y = -1; y < 2; ++y)
		{
			FTilePosition NeighborPosition(TilePosition.Height + x, TilePosition.Width + y);
			if (TilePosition == NeighborPosition) continue;
			if (IsValidTilePosition(NeighborPosition))
			{
				NeighborPositions.Add(NeighborPosition);
			}
		}
	}
	return NeighborPositions;
}

void SGameBoard::SetCurrentGameSettings(const FMinesweeperSettings lCurrentGameSettings)
{
	CurrentGameSettings = lCurrentGameSettings;
	NumberOfTilesToReveal = CurrentGameSettings.Height * CurrentGameSettings.Width - CurrentGameSettings.NumberOfMines;
}


bool SGameBoard::IsValidTilePosition(const FTilePosition TilePosition) const
{
	return Tiles.ContainsByPredicate([&TilePosition](FTileWithPosition& TileWithPosition) {return (TilePosition == TileWithPosition.Position); });
}

STile* SGameBoard::GetTileFromPosition(const FTilePosition TilePosition) const
{
	for (const FTileWithPosition& TileWithPosition : Tiles)
	{
		if (TileWithPosition.Position == TilePosition)
		{
			return TileWithPosition.Tile.Get();
		}
	}
	return nullptr;
}

void SGameBoard::SetGameState(const EGameState NewGameState)
{
	if (!MainWindow.IsValid()) return;

	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();
	GameState = NewGameState;

	switch (GameState)
	{
	case Playing:
		MainWindow.Get()->SetGameStateText(FText::FromString(""));
		MainWindow.Get()->SetGameStateTextColor(FColor(0, 0, 0));
		return;
	case Loss:
		MainWindow.Get()->SetGameStateText(FText::FromString("Loss!"));
		MainWindow.Get()->SetGameStateTextColor(StyleSet->GetColor("MinesweeperStyle.LossColor"));
		break;
	case Victory:
		MainWindow.Get()->SetGameStateText(FText::FromString("Victory!"));
		MainWindow.Get()->SetGameStateTextColor(StyleSet->GetColor("MinesweeperStyle.VictoryColor"));
		break;
	}
	//	Reveal the Mines if Loss or Victory
	for (FTileWithPosition& TileWithPosition : Tiles)
	{
		if (!TileWithPosition.Tile.IsValid()) continue;

		if (TileWithPosition.Tile.Get()->GetIsMineTile())
		{
			TileWithPosition.Tile.Get()->DisplayMine();
		}
	}
}