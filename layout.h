#pragma once
#include "../glm/glm/glm.hpp"

namespace graphic {
	namespace layout {
		template<class... Unit>
		glm::ivec2 MaxSize() {
			glm::ivec2 max{ 0,0 };
			(std::invoke([&]() {
				glm::ivec2 size{ Unit::Size() };
				max.x = max.x < size.x ? size.x : max.x;
				max.y = max.y < size.y ? size.y : max.y;
				}), ...);
			return max;
		}
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

		template<class... Unit>
		struct Vertical {
			static glm::ivec2 Size(const glm::ivec2 step) {
				return { step.x, step.y * sizeof...(Unit) };
			}
			glm::ivec2 Order(int& index, const glm::ivec2 pos, const glm::ivec2 size) const;
		};
		template<class... Unit>
		glm::ivec2 Vertical<Unit...>::Order(int& index,
			const glm::ivec2 pos, const glm::ivec2 step) const {
			float H = step.y * sizeof...(Unit) * .5f;
			int Y = H - step.y * (.5f + index++);
			return { pos.x, pos.y + Y };
		}
	}
}
