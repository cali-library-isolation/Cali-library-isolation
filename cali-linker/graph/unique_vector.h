#ifndef LLVMREWRITER_UNIQUE_VECTOR_H
#define LLVMREWRITER_UNIQUE_VECTOR_H

#include <vector>

namespace basegraph {

	// Unique vector - a set implementation based on vectors (more space-efficient, fast enough for small sets)
	template<typename Tp>
	class unique_vector {
	public:
		typedef typename std::vector<Tp>::size_type size_type;
		typedef typename std::vector<Tp>::iterator iterator;
		typedef typename std::vector<Tp>::const_iterator const_iterator;
		std::vector<Tp> vec;

		void push_back(const Tp &x) {
			insert(x);
		}

		/**
		 *
		 * @param x
		 * @return true iff the edge did not exist before
		 */
		bool insert(const Tp &x) {
			for (auto it = vec.begin(); it != vec.end(); it++)
				if (x == *it)
					return false;
			vec.push_back(x);
			return true;
		}

		bool insert_unique(const Tp &x) {
			vec.push_back(x);
			return true;
		}

		size_t size() const { return vec.size(); }

		iterator begin() { return vec.begin(); }

		iterator end() { return vec.end(); }

		bool remove(const Tp &x) {
			for (auto it = vec.begin(); it != vec.end(); ++it) {
				if (*it == x) {
					vec.erase(it);
					return true;
				}
			}
			return false;
		}

		void clear() { vec.clear(); }

		inline Tp &operator[](size_t index) {
			return vec[index];
		}
	};

}

#endif //LLVMREWRITER_UNIQUE_VECTOR_H
