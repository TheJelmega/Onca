#pragma once
#include "core/allocator/IAllocator.h"

namespace Onca::Alloc
{
	/**
	 * \brief An allocator that allocates memory from subdivided blocks
	 *
	 * A buddy allocator exist of a big block of memory that can be subdivided into separate smaller chunks.
	 * When memory is allocated, the smallest sized subdivision is found that fits the memory, which is then marked as used,
	 * after which the higher subdivision are set to split.
	 * Maximum alignment is determined by the smallest block size.
	 *
	 * Below is an example of a buddy allocator with 4 subdivisions and their indices
	 *
	 *                                                                                   div idx
	 * +-------------------------------------------------------------------------------+
	 * |                                       0                                       | 0
	 * +---------------------------------------+---------------------------------------+
	 * |                   1                   |                   2                   | 1
	 * +-------------------+-------------------+-------------------+-------------------+
	 * |         3         |         4         |         5         |         6         | 2
	 * +---------+---------+---------+---------+---------+---------=---------+---------+
	 * |    7    |    8    |    9    |   10    |   11    |   12    |   13    |   14    | 3
	 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
	 * | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 4
	 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
	 *
	 * \tparam Size Size of the managed memory
	 * \tparam MaxSubDivisions Maximum number of subdivision
	 *
	 */
	template<usize Size, u8 MaxSubDivisions>
	class BuddyAllocator final : public IMemBackedAllocator
	{
	private:
		static constexpr u8 FreeFlag  = 0b00;
		static constexpr u8 SplitFlag = 0b01;
		static constexpr u8 UsedFlag  = 0b10;

	public:
		/**
		 * Create a buddy allocator
		 * \param[in]pBackingAlloc Allocator to create the underlying memory block
		 */
		BuddyAllocator(IAllocator* pBackingAlloc) noexcept;
		BuddyAllocator(BuddyAllocator&&) noexcept = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;

	public:
		/**
		 * Get the number of bytes needed to managed the divisions
		 * \return Number of bytes needed to manage the divisions
		 */
		static constexpr auto CalculateManagementSize() noexcept -> usize
		{
			constexpr usize numFlags = (1ull << (MaxSubDivisions + 1)) - 1;
			constexpr usize numManagmentBytes = (numFlags + 3) / 4;
			constexpr usize numBlocks = (numManagmentBytes + SmallestBlockSize - 1) / SmallestBlockSize;
			return numBlocks * SmallestBlockSize;
		}

		/**
		 * Calculate the size class and block size for an allocation of a certain size
		 * \param[in]size Size of allocation
		 * \return Size class and block size for allocation
		 */
		auto CalculateSizeClassAndBlockSize(usize size) noexcept -> Tuple<usize, usize>;

		/**
		 * Get the index of a free division fitting the requested size class
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]neededClass Size class needed for allocation
		 * \return Index of a free division, or usize(-1) if no free division is found
		 */
		auto GetIdx(u8* pManagementInfo, usize neededClass) noexcept -> usize;

		/**
		 * Get the index of a free division fitting the requested size class
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]neededClass Size class needed for allocation
		 * \param[in]curClass Current size class to work in
		 * \param[in]curDivIdx CUrrent division index
		 * \return Index of a free division, or usize(-1) if no free division is found
		 */
		auto GetSubIdx(u8* pManagementInfo, usize neededClass, usize curClass, usize curDivIdx) noexcept -> usize;

		/**
		 * Get the flag for the division at a certain index
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]divIdx Division index
		 * \return Flag of the division
		 */
		auto GetDivFlag(u8* pManagementInfo, usize divIdx) noexcept -> u8;
		/**
		 * Get the flags for both divisions at a certain index
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]divIdx Division index of the left child
		 * \return Flag of the division
		 */
		auto GetDivFlags(u8* pManagementInfo, usize divIdx) noexcept -> Tuple<u8, u8>;
		/**
		 * Set the flag for the division at a certain index
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]divIdx Division index
		 * \param[in]flag Flag to se
		 */
		void SetDivFlag(u8* pManagementInfo, usize divIdx, u8 flag) noexcept;

		/**
		 * Mark a division as used and update the parent divisions
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]divIdx Index to mark
		 */
		void Mark(u8* pManagementInfo, usize divIdx) noexcept;
		/**
		 * Unmark a division as used and update the parent divisions
		 * \param[in]pManagementInfo Pointer to management info
		 * \param[in]divIdx Index to unmark
		 */
		void Unmark(u8* pManagementInfo, usize divIdx) noexcept;

		static constexpr usize SmallestBlockSize = Size >> MaxSubDivisions;
		static constexpr usize ManagementSize = CalculateManagementSize();
		
		Threading::Mutex m_mutex;         ///< Mutex to guard division flag modifications
	};
}

#include "BuddyAllocator.inl"