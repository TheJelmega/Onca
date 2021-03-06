#pragma once
#if __RESHARPER__
#include "InPlaceStackAllocator.h"
#endif

#include "core/Assert.h"

namespace Onca::Alloc
{
	template<usize Size, usize MaxAlignment>
	InPlaceStackAllocator<Size, MaxAlignment>::InPlaceStackAllocator() noexcept
		: m_mem()
		, m_head(0)
	{
		STATIC_ASSERT(Math::IsPowOf2(MaxAlignment), "Max alignment needs to be a power of 2");
		STATIC_ASSERT(Size & ~(MaxAlignment - 1), "Memory size needs to be a multiple of maxAlignment");
	}

	template<usize Size, usize MaxAlignment>
	auto InPlaceStackAllocator<Size, MaxAlignment>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= MaxAlignment, "Alignment cannot be larger than is allowed by the allocator");
		ASSERT(Math::IsPowOf2(align), "Alignment needs to be a power of 2");

		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (MaxAlignment - diff) & mask;
		const usize paddedSize = size + padding;

		if (m_head + paddedSize > m_mem + Size) UNLIKELY
			return nullptr;

		u8* ptr = m_head;
		m_head += paddedSize;

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, padding, isBacking);
#endif

		return { ptr, this, Math::Log2(align), size, isBacking };
	}

	template<usize Size, usize MaxAlignment>
	void InPlaceStackAllocator<Size, MaxAlignment>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
#if ENABLE_ASSERT || ENABLE_ALLOC_STATS
		const usize align = mem.Align();
		const usize size = mem.Size();
		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (MaxAlignment - diff) & mask;
		const usize paddedSize = size + padding;

		ASSERT(mem.Ptr() == m_head - paddedSize, "Deallocations can only happen from the top of the stack");
#endif
		
		m_head = mem.Ptr();

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(size, padding, mem.IsBackingMem());
#endif
	}

	template <usize Size, usize MaxAlignment>
	bool InPlaceStackAllocator<Size, MaxAlignment>::OwnsInternal(const MemRef<u8>& mem) noexcept
	{
		u8* ptr = mem.Ptr();
		return ptr >= m_mem && ptr < m_mem + Size;
	}
}
