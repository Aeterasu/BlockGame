#include "sprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace blockgame
{
    void DrawSprite(const Sprite& sprite, const glm::mat4& projection, GLuint quadVao)
    {
        GLuint program = sprite.shader->id;

        if (!sprite.shader || program == 0)
        {
            return;
        }

        glUseProgram(program);

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(sprite.position, 0.0f));
        model = glm::translate(model, glm::vec3(sprite.size * 0.5f, 0.0f));
        model = glm::rotate(model, sprite.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(sprite.size * -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(sprite.size, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(program, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform4fv(glGetUniformLocation(program, "uTint"), 1, glm::value_ptr(sprite.tint));

        if (sprite.texture && sprite.texture->id != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sprite.texture->id);
            glUniform1i(glGetUniformLocation(program, "uTexture"), 0);
        }

        glBindVertexArray(quadVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
} // namespace blockgame
