// Mathieu Jacq - Trial Task

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SMinesweeperWindow.h"
#include "SGameBoard.h"

class SMinesweeperWindow;

class MINESWEEPER_API STile : public SOverlay
{
public:
	
	SLATE_BEGIN_ARGS(STile)
	{}
		SLATE_ARGUMENT(TSharedPtr<SGameBoard>, GameBoard)
		SLATE_ARGUMENT(FTilePosition, Position)

		SLATE_END_ARGS()

	STile() {}

	void Construct(const FArguments& InArgs);
	
	FReply OnButtonClicked();
	
	//	Is called when this tile is clicked. If 'this' doesn't have adjacent mines, also call it on Neighbors
	void Reveal_recursive();

	void PlaceMine() { bIsMineTile = true; }
	bool GetIsMineTile() const { return bIsMineTile; }

	//	Display Graphic elements when the tile is revealed
	void DisplayMine();
	void DisplayNearbyMines(const int32 NumOfAdjacentMines);
	void DisplayEmptyTile();
	//	---

protected:

	TSharedPtr<SGameBoard> GameBoard;
	TSharedPtr<SButton> Button;
	TSharedPtr<SColorBlock> Background;

	FTilePosition Position;
	bool bIsMineTile = false;
	bool bIsRevealed = false;
};
