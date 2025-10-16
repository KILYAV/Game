#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>

namespace bitmap {
	struct Red {
		unsigned char r;
		static inline constexpr unsigned GL_XXX = GL_RED;
	};
	struct RGB :
		Red
	{
		unsigned char g;
		unsigned char b;
		static inline constexpr unsigned GL_XXX = GL_RGB;
	};
	struct RGBA :
		RGB
	{
		unsigned char a;
		static inline constexpr unsigned GL_XXX = GL_RGBA;
	};
	template<typename Pixel>
	class BitMap :
		std::unique_ptr<Pixel[]>
	{
	private:
		glm::ivec2 size;
	public:
		std::unique_ptr<Pixel[]>::operator bool;
		BitMap() = default;
		BitMap(BitMap&& bitmap);
		BitMap(const BitMap& bitmap);
		BitMap(const Pixel* data, const glm::ivec2 size);
		void operator= (const BitMap& other);

		BitMap& SetSize(const glm::ivec2 size);
		BitMap& SetPixel(const glm::ivec2 pos, const Pixel pixel);
		BitMap& Insert(const BitMap& other, const glm::ivec2 pos);
		BitMap& Fill(const Pixel pixel);
		BitMap& Border(const Pixel pixel);
		BitMap& MirrorXX();
		bool Empty() const {
			return size == glm::ivec2{ 0, 0 };
		}
		const BitMap& Print() const;

		Pixel GetPixel(const glm::ivec2 pos) const;

		const unsigned GL_XXX() const {
			return Pixel.GL_XXX;
		}
		const unsigned char* Data() const;
		const glm::ivec2 GetSize() const {
			return size;
		}
	private:
		std::unique_ptr<Pixel[]> Data(const Pixel* data, const glm::ivec2 size);
	};
	template<typename Pixel>
	std::unique_ptr<Pixel[]> BitMap<Pixel>::Data(const Pixel* data, const glm::ivec2 size) {
		int count = size.x * size.y;
		if (count) {
			Pixel* bitmap{ new Pixel[count]{0} };
			if (data) {
				for (unsigned index = 0; index < count; ++index) {
					bitmap[index] = data[index];
				}
			}
			return std::unique_ptr<Pixel[]>{ bitmap };
		}
		else
			return std::unique_ptr<Pixel[]>{};
	}
	template<typename Pixel>
	BitMap<Pixel>::BitMap(BitMap&& bitmap) :
		std::unique_ptr<Pixel[]>{ std::move(static_cast<std::unique_ptr<Pixel[]>&>(bitmap)) },
		size{ std::invoke([&]() {
			auto temp = bitmap.size;
			bitmap.size = { 0, 0 };
			return temp;
			}) }
	{};
	template<typename Pixel>
	BitMap<Pixel>::BitMap(const BitMap& bitmap) :
		BitMap{ bitmap.get(), bitmap.size }
	{}
	template<typename Pixel>
	BitMap<Pixel>::BitMap(const Pixel* data, const glm::ivec2 size) :
		std::unique_ptr<Pixel[]>{ Data(data,size) },
		size{ size }
	{}
	template<typename Pixel>
	void BitMap<Pixel>::operator= (const BitMap& other) {
		static_cast<std::unique_ptr<Pixel[]>&>(*this) = Data(other.get(), other.size);
		size = other.size;
	}
	template<typename Pixel>
	BitMap<Pixel>& BitMap<Pixel>::SetSize(const glm::ivec2 new_size) {
		static_cast<std::unique_ptr<Pixel[]>&>(*this) = Data(nullptr, new_size);
		size = new_size;
		return *this;
	}
	template<typename Pixel>
	BitMap<Pixel>& BitMap<Pixel>::SetPixel(const glm::ivec2 pos, const Pixel pixel) {
		if (0 <= pos.x || pos.x < size.x || 0 <= pos.y || pos.y < size.y) {
			this->get()[pos.x + pos.y * size.x] = pixel;
		}
		return *this;
	}
	template<typename Pixel>
	BitMap<Pixel>& BitMap<Pixel>::Insert(const BitMap& other, const glm::ivec2 pos) {
		for (unsigned y = 0; y < other.GetSize().y; ++y) {
			for (unsigned x = 0; x < other.GetSize().x; ++x) {
				this->SetPixel({ pos.x + x,pos.y + y }, other.GetPixel({ x,y }));
			}
		}
		return *this;
	}
	template<typename Pixel>
	BitMap<Pixel>& BitMap<Pixel>::Fill(const Pixel pixel) {
		for (unsigned y = 0; y < size.y; ++y) {
			for (unsigned x = 0; x < size.x; ++x) {
				SetPixel({ x,y }, pixel);
			}
		}
		return *this;
	}
	template<typename Pixel>
	BitMap<Pixel>& BitMap<Pixel>::MirrorXX() {
		auto halfY{ size.y >> 1 };
		for (unsigned d = 0, s = size.y - 1; d < halfY; ++d, --s) {
			for (unsigned x = 0; x < size.x; ++x) {
				Pixel temp = this->get()[x + d * size.x];
				this->get()[x + d * size.x] = this->get()[x + s * size.x];
				this->get()[x + s * size.x] = temp;
			}
		}
		return *this;
	}
	template<typename Pixel>
	BitMap<Pixel>& BitMap<Pixel>::Border(const Pixel pixel) {
		for (auto y : { 0,size.y - 1 }) {
			for (auto x = 0; x < size.x; ++x) {
				this->get()[x + y * size.x] = pixel;
			}
		}
		for (auto x : { 0,size.x - 1 }) {
			for (auto y = 0; y < size.y; ++y) {
				this->get()[x + y * size.x] = pixel;
			}
		}
		return *this;
	}
	template<typename Pixel>
	const BitMap<Pixel>& BitMap<Pixel>::Print() const {
		for (unsigned y = 0; y < size.y; ++y) {
			for (unsigned x = 0; x < size.x; ++x) {
				if (127 < this->GetPixel({ x,y }).r)
					std::cout << '#';
				else
					std::cout << '.';
			}
			std::cout << '\n';
		}
		return *this;
	}
	template<typename Pixel>
	Pixel BitMap<Pixel>::GetPixel(const glm::ivec2 pos) const {
		if (0 <= pos.x || pos.x < size.x || 0 <= pos.y || pos.y < size.y)
			return this->get()[pos.x + pos.y * size.x];
	}
	template<typename Pixel>
	const unsigned char* BitMap<Pixel>::Data() const {
		return static_cast<const unsigned char*>(static_cast<const void*>(this->get()));
	}
}