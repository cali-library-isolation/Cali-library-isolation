#ifndef LIBIPC_BYTE_BITSET_H
#define LIBIPC_BYTE_BITSET_H

#pragma pack(1)

#include <cstdint>
#include <cstddef>

struct ByteBitset {
	uint8_t data;

	inline size_t count() {
		return __builtin_popcount(data);
	}

	inline bool all() {
		return data == 0xff;
	}

	inline bool none() {
		return data == 0;
	}

	inline size_t size() {
		return sizeof(data);
	}

	bool operator[](int index) {
		return (data & (1 << index)) != 0;
	}

	inline void set(int index, bool bit) {
		if (bit)
			data |= 1u << index;
		else
			data ^= data & (1u << index);
	}
};

#endif //LIBIPC_BYTE_BITSET_H
