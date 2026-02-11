// Fill out your copyright notice in the Description page of Project Settings.


#include "SMidterialWidgetMI.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMidterialWidgetMI::Construct(const FArguments& InArgs)
{
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.Padding(5.0f, 5.0f)
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(5.0f, 0.0f, 10.0f, 0.0f)
			.AutoWidth()
			[
				SNew(STextBlock)
					.Text(FText::FromString("Parent"))
			]
			+SHorizontalBox::Slot()
			[
				SNew(SObjectPropertyEntryBox)

				.ObjectPath(TAttribute<FString>(this, &SMidterialWidgetMI::GetMaterialPath))
				.OnObjectChanged(FOnSetObject::CreateSP(this, &SMidterialWidgetMI::OnEntryMaterialChanged))

				.AllowedClass(UMaterial::StaticClass())
				.DisplayBrowse(true)
				.EnableContentPicker(true)
							
				.DisplayThumbnail(true)
				.ThumbnailPool(AssetThumbnailPool)
			]
		]
		+SVerticalBox::Slot()
		.Padding(5.0f, 5.0f, 5.0f, 0.0f)
		.AutoHeight()
		[
			SNew(SHeaderRow)
			+SHeaderRow::Column("Textures")
			[
				SNew(STextBlock)
					.Text(FText::FromString("Selected Textures"))
			]
		]
		+SVerticalBox::Slot()
		.Padding(5.0f, 0.0f, 5.0f, 5.0f)
		.FillHeight(1.0f)
		[
			SNew(SScrollBox)
			+SScrollBox::Slot()
			.Padding(5.0f, 0.0f)
			[
				SAssignNew(ListViewWidget, SListView<TSharedPtr<FString>>)
				.ListItemsSource(&Items)
				.SelectionMode(ESelectionMode::None)
				.OnGenerateRow(this, &SMidterialWidgetMI::OnGenerateRowForList)
			]
		]
		+SVerticalBox::Slot()
		.Padding(5.0f, 5.0f)
		.VAlign(VAlign_Bottom)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SButton)
				.Text(FText::FromString("Assign Textures"))
				.HAlign(HAlign_Center)
			]
		]
	];
}

void SMidterialWidgetMI::OnEntryMaterialChanged(const FAssetData& InData)
{
	UE_LOG(LogTemp, Warning, TEXT("Entry Object Changed"))
	MaterialPath = InData.GetObjectPathString();
}

FString SMidterialWidgetMI::GetMaterialPath() const
{
	return MaterialPath;
}

void SMidterialWidgetMI::AddListItem(TSharedPtr<FString> Item)
{
	Items.Add(Item);

	ListViewWidget->RequestListRefresh();
}

TSharedRef<ITableRow> SMidterialWidgetMI::OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	//Create the row
	FString PathName{}, PathNameNoExtension{};

	PathName = *Item.Get();
	PathName.Split(TEXT("."), &PathNameNoExtension, nullptr);
	

	return
		SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(PathNameNoExtension))
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
