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
		template<class Shader, class Vertex>
		struct Object :
			Shader,
			Vertex,
			texture::Texture
		{
		public:
			void Paint() const;
			Object() = default;
		};
		template<class Shader, class Vertex>
		void Object<Shader, Vertex>::Paint() const {
			Shader::Bind();
			Vertex::Bind();
			texture::Texture::Bind();

			glDrawArrays(GL_POINTS, 0, Vertex::count);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};

		using Rectangle = Object<shader::Rectangle, vertex::Rectangle>;
	}
	namespace list {
		template<class Value>
		struct Member {
			using type = Value;
			Value value;

			template<typename... Input_t>
			Member(Input_t... input_t) :
				value{ input_t... }
			{}
		};
		template<class... Value>
		struct List :
			Member<Value>...
		{
			List() = default;
			template<class Order, typename... Input_t>
			List(const Order* order, const Input_t... input) :
				List{ 0, order, input... }
			{}
		private:
			template<class Order, typename pos_t, typename step_t, typename size_t>
			List(int index, Order* order, const pos_t pos, const step_t step, const size_t size) :
				Member<Value>{ order->Order(index, pos, step), size }...
			{}
		public:
			void Paint();
			void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
			bool Region(const glm::ivec2 pos);
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
		void List<Value...>::CallBack(const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			(std::invoke([&]() {
				static_cast<Member<Value>*>(this)->value.CallBack(pos, mouse);
				}), ...);
		}
	}
}