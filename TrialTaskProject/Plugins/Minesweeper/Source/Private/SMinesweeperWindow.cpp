// Mathieu Jacq - Trial Task


#include "SMinesweeperWindow.h"
#include "SlateOptMacros.h"
#include "MinesweeperStyle.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "SGameBoard.h"
#include "Widgets/Layout/SScaleBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesweeperWindow::Construct(const FArguments& InArgs)
{
	TSharedPtr<FSlateStyleSet> StyleSet = FMinesweeperStyle::Get();

	ChildSlot
	[
		//	Vertical Box containing all the different parts of the window
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(15.f)
		[
			//	Horizontal Box containing the Settings
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(15.f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Font(StyleSet->GetFontStyle("MinesweeperStyle.SettingsText"))
					.Text(FText::FromString("Height:"))
				]
				+SHorizontalBox::Slot()
				[
					SAssignNew(HeightSlider, SSpinBox<int32>)
					.MinValue(HeightMin)
					.MaxValue(HeightMax)
					.Value(Settings.Height)
					.OnValueChanged(this, &SMinesweeperWindow::OnHeightChanged)
				]
			]
			+ SHorizontalBox::Slot()
			.Padding(15.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Font(StyleSet->GetFontStyle("MinesweeperStyle.SettingsText"))
					.Text(FText::FromString("Width:"))
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(WidthSlider, SSpinBox<int32>)
					.MinValue(WidthMin)
					.MaxValue(WidthMax)
					.Value(Settings.Width)
					.OnValueChanged(this, &SMinesweeperWindow::OnWidthChanged)
				]
			]
			+ SHorizontalBox::Slot()
			.Padding(15.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Font(StyleSet->GetFontStyle("MinesweeperStyle.SettingsText"))
					.Text(FText::FromString("Mines:"))
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(MinesSlider, SSpinBox<int32>)
					.SupportDynamicSliderMaxValue(true)
					.MinValue(2)
					.MaxValue(Settings.GetMaxNumberOfMines())
					.Value(Settings.NumberOfMines)
					.OnValueChanged(this, &SMinesweeperWindow::OnNumberOfMinesChanged)
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(10.f)
			[
				SNew(SButton)
				.Text(FText::FromString("Generate new grid"))
				.OnClicked(this, &SMinesweeperWindow::OnGenerateNewGridButtonClicked)
			]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(10.f)
			[
				SAssignNew(GameStateTextBlock, STextBlock)
				.Font(StyleSet->GetFontStyle("MinesweeperStyle.GameResultText"))
				.Text(FText::FromString(""))
			]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
			[
				SNew(SBox)
				.Padding(10.f)
				[
					SAssignNew(GameBoard, SGameBoard)
					.MainWindow(SharedThis(this))
				]
			]
	];
	//	Generate the Grid (when the window is opened)
	GameBoard.Get()->SetCurrentGameSettings(Settings);
	GameBoard.Get()->GenerateGrid();
	//	---
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperWindow::OnHeightChanged(int32 NewHeight)
{
	Settings.Height = NewHeight;

	//	Set the Max NumberOfMines to match the current settings
	if (!MinesSlider.IsValid()) return;
	int32 MaxNumOfMines = Settings.GetMaxNumberOfMines();
	MinesSlider.Get()->SetMaxSliderValue(MaxNumOfMines);
	MinesSlider.Get()->SetMaxValue(MaxNumOfMines);
	if (MinesSlider.Get()->GetValue() > MaxNumOfMines) MinesSlider.Get()->SetValue(MaxNumOfMines);
}

void SMinesweeperWindow::OnWidthChanged(int32 NewWidth)
{
	Settings.Width = NewWidth;

	//	Set the Max NumberOfMines to match the current settings
	if (!MinesSlider.IsValid()) return;
	int32 MaxNumOfMines = Settings.GetMaxNumberOfMines();
	MinesSlider.Get()->SetMaxSliderValue(MaxNumOfMines);
	MinesSlider.Get()->SetMaxValue(MaxNumOfMines);
	if (MinesSlider.Get()->GetValue() > MaxNumOfMines) MinesSlider.Get()->SetValue(MaxNumOfMines);
}

void SMinesweeperWindow::OnNumberOfMinesChanged(int32 NewNumberOfMines)
{
	Settings.NumberOfMines = NewNumberOfMines;
}

FReply SMinesweeperWindow::OnGenerateNewGridButtonClicked()
{
	if (!GameBoard.IsValid()) return FReply::Handled();

	GameBoard.Get()->SetCurrentGameSettings(Settings);
	GameBoard.Get()->GenerateGrid();
	return FReply::Handled();
}

void SMinesweeperWindow::SetGameStateText(const FText Text)
{
	if (!GameStateTextBlock.IsValid()) return;

	GameStateTextBlock.Get()->SetText(Text);
}

void SMinesweeperWindow::SetGameStateTextColor(const FLinearColor Color)
{
	if (!GameStateTextBlock.IsValid()) return;

	GameStateTextBlock.Get()->SetColorAndOpacity(Color);
}


