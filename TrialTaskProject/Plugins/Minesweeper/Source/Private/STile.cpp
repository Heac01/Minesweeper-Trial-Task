// Mathieu Jacq - Trial Task


#include "STile.h"
#include "MinesweeperStyle.h"
#include "Misc/Paths.h"


void STile::Construct(const FArguments& InArgs)
{
	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();

	GameBoard = InArgs._GameBoard;
	Position = InArgs._Position;

	AddSlot()
	.Padding(1.f)
	[
		SNew(SBox)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(Background, SColorBlock)
			.Color(StyleSet->GetColor("MinesweeperStyle.Background"))
		]
	];

	AddSlot()
	.Padding(2.f)
	[
		SAssignNew(Button, SButton)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.OnClicked(this, &STile::OnButtonClicked)
	];
}

FReply STile::OnButtonClicked()
{
	if (!GameBoard.IsValid()) return FReply::Handled();

	GameBoard->TryToPlaceMines(Position);

	if (GameBoard->GetGameState() != Playing) return FReply::Handled();

	Reveal_recursive();

	return FReply::Handled();
}

void STile::Reveal_recursive()
{
	if (bIsRevealed) return;
	if (!GameBoard.IsValid()) return;

	bIsRevealed = true;

	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();

	if (bIsMineTile)
	{
		DisplayMine();
		GameBoard.Get()->SetGameState(Loss);
	}
	else
	{
		int32 NumOfAdjacentMines = 0;
		TArray<FTilePosition> NeighborPositions = GameBoard->GetNeighborPositions(Position);

		for (FTilePosition& NeighborPosition : NeighborPositions)
		{
			STile* NeighborTile = GameBoard->GetTileFromPosition(NeighborPosition);
			if (NeighborTile != nullptr)
			{
				if (NeighborTile->bIsMineTile)
				{
					++NumOfAdjacentMines;
				}
			}
		}
		if (NumOfAdjacentMines == 0)
		{
			DisplayEmptyTile();
			//	Reveal adjacent tiles
			for (FTilePosition& NeighborPosition : NeighborPositions)
			{
				STile* NeighborTile = GameBoard->GetTileFromPosition(NeighborPosition);
				if (NeighborTile != nullptr)
				{
					NeighborTile->Reveal_recursive();
				}
			}
		}
		else
		{
			DisplayNearbyMines(NumOfAdjacentMines);
		}
		--GameBoard.Get()->NumberOfTilesToReveal;
		if (GameBoard.Get()->NumberOfTilesToReveal <= 0)
		{
			GameBoard.Get()->SetGameState(Victory);
		}
	}	
}

void STile::DisplayMine()
{
	if (Button.IsValid()) Button->SetVisibility(EVisibility::Hidden);

	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();

	AddSlot()
		.Padding(4.f)
		[
			SNew(SBox)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
		[
			SNew(SImage)
			.Image(StyleSet->GetBrush("MinesweeperStyle.MineImage"))
		]
		];
}

void STile::DisplayNearbyMines(const int32 NumOfAdjacentMines)
{
	if (Button.IsValid()) Button->SetVisibility(EVisibility::Hidden);

	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();

	AddSlot()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
			.Text(FText::AsNumber(NumOfAdjacentMines))
			.ColorAndOpacity(StyleSet->GetColor(FName("MinesweeperStyle.Color" + FString::FromInt(NumOfAdjacentMines))))
		];
}

void STile::DisplayEmptyTile()
{
	if (Button.IsValid()) Button->SetVisibility(EVisibility::Hidden);
}