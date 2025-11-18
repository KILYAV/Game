#pragma once
#include <string>
#include "frame.h"
#include "resource.h"

namespace graphic::shader::id {
	unsigned GetShader(const char* cstr, int vert, int frag, int geom = 0);

	template<const char* str, int vert, int frag, int geom = 0>
	struct Shader {
		inline static constexpr std::string_view label{ str };
		inline static const unsigned ID{ GetShader(str, vert, frag, geom) };
	};
}
namespace graphic::uniform {
	template<class Shader>
	struct Status :
		Shader
	{
	private:
		int local = 0;
		inline static int global = 0;
	public:
		inline static constexpr std::string_view label{ "status" };
		static constexpr int focus = 0x01;
		static constexpr int invert = 0x02;
		static constexpr int border = 0x04;

		inline static const int uniform{ std::invoke([&]() {
			std::string str{ std::string{ Shader::label } + "_" + std::string{ label } };
			return glGetUniformLocation(Shader::ID, str.data());
			}) };
		Status(const int value = global) :
			local{ value }
		{
			global = local;
			glUniform1i(uniform, global);
		}
		void Bind() const {
			if (global != local) {
				global = local;
				glUniform1i(uniform, global);
			}
		}
		int Get() const {
			return local;
		}
		bool SetFocus(const bool select) {
			return Bool(select, focus);
		}
		bool SetInvert(const bool select) {
			return Bool(select, invert);
		}
		bool SetBorder(const bool select) {
			return Bool(select, border);
		}
	private:
		bool Bool(const bool select, const int value) {
			select ? local = local | value : local = local & ~value;
			return select;
		}
	};
}
namespace graphic::shader {
	template<class Base, template<class> class... Uniform>
	struct Shader :
		Base,
		Uniform<Base>...
	{
		void Bind() const;
	};
	template<class Base, template<class> class... Uniform>
	void Shader<Base, Uniform...>::Bind() const {
		glUseProgram(Shader::ID);
		(std::invoke([&]() {
			Uniform<Base>::Bind();
			}), ...);
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

