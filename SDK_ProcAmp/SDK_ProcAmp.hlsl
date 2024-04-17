/*************************************************************************
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 * Copyright 2023 Adobe
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains
 * the property of Adobe and its suppliers, if any. The intellectual
 * and technical concepts contained herein are proprietary to Adobe
 * and its suppliers and are protected by all applicable intellectual
 * property laws, including trade secret and copyright laws.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Adobe.
 **************************************************************************/

/*
 * Buffers associated with the shader
 * Read-Write/Write buffers are registered as UAVs while
 * Read-only buffers are registered as SRVs
 * See u0 is bound as UAV in RootSignature below
 */
RWByteAddressBuffer mIOImage : register(u0);

cbuffer cb : register(b0)
{
    int     mPitch;
    int     mIs16f;
    int     mWidth;
    int     mHeight;
};

[numthreads(16, 16, 1)]
[RootSignature("DescriptorTable(CBV(b0), visibility=SHADER_VISIBILITY_ALL),DescriptorTable(UAV(u0), visibility=SHADER_VISIBILITY_ALL)")]
void main(uint3 inXY : SV_DispatchThreadID)
{
    if (inXY.x < mWidth && inXY.y < mHeight)
    {
        uint index = mPitch * inXY.y + inXY.x * (mIs16f ? sizeof(half4) : sizeof(float4));
        float4 pixel = mIs16f ? float4(mIOImage.Load<half4>(index)) : mIOImage.Load<float4>(index);

        // Output the RGB values directly without any processing
        mIOImage.Store(index, pixel);
    }
}
