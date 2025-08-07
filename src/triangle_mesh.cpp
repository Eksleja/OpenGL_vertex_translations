#include "triangle_mesh.h"
#include <vector>
#include <glad/glad.h>

TriangleMesh::TriangleMesh() {
    std::vector<float> data = {
        // === Main fiber (short rectangular prism) ===
        // Front face (z = 0.05)
        -2.0f, -0.1f,  0.05f,  1.0f, 0.0f, 0.0f,  // 0 bottom-left-front
         2.0f, -0.1f,  0.05f,  0.0f, 1.0f, 0.0f,  // 1 bottom-right-front
         2.0f,  0.1f,  0.05f,  0.0f, 0.0f, 1.0f,  // 2 top-right-front
        -2.0f,  0.1f,  0.05f,  1.0f, 1.0f, 0.0f,  // 3 top-left-front

        // Back face (z = -0.05)
        -2.0f, -0.1f, -0.05f,  1.0f, 0.0f, 1.0f,  // 4 bottom-left-back
         2.0f, -0.1f, -0.05f,  0.0f, 1.0f, 1.0f,  // 5 bottom-right-back
         2.0f,  0.1f, -0.05f,  1.0f, 1.0f, 1.0f,  // 6 top-right-back
        -2.0f,  0.1f, -0.05f,  0.5f, 0.5f, 0.5f,  // 7 top-left-back

    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2,
        0, 2, 3,

        // Back face
        4, 6, 5,
        4, 7, 6,

        // Sides
        0, 4, 1,
        1, 4, 5,

        1, 5, 2,
        2, 5, 6,

        2, 6, 3,
        3, 6, 7,

        3, 7, 0,
        0, 7, 4,

    };

    index_count = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
