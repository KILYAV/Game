#pragma once
#include <string>
#include "frame.h"
#include "resource.h"

namespace graphic {
	namespace shader {
		namespace id {
			unsigned GetShader(const char* cstr, int vert, int frag, int geom = 0);

			template<const char* str, int vert, int frag, int geom = 0>
			struct Shader {
				inline static constexpr std::string_view label{ str };
				inline static const unsigned shader{ GetShader(str, vert, frag, geom) };
				Shader() {
					glUseProgram(shader);
				}
			};
		}
	}
	namespace uniform {
		namespace block {
			struct Ranges {
			protected:
				Ranges() :
					ranges{}
				{}
				const unsigned ranges;
			private:
				inline static unsigned ranges_ = 0;
			};

			unsigned GetBlock(const unsigned ranges, const unsigned size);
			template<class... Shader>
			struct Button :
				Ranges
			{
			private:
				struct Block {
					int status;
				};
			public:
				inline static constexpr std::string_view label{ "Button" };
				inline static const unsigned uniform{ GetBlock(ranges, sizeof(Block)) };
			protected:
				Button();
			private:
				inline static Block block;
			};
			template<class... Shader>
			Button<Shader...>::Button() {
				(std::invoke([]() {
					glUniformBlockBinding(Shader::shader,
						glGetUniformBlockIndex(Shader::shader, label), ranges);
					}), ...);
			}

			template<class Base, template<class> class... Block>
			struct Shader :
				Base,
				Block<Base>...
			{};
		}
		/*
		template<class Shader>
		struct BackgroundColor :
			Shader
		{
			using type = glm::vec4;
			inline static const int uniform{ glGetUniformLocation(Shader::shader, "background_color") };
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
		*/
		template<class Shader>
		struct Status :
			Shader
		{
			inline static constexpr std::string_view label{ "status" };
			static constexpr int invert = 0x1;
			static constexpr int border = 0x2;

			using type = int;
			inline static const int uniform{ std::invoke([&]() {
				std::string str{ std::string{ Shader::label } + "_" + std::string{ label } };
				return glGetUniformLocation(Shader::shader, str.data());
				}) };
		//protected:
			Status(const int new_value = value) {
				value = new_value;
				glUniform1i(uniform, value);
			}
			int Set(const int new_value) const {
				if (value != new_value) {
					value = new_value;
					glUniform1i(uniform, value);
				}
				return value;
			}
			int Get() const {
				return value;
			}
			int Invert(bool select) {
				return select ? Set(value | invert) : Set(value & ~invert);
			}
			int Border(bool select) {
				return select ? Set(value | border) : Set(value & ~border);
			}
		private:
			inline static int value;
		};
	}
	namespace shader {
		template<class Base, template<class> class... Uniform>
		struct Shader :
			Base,
			Uniform<Base>...
		{};
	}
}
namespace graphic::shader::id {
	namespace label {
		static constexpr char rectangle[] = "rectangle";
	}
	using Rectangle = Shader<
		label::rectangle,
		IDR_RECTANGLE_SHADER_VERT,
		IDR_RECTANGLE_SHADER_FRAG,
		IDR_RECTANGLE_SHADER_GEOM>;
}
namespace graphic::uniform::block {
	//using Rectangle = Shader<graphic::shader::id::Rectangle, Status>;
}
namespace graphic::shader {
	using Rectangle = Shader<id::Rectangle, uniform::Status>;
}

