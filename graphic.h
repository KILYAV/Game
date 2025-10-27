#pragma once
#include <tuple>
#include <array>
#include <vector>
#include <functional>
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "resource.h"
#include "shader.h"
#include "vertex.h"

namespace graphic {
	namespace object {
		template<class Shader, template<class> class Vertex, class... Texture>
		struct Object :
			Shader,
			Vertex<Texture...>,
			Texture...
		{
		public:
			using input_t = Vertex<Texture...>::input_t;
			void ThisCallPaint();
			bool ThisCallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		protected:
			template<typename Input_t>
			Object(Input_t input) :
				Shader{},
				Vertex<Texture...>{ input },
				Texture{}...
			{
				Shader::Invert(true);
			}
		};
		template<class Shader, template<class> class Vertex, class... Texture>
		void Object<Shader, Vertex, Texture...>::ThisCallPaint() {
			Shader::Bind();
			Vertex<Texture...>::Bind();
			texture::Bind<Texture...>();

			glDrawArrays(GL_POINTS, 0, Vertex<Texture...>::size);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};
		template<class Shader, template<class> class Vertex, class... Texture>
		bool Object<Shader, Vertex, Texture...>::ThisCallBack(const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			if (Vertex<Texture...>::Region(pos)) {
				Shader::Focus(true);
				return true;
			}
			else {
				Shader::Focus(false);
				return false;
			}
		}

		template<class... Texture>
		using Rectangle = Object<shader::Rectangle, vertex::Rectangle, Texture...>;
	}
	namespace list {
		template<class Value>
		struct Member {
			using type = Value;
			Value value;
		};
		template<template<size_t> class Layout, class... Value>
		struct List :
			Member<Value>...
		{
			using input_t = Layout<sizeof...(Value)>::input_t;
			bool Region(const glm::ivec2 pos) const;
			static bool CallBack(void* instant, const glm::ivec2 pos,
				std::optional<std::tuple<int, int, int>> mouse) {
				return static_cast<List<Layout, Value...>*>(instant)->ThisCallBack(pos, mouse);
			}
			static void CallPaint(void* instant) {
				static_cast<List<Layout, Value...>*>(instant)->ThisCallPaint();
			}
			List(const input_t input) :
				List{ input, 0 }
			{}
		protected:
			bool ThisCallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
			void ThisCallPaint();
		private:
			List(const input_t input, int index) :
				Member<Value>{ Layout<sizeof...(Value)>::Order(input, index) }...
			{}
		};
		template<template<size_t> class Layout, class... Value>
		bool List<Layout, Value...>::Region(const glm::ivec2 pos) const {
			bool result = false;
			(std::invoke([]() {
				result = result || static_cast<Member<Value>*>(this)->value.Region(pos);
				}), ...);
			return result;
		}
		template<template<size_t> class Layout, class... Value>
		void List<Layout, Value...>::ThisCallPaint() {
			(std::invoke([&]() {
				static_cast<Member<Value>*>(this)->value.ThisCallPaint();
				}), ...);
		}
		template<template<size_t> class Layout, class... Value>
		bool List<Layout, Value...>::ThisCallBack(const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			bool result = true;
			(std::invoke([&]() {
				static_cast<Member<Value>*>(this)->value.ThisCallBack(pos, mouse);
				}), ...);
			return result;
		}
		namespace layout {
			template<size_t count>
			struct Vertical {
				using input_t = std::pair<std::pair<int, std::array<int, count>>, glm::ivec2>;

				static std::pair<glm::ivec2, glm::ivec2> Order(const input_t input, int& index) ;
			};
			template<size_t count>
			std::pair<glm::ivec2, glm::ivec2> Vertical<count>::Order(const input_t input, int& index) {
				auto& pos{ input.first };
				auto& size{ input.second };
				auto& X{ pos.first };
				auto& Y{ pos.second };
				glm::ivec2 result;
				result.x = X;
				result.y = Y[index++];
				return { result, size };
			}
		}
	}
}