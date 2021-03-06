#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"

namespace Onca::Debugger
{

	/**
	 * Check if a debugger is attached to the program
	 * \return Whether a debugger is attached to the program
	 */
	CORE_API auto IsAttached() noexcept -> bool;
	/**
	 * Print a string to the debugger output
	 * \param[in] str String to print
	 */
	CORE_API void OutputDebugString(const String& str) noexcept;

}
