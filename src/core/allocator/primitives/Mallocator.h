#pragma once
#include "core/allocator/IAllocator.h"

namespace Onca::Alloc
{
	/**
	 * \brief An allocator that uses malloc
	 */
	class CORE_API Mallocator final : public IAllocator
	{
	public:
		Mallocator() = default;
		Mallocator(Mallocator&&) = default;
		~Mallocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;
		auto OwnsInternal(const MemRef<u8>& mem) noexcept -> bool override;
	};
}
