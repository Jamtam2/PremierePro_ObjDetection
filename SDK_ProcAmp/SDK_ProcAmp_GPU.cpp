#include "SDK_ProcAmp.h"
#include "PrGPUFilterModule.h"
#include <vector>
#include <wrl/client.h>
using namespace Microsoft::WRL;
#include <d3d12.h>
#include <dxgi1_6.h>
#include "DirectXUtils.h"

struct ProcAmp2Params {
    int mPitch;
    int mWidth;
    int mHeight;
};

struct DXContext {
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<ID3D12GraphicsCommandList> commandList;
    ComPtr<ID3D12PipelineState> pipelineState;
    ComPtr<ID3D12RootSignature> rootSignature;

    bool Initialize(ID3D12Device* device, ID3D12CommandQueue* commandQueue) {
        return true;  // Initialization logic here
    }

    void SetupCommandList() {
        // Setup command list here
    }

    void DispatchShader(int width, int height, const ProcAmp2Params& params, ID3D12Resource* src, ID3D12Resource* dst) {
        // Dispatch compute shader logic here
    }
};

static std::vector<std::shared_ptr<DXContext>> sDXContextCache;

class ProcAmp2 : public PrGPUFilterBase {
public:
    prSuiteError Initialize(PrGPUFilterInstance* ioInstanceData) override {
        PrGPUFilterBase::Initialize(ioInstanceData);

        if (mDeviceIndex >= sDXContextCache.size()) {
            sDXContextCache.resize(mDeviceIndex + 1);
        }

        if (!sDXContextCache[mDeviceIndex]) {
            auto dxContext = std::make_shared<DXContext>();
            if (!dxContext->Initialize((ID3D12Device*)mDeviceInfo.outDeviceHandle, (ID3D12CommandQueue*)mDeviceInfo.outCommandQueueHandle)) {
                return suiteError_Fail;
            }
            sDXContextCache[mDeviceIndex] = dxContext;
        }
        return suiteError_NoError;
    }

    prSuiteError Render(const PrGPUFilterRenderParams* inRenderParams, const PPixHand* inFrames, csSDK_size_t inFrameCount, PPixHand* outFrame) override {
        PrPixelFormat pixelFormat;
        prRect theRect;
        mPPixSuite->GetPixelFormat(inFrames[0], &pixelFormat);
        mPPixSuite->GetBounds(inFrames[0], &theRect);
        int width = theRect.right - theRect.left;
        int height = theRect.bottom - theRect.top;
        csSDK_int32 rowBytes = 0;
        mPPixSuite->GetRowBytes(inFrames[0], &rowBytes);

        ProcAmp2Params params = { rowBytes, width, height };

        char* srcFrameData = nullptr;
        char* dstFrameData = nullptr;
        mPPixSuite->GetPixels(inFrames[0], PrPPixBufferAccess_ReadOnly, reinterpret_cast<char**>(&srcFrameData));
        mPPixSuite->GetPixels(*outFrame, PrPPixBufferAccess_ReadWrite, reinterpret_cast<char**>(&dstFrameData));

        ID3D12Resource* srcFrameResource = reinterpret_cast<ID3D12Resource*>(srcFrameData);
        ID3D12Resource* dstFrameResource = reinterpret_cast<ID3D12Resource*>(dstFrameData);

        auto& dxContext = sDXContextCache[mDeviceIndex];
        dxContext->SetupCommandList();
        dxContext->DispatchShader(width, height, params, srcFrameResource, dstFrameResource);

        return suiteError_NoError;
    }

    static prSuiteError Shutdown(piSuitesPtr piSuites, csSDK_int32 inIndex) {
        if (inIndex < sDXContextCache.size() && sDXContextCache[inIndex]) {
            sDXContextCache[inIndex].reset();
        }
        return suiteError_NoError;
    }
};

DECLARE_GPUFILTER_ENTRY(PrGPUFilterModule<ProcAmp2>)
