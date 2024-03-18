#pragma once
#include <functional>



namespace sve {

	// from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>
	void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
		seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		(hashCombine(seed, rest), ...);
	};

}

namespace std {
	template <>
	struct hash<sve::Model::Vertex> {
		size_t operator()(sve::Model::Vertex const& vertex) const {
			size_t seed = 0;
			sve::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}