#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"
#include "core/threading/Sync.h"

namespace Onca::Alloc
{
	/**
	 * Contains statistics about an allocator
	 */
	struct AllocatorStats
	{
		Threading::Mutex m_statMutex; ///< Mutex protecting stats
		
		usize curMemoryUse       = 0; ///< Current memory allocated
		usize curAllocs          = 0; ///< Current allocations
		usize curOverhead        = 0; ///< Current memory overhead
		usize curBackingMemory   = 0; ///< Current backing memory

		usize maxMemoryUsed      = 0; ///< Maximum concurrent memory allocated
		usize maxAllocs          = 0; ///< Maximum concurrent allocations
		usize maxOverhead        = 0; ///< Maximum concurrent overhead
		usize maxBackingMemory   = 0; ///< Max concurrent backing memory
		
		usize totalMemoryUsed    = 0; ///< Total memory allocated over the allocator's lifetime
		usize totalAllocs        = 0; ///< Total allocation over the allocator's lifetime
		usize totalOverhead      = 0; ///< Total overhead over the allocation's lifetime
		usize totalBackingMemory = 0; ///< Total backing memory over the allocation's lifetime

		usize lastDefragMoved    = 0; ///< Memory moved during last defragmentation
		usize lastDefragFreed    = 0; ///< Memory freed during last defragmentation
		usize totalDefragMoved   = 0; ///< Total memory moved during all defragmentations
		usize totalDefragFreed   = 0; ///< Total memory freed during all defragmentations

		/**
		 * Add a memory allocation to the stats
		 * \param[in] memUse Memory used by the allocation
		 * \param[in] overhead Memory overhead
		 * \param[in] isBacking Whether the memory backs another allocator
		 */
		void AddAlloc(usize memUse, usize overhead, bool isBacking) noexcept;
		/**
		 * Remove a memory allocation to the stats
		 * \param[in] memUse Memory used by the allocation
		 * \param[in] overhead Memory overhead
		 * \param[in] isBacking Whether the memory backs another allocator
		 */
		void RemoveAlloc(usize memUse, usize overhead, bool isBacking) noexcept;
		/**
		 * Reset the current memory stats
		 */
		void ResetCur() noexcept;

		/**
		 * Get the current memory statistics
		 * \param[out] memUse Current memory use
		 * \param[out] numAllocs Current number of allocations
		 * \param[out] overhead Current memory overhead
		 * \param[out] backingMem Current backing memory
		 */
		void GetCurStats(usize& memUse, usize& numAllocs, usize& overhead, usize& backingMem) noexcept;
	};

	// TODO: Go over ownership system + possible redo
	class CORE_API IAllocator
	{
	public:
		virtual ~IAllocator() noexcept = default;

		/**
		 * \brief Allocate a MemRef
		 * \tparam T Type to allocate memory for
		 * \param[in] size Minimum size of the allocation
		 * \param[in] align Alignment of the allocation
		 * \param[in] isBacking Whether the memory backs another allocator
		 * \return A MemRef with info about the allocated memory
		 */
		template<typename T>
		auto Allocate(usize size = sizeof(T), u16 align = alignof(T), bool isBacking = false) noexcept -> MemRef<T>;
		
		/**
		 * \brief Deallocate a MemRef
		 * \tparam T Underlying type of the MemRef
		 * \param[in] mem [move] MemRef to deallocate
		 */
		template<typename T>
		void Deallocate(MemRef<T>&& mem) noexcept;

		/**
		 * \brief 
		 * \tparam T Underlying type of the MemRef
		 * \param[in] mem MemRef to check ownership for
		 * \return True if the memory is owned by this allocator
		 */
		template<typename T>
		auto Owns(const MemRef<T>& mem) noexcept -> bool;

		/**
		 * Get statistics for this allocator 
		 * \return Alloc stats
		 */
		auto GetAllocStats() noexcept -> AllocatorStats&;

	protected:
		/**
		 * \brief Allocate a raw memory pointer
		 * \param[in] size Min size of the allocation (implementation dependent)
		 * \param[in] align Min alignment of the allocation
		 * \param[in] isBacking Whether the memory backs another allocator
		 * \return Tuple with the raw pointer to allocated memory (may be nullptr) and the allocator used to allocate (for nested allocators)
		 */
		virtual auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> = 0;
		/**
		 * \brief Deallocate a raw memory pointer
		 * \param[in] mem MemRef to deallocate
		 */
		virtual void DeallocateRaw(MemRef<u8>&& mem) noexcept = 0;
		/**
		 * \brief Check if the allocation was allocated by this allocator (this or child allocator)
		 * \param[in] mem MemRef to check
		 * \return If the allocation is owned by the allocator
		 */
		virtual auto OwnsInternal(const MemRef<u8>& mem) noexcept -> bool;
		
#if ENABLE_ALLOC_STATS
		AllocatorStats m_stats;
#endif
	};

	/**
	 * Allocator backed by a block of memory
	 */
	class CORE_API IMemBackedAllocator : public IAllocator
	{
	public:
		IMemBackedAllocator(MemRef<u8>&& mem) noexcept;
		~IMemBackedAllocator() noexcept override;

		/**
		 * \brief Allocate a raw memory pointer
		 * \param[in] size Min size of the allocation (implementation dependent)
		 * \param[in] align Min alignment of the allocation
		 * \param[in] isBacking Whether the memory backs another allocator
		 * \return Tuple with the raw pointer to allocated memory (may be nullptr) and the allocator used to allocate (for nested allocators)
		 */
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		/**
		 * \brief Deallocate a raw memory pointer
		 * \param[in] mem MemRef to deallocate
		 */
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;
		/**
		 * \brief Check if the allocation was allocated by this allocator (this or child allocator)
		 * \param[in] mem MemRef to check
		 * \return If the allocation is owned by the allocator
		 */
		auto OwnsInternal(const MemRef<u8>& mem) noexcept -> bool override;

	protected:
		MemRef<u8> m_mem; ///< Memory backing allocator
	};

	template<typename T>
	concept ImplementsIAllocator = DerivesFrom<T, IAllocator>;

}

#include "IAllocator.inl"