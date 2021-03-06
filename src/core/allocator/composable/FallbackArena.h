#pragma once
#include "core/allocator/IAllocator.h"

namespace Onca::Alloc
{
	/**
	 * An allocator that can fallback to another allocator if the main allcoator fails to allocate
	 * \tparam MainAlloc Main allocator type
	 * \tparam FallbackAlloc Allocator type to use when the main allocator is out of memory
	 */
	template<ImplementsIAllocator MainAlloc, ImplementsIAllocator FallbackAlloc>
	class FallbackArena final : public IAllocator
	{
	public:
		/**
		 * Create a fallback arena
		 * \param[in] main Main allocator
		 * \param[in] fallback Allocator to use when main allocator cannot allocate
		 */
		FallbackArena(MainAlloc&& main, FallbackAlloc&& fallback);
		FallbackArena(FallbackArena&& main) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;

	private:
		MainAlloc     m_main;     ///< Main allocator
		FallbackAlloc m_fallback; ///< Fallback allocator
	};

	
}

#include "FallbackArena.inl"