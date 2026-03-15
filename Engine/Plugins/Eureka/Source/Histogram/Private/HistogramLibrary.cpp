// Fill out your copyright notice in the Description page of Project Settings.


#include "HistogramLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TextureResource.h"

void UHistogramLibrary::CalculateHistogram(class UTextureRenderTarget2D* RenderTarget, TArray<float>& OutHistogram)
{
	// 初始化输出数组，确保有 256 个元素且初始为 0
	OutHistogram.Init(0.0f, 256);

	// 安全检查：确保传入的 RT 有效并且已经分配了渲染资源
	if (!RenderTarget || !RenderTarget->GameThread_GetRenderTargetResource())
	{
		return;
	}

	FRenderTarget* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
	TArray<FColor> RawPixels;

	// 从 RT 中读取像素数据 (这是个耗时操作，所以蓝图里不能每帧调)
	if (!RenderTargetResource->ReadPixels(RawPixels))
	{
		return;
	}

	if (RawPixels.Num() == 0)
	{
		return;
	}

	// 准备 256 个“桶”用来计数
	TArray<int32> Bins;
	Bins.Init(0, 256);
	int32 MaxCount = 0;

	// 遍历所有像素
	for (const FColor& Pixel : RawPixels)
	{
		// 计算灰度 (Luminance) - 使用标准的 sRGB 权重
		float Luminance = 0.299f * Pixel.R + 0.587f * Pixel.G + 0.114f * Pixel.B;

		// 四舍五入并限制在 0-255 范围内，防止越界
		int32 BinIndex = FMath::Clamp(FMath::RoundToInt(Luminance), 0, 255);
		
		// 对应色阶的计数 +1
		Bins[BinIndex]++;

		// 记录出现最多的色阶的数量，用于后续归一化
		if (Bins[BinIndex] > MaxCount)
		{
			MaxCount = Bins[BinIndex];
		}
	}

	// 归一化：将计数转换为 0.0 到 1.0 的比例，方便 UI 直接绘制高度
	if (MaxCount > 0)
	{
		for (int32 i = 0; i < 256; ++i)
		{
			// 为了防止某个极端的色阶（比如全黑或全白）数量爆表导致其他色阶在UI上看不见，
			// 有时可以对结果做一个轻微的 Pow(x, 0.5) 也就是开根号处理来平滑视觉效果。
			// 这里先提供线性的基础版。
			OutHistogram[i] = static_cast<float>(Bins[i]) / static_cast<float>(MaxCount);
		}
	}
}
