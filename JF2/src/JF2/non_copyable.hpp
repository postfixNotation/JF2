#ifndef NON_COPYABLE_HPP_
#define NON_COPYABLE_HPP_

namespace jf2 {
	struct NonCopyable {
		NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator= (const NonCopyable&) = delete;
	};
}

#endif // NON_COPYABLE_HPP_
