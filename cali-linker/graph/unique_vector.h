#ifndef LLVMREWRITER_UNIQUE_VECTOR_H
#define LLVMREWRITER_UNIQUE_VECTOR_H

#include <vector>

namespace basegraph {

	// Unique vector - a set implementation based on vectors (more space-efficient, fast enough for small sets)
	template<typename _Tp>
	class unique_vector {
	public:
		typedef typename std::vector<_Tp>::size_type size_type;
		typedef typename std::vector<_Tp>::iterator iterator;
		typedef typename std::vector<_Tp>::const_iterator const_iterator;
		std::vector<_Tp> vec;

		void push_back(const _Tp &x) {
			insert(x);
		}

		/**
		 *
		 * @param x
		 * @return true iff the edge did not exist before
		 */
		bool insert(const _Tp &x) {
			for (auto it = vec.begin(); it != vec.end(); it++)
				if (x == *it)
					return false;
			vec.push_back(x);
			return true;
		}

		bool insert_unique(const _Tp &x) {
			vec.push_back(x);
			return true;
		}

		size_t size() const { return vec.size(); }

		iterator begin() { return vec.begin(); }

		iterator end() { return vec.end(); }

		void remove(const _Tp &x) {
			for (auto it = vec.begin(); it != vec.end(); ++it) {
				if (*it == x) {
					vec.erase(it);
					return;
				}
			}
		}

		void clear() { vec.clear(); }

		inline _Tp &operator[](size_t index) {
			return vec[index];
		}
	};

}

#endif //LLVMREWRITER_UNIQUE_VECTOR_H
