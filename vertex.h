#pragma once
#include "frame.h"
#include "texture.h"

namespace graphic {
	namespace shape {
		namespace rectangle {
			glm::vec4 GetRegion(const glm::ivec2 pos, const glm::ivec2 size);
			glm::vec4 GetTexture(const glm::ivec2 pos, const glm::ivec2 size_tex, const glm::ivec2 size_rect);
			constexpr size_t size = 1;
			template<size_t count>
			struct Rectangle {
				static constexpr size_t size = rectangle::size;
				using input_t = std::array<glm::ivec2, (count + 1) * 2>;
				using output_t = std::array<glm::vec4, (count + 1) * 1>;

				static output_t Get(input_t input);
			};
			template<size_t count>
			Rectangle<count>::output_t Rectangle<count>::Get(input_t input) {
				output_t output;
				output[0] = GetRegion(input[0], input[1]);
				for (int index = 1, max = count + 1; index < max; ++index) {
					output[index] = GetTexture(input[index * 2], input[index * 2 + 1], input[1]);
				}
				return output;
			}
		}
	}
	namespace vertex {
		template<template<size_t> class Shape, class... Texture>
		class Array :
			public Shape<sizeof...(Texture)>
		{
		public:
			const unsigned VAO;
			const unsigned VBO;
		protected:
			using input_t = Shape<sizeof...(Texture)>::input_t;
			Array();
			Array(const input_t& input);
		protected:
			void LoadVertex(const input_t& input) const;
		};
		template<template<size_t> class Shape, class... Texture>
		Array<Shape, Texture...>::Array() :
			VAO{ std::invoke([]() {
				unsigned VAO;
				glGenVertexArrays(1, &VAO);
				return VAO;
			}) },
			VBO{ std::invoke([]() {
				unsigned VBO;
				glGenBuffers(1, &VBO);
				return VBO;
			}) }
		{}
		template<template<size_t> class Shape, class... Texture>
		Array<Shape, Texture...>::Array(const input_t& input) :
			Array{}
		{
			LoadVertex(input);
		}
		template<template<size_t> class Shape, class... Texture>
		void Array<Shape, Texture...>::LoadVertex(const input_t& input) const {
			using array_t = Shape<sizeof...(Texture)>::output_t;
			array_t array{ Shape<sizeof...(Texture)>::Get(input) };

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(array_t), array.data(), GL_STATIC_DRAW);

			for (unsigned index = 0, max = sizeof...(Texture) + 1, offset = 0; index < max; ++index) {
				glVertexAttribPointer(index, sizeof(array_t::value_type) / sizeof(float),
					GL_FLOAT, GL_FALSE, sizeof(array_t), (void*)offset);
				glEnableVertexAttribArray(index);
				offset += sizeof(array_t::value_type);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		template<class... Texture>
		using Rectangle = Array<shape::rectangle::Rectangle, Texture...>;

		class Element {
		public:
			const unsigned EBO;
			const unsigned size;
		protected:
			Element(const std::vector<unsigned>& indices = {});
			void BindElement() const {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			}
		};
	}
}