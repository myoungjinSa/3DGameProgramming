cbuffer cbSetting : register(b0)
{
	int gBlurRadius;

	// Support up to 11 blur weights.
	// �ִ� 11 ���� �帲 ����ġ�� �����մϴ�.
	float w0;
	float w1;
	float w2;
	float w3;
	float w4;
	float w5;
	float w6;
	float w7;
	float w8;
	float w9;
	float w10;
}

static const int gMaxBlurRadius = 5;
Texture2D gtxtBlurScene : register(t23);
RWTexture2D<float4> RWgtxtBlurScene : register(u0);

#define N 256
#define CacheSize (N + 2*gMaxBlurRadius)	// ���� �޸� n+2R

groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void HorzBlurCS(int3 groupThreadID : SV_GroupThreadID, int3 dispatchThreadID : SV_DispatchThreadID)
{
	// �� �迭�� �ε����� �ִ´�.
	float weights[11] = { w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10 };

	if (groupThreadID.x < gBlurRadius)
	{
		// �̹��� ���� �����ڸ� �ٱ��� ǥ���� ���� �����ڸ� ǥ������ �����Ѵ�.
		int x = max(dispatchThreadID.x - gBlurRadius, 0);
		gCache[groupThreadID.x] = gtxtBlurScene[int2(x, dispatchThreadID.y)];
	}
	if (groupThreadID.x >= N - gBlurRadius)
	{
		// �̹��� �����ڸ� �ٱ��� ǥ���� ������ �����ڸ� ǥ������ �����Ѵ�.
		int x = min(dispatchThreadID.x + gBlurRadius, gtxtBlurScene.Length.x - 1);
		gCache[groupThreadID.x + 2 * gBlurRadius] = gtxtBlurScene[int2(x, dispatchThreadID.y)];
	}
	// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
	gCache[groupThreadID.x + gBlurRadius] = gtxtBlurScene[min(dispatchThreadID.xy, gtxtBlurScene.Length.xy - 1)];

	// ��� �����尡 �� ������ ������ ������ ��ٸ���.
	GroupMemoryBarrierWithGroupSync();

	// ���� �� �ȼ��� �帮���Ѵ�.

	float4 blurColor = float4(0, 0, 0, 0);

	for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.x + gBlurRadius + i;

		blurColor += weights[i + gBlurRadius] * gCache[k];
	}


	RWgtxtBlurScene[dispatchThreadID.xy] = blurColor;

}

[numthreads(1, N, 1)]
void VertBlurCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	float weights[11] = { w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10 };

	if (groupThreadID.y < gBlurRadius)
	{
		int y = max(dispatchThreadID.y - gBlurRadius, 0);
		gCache[groupThreadID.y] = gtxtBlurScene[int2(dispatchThreadID.x, y)];

	}
	if (groupThreadID.y >= N - gBlurRadius)
	{
		int y = min(dispatchThreadID.y + gBlurRadius, gtxtBlurScene.Length.y - 1);
		gCache[groupThreadID.y + 2 * gBlurRadius] = gtxtBlurScene[int2(dispatchThreadID.x, y)];

	}
	gCache[groupThreadID.y + gBlurRadius] = gtxtBlurScene[min(dispatchThreadID.xy, gtxtBlurScene.Length.xy - 1)];

	GroupMemoryBarrierWithGroupSync();

	//���� �� �ȼ��� �帰��.
	float4 blurColor = float4(0, 0, 0, 0);

	for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.y + gBlurRadius + i;

		blurColor += weights[i + gBlurRadius] * gCache[k];
	}

	RWgtxtBlurScene[dispatchThreadID.xy] = blurColor;

}