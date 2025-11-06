#pragma once
#include "../glm/glm/glm.hpp"

namespace graphic {
	namespace layout {
		template<class... Unit>
		struct Horison {
			static glm::ivec2 Size(const glm::ivec2 step) {
				return { step.x * sizeof...(Unit), step.y };
			}
			glm::ivec2 Order(int& index, const glm::ivec2 pos, const glm::ivec2 size) const;
		};
		template<class... Unit>
		glm::ivec2 Horison<Unit...>::Order(int& index,
			const glm::ivec2 pos, const glm::ivec2 size) const {
			float W = size.x * sizeof...(Unit) * .5f;
			int X = size.x * (.5f + index++) - W;
			return { pos.x + X, pos.y };
		}

		struct Vertical {
			static glm::ivec2 Size(const glm::ivec2 step, const int count) {
				return { step.x, step.y * count };
			}
			static glm::ivec2 Order(const glm::ivec2 pos, const glm::ivec2 size,
				const int count, const int index);
		};
		inline glm::ivec2 Vertical::Order(const glm::ivec2 pos, const glm::ivec2 size,
			const int count, const int index) {
			float H = size.y * count * .5f;
			int Y = H - size.y * (.5f + index);
			return { pos.x, pos.y + Y };
		}
	}
}
