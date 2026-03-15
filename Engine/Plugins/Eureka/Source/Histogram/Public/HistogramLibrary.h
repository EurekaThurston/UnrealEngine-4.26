// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HistogramLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HISTOGRAM_API UHistogramLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 从 RenderTarget 读取像素并计算 256 阶灰度直方图
	 * @param RenderTarget 输入的渲染目标 (格式需为 RGBA8)
	 * @param OutHistogram 输出的直方图数组 (0-255阶，高度已归一化到 0.0 - 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Eureka|Histogram")
	static void CalculateHistogram(class UTextureRenderTarget2D* RenderTarget, TArray<float>& OutHistogram);
};
