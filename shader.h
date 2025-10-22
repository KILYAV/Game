#pragma once
#include <tuple>
#include <array>
#include <vector>
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "resource.h"

namespace graphic {
	namespace shader {
		namespace id {
			unsigned GetShader(const wchar_t* type, int vert, int frag, int geom = 0);

			template<const wchar_t* type, int vert, int frag, int geom = 0>
			struct Shader {
				inline static const unsigned shader{ GetShader(type, vert, frag, geom) };
				Shader() {
					glUseProgram(shader);
				}
			};
			namespace label {
				static constexpr wchar_t rectangle[] = L"RECTANGLE";
			}
			using Rectangle = Shader<
				label::rectangle,
				IDR_RECTANGLE_SHADER_VERT,
				IDR_RECTANGLE_SHADER_FRAC,
				IDR_RECTANGLE_SHADER_GEOM>;
		}
	}
	namespace uniform {
		template<class Shader>
		struct BackgroundColor :
			Shader
		{
			using type = glm::vec4;
			inline static const int uniform{ glGetUniformLocation(Shader::shader, "background_color")};
		protected:
			BackgroundColor() :
				BackgroundColor{ value }
			{}
			BackgroundColor(const glm::vec4 i_value) {
				value = i_value;
				glUniform4f(uniform, value.x, value.y, value.z, value.w);
			}
			glm::vec4 Set(const glm::vec4 new_value) const {
				if (value != new_value) {
					value = new_value;
					glUniform4f(uniform, value.x, value.y, value.z, value.w);
				}
				return value;
			}
			glm::vec4 Get() const {
				return value;
			}
		private:
			inline static glm::vec4 value{ 1., 1., 1., 1. };
		};
	}
	namespace shader {
		template<class Base, template<class> class... Uniform>
		struct Shader :
			Base,
			Uniform<Base>...
		{
			Shader() = default;
		};
		using Rectangle = Shader<id::Rectangle, uniform::BackgroundColor>;
	}
}