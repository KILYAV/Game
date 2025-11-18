#pragma once
#include <string>
#include <vector>
#include <variant>
#include "../glm/glm/glm.hpp"
#include "bitmap.h"

namespace graphic {
	namespace texture {
		struct Texture {
			struct Data {
				glm::ivec2 center;
				glm::ivec2 size;
				unsigned ID;
				operator const glm::ivec4() const {
					return *static_cast<const glm::ivec4*>(static_cast<const void*>(this));
				}
			};
		private:
			std::vector<Data> texture;
			unsigned index;
		public:
			static constexpr size_t count = 1;

			unsigned size() const {
				return texture.size();
			}
			Texture& AddTexture(const int ID);
			Texture& AddTexture(const wchar_t* label);
			Texture& AddTexture(const Texture& other);
			bool Bind(const unsigned active = GL_TEXTURE0) const;

			void SetCenter(const glm::ivec2 pos = { 0,0 });
			glm::ivec2 GetSize() const;
			const Data& GetTexture() const;
		};
		struct Scroll {
		private:
			std::vector<std::vector<bitmap::BitMap<bitmap::Red>>> scroll;
		public:
			Scroll& AddVector(std::vector<std::wstring>&& string);
			Texture Release();
		};
	}
}