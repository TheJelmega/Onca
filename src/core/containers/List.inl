#pragma once
#if __RESHARPER__
#include "List.h"
#endif


namespace Onca
{
	template <typename T>
	List<T>::Iterator::Iterator() noexcept
		: m_node()
	{
	}

	template <typename T>
	auto List<T>::Iterator::operator->() const noexcept -> T*
	{
		return &m_node->val;
	}

	template <typename T>
	auto List<T>::Iterator::operator*() const noexcept -> T&
	{
		return m_node->val;
	}

	template <typename T>
	auto List<T>::Iterator::operator++() noexcept -> Iterator
	{
		if (m_node)
			m_node = m_node->next;
		return *this;
	}

	template <typename T>
	auto List<T>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator tmp{ *this };
		operator++();
		return tmp;
	}

	template <typename T>
	auto List<T>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it = *this;
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <typename T>
	auto List<T>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template <typename T>
	auto List<T>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_node == other.m_node;
	}

	template <typename T>
	auto List<T>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename T>
	List<T>::Iterator::Iterator(const NodeRef& node) noexcept
		: m_node(node)
	{
	}

	template <typename T>
	List<T>::List(Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
	}

	template <typename T>
	List<T>::List(usize count, Alloc::IAllocator& alloc) noexcept requires NoThrowDefaultConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		FillDefault(count);
	}

	template <typename T>
	List<T>::List(usize count, const T& val, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Fill(count, val);
	}

	template <typename T>
	List<T>::List(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(il);
	}

	template <typename T>
	template <ForwardIterator It>
	List<T>::List(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(begin, end);
	}

	template <typename T>
	List<T>::List(const List& other) noexcept requires CopyConstructible<T>
		: m_head(other.GetAllocator())
		, m_tail(nullptr)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T>
	List<T>::List(const List& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T>
	List<T>::List(List&& other) noexcept
		: m_head(Move(other.m_head))
		, m_tail(Move(other.m_tail))
	{
	}

	template <typename T>
	List<T>::~List() noexcept
	{
		Clear();
	}

	template <typename T>
	auto List<T>::operator=(const InitializerList<T>& il) noexcept -> List<T>& requires CopyConstructible<T>
	{
		Assign(il);
		return *this;
	}

	template <typename T>
	auto List<T>::operator=(const List& other) noexcept -> List<T>& requires CopyConstructible<T>
	{
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <typename T>
	auto List<T>::operator=(List&& other) noexcept -> List<T>&
	{
		Clear();
		m_head = Move(other.m_head);
		m_tail = Move(other.m_tail);
		return *this;
	}

	template <typename T>
	template <ForwardIterator It>
	void List<T>::Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (It it = begin; it != end; ++it)
			Add(Move(T{ *it }));
	}

	template <typename T>
	void List<T>::Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (const T* it = il.begin(); it != il.end(); ++it)
			Add(Move(T{ *it }));
	}

	template <typename T>
	void List<T>::Fill(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			Add(Move(T{ val }));
	}

	template <typename T>
	void List<T>::FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			EmplaceBack();
	}

	template <typename T>
	void List<T>::Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>
	{

		Iterator it{ m_head };
		Iterator end;
		Iterator last;
		usize i = 0;

		for (; i < newSize && it != end; ++i, ++it)
			last = it;

		// too large
		if (it != end)
		{
			m_tail = last.m_node;
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
	void List<T>::Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>
	{

		Iterator it{ m_head };
		Iterator end;
		Iterator last;
		usize i = 0;

		for (; i < newSize && it != end; ++i, ++it)
			last = it;

		// too large
		if (it != end)
		{
			m_tail = last.m_node;
			m_tail->next = nullptr;
			NodeRef curNode = it.m_node;
			do
			{
				NodeRef next = curNode->next;
				curNode.Dealloc();
				curNode = Move(next);
			} while (curNode);
		}
		// too small
		else if (i < newSize)
		{
			for (; i < newSize; ++i)
				EmplaceBack();
		}
	}

	template <typename T>
	void List<T>::Add(const T& val) noexcept requires CopyConstructible<T>
	{
		Add(Move(T{ val }));
	}

	template <typename T>
	void List<T>::Add(T&& val) noexcept
	{
		NodeRef node = CreateNode(Move(val));

		if (m_tail)
		{
			m_tail->next = node;
			m_tail = Move(node);
		}
		else
		{
			m_tail = m_head = Move(node);
		}
	}

	template <typename T>
	void List<T>::Add(const List& other) requires CopyConstructible<T>
	{
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Add(Move(T{ it.m_node->val }));
	}

	template <typename T>
	void List<T>::Add(List&& other)
	{
		if (other.IsEmpty())
			return;

		if (m_head.GetAlloc() == other.GetAllocator())
		{
			if (m_tail)
			{
				m_tail->next = other.m_head;
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
	void List<T>::EmplaceBack(Args&&... args) noexcept
	{
		Add(Move(T{ args... }));
	}

	template <typename T>
	auto List<T>::InsertAfter(ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		return InsertAfter(it, T{ val });
	}

	template <typename T>
	auto List<T>::InsertAfter(ConstIterator& it, T&& val) noexcept -> Iterator
	{
		ASSERT(it.m_node, "Iterator out of bounds");

		NodeRef curNode = it.m_node;

		NodeRef node = CreateNode(Move(val));

		node->next = curNode->next;
		curNode->next = node;

		if (!node->next)
			m_tail = node;

		return Iterator{ node };
	}

	template <typename T>
	auto List<T>::InsertAfter(ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		NodeRef node = it.m_node;
		NodeRef end = node->next;
		for (usize i = 0; i < count; ++i)
		{
			NodeRef next = CreateNode(Move(T{ val }));
			node->next = next;
			node = next;
		}

		if (end)
			node->next = end;
		else
			m_tail = node;
		
		return it + 1;
	}

	template <typename T>
	template <ForwardIterator It>
	auto List<T>::InsertAfter(ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>
	{
		NodeRef node = it.m_node;
		NodeRef endNode = node->next;
		for (It valIt = begin; valIt != end; ++valIt)
		{
			NodeRef next = CreateNode(Move(T{ *valIt }));
			node->next = next;
			node = next;
		}

		if (endNode)
			node->next = endNode;
		else
			m_tail = node;

		return it + 1;
	}

	template <typename T>
	auto List<T>::InsertAfter(ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>
	{
		return InsertAfter(it, il.begin(), il.end());
	}

	template <typename T>
	auto List<T>::InsertAfter(ConstIterator& it, const List& other) noexcept -> Iterator requires CopyConstructible<T>
	{
		if (other.IsEmpty())
			return it;

		return InsertAfter(it, other.Begin(), other.End());
	}

	template <typename T>
	auto List<T>::InsertAfter(ConstIterator& it, List&& other) noexcept -> Iterator
	{
		if (other.IsEmpty())
			return it;

		NodeRef prev = it.m_node;
		NodeRef endNode = prev->next;

		if (m_head.GetAlloc() == other.GetAllocator())
		{
			prev->next = other.m_head;
			if (endNode)
				other.m_tail->next = endNode;
			else
				m_tail = other.m_tail;
		}
		else
		{
			NodeRef otherNOde = other.m_head;
			while (otherNOde)
			{
				NodeRef node = CreateNode(Move(otherNOde->val));
				prev->next = node;
				prev = node;

				NodeRef next = otherNOde->next;
				otherNOde.Dealloc();
				otherNOde = next;
			}

			if (endNode)
				prev->next = endNode;
			else
				m_tail = prev;
		}
		
		other.m_head = other.m_tail = nullptr;
		return it + 1;
	}

	template <typename T>
	template <typename ... Args>
		requires ConstructableFrom<T, Args...>
	auto List<T>::EmplaceAfter(ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		return InsertAfter(it, Move(T{ args... }));
	}

	template <typename T>
	void List<T>::AddFront(const T& val) noexcept requires CopyConstructible<T>
	{
		AddFront(Move(T{ val }));
	}

	template <typename T>
	void List<T>::AddFront(T&& val) noexcept
	{
		NodeRef node = CreateNode(Move(val));
		node->next = m_head;
		m_head = node;
	}

	template <typename T>
	void List<T>::AddFront(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		for (usize i = 0; i < count; ++i)
			AddFront(Move(T{ val }));
	}

	template <typename T>
	template <ForwardIterator It>
	void List<T>::AddFront(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		for (It it = begin; it < end; ++it)
			AddFront(Move(T{ *it }));
	}

	template <typename T>
	void List<T>::AddFront(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		for (T* it = il.begin(), end = il.end(); it < end; ++it)
			AddFront(Move(T{ *it }));
	}

	template <typename T>
	void List<T>::AddFront(const List& other) noexcept requires CopyConstructible<T>
	{
		if (other.IsEmpty())
			return;

		AddFront(Move(T{ *other.Begin() }));
		Iterator curIt{ m_head };
		for (Iterator it = other.Begin() + 1, end = other.End(); it != end; ++it)
			curIt = InsertAfter(curIt, Move(T{ *it }));
	}

	template <typename T>
	void List<T>::AddFront(List&& other) noexcept
	{
		if (other.IsEmpty())
			return;

		NodeRef node = other.m_head;
		AddFront(Move(node->val));
		NodeRef next = node->next;
		node.Dealloc();
		node = next;

		Iterator curIt{ m_head };
		while (node)
		{
			curIt = InsertAfter(curIt, Move(node->val));
			next = node->next;
			node.Dealloc();
			node = next;
		}
		other.m_tail = other.m_head = NodeRef{ nullptr };
	}

	template <typename T>
	template <typename ... Args>
		requires ConstructableFrom<T, Args...>
	void List<T>::EmplaceFront(Args&&... args) noexcept
	{
		AddFront(Move(T{ args... }));
	}

	template <typename T>
	void List<T>::Clear() noexcept
	{
		if (!m_head)
			return;

		NodeRef node = m_head;
		Alloc::IAllocator* pAlloc = m_head.GetAlloc();
		while (node)
		{
			node->val.~T();
			NodeRef next = node->next;
			node.Dealloc();
			node = next;
		}
		MemClearData(*this);
		m_head = NodeRef{ pAlloc };
	}

	template <typename T>
	void List<T>::Pop() noexcept
	{
		NodeRef node = m_head;
		NodeRef next = node->next;
		if (!next)
		{
			Alloc::IAllocator* pAlloc = m_head.GetAlloc();
			m_head = NodeRef{ pAlloc };
			m_tail = nullptr;
			node->val.~T();
			node.Dealloc();
		}
		else 
		{
			while (next->next)
			{
				node = next;
				next = next->next;
			}

			next->val.~T();
			next.Dealloc();
			node->next = nullptr;
			m_tail = node;
		}
	}

	template <typename T>
	void List<T>::PopFront() noexcept
	{
		NodeRef newHead = m_head->next;
		if (newHead)
		{
			m_head->val.~T();
			m_head.Dealloc();
			m_head = newHead;
		}
		else
		{
			Alloc::IAllocator* pAlloc = m_head.GetAlloc();
			m_head->val.~T();
			m_head.Dealloc();
			m_head = NodeRef{ pAlloc };
			m_tail = nullptr;
		}
	}

	template <typename T>
	void List<T>::EraseAfter(const Iterator& it) noexcept
	{
		EraseAfter(it, 1);
	}

	template <typename T>
	void List<T>::EraseAfter(const Iterator& it, usize count) noexcept
	{
		NodeRef begin = it.m_node;
		NodeRef next = begin->next;

		if (!next)
			return;

		usize i = 0;
		for (; i < count && next; ++i)
		{
			NodeRef tmp = next->next;
			next->val.~T();
			next.Dealloc();
			next = tmp;
		}
		ASSERT(i == count, "Count too large");

		begin->next = next;
		if (!next)
			m_tail = begin;
	}

	template <typename T>
	void List<T>::EraseAfter(const Iterator& begin, const Iterator& end) noexcept
	{
		NodeRef beginNode = begin.m_node;
		NodeRef next = begin.m_node->next;

		if (!next || next == end.m_node)
			return;

		do
		{
			NodeRef tmp = next->next;
			next->val.~T();
			next.Dealloc();
			next = tmp;
		}
		while (next != end.m_node);

		beginNode->next = next;
		if (!next)
			m_tail = beginNode;
	}

	template <typename T>
	void List<T>::Reverse() noexcept
	{
		NodeRef node = m_head;
		if (!node)
			return;

		NodeRef next = node->next;
		while (next)
		{
			NodeRef tmp = next->next;
			next->next = node;
			node = next;
			next = tmp;
		}

		m_head->next = nullptr;
		NodeRef head = Move(m_head);
		m_head = Move(m_tail);
		m_tail = Move(head);
	}


	template <typename T>
	auto List<T>::Size() const noexcept -> usize
	{
		usize size = 0;
		for (Iterator it{ m_head }, end{}; it != end; ++it)
			++size;
		return size;
	}

	template <typename T>
	auto List<T>::IsEmpty() const noexcept -> bool
	{
		return !m_head;
	}

	template <typename T>
	auto List<T>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_head.GetAlloc();
	}

	template <typename T>
	auto List<T>::Front() noexcept -> T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_head->val;
	}

	template <typename T>
	auto List<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_head->val;
	}

	template <typename T>
	auto List<T>::Back() noexcept -> T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_tail->val;
	}

	template <typename T>
	auto List<T>::Back() const noexcept -> const T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_tail->val;
	}

	template <typename T>
	auto List<T>::Begin() noexcept -> Iterator
	{
		return Iterator{ m_head };
	}

	template <typename T>
	auto List<T>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_head };
	}

	template <typename T>
	auto List<T>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <typename T>
	auto List<T>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <typename T>
	auto List<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T>
	auto List<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto List<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto List<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T>
	auto List<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto List<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto List<T>::CreateNode(T&& val) noexcept -> NodeRef
	{
		NodeRef node = m_head.GetAlloc()->template Allocate<Node>();
		Node* pNode = node.Ptr();
		new (&pNode->next) NodeRef{ nullptr };
		new (&pNode->val) T{ Move(val) };
		return node;
	}
}
