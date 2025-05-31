#pragma once
#include <vector>
#include <string>
#include "graphics.h"

namespace object {
	namespace button {
		class Exit {
		public:
			static constexpr const wchar_t label[5] = L"EXIT";
		};
	}
	class Button :
		public graphics::Window
	{
	public:
		const graphics::uniforms::Rectangle border;
		Button(const wchar_t* label, const glm::vec2 center, const glm::vec2 size);
	};
	class Menu :
		graphics::Window,
		std::vector<Button>
	{
	public:
		const graphics::uniforms::Rectangle border;
	private:
		struct Data {
			const glm::vec2 center;
			const glm::vec2 size;
			const glm::vec4 rectangle;
			const std::vector<std::wstring> labels;
		};
	public:
		const Menu& Run() const;
		Menu(const Data data);

		static void CallBack() {};
		template<class... labels>
		static std::vector<std::wstring> Labels(labels... label) {
			return std::vector<std::wstring>{ std::wstring{ label }... };
		}
		static Data Init(const glm::vec2 center, const std::vector<std::wstring> labels);
	};
	extern object::Menu mn;
}
