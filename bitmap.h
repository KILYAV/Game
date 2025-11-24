#pragma once
#include <memory>
#include <iostream>
#include "../glm/glm/glm.hpp"
//#include "../glad/include/glad/glad.h"

namespace bitmap {
	struct Red {
		unsigned char r = 0;
		//inline static constexpr unsigned GL_XXX = GL_RED;
	};
	struct RGB :
		Red
	{
		unsigned char g = 0;
		unsigned char b = 0;
		//inline static constexpr unsigned GL_XXX = GL_RGB;
	};
	struct RGBA :
		RGB
	{
		unsigned char a = 0;
		//inline static constexpr unsigned GL_XXX = GL_RGBA;
	};
	template<typename Pixel>
	class Bitmap {
	private:
		std::unique_ptr<Pixel[]> ptr;
		glm::ivec2 size;
	public:
		operator bool() {
			return static_cast<bool>(ptr);
		}
		Bitmap() = default;
		Bitmap(Bitmap&& bitmap);
		Bitmap(const Bitmap& bitmap);
		Bitmap(const glm::ivec2 size, const Pixel pixel = Pixel{});
		Bitmap(const glm::ivec2 size, const Pixel* data);

		Bitmap& operator= (Bitmap&& other);
		Bitmap& operator= (const Bitmap& other);

		Bitmap& operator+ (const Bitmap& other);

		Bitmap& SetSize(const glm::ivec2 size, const glm::ivec2 pos = { 0,0 }, const Pixel pixel = Pixel{});
		Bitmap& SetPixel(const glm::ivec2 pos, const Pixel pixel);
		Bitmap& Insert(const Bitmap& other, const glm::ivec2 pos = { 0,0 });
		Bitmap& Fill(const Pixel pixel);
		Bitmap& MirrorXX();
		bool Empty() const {
			return size == glm::ivec2{ 0, 0 };
		}
		const Bitmap& Print() const;

		Pixel GetPixel(const glm::ivec2 pos) const;

		const unsigned GL_XXX() const {
			return Pixel::GL_XXX;
		}
		const void* Data() const {
			return static_cast<const void*>(ptr.get());
		}
		const glm::ivec2 GetSize() const {
			return size;
		}
	};
	template<typename Pixel>
	Bitmap<Pixel>::Bitmap(Bitmap&& bitmap) :
		ptr{ bitmap.ptr.release() },
		size{ std::invoke([&]() {
			auto temp = bitmap.size;
			bitmap.size = { 0, 0 };
			return temp;
			}) }
	{};
	template<typename Pixel>
	Bitmap<Pixel>::Bitmap(const Bitmap& bitmap) :
		Bitmap{ bitmap.size, bitmap.ptr.get() }
	{}
	template<typename Pixel>
	Bitmap<Pixel>::Bitmap(const glm::ivec2 size, const Pixel pixel) :
		ptr{ size.x * size.y ? new Pixel[size.x * size.y]{ pixel } : nullptr },
		size{ size }
	{}
	template<typename Pixel>
	Bitmap<Pixel>::Bitmap(const glm::ivec2 size, const Pixel* data) :
		ptr{ size.x * size.y ? new Pixel[size.x * size.y]{} : nullptr },
		size{ size }
	{
		if (ptr && data)
			std::memcpy(ptr.get(), data, size.x * size.y * sizeof(Pixel));
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::operator= (Bitmap&& other) {
		ptr = std::unique_ptr<Pixel[]>{ other.ptr.release() };
		size = other.size;
		other.size = {0,0};
		return *this;
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::operator= (const Bitmap& other) {
		return *this = Bitmap<Pixel>{ other.size, other.ptr.get() };
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::operator+ (const Bitmap& other) {
		const int height{ other.size.y > size.y ? other.size.y : size.y };
		const int width{ other.size.x + size.x };
		Bitmap<Pixel> temp{ { width, height } };
		temp.Insert(*this, { 0, (height - size.y + 1) >> 1 });
		temp.Insert(other, { size.x, (height - other.size.y + 1) >> 1 });
		return *this = std::move(temp);
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::SetSize(const glm::ivec2 new_size, const glm::ivec2 pos, const Pixel pixel) {
		const glm::ivec2 offset{ (new_size.x - size.x + 1) >> 1, (new_size.y - size.y + 1) >> 1 };
		return *this = Bitmap<Pixel>{ new_size, pixel }.Insert(*this, offset);
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::SetPixel(const glm::ivec2 pos, const Pixel pixel) {
		if (0 <= pos.x && pos.x < size.x && 0 <= pos.y && pos.y < size.y) {
			ptr.get()[pos.x + pos.y * size.x] = pixel;
		}
		return *this;
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::Insert(const Bitmap& other, const glm::ivec2 pos) {
		for (unsigned y = 0; y < other.GetSize().y; ++y) {
			for (unsigned x = 0; x < other.GetSize().x; ++x) {
				SetPixel({ pos.x + x, pos.y + y }, other.GetPixel({ x,y }));
			}
		}
		return *this;
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::Fill(const Pixel pixel) {
		for (unsigned y = 0; y < size.y; ++y) {
			for (unsigned x = 0; x < size.x; ++x) {
				SetPixel({ x,y }, pixel);
			}
		}
		return *this;
	}
	template<typename Pixel>
	Bitmap<Pixel>& Bitmap<Pixel>::MirrorXX() {
		auto halfY{ size.y >> 1 };
		for (unsigned d = 0, s = size.y - 1; d < halfY; ++d, --s) {
			for (unsigned x = 0; x < size.x; ++x) {
				Pixel temp = ptr.get()[x + d * size.x];
				ptr.get()[x + d * size.x] = ptr.get()[x + s * size.x];
				ptr.get()[x + s * size.x] = temp;
			}
		}
		return *this;
	}
	template<typename Pixel>
	const Bitmap<Pixel>& Bitmap<Pixel>::Print() const {
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
	Pixel Bitmap<Pixel>::GetPixel(const glm::ivec2 pos) const {
		if (0 <= pos.x && pos.x < size.x && 0 <= pos.y && pos.y < size.y)
			return ptr.get()[pos.x + pos.y * size.x];
		else
			return Pixel{};
	}
	template<typename Pixel>
	glm::ivec2 MaxSize(const std::vector<Bitmap<Red>>& vector) {
		glm::ivec2 max{ 0,0 };
		for (auto& elem : vector) {
			auto size{ elem.GetSize() };
			max.x = max.x > size.x ? max.x : size.x;
			max.y = max.y > size.y ? max.y : size.y;
		}
	}
	template<typename Pixel>
	Bitmap<Pixel> operator+ (const Bitmap<Pixel>& left, const Bitmap<Pixel>& right) {
		const glm::ivec2 size_left{ left.GetSize() };
		const glm::ivec2 size_right{ right.GetSize() };
		const int height{ size_left.y > size_right.y ? size_left.y : size_right.y };

		Bitmap<Pixel> result{ { size_left.x + size_right.x, height } };
		result.Insert(left, { 0, (height - size_left.y + 1) >> 1 });
		result.Insert(right, { size_left.x, (height - size_right.y + 1) >> 1 });

		return result;
	}
}