// Mathieu Jacq - Trial Task

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


struct FMinesweeperSettings
{
	//	Default values when the Window is opened
	int32 Width = 10;
	int32 Height = 10;
	int32 NumberOfMines = 16;
	//	---

	int32 GetNumberOfTiles() const { return Width * Height; }
	int32 GetNumberOfEmptyTiles() const { return GetNumberOfTiles() - NumberOfMines; }
	int32 GetHighestLine() const { return Height > Width ? Height : Width; }
	int32 GetMaxNumberOfMines() const { return Height * Width * 0.5f; }
};

class STile;
class SGameBoard;
template<typename NumericType> class SSpinBox;

class MINESWEEPER_API SMinesweeperWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWindow)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void OnHeightChanged(int32 NewHeight);
	void OnWidthChanged(int32 NewWidth);
	void OnNumberOfMinesChanged(int32 NewNumberOfMines);
	FReply OnGenerateNewGridButtonClicked();

	void SetGameStateText(const FText Text);
	void SetGameStateTextColor(const FLinearColor Color); 

protected:

	FMinesweeperSettings Settings;
	TSharedPtr<SSpinBox<int32>> HeightSlider;
	TSharedPtr<SSpinBox<int32>> WidthSlider;
	TSharedPtr<SSpinBox<int32>> MinesSlider;

	TSharedPtr<SGameBoard> GameBoard;
	TSharedPtr<STextBlock> GameStateTextBlock;

	//	Sliders Min and Max
	int32 HeightMin = 4;
	int32 HeightMax = 20;
	int32 WidthMin = 4;
	int32 WidthMax = 20;
	//	---
};
