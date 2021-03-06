#pragma once
#if __RESHARPER__
#include "Dlist.h"
#endif

namespace Onca
{
	template <typename T>
	DList<T>::Iterator::Iterator() noexcept
		: m_node()
	{
	}

	template <typename T>
	auto DList<T>::Iterator::operator->() const noexcept -> T*
	{
		return &m_node->val;
	}

	template <typename T>
	auto DList<T>::Iterator::operator*() const noexcept -> T&
	{
		return m_node->val;
	}

	template <typename T>
	auto DList<T>::Iterator::operator++() noexcept -> Iterator
	{
		if (m_node)
			m_node = m_node->next;
		return *this;
	}

	template <typename T>
	auto DList<T>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ m_node };
		operator++();
		return it;
	}

	template <typename T>
	auto DList<T>::Iterator::operator--() noexcept -> Iterator
	{
		if (m_node)
			m_node = m_node->prev;
		return *this;
	}

	template <typename T>
	auto DList<T>::Iterator::operator--(int) noexcept -> Iterator
	{
		Iterator it{ m_node };
		operator--();
		return it;
	}

	template <typename T>
	auto DList<T>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_node };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <typename T>
	auto DList<T>::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_node };
		for (usize i = 0; i < count; ++i)
			--it;
		return it;
	}

	template <typename T>
	auto DList<T>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template <typename T>
	auto DList<T>::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator--();
		return *this;
	}

	template <typename T>
	auto DList<T>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_node == other.m_node;
	}

	template <typename T>
	auto DList<T>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename T>
	DList<T>::Iterator::Iterator(const MemRef<Node>& node) noexcept
		: m_node(node)
	{
	}

	template <typename T>
	DList<T>::DList(Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
	}

	template <typename T>
	DList<T>::DList(usize count, Alloc::IAllocator& alloc) noexcept requires NoThrowDefaultConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		FillDefault(count);
	}

	template <typename T>
	DList<T>::DList(usize count, const T& val, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Fill(count, val);
	}

	template <typename T>
	DList<T>::DList(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(il);
	}

	template <typename T>
	template <ForwardIterator It>
	DList<T>::DList(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(begin, end);
	}

	template <typename T>
	DList<T>::DList(const DList& other) noexcept requires CopyConstructible<T>
		: m_head(other.GetAllocator())
		, m_tail(nullptr)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T>
	DList<T>::DList(const DList& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T>
	DList<T>::DList(DList&& other) noexcept
		: m_head(Move(other.m_head))
		, m_tail(Move(other.m_tail))
	{
	}

	template <typename T>
	DList<T>::~DList() noexcept
	{
		Clear();
	}

	template <typename T>
	auto DList<T>::operator=(const InitializerList<T>& il) noexcept -> DList<T>& requires CopyConstructible<T>
	{
		Assign(il);
		return *this;
	}

	template <typename T>
	auto DList<T>::operator=(const DList& other) noexcept -> DList<T>& requires CopyConstructible<T>
	{
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <typename T>
	auto DList<T>::operator=(DList&& other) noexcept -> DList<T>&
	{
		Clear();
		m_head = Move(other.m_head);
		m_tail = Move(other.m_tail);
		return *this;
	}

	template <typename T>
	template <ForwardIterator It>
	void DList<T>::Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (It it = begin; it != end; ++it)
			Add(Move(T{ *it }));
	}

	template <typename T>
	void DList<T>::Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (const T* it = il.begin(); it != il.end(); ++it)
			Add(Move(T{ *it }));
	}

	template <typename T>
	void DList<T>::Fill(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			Add(Move(T{ val }));
	}

	template <typename T>
	void DList<T>::FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			EmplaceBack();
	}

	template <typename T>
	void DList<T>::Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>
	{
		Iterator it{ m_head };
		Iterator end;
		usize i = 0;

		for (; i < newSize && it != end; ++i, ++it)
			EMPTY_FOR_BODY;

		// too large
		if (it != end)
		{
			m_tail = it.m_node->prev;
			m_tail->next = {};
			NodeRef curNode = it.m_node;
			do
			{
				NodeRef next = curNode->next;
				curNode.Dealloc();
				curNode = Move(next);
			} while (curNode);
		}
		// too small
		else
		{
			for (; i < newSize; ++i)
				Add(val);
		}
	}

	template <typename T>
	void DList<T>::Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>
	{
		Iterator it{ m_head };
		Iterator end;
		usize i = 0;

		for (; i < newSize && it != end; ++i, ++it)
			EMPTY_FOR_BODY;

		// too large
		if (it != end)
		{
			m_tail = it.m_node->prev;
			m_tail->next = {};
			NodeRef curNode = it.m_node;
			do
			{
				NodeRef next = curNode->next;
				curNode.Dealloc();
				curNode = Move(next);
			} while (curNode);
		}
		// too small
		else
		{
			for (; i < newSize; ++i)
				EmplaceBack();
		}
	}

	template <typename T>
	void DList<T>::Add(const T& val) noexcept requires CopyConstructible<T>
	{
		Add(std::move(T{ val }));
	}

	template <typename T>
	void DList<T>::Add(T&& val) noexcept
	{
		NodeRef node = CreateNode(Move(val));

		if (m_tail)
		{
			m_tail->next = node;
			node->prev = m_tail;
			m_tail = node;
		}
		else
		{
			m_tail = m_head = node;
		}
	}

	template <typename T>
	void DList<T>::Add(const DList& other) requires CopyConstructible<T>
	{
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Add(Move(T{ *it }));
	}

	template <typename T>
	void DList<T>::Add(DList&& other)
	{
		if (other.IsEmpty())
			return;

		if (m_head.GetAlloc() == other.GetAllocator())
		{
			if (m_tail)
			{
				m_tail->next = other.m_head;
				other.m_head->prev = m_tail;
				m_tail = other.m_tail;
			}
			else
			{
				m_head = other.m_head;
				m_tail = other.m_tail;
			}
		}
		else
		{
			NodeRef node = other.m_head;
			while (node)
			{
				Add(Move(node->val));
				NodeRef next = node->next;
				node.Dealloc();
				node = next;
			}
		}
		other.m_head = other.m_tail = nullptr;
	}

	template <typename T>
	template <typename ... Args>
		requires ConstructableFrom<T, Args...>
	void DList<T>::EmplaceBack(Args&&... args) noexcept
	{
		Add(Move(T{ args... }));
	}

	template <typename T>
	auto DList<T>::Insert(ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		return Insert(it, Move(T{ val }));
	}

	template <typename T>
	auto DList<T>::Insert(ConstIterator& it, T&& val) noexcept -> Iterator
	{
		NodeRef next = it.m_node;
		NodeRef prev = next->prev;

		NodeRef node = CreateNode(Move(val));
		node->next = next;
		next->prev = node;

		if (prev)
		{
			node->prev = prev;
			prev->next = node;
		}
		else
		{
			m_head = node;
		}

		return Iterator{ node };
	}

	template <typename T>
	auto DList<T>::Insert(ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		NodeRef next = it.m_node;
		NodeRef prev = next->prev;

		NodeRef firstNode = CreateNode(Move(T{ val }));
		if (prev)
		{
			firstNode->prev = prev;
			prev->next = firstNode;
		}
		else
		{
			m_head = firstNode;
		}

		prev = firstNode;
		for (usize i = 1; i < count; ++i)
		{
			NodeRef node = CreateNode(Move(T{ val }));
			node->prev = prev;
			prev->next = node;
			prev = node;
		}

		prev->next = next;
		next->prev = prev;

		return Iterator{ firstNode };
	}

	template <typename T>
	template <ForwardIterator It>
	auto DList<T>::Insert(ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>
	{
		NodeRef endNode = it.m_node;
		NodeRef prev = endNode->prev;

		It valIt = begin;
		NodeRef firstNode = CreateNode(Move(T{ *valIt }));
		if (prev)
		{
			firstNode->prev = prev;
			prev->next = firstNode;
		}
		else
		{
			m_head = firstNode;
		}

		prev = firstNode;
		++valIt;
		for (; valIt != end; ++valIt)
		{
			NodeRef node = CreateNode(Move(T{ *valIt }));
			node->prev = prev;
			prev->next = node;
			prev = node;
		}

		if (endNode)
		{
			prev->next = endNode;
			endNode->prev = prev;
		}
		else
		{
			m_tail = prev;
		}

		return Iterator{ firstNode };
	}

	template <typename T>
	auto DList<T>::Insert(ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>
	{
		return Insert(it, il.begin(), il.end());
	}

	template <typename T>
	auto DList<T>::Insert(ConstIterator& it, const DList& other) noexcept -> Iterator requires CopyConstructible<T>
	{
		return Insert(it, other.Begin(), other.End());
	}

	template <typename T>
	auto DList<T>::Insert(ConstIterator& it, DList&& other) noexcept -> Iterator
	{
		if (other.IsEmpty())
			return it;

		NodeRef endNode = it.m_node;
		NodeRef prev = endNode->prev;

		NodeRef firstNode;
		if (m_head.GetAlloc() == other.GetAllocator())
		{
			if (prev)
			{
				other.m_head->prev = prev;
				prev->next = other.m_head;
			}
			else
			{
				m_head = other.m_head;
			}
			if (endNode)
			{
				other.m_tail->next = endNode;
				endNode->prev = other.m_tail;
			}
			firstNode = other.m_head;
		}
		else
		{
			NodeRef otherNode = other.m_head;

			firstNode = CreateNode(Move(otherNode->val));
			if (prev)
			{
				firstNode->prev = prev;
				prev->next = firstNode;
			}
			else
			{
				m_head = firstNode;
			}

			NodeRef tmp = otherNode->next;
			otherNode.Dealloc();
			otherNode = tmp;

			prev = firstNode;
			while (otherNode)
			{
				NodeRef node = CreateNode(Move(otherNode->val));
				node->prev = prev;
				prev->next = node;
				prev = node;

				NodeRef tmp = otherNode->next;
				otherNode.Dealloc();
				otherNode = tmp;
			}

			if (endNode)
			{
				prev->next = endNode;
				endNode->prev = prev;
			}
			else
			{
				m_tail = prev;
			}
		}

		other.m_head = other.m_tail = nullptr;
		return Iterator{ firstNode };
	}

	template <typename T>
	template <typename ... Args>
		requires ConstructableFrom<T, Args...>
	auto DList<T>::Emplace(ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		return Insert(it, Move(T{ args... }));
	}

	template <typename T>
	void DList<T>::AddFront(const T& val) noexcept requires CopyConstructible<T>
	{
		AddFront(Move(T{ val }));
	}

	template <typename T>
	void DList<T>::AddFront(T&& val) noexcept
	{
		NodeRef node = CreateNode(Move(val));
		node->next = m_head;
		if (m_head)
			m_head->prev = node;
		else
			m_tail = node;
		m_head = node;
	}

	template <typename T>
	void DList<T>::AddFront(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		NodeRef prev;
		for (usize i = 0; i < count; ++i)
		{
			NodeRef node = CreateNode(Move(T{ val }));
			node->prev = prev;
			if (prev)
				prev->next = node;
			prev = node;
		}

		prev->next = m_head;
		if (m_head)
			m_head->prev = prev;
		else
			m_tail = prev;
		m_head = prev;
	}

	template <typename T>
	template <ForwardIterator It>
	void DList<T>::AddFront(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		if (begin == end)
			return;

		It it = begin;
		NodeRef firstNode = CreateNode(Move(T{ *it }));

		NodeRef prev = firstNode;
		++it;
		for (; it != end; ++it)
		{
			NodeRef node = CreateNode(Move(T{ *it }));
			node->prev = prev;
			if (prev)
				prev->next = node;
			prev = node;
		}

		prev->next = m_head;
		if (m_head)
			m_head->prev = firstNode;
		else
			m_tail = firstNode;
		m_head = firstNode;
	}

	template <typename T>
	void DList<T>::AddFront(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		AddFront(il.begin(), il.end());
	}

	template <typename T>
	void DList<T>::AddFront(const DList& other) noexcept requires CopyConstructible<T>
	{
		AddFront(other.Begin(), other.End());
	}

	template <typename T>
	void DList<T>::AddFront(DList&& other) noexcept
	{
		if (other.IsEmpty())
			return;

		NodeRef prev;
		NodeRef otherNode = other.m_head;

		if (m_head.GetAlloc() == other.GetAllocator())
		{
			if (m_head)
			{
				m_head->prev = other.m_tail;
				other.m_tail->next = m_head;
				m_head = other.m_head;
			}
			else
			{
				m_head = other.m_head;
				m_tail = other.m_tail;
			}
		}
		else
		{
			NodeRef firstNode = CreateNode(Move(T{ otherNode->val }));
			NodeRef tmp = otherNode->next;
			otherNode.Dealloc();
			otherNode = tmp;

			prev = firstNode;
			while (otherNode)
			{
				NodeRef node = CreateNode(Move(T{ otherNode->val }));
				node->prev = prev;
				if (prev)
					prev->next = node;
				prev = node;

				NodeRef tmp = otherNode->next;
				otherNode.Dealloc();
				otherNode = tmp;
			}

			prev->next = m_head;
			if (m_head)
				m_head->prev = firstNode;
			else
				m_tail = firstNode;
			m_head = firstNode;
		}
		other.m_head = other.m_tail = nullptr;
	}

	template <typename T>
	template <typename ... Args>
		requires ConstructableFrom<T, Args...>
	void DList<T>::EmplaceFront(Args&&... args) noexcept
	{
		AddFront(Move(T{ args... }));
	}

	template <typename T>
	void DList<T>::Clear() noexcept
	{
		NodeRef node = m_head;
		Alloc::IAllocator* pAlloc = node.GetAlloc();
		while (node)
		{
			NodeRef next = node->next;
			node->val.~T();
			node.Dealloc();
			node = next;
		}
		m_head = NodeRef{ pAlloc };
		m_tail = nullptr;
	}

	template <typename T>
	void DList<T>::Pop() noexcept
	{
		if (!m_tail)
			return;

		NodeRef node = m_tail;
		if (node->prev)
		{
			m_tail = node->prev;
			m_tail->next = nullptr;
		}
		else
		{
			Alloc::IAllocator* pAlloc = m_head.GetAlloc();
			m_head = NodeRef{ pAlloc };
			m_tail = nullptr;
		}

		node->val.~T();
		node.Dealloc();
	}

	template <typename T>
	void DList<T>::PopFront() noexcept
	{
		if (!m_head)
			return;

		NodeRef node = m_head;
		if (node->next)
		{
			m_head = node->next;
			m_head->prev = nullptr;
		}
		else
		{
			Alloc::IAllocator* pAlloc = m_head.GetAlloc();
			m_head = NodeRef{ pAlloc };
			m_tail = nullptr;
		}

		node->val.~T();
		node.Dealloc();
	}

	template <typename T>
	void DList<T>::Erase(const Iterator& it) noexcept
	{
		Erase(it, 1);
	}

	template <typename T>
	void DList<T>::Erase(const Iterator& it, usize count) noexcept
	{
		Alloc::IAllocator* pAlloc = m_head.GetAlloc();
		NodeRef node = it.m_node;
		NodeRef prev = node->prev;

		for (usize i = 0; i < count; ++i)
		{
			NodeRef next = node->next;
			node->val.~T();
			node.Dealloc();
			node = next;
		}

		if (node)
		{
			if (prev)
			{
				prev->next = node;
				node->prev = prev;
			}
			else
			{
				m_head = node;
			}
		}
		else
		{
			if (prev)
			{
				m_tail = prev;
			}
			else
			{
				m_head = NodeRef{ pAlloc };
				m_tail = nullptr;
			}
		}
	}

	template <typename T>
	void DList<T>::Erase(const Iterator& begin, const Iterator& end) noexcept
	{
		Alloc::IAllocator* pAlloc = m_head.GetAlloc();
		NodeRef node = begin.m_node;
		NodeRef prev = node->prev;

		while (node != end.m_node)
		{
			NodeRef next = node->next;
			node->val.~T();
			node.Dealloc();
			node = next;
		}

		if (node)
		{
			if (prev)
			{
				prev->next = node;
				node->prev = prev;
			}
			else
			{
				m_head = node;
			}
		}
		else
		{
			if (prev)
			{
				m_tail = prev;
			}
			else
			{
				m_head = NodeRef{ pAlloc };
				m_tail = nullptr;
			}
		}
	}

	template <typename T>
	void DList<T>::Reverse() noexcept
	{
		NodeRef node = m_head;
		while (node)
		{
			NodeRef next = node->next;
			node->next = node->prev;
			node->prev = next;
			node = next;
		}
		node = Move(m_head);
		m_head = Move(m_tail);
		m_tail = Move(node);
	}

	template <typename T>
	auto DList<T>::Size() const noexcept -> usize
	{
		usize size = 0;
		for (NodeRef node = m_head; node; node = node->next)
			++size;
		return size;
	}

	template <typename T>
	auto DList<T>::IsEmpty() const noexcept -> bool
	{
		return !m_head;
	}

	template <typename T>
	auto DList<T>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_head.GetAlloc();
	}

	template <typename T>
	auto DList<T>::Front() noexcept -> T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_head->val;
	}

	template <typename T>
	auto DList<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_head->val;
	}

	template <typename T>
	auto DList<T>::Back() noexcept -> T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_tail->val;
	}

	template <typename T>
	auto DList<T>::Back() const noexcept -> const T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_tail->val;
	}

	template <typename T>
	auto DList<T>::Begin() noexcept -> Iterator
	{
		return Iterator{ m_head };
	}

	template <typename T>
	auto DList<T>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_head };
	}

	template <typename T>
	auto DList<T>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <typename T>
	auto DList<T>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <typename T>
	auto DList<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T>
	auto DList<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto DList<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto DList<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T>
	auto DList<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto DList<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto DList<T>::CreateNode(T&& val) noexcept -> NodeRef
	{
		NodeRef node = m_head.GetAlloc()->template Allocate<Node>();
		new (&node->val) T{ Move(val) };
		node->prev = node->next = nullptr;
		return node;
	}
}
