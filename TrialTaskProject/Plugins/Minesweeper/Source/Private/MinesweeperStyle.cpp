// Mathieu Jacq - Trial Task

#include "MinesweeperStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Misc/Paths.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateStyleRegistry.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperStyle::StyleInstance = nullptr;


void FMinesweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}

	StyleInstance->Set("MinesweeperStyle.SettingsText", FCoreStyle::GetDefaultFontStyle("Regular", 16));
	StyleInstance->Set("MinesweeperStyle.DefaultTileSize", 50.f);
	StyleInstance->Set("MinesweeperStyle.GameResultText", FCoreStyle::GetDefaultFontStyle("Bold", 14));
	StyleInstance->Set("MinesweeperStyle.VictoryColor", FColor(55, 237, 0));
	StyleInstance->Set("MinesweeperStyle.LossColor", FColor(237, 0, 0));
	StyleInstance->Set("MinesweeperStyle.TileText", FCoreStyle::GetDefaultFontStyle("Bold", 12));
	StyleInstance->Set("MinesweeperStyle.Background", FColor(80, 80, 80));
	StyleInstance->Set("MinesweeperStyle.MineImage", new FSlateImageBrush(StyleInstance->GetContentRootDir() / "Minesweeper_Mine.png", FVector2D(10.f, 10.f)));
	StyleInstance->Set("MinesweeperStyle.Color1", FColor(52, 124, 224));
	StyleInstance->Set("MinesweeperStyle.Color2", FColor(9, 255, 0));
	StyleInstance->Set("MinesweeperStyle.Color3", FColor(255, 0, 0));
	StyleInstance->Set("MinesweeperStyle.Color4", FColor(7, 0, 138));
	StyleInstance->Set("MinesweeperStyle.Color5", FColor(69, 44, 44));
	StyleInstance->Set("MinesweeperStyle.Color6", FColor(61, 235, 185));
	StyleInstance->Set("MinesweeperStyle.Color7", FColor(0, 0, 0));
	StyleInstance->Set("MinesweeperStyle.Color8", FColor(105, 105, 105));
	
}

void FMinesweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMinesweeperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MinesweeperStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMinesweeperStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MinesweeperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Minesweeper")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Minesweeper.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FMinesweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

TSharedPtr<FSlateStyleSet> FMinesweeperStyle::Get()
{
	return StyleInstance;
}
