#ifndef LLVMREWRITER_COLOR_MAP_H
#define LLVMREWRITER_COLOR_MAP_H

#include <vector>

namespace basegraph {

	enum Color {
		WHITE = 0,
		GRAY = 1,
		GREEN = 2,
		BLACK = 3
	};


	class ColorMap {
		typedef long unsigned int Vertex;
		std::vector<uint8_t> colors;
		// long min_pos = 0;
		// long max_pos = -1;
	public:
		template<class Graph>
		explicit ColorMap(Graph &graph) : colors(graph.capacity() / 4 + 512) {
			// std::cout << "CAPACITY = " << graph.capacity() << "  " << colors.size() << std::endl;
		}

		ColorMap(const ColorMap&) = delete;

		inline void init() {
			// std::fill(colors.begin(), colors.end(), c);
			// if (max_pos < 0) {
			bzero(colors.data(), sizeof(uint8_t) * colors.size());
			/*} else if (min_pos <= max_pos) {
				bzero(colors.data() + min_pos, sizeof(uint8_t) * (max_pos - min_pos + 1));
			}
			min_pos = colors.size()-1;
			max_pos = 0;*/
		}

		inline void put(Vertex v, Color c) {
			assert(v >> 2 < colors.size());
			auto x = colors[v >> 2];
			x &= ~(3 << (v & 3) << (v & 3));
			colors[v >> 2] = c << (v & 3) << (v & 3);
			// min_pos = (v >> 2) < min_pos ? (v >> 2) : min_pos;
			// max_pos = (v >> 2) > max_pos ? (v >> 2) : max_pos;
		}

		/**
		 * Faster put which allows only white => gray, white => black and gray => black transitions.
		 * @param v
		 * @param c
		 */
		inline void put_fast(Vertex v, Color c) {
			assert(v >> 2 < colors.size());
			colors[v >> 2] |= c << (v & 3) << (v & 3);
			// min_pos = (v >> 2) < min_pos ? (v >> 2) : min_pos;
			// max_pos = (v >> 2) > max_pos ? (v >> 2) : max_pos;
		}

		inline Color get(Vertex v) {
#ifdef NDEBUG
			return (Color) ((colors[v >> 2] >> (v & 3) >> (v & 3)) & 3);
#else
			return (Color) ((colors.at(v >> 2) >> (v & 3) >> (v & 3)) & 3);
#endif
		}

	};

};

#endif //LLVMREWRITER_COLOR_MAP_H
