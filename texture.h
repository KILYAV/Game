#pragma once
#include "../glm/glm/glm.hpp"

namespace graphic {
	namespace texture {
		namespace data {
			struct Texture {
				unsigned ID;
				glm::ivec2 size;
			};
		}
		namespace id {
			data::Texture GetTexture(const int ID);
			template<const int ID>
			class Texture {
			public:
				inline static const data::Texture texture{ GetTexture(ID) };
			protected:
				static void BindTexture() {
					glBindTexture(GL_TEXTURE_2D, texture.ID);
				}
			};
		}
		namespace str {
			data::Texture GetTexture(const wchar_t* label);
			template<const wchar_t* label>
			class Texture {
			public:
				inline static const data::Texture texture{ GetTexture(label) };
			protected:
				static void BindTexture() {
					glBindTexture(GL_TEXTURE_2D, texture.ID);
				}
			};
		}
		namespace background {
			using Main = id::Texture<IDR_BACKDROP_JPG>;
		}
		namespace label {
			static constexpr wchar_t exit[] = L"EXIT";
		}
		namespace button {
			using Exit = str::Texture<label::exit>;
		}
	}
}