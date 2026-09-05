#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <variant>

namespace blockgame
{
	using QuadHandle = std::uint32_t;

	using UniformValue = std::variant<float, int, glm::vec2, glm::vec3, glm::vec4, glm::mat4, const Texture*>;

	struct Quad
	{
		glm::vec2 position{0.0f, 0.0f};
		glm::vec2 size{32.0f, 32.0f};
		float rotation = 0.0f;

		std::int32_t zIndex = 0;

		const Shader* shader = nullptr;

		std::unordered_map<std::string, UniformValue> uniforms;

		void ApplyTexture(const Texture* tex);
		void ApplyCustomShader(const Shader* shd);
		void SetUniform(const std::string& name, UniformValue value);
	};

	void DrawQuad(const Quad& quad, const glm::mat4& projection, GLuint quadVao);
} // namespace blockgame
