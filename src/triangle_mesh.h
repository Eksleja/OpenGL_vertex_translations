#pragma once

class TriangleMesh {
public:
    TriangleMesh();
    void draw();
    ~TriangleMesh();

private:
    unsigned int VBO, VAO, EBO;
    unsigned int index_count;
};
