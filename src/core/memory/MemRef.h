#pragma once
#include "core/MinInclude.h"

namespace Onca
{
	namespace Alloc
	{
		class IAllocator;
	}

#pragma pack(push, 1)

	/**
	 * \brief Struct that encapsulates all info for an allocation
	 * \tparam T Underlying type
	 *
	 * \note If both MemRefs are invalid, == or != can return invalid values, depending on the use-case, as it checks if the referenced allocators are the same
	 */
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
		MemRef(T* pAddr, Alloc::IAllocator* pAlloc, u8 log2Align, usize size, bool isBacking) noexcept;

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
		void Dealloc() noexcept;

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

		/**
		 * Set the allocator of the memory
		 * \param[in] pAlloc Pointer to the allocator
		 * \warning This function should only be used in allocators that depend on sub-allocators
		 * \warning Setting an allocator that does not match the allocation is undefined behavior
		 */
		void SetAlloc(Alloc::IAllocator* pAlloc) noexcept;
		
		auto operator->() const noexcept -> T*;
		auto operator*() const noexcept -> T&;
		
		explicit operator bool() const noexcept;

		template<typename U>
		auto operator==(const MemRef<U>& other) const noexcept -> bool;
		template<typename U>
		auto operator!=(const MemRef<U>& other) const noexcept -> bool;

	private:
		T*                 m_pAddr;                                           ///< Pointer
		Alloc::IAllocator* m_pAlloc;                                          ///< Allocator used to allocate the allocation
		usize              m_log2Align    : 7;                                ///< Log2 of alignment
		usize              m_isBackingMem : 1;                                ///< Whether the memory backs an allocator
		usize              m_size         : (sizeof(usize) - sizeof(u8)) * 8; ///< Size of the allocation
	};

	
	static_assert(sizeof(MemRef<u8>) == 24, "Invalid MemRef<T> size");

#pragma pack(pop)

	/**
	 * Copy a region of memory from one location to another
	 * \tparam T Type of MemRef
	 * \param[in] dst Destination MemRef
	 * \param[in] src Source MemRef
	 * \param[in] numElems Number of elements to copy
	 * \note Regions cannot overlap
	 */
	template <typename T>
	void MemRefCpy(MemRef<T>& dst, const MemRef<T>& src, usize numElems) noexcept;
	/**
	 * Copy a region of memory from one location to another
	 * \tparam T Type of MemRef
	 * \param[in] dst Destination MemRef
	 * \param[in] dstOffset Offset in destination MemRef
	 * \param[in] src Source MemRef
	 * \param[in] srcOffset Offset in source MemRef
	 * \param[in] numElems Number of elements to copy
	 * \note Regions cannot overlap
	 */
	template <typename T>
	void MemRefCpy(MemRef<T>& dst, usize dstOffset, const MemRef<T>& src, usize srcOffset, usize numElems) noexcept;

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