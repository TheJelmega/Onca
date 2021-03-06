#pragma once
#include "core/allocator/IAllocator.h"

namespace Onca::Alloc
{
	/**
	 * \brief A linear allocator (can not deallocate, only can reset) (NOT threadsafe)
	 *
	 * A linear allocator holds a chunk of memory, from which it can allocate memory from,
	 * but it can only allocate, as it has no knowledge on how to deallocate anything.
	 * Any new allocation made on the allocator, will be appended to the end and this will happen until the entire allocator is reset,
	 * allowing new memory to start at the beginning, but invalidating all former allocations made.
	 * Alignment is done using padding between allocations.
	 *
	 * begin                    head
	 * v                        v
	 * +------------------------+-----------------
	 * |4C696E65617229416C6C6F63|?????????????????
	 * +------------------------+-----------------
	 *
	 * \tparam Size Size of the managed memory
	 * \tparam BaseAlignment Alignment of initial offset
	 */
	template<usize Size, usize BaseAlignment>
	class LinearAllocator final : public IMemBackedAllocator
	{
	public:
		/**
		 * Create a linear allocator
		 * \param[in]pBackingAlloc Allocator to create the underlying memory block
		 */
		LinearAllocator(IAllocator* pBackingAlloc);
		LinearAllocator(LinearAllocator&&) = default;

		/**
		 * Reset the head of the allocator
		 * \note This will invalidate all memory owned by this allocator
		 */
		void Reset() noexcept;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;

	private:
		u8*        m_head; ///< Current location in the linear allocator
	};

}

#include "LinearAllocator.inl"