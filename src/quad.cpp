#include "quad.h"

#include "shader_storage.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace blockgame
{
	void Quad::ApplyTexture(const Texture* tex)
	{
		shader = &shaderStorage.spriteShader;
		uniforms["uTexture"] = tex;
		uniforms["uTint"] = glm::vec4(1.0f);
	}

	void Quad::ApplyCustomShader(const Shader* shd)
	{
		shader = shd;
	}

	void Quad::SetUniform(const std::string& name, UniformValue value)
	{
		uniforms[name] = value;
	}

	void DrawQuad(const Quad& quad, const glm::mat4& projection, GLuint quadVao)
	{
		GLuint program = quad.shader->id;

		if (!quad.shader || program == 0)
		{
			return;
		}

		glUseProgram(program);

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(quad.position, 0.0f));
		model = glm::translate(model, glm::vec3(quad.size * 0.5f, 0.0f));
		model = glm::rotate(model, quad.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(quad.size * -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(quad.size, 1.0f));

		glUniformMatrix4fv(glGetUniformLocation(program, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

		for (const auto& [name, value] : quad.uniforms)
		{
			GLuint loc = glGetUniformLocation(program, name.c_str());

			std::visit(
				[loc](const auto& v)
				{
					using T = std::decay_t<decltype(v)>;

					if constexpr (std::is_same_v<T, float>)
					{
						glUniform1f(loc, v);
					}
					else if constexpr (std::is_same_v<T, int>)
					{
						glUniform1i(loc, v);
					}
					else if constexpr (std::is_same_v<T, glm::vec2>)
					{
						glUniform2fv(loc, 1, glm::value_ptr(v));
					}
					else if constexpr (std::is_same_v<T, glm::vec3>)
					{
						glUniform3fv(loc, 1, glm::value_ptr(v));
					}
					else if constexpr (std::is_same_v<T, glm::vec4>)
					{
						glUniform4fv(loc, 1, glm::value_ptr(v));
					}
					else if constexpr (std::is_same_v<T, glm::mat4>)
					{
						glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
					}
					else if constexpr (std::is_same_v<T, const Texture*>)
					{
						if (v && v->id != 0)
						{
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, v->id);
							glUniform1i(loc, 0);
						}
					}
				},
				value);
		}

		glBindVertexArray(quadVao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
} // namespace blockgame
