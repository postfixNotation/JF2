#ifndef NON_MOVEABLE_HPP_
#define NON_MOVEABLE_HPP_

namespace nxt {
	struct NonMoveable {
		NonMoveable() = default;
		NonMoveable(NonMoveable&&) = delete;
		NonMoveable& operator= (NonMoveable&&) = delete;
	};
}

#endif // NON_MOVEABLE_HPP_
