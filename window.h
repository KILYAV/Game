#pragma once
#include <map>
#include "graphic.h"
#include "function.h"

namespace window {
	struct Window {
		virtual void Paint() const = 0;
		virtual void ReSize(const glm::ivec2 pos, const glm::ivec2 size) = 0;
		virtual void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) = 0;
		virtual glm::ivec2 DefaultSize() = 0;

		virtual const wchar_t* Label() const {
			return nullptr;
		}
		virtual Window* operator[] (const wchar_t* label) {
			return nullptr;
		}
	};
}
namespace window::button {
	struct Button :
		Window,
		graphic::Rectangle
	{
	public:
		const wchar_t* const label;
		const int wParam, lParam;

		using Rectangle = graphic::Rectangle;

		Button(const wchar_t* const i_label, const int i_wParam, const int i_lParam) :
			label{ i_label },
			wParam{ i_wParam },
			lParam{ i_lParam }
		{
			Rectangle::Shader::SetBorder(true);
			Rectangle::Shader::SetInvert(true);
		}
		void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		void ReSize(const glm::ivec2 pos, const glm::ivec2 size);
		void Paint() const {
			Rectangle::Paint();
		}
		const wchar_t* Label() const {
			return label;
		}
		glm::ivec2 DefaultSize();
		Button& SetTexture(const graphic::texture::Texture& text) {
			static_cast<graphic::texture::Texture&>(*this) = text;
			return *this;
		}
	};
	struct Scroll :
		Button
	{
	private:
		std::vector<graphic::texture::Texture> textures;
		unsigned index = 0;
	public:
		using Button::Button;
		void operator= (const std::vector<graphic::texture::Texture>& vector);
		void SetTexture();
		void UpTexture();
	};
	namespace label {
		static constexpr wchar_t exit[] = L"Exit";
		static constexpr wchar_t apply[] = L"APPLY";
		static constexpr wchar_t cancel[] = L"CANCEL";
		static constexpr wchar_t options[] = L"Options";
		static constexpr wchar_t size_screen[] = L"Size screen";
		static constexpr wchar_t full_screen[] = L"Full screen";
		static constexpr wchar_t module_design[] = L"Module design";
	}
}
namespace window::menu {
	struct Menu :
		graphic::Rectangle
	{
		graphic::layout::Layout layout;
		std::vector<std::unique_ptr<Window>> objects;
	public:
		using Rectangle = graphic::Rectangle;

		Menu() {
			Rectangle::Shader::SetBorder(true);
		}
		template<class Object>
		void AddObject(Object obj) {
			objects.push_back(std::unique_ptr<window::Window>{ static_cast<window::Window*>(obj) });
		}
		glm::ivec2 DefaultSize() const;
		void Paint();
		void SetSize(const glm::ivec2 size);

		void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		bool CallMessage(int lParam, int wParam) {
			return false;// (*call_message)(lParam, wParam);
		}

		Window* operator[] (const wchar_t* label);
	};
	struct FullScreen :
		Menu
	{

	};

	void Main();
}