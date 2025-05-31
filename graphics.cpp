#include <Windows.h>
#include "graphics.h"
#include "setting.h"
#include "frame.h"

using namespace graphics;
vertex::Array::Array() :
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
void vertex::Array::BindBuffer() const {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}
void vertex::Array::VertexAttribPointer(
	unsigned attr, unsigned count, unsigned type, unsigned size, unsigned offset
) const {
	glVertexAttribPointer(attr, count, type, GL_FALSE, size, (void*)offset);
	glEnableVertexAttribArray(attr);
};
std::vector<std::tuple<glm::vec2, glm::vec2>> vertex::Rectangle(const glm::vec4 rectangle) {
	float x{ (rectangle.x * 2) / frm.size.floating.x - 1 };
	float y{ (rectangle.y * 2) / frm.size.floating.y - 1 };
	float z{ (rectangle.z * 2) / frm.size.floating.x - 1 };
	float w{ (rectangle.w * 2) / frm.size.floating.y - 1 };
	return std::vector<std::tuple<glm::vec2, glm::vec2>>{
		{ { x, y }, { 0., 1. } },
		{ { z, y }, { 1., 1. } },
		{ { x, w }, { 0., 0. } },
		{ { z, w }, { 1., 0. } }
	};
};
vertex::Element::Element(const std::vector<unsigned>& indices) :
	EBO{ std::invoke([]() {
		unsigned EBO;
		glGenBuffers(1, &EBO);
		return EBO;
	}) },
	size{ static_cast<unsigned>(indices.size()) }
{
	LoadElement(indices);
}
void vertex::Element::BindBuffer() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}
void vertex::Element::LoadElement(const std::vector<unsigned>& indices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned),
		static_cast<const char*>(static_cast<const void*>(indices.data())), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	size = indices.size();
}
Texture::Texture() :
	texture{ std::invoke([]() {
		unsigned texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 0., 0., 0., 0. };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return texture;
	}) }
{}
unsigned Texture::BindTexture() const {
	if (glIsTexture(texture)) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	return texture;
}
const Graphics& Graphics::BindBuffer() const {
	Array::BindBuffer();
	if (Element::Size()) {
		Element::BindBuffer();
	}
	return *this;
}
const Graphics& Graphics::BindTexture() const {
	Texture::BindTexture();
	return *this;
}
unsigned Shader::CompileShader(const int glsl, const wchar_t* type, const int GL_XXXX_SHADER) {
	auto resource{ FindResource(NULL, MAKEINTRESOURCE(glsl), type) };
	unsigned shader = 0;
	if (resource) {
		auto data{ LockResource(LoadResource(NULL, resource)) };
		auto ptr = static_cast<const char*>(data);
		shader = glCreateShader(GL_XXXX_SHADER);
		glShaderSource(shader, 1, &ptr, NULL);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success)
		{
			//std::cout << ptr << "\n";
			if (GL_VERTEX_SHADER == GL_XXXX_SHADER)
				std::cout << "vertex shader - ok\n";
			else if (GL_FRAGMENT_SHADER == GL_XXXX_SHADER)
				std::cout << "fragment shader - ok\n";
		}
		else {
			if (GL_VERTEX_SHADER == GL_XXXX_SHADER)
				std::cout << "vertex shader - fail\n";
			else if (GL_FRAGMENT_SHADER == GL_XXXX_SHADER)
				std::cout << "fragment shader - fail\n";
		}
	}
	return shader;
}
unsigned Shader::LinkShader(const int vertex, const int fragment, const int geometry) {
	auto shader{ glCreateProgram() };
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	if (geometry)
		glAttachShader(shader, geometry);
	glLinkProgram(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success)
	{
		std::cout << "link shader - ok\n";
	}
	else {
		std::cout << "link shader - fail\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry)
		glDeleteShader(geometry);
	return shader;
}
std::map<const char*, unsigned> uniforms::GetUniforms(
	const unsigned shader, const std::vector<const char*> labels) {
	std::map<const char*, unsigned> uniforms;
	for (const auto& ptr : labels) {
		uniforms.emplace(ptr, glGetUniformLocation(shader, ptr));
	}
	return uniforms;
}
bool uniforms::Rectangle::operator== (const glm::ivec2 pos) const {
	return  value.x < pos.x && pos.x < value.z && value.y < pos.y && pos.y < value.w;
}
uniforms::Rectangle uniforms::Rectangle::operator- (const int border) const {
	return glm::vec4{ value.x + border, value.y + border, value.z - border, value.w - border };
}
