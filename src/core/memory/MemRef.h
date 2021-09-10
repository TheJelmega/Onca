#pragma once
#include "core/MinInclude.h"

namespace Core
{
	namespace Alloc
	{
		class IAllocator;
	}

	/**
	 * \brief Struct that encapsulates all info for an allocation
	 * \tparam T Underlying type
	 *
	 * \note If both MemRefs are invalid, == or != can return invalid values, depending on the use-case, as it checks if the referenced allocators are the same
	 */
	// TODO: introduce pointer caching system as converting handles to pointer can be slow
	template<typename T>
	class MemRef
	{
	public:
		/**
		 * Create an invalid MemRef
		 */
		MemRef() noexcept;
		/**
		 * Create an invalid MemRef
		 */
		MemRef(nullptr_t) noexcept;
		/**
		 * Create an 'invalid' MemRef with an allocator that can be used for allocations
		 * \param[in] pAlloc Allocator to be used
		 */
		explicit MemRef(Alloc::IAllocator* pAlloc) noexcept;
		MemRef(usize handle, Alloc::IAllocator* pAlloc, u8 log2Align, usize size, bool isBacking) noexcept;

		MemRef(const MemRef& other) noexcept;
		MemRef(MemRef&& other) noexcept;

		auto operator=(nullptr_t) noexcept -> MemRef<T>&;
		auto operator=(const MemRef& other) noexcept -> MemRef<T>&;
		auto operator=(MemRef&& other) noexcept -> MemRef<T>&;
		
		/**
		 * Get the actual pointer to memory
		 * \return Pointer to underlying memory
		 */
		auto Ptr() const noexcept -> T*;
		/**
		 * Get the allocator used for the allocation
		 * \return Allocator used for the allocation
		 */
		auto GetAlloc() const noexcept -> Alloc::IAllocator*;
		/**
		 * Get the alignment of the allocation
		 * \return Alignment of the allocation
		 */
		auto Align() const noexcept -> u16;
		/**
		 * Get the size of the allocation
		 * \return Size of the allocation
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Get the raw handle
		 * \return Get the raw handle
		 * \note This function is only useful to allocators, as they understand how their handles are stored
		 */
		auto GetRawHandle() const noexcept -> usize;
		/**
		 * Check if the memory backs an allocator
		 * \return Whether the memory backs an allocator
		 */
		auto IsBackingMem() const noexcept -> bool;
		/**
		 * Checks whether the allocation is valid
		 * \return Whether the allocation is valid
		 */
		auto IsValid() const noexcept -> bool;
		/**
		 * Deallocates the allocation
		 */
		auto Dealloc() noexcept -> void;

		/**
		 * \brief Cast a MemRef to use a new underlying type
		 * \tparam U New underlying type
		 * \return Casted MemRef
		 */
		template<typename U>
		auto As() noexcept -> MemRef<U>;
		/**
		 * \brief Cast a MemRef to use a new underlying type
		 * \tparam U New underlying type
		 * \return Casted MemRef
		 */
		template<typename U>
		auto As() const noexcept -> const MemRef<U>&;
		
		auto operator->() const noexcept -> T*;
		auto operator*() const noexcept -> T&;
		
		explicit operator bool() const noexcept;

		template<typename U>
		auto operator==(const MemRef<U>& other) const noexcept -> bool;
		template<typename U>
		auto operator!=(const MemRef<U>& other) const noexcept -> bool;

	private:

		usize              m_handle;                                          ///< Handle to allocated memory (~usize(0) == Invalid handle)
		Alloc::IAllocator* m_pAlloc;                                          ///< Allocator used to allocate the allocation
		u8                 m_log2Align    : 7;                                ///< Log2 of alignment
		bool               m_isBackingMem : 1;                                ///< Whether the memory backs an allocator
		usize              m_size         : (sizeof(usize) - sizeof(u8)) * 8; ///< Size of the allocation
	};

	/**
	 * \brief Concept defining a valid deleter for a memory reference
	 * \tparam D Type of the deleter
	 * \tparam T Underlying type of the MemRef
	 */
	template<typename D, typename T>
	concept MemRefDeleter = requires(D d, MemRef<T>&& ref)
	{
		DefaultConstructible<D>;
		{ D{}(Move(ref)) } noexcept;
	};
	
}

#include "MemRef.inl"