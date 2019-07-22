#include "MemoryManager.h"
#include <malloc.h>

using namespace Acmen;
namespace Acmen
{
    static const uint32_t kBlockSize[] = {
        // 4-increments
        4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
        52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 
        // 32-increments
        128, 160, 192, 224, 256, 288, 320, 352, 384, 
        416, 448, 480, 512, 544, 576, 608, 640, 
        // 64-increments
        704, 768, 832, 896, 960, 1024
    };

    static const uint32_t kPageSize = 8192;
    static const uint32_t kAllignment = 4;

    static const uint32_t kNumBlockSize = sizeof(kBlockSize) / sizeof(kBlockSize[0]);
    static const uint32_t kMaxBlockSize = kBlockSize[kNumBlockSize - 1];
}

int Acmen::MemoryManager::Initialize()
{
    static bool s_bInitialized = false;
    if(!s_bInitialized)
    {
        m_pBlockSizeLookup = new size_t[kMaxBlockSize + 1];
        size_t j = 0;
        for(size_t i = 0; i < kMaxBlockSize; i ++)
        {
            if(i > kBlockSize[j]) ++j;
            m_pBlockSizeLookup[i] = j;
        }

        m_pAllocators = new Allocator[kNumBlockSize];
        for (size_t i = 0; i < kNumBlockSizes; i++)
        {
            m_pAllocators[i].Reset(kBlockSizes[i], kPageSize, kAlignment);
        }
        s_bInitialized = true;
    }
    return 0;
}