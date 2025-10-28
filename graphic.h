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
			void Paint();
		protected:
			template<typename... Input_t>
			Object(Input_t... input) :
				Shader{},
				Vertex<Texture...>{ input... },
				Texture{}...
			{
				Shader::Invert(true);
			}
		};
		template<class Shader, template<class> class Vertex, class... Texture>
		void Object<Shader, Vertex, Texture...>::Paint() {
			Shader::Bind();
			Vertex<Texture...>::Bind();
			texture::Bind<Texture...>();

			glDrawArrays(GL_POINTS, 0, Vertex<Texture...>::size);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};

		template<class... Texture>
		using Rectangle = Object<shader::Rectangle, vertex::Rectangle, Texture...>;
	}
	namespace list {
		template<class Value>
		struct Member {
			using type = Value;
			Value value;
		};
		template<class... Value>
		struct List :
			Member<Value>...
		{
			template<class Order, typename... Input_t>
			List(const Order* order, const Input_t... input) :
				List{ 0, order, input... }
			{}
		private:
			template<typename Order, typename... Input_t>
			List(int index, Order* order, Input_t... input) :
				Member<Value>{ order->Order(index, input...) }...
			{}
		public:
			void Paint();
			bool Region(const glm::ivec2 pos);
			bool CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		};
		template<class... Value>
		void List<Value...>::Paint() {
			(std::invoke([&]() {
				static_cast<Member<Value>*>(this)->value.Paint();
				}), ...);
		}
		template<class... Value>
		bool List<Value...>::Region(const glm::ivec2 pos) {
			bool result = false;
			(std::invoke([&]() {
				result = result || static_cast<Member<Value>*>(this)->value.Region(pos);
				}), ...);
			return result;
		}
		template<class... Value>
		bool List<Value...>::CallBack(const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			bool result = false;
			(std::invoke([&]() {
				result = result || static_cast<Member<Value>*>(this)->value.CallBack(pos, mouse);
				}), ...);
			return result;
		}
	}
	namespace layout {
		template<class... Unit>
		struct Vertical {
			glm::ivec2 UnitSize() const;
			std::array<int, sizeof...(Unit)> UnitPos(
				const glm::ivec2 size, const float scale) const;
			std::pair<glm::ivec2, glm::ivec2> Order(int& index,
				const std::array<int, sizeof...(Unit)>& pos, const glm::ivec2 size) const;
		};
		template<class... Unit>
		glm::ivec2 Vertical<Unit...>::UnitSize() const {
			glm::ivec2 size{ 0,0 };
			(std::invoke([&]() {
				size.x = size.x < Unit::texture.size.x ? Unit::texture.size.x : size.x;
				size.y = size.y < Unit::texture.size.y ? Unit::texture.size.y : size.y;
				}), ...);
			return size;
		}
		template<class... Unit>
		std::array<int, sizeof...(Unit)> Vertical<Unit...>::UnitPos(
			const glm::ivec2 size, const float scale) const {
			std::array<int, sizeof...(Unit)> pos;
			float Y = size.y * scale;
			float H = Y * sizeof...(Unit) * .5f;
			for (int index = 0; index < sizeof...(Unit); ++index) {
				pos[index] = H - Y * (.5f + index);
			}
			return pos;
		}
		template<class... Unit>
		std::pair<glm::ivec2, glm::ivec2> Vertical<Unit...>::Order(int& index,
			const std::array<int, sizeof...(Unit)>& pos, const glm::ivec2 size) const {
			return { { 0, pos[index++] }, size};
		}
	}
}