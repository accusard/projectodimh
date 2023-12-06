// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#include "POdimhWidget.h"
#include "ProjectOdimh/UserInterface/SelectButton.h"
#include "Components/Button.h"
#include "ProjectOdimh/Data/AssetLoader.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"



UPOdimhWidget::UPOdimhWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPOdimhWidget::NativeConstruct()
{
    Super::NativeConstruct();

    TArray<UWidget*> WidgetList;
    WidgetTree->GetAllWidgets(WidgetList);
    
    for(UWidget* Widget : WidgetList)
    {
        if(USelectButton* Button = Cast<USelectButton>(Widget))
        {
            Button->OnReleased.AddUniqueDynamic(Button, &USelectButton::BroadcastEvent);
        }
    }
}

FText UPOdimhWidget::LoadTooltipTextFromTable(UDataTable* Table, const FName& RowName)
{
    if(Table)
    {
        return UAssetLoader::GetTextFromRow(Table, RowName, "Tooltip")->Text;
    }

    return FText();
}

