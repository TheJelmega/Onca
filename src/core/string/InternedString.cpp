#include "InternedString.h"

namespace Onca
{
	InternedString::InternedString() noexcept
	{
	}

	InternedString::InternedString(const String& string) noexcept
		: m_id(GetInternedStringManager().AddString(string))
	{
	}

	auto InternedString::operator=(const String& string) noexcept -> InternedString&
	{
		m_id = GetInternedStringManager().AddString(string);
		return *this;
	}

	auto InternedString::operator==(const InternedString& other) const noexcept -> bool
	{
		return m_id == other.m_id;
	}

	auto InternedString::operator==(const String& other) const noexcept -> bool
	{
		return Get() == other;
	}

	auto InternedString::Get() const noexcept -> const String&
	{
		return GetInternedStringManager().GetString(m_id);
	}

	auto InternedString::ToString() const noexcept -> String
	{
		return Get();
	}

	auto InternedString::Id() const noexcept -> StringId
	{
		return m_id;
	}

	auto InternedString::IsEmpty() const noexcept -> bool
	{
		return m_id == 0;
	}

	InternedString::operator const String&() const noexcept
	{
		return Get();
	}

	auto operator==(const String& str, const InternedString& interned) noexcept -> bool
	{
		return str == interned.Get();
	}

	InternedStringManager::InternedStringManager() noexcept
	{
	}
	
	void InternedStringManager::Shutdown() noexcept
	{
		m_strings.Clear(true);
	}

	auto InternedStringManager::AddString(const String& str) noexcept -> StringId
	{
		StringId id{ str };
		m_strings.TryInsert(id, str);
		return id;
	}

	auto InternedStringManager::GetString(StringId id) const noexcept -> const String&
	{
		auto it = m_strings.Find(id);
		if (it != m_strings.End())
			it->second;

		static String empty;
		return empty;
	}

	auto GetInternedStringManager() noexcept -> InternedStringManager&
	{
		static InternedStringManager manager;
		return manager;
	}
}
