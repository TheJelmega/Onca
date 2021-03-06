#pragma once
#if __RESHARPER__
#include "PoolAllocator.h"
#endif

namespace Onca::Alloc
{
	template<usize BlockSize, usize NumBlocks>
	PoolAllocator<BlockSize, NumBlocks>::PoolAllocator(IAllocator* pBackingAlloc) noexcept
		: IMemBackedAllocator(nullptr)
		, m_head(0)
	{
		STATIC_ASSERT(Math::IsPowOf2(BlockSize), "Block size needs to be a power of 2");
		STATIC_ASSERT(BlockSize >= sizeof(u8*), "Block size needs to be larger or equal than the size of a pointer");
		STATIC_ASSERT(NumBlocks != 0, "Needs to have at least 1 block");
		
		constexpr usize memSize = BlockSize * NumBlocks;
		m_mem = pBackingAlloc->Allocate<u8>(memSize, BlockSize, true);

		// Setup free blocks
		m_head = m_mem.Ptr();
		u8* pEnd = m_head + memSize - BlockSize;

		for (u8* ptr = m_head; ptr < pEnd;)
		{
			u8* pNext = ptr + BlockSize;
			*reinterpret_cast<u8**>(ptr) = pNext;
			ptr = pNext;
		}
	}

	template<usize BlockSize, usize NumBlocks>
	PoolAllocator<BlockSize, NumBlocks>::PoolAllocator(PoolAllocator&& other) noexcept
		: IMemBackedAllocator(Move(other.m_mem))
		, m_head(Move(other.m_head))
	{
	}
	
	template<usize BlockSize, usize NumBlocks>
	auto PoolAllocator<BlockSize, NumBlocks>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= BlockSize, "Cannot have a greater aligment than the blocksize");
		ASSERT(size <= BlockSize, "Cannot allocate more than the blocksize");
		ASSERT(Math::IsPowOf2(align), "Alignment needs to be a power of 2");
		
		u8* ptr = m_head;
		u8* next;
		do
		{
			if (!ptr)
				return nullptr;

			next = *reinterpret_cast<u8**>(ptr);
		}
		while (!m_head.CompareExchangeWeak(ptr, next));

#if ENABLE_ALLOC_STATS
		const usize overhead = BlockSize - size;
		m_stats.AddAlloc(size, overhead, isBacking);
#endif
		
		return { ptr, this, Math::Log2(align), size, isBacking };
	}

	template<usize BlockSize, usize NumBlocks>
	void PoolAllocator<BlockSize, NumBlocks>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
		u8* ptr = mem.Ptr();
		u8** pBlock = reinterpret_cast<u8**>(ptr);
		u8* cur;
		do
		{
			cur = m_head;
			*pBlock = cur;
		}
		while (!m_head.CompareExchangeWeak(cur, ptr));

#if ENABLE_ALLOC_STATS
		const usize size = mem.Size();
		const usize overhead = BlockSize - size;
		m_stats.RemoveAlloc(size, overhead, mem.IsBackingMem());
#endif
	}
}
