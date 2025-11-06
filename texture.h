#pragma once
#include <vector>
#include <string>
#include "../glm/glm/glm.hpp"

namespace graphic {
	namespace texture {
		struct Texture {
			struct Data {
				unsigned ID;
				glm::ivec2 pos;
				glm::ivec2 size;
				glm::ivec2 scale;
			};
		private:
			std::vector<Data> texture;
		public:
			Texture() = default;
			Texture(const std::vector<std::wstring> labels);

			void AddTexture(const int ID);
			void AddTexture(const wchar_t* label);
			void Bind() const;

			const std::vector<Data> GetTexture() const {
				return texture;
			}

			glm::ivec2 MaxSize() const;
		};

		namespace data {
			struct Texture {
				unsigned ID;
				glm::ivec2 size;
			};
		}
		namespace id {
			data::Texture GetTexture(const int ID);
			template<const int ID>
			struct Texture {
				inline static const data::Texture texture{ GetTexture(ID) };
				glm::ivec2 Size() {
					return texture.size;
				}
			};
		}
		namespace str {
			data::Texture GetTexture(const wchar_t* label);
			template<const wchar_t* label>
			struct Texture {
				inline static const data::Texture texture{ GetTexture(label) };
				glm::ivec2 Size() {
					return texture.size;
				}
			};
		}
		template<class... Texture>
		void Bind() {
			unsigned texture{ GL_TEXTURE0 };
			(std::invoke([&]() {
				glActiveTexture(texture++);
				glBindTexture(GL_TEXTURE_2D, Texture::texture.ID);
				}), ...);
		}
	}
}