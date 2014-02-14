#pragma once
#include <string>
#include <vector>

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 40300
// Ugly hack for decltype presence.
#define decltype __typeof__
#endif

namespace Chthon { /// @defgroup Utils Global utilities
/// @{

/// Delays execution for specified amount of time.
void delay(unsigned msec);

/** Generic function for determination of sign of value.
 * Value type must support operators `<`, `>`, and `-` and have default ctor.
 * @param value value to determine sign.
 * @return -1 for negative, 1 for positive, and 0 for zero.
 */
template<typename T>
int sign(T value)
{
	return (T() < value) - (value < T());
}

template<typename T>
const T & bound(const T & min, const T & value, const T & max)
{
	return std::max(min, std::min(value, max));
}

/// @cond INTERNAL
/// Structure for `foreach` internal implementation needs.
template<class T>
struct ForeachIterator {
	T iterator;
	bool done;
	bool brk;
	ForeachIterator(T t): iterator(t), done(false), brk(false)  { }
	operator bool() const { return true; }
	bool is_done() { if(!done) { brk = true; };  return done; }
	void mark_done() { brk = false; done = true;  }
	void reset_done() { done = false;  }
};
#define FOREACH_PP_CAT(a, b) FOREACH_PP_CAT_I(a, b)
#define FOREACH_PP_CAT_I(a, b) a ## b
/// Generates unique vartiable name using format: base_LINE__
/// It is needed by `foreach` macro because of shadowing loop variables in nested loops.
#define UNIQUE_NAME(base) FOREACH_PP_CAT(base, __LINE__)
/// @endcond

/** Generic container iteration macro.
 * Container must support methods `begin()`, `end()` which returns forward iterator.
 * @param expression expression which defines iterator variable, e.g. `const int & value`.
 * @param container container to iterate.
 *
 * Example of use:
 * @code{.cpp}
 * std::vector<int> v{1, 2, 3};
 * foreach(int & i, v) {
 * 	i *= i;	
 * }
 * // v == {1, 4, 9};
 * @endcode
 */
#define foreach(expression, container) \
	if(Chthon::ForeachIterator<decltype(container.begin())> UNIQUE_NAME(it) = container.begin()) \
        for(; UNIQUE_NAME(it).iterator != container.end() && !UNIQUE_NAME(it).brk; ++UNIQUE_NAME(it).iterator, UNIQUE_NAME(it).reset_done()) \
            for(expression = *UNIQUE_NAME(it).iterator; !UNIQUE_NAME(it).is_done(); UNIQUE_NAME(it).mark_done())

/** Determines size of a static allocated array.
 *
 * Example of use:
 * @code{.cpp}
 * int a[] = {1, 2, 3};
 * // size_of_array(a) == 3;
 * @endcode
 */
template<class T, size_t N>
size_t size_of_array(T (&)[N]) { return N; }

/// Splits string into parts using specified separator.
void split(const std::string & s, std::vector<std::string> & tokens, char delimeter = '\n');

/// Splits string into parts using specified separator (overloaded).
std::vector<std::string> split(const std::string & s, char delimeter = '\n');

/// Returns true if string starts with specified start pattern, otherwise returns false.
bool starts_with(const std::string & s, const std::string & start);

/// Returns true if string ends with specified end pattern, otherwise returns false.
bool ends_with(const std::string & s, const std::string & end);

}

/** Generic append operator for vector
 *
 * Example of use:
 * @code{.cpp}
 * std::vector<int> v;
 * v << 1 << 2 << 3;
 * // v == {1, 2, 3};
 * @endcode
 */
template<class T>
std::vector<T> & operator<<(std::vector<T> & out, const T & t)
{
	out.push_back(t);
	return out;
}
/// Overloaded append operator for vector of string and C-strings.
std::vector<std::string> & operator<<(std::vector<std::string> & out, const char * t);

/// @}
