// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmo/CustomVisualizer.h"
#include "MapLayoutViewer.h"
//#include "Util/Ini.h"

CustomVisualizer::CustomVisualizer()
{
}

CustomVisualizer::~CustomVisualizer()
{
}

void CustomVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UMapLayoutViewer* mapLayoutViewer = Cast<UMapLayoutViewer>(Component);
	if (mapLayoutViewer)
	{
		if (mapLayoutViewer->mapFileilePath != filePath) {
			filePath = mapLayoutViewer->mapFileilePath;
			mapData = GenerateMapData(&filePath);
		}
		showLayout = mapLayoutViewer->showLayout;
	}

	if (showLayout) {
		DrawWireBox(PDI, FBox(FVector::Zero(), FVector(boxSpacing.X * xSize, 0, boxSpacing.Z * zSize)), worldborderColor, 1);
		for (int z = 0;z < mapData.size();z++) {
			for (int x = 0;x < mapData[z].size();x++) {
				if (mapData[z][x] != 0) {
					int zPos = (mapData.size() - 1 - z) * boxSpacing.Z;
					FVector boxPos = FVector(x * boxSpacing.X, 0, zPos);
					DrawWireBox(PDI, FBox(boxPos, boxPos + boxSize), GetTileColor(mapData[z][x]), 1);
				}
			}
		}
	}
}

vector<vector<int>> CustomVisualizer::GenerateMapData(const FString* mapFilePath) {
	auto generatedData = vector<vector<int>>();
	//if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*mapFilePath))return generatedData;

	//Ini ini = Ini(mapFilePath);
	//FString data = ini[TEXT("map")].Get<FString>("data");
	FString data = TEXT("00000000000000000000000000000000000000003000002222221111111111111");
	//FString size = ini[TEXT("info")].Get<FString>("size");
	//FString xData, zData;
	//size.Split(TEXT(","), &xData, &zData);
	//xSize = FCString::Atoi(*xData);
	xSize = 13;
	//zSize = FCString::Atoi(*zData);
	zSize = 5;

	generatedData.clear();
	for (int z = 0;z < zSize;z++) {
		generatedData.push_back(vector<int>());
		for (int x = 0;x < xSize;x++) {
			TCHAR c = data[z * xSize + x];
			generatedData[z].push_back(FCString::Atoi(&c));
		}
	}

	return generatedData;
}

FColor CustomVisualizer::GetTileColor(int tileID) {
	switch (tileID) {
	case 1:
		return groundColor;
	case 2:
		return monsterSpawnPosColor;
	case 3:
		return portalColor;
	default:
		return FColor::Magenta;
	}
}