#include "Allocator.h"
#include <cassert>
#include <cstring>

#ifndef ALIGN
#define ALIGN(x, a)     (((x) + ((a) - 1)) & ~((a) - 1))
#endif

using namespace Acmen;

Acmen::Allocator::Allocator(size_t data_size, size_t page_size, size_t alignment)
    :m_pPageList(nullptr), m_pFreeList(nullptr)
{
    Reset(data_size, page_size, alignment);
}

Acmen::Allocator::~Allocator()
{
    FreeAll();
}


void Acmen::Allocator::Reset(size_t data_size, size_t page_size, size_t alignment)
{
    FreeAll();
    m_szDataSize = data_size;
    m_szPageSize = page_size;

    size_t minimal_size = (sizeof(BlockHeader) > m_szDataSize) ? sizeof(BlockHeader) : m_szDataSize;
    #if defined(_DEBUG)
        
    #endif
}


