#pragma once
#include <string>
#include <vector>
#include <variant>
#include "../glm/glm/glm.hpp"
#include "bitmap.h"

namespace graphic {
	namespace texture {
		struct Texture {
		private:
			glm::ivec2 size;
			unsigned bit;
			unsigned ID;
		public:
			static constexpr size_t count = 1;

			Texture() = default;
			Texture(const Texture& texture) = default;
			Texture(const glm::ivec2 size, const unsigned bit, const void* data);
			template<class Pixel>
			Texture(const bitmap::Bitmap<Pixel>& bitmap) :
				Texture{ bitmap.GetSize(), sizeof(Pixel), bitmap.Data() }
			{}
			Texture(const int ID);

			void Bind(const unsigned active) const {
				glActiveTexture(active);
				glBindTexture(GL_TEXTURE_2D, ID);
			}
			glm::ivec2 GetSize() const {
				return size;
			}
		};
		void Bind(const std::vector<Texture>& texture);
		glm::ivec2 MaxSize(const std::vector<Texture>& texture);
	}
}