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
			struct Texture {
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
			struct Texture {
				inline static const data::Texture texture{ GetTexture(label) };
			protected:
				static void BindTexture() {
					glBindTexture(GL_TEXTURE_2D, texture.ID);
				}
			};
		}
	}
}