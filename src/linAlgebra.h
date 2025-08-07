#include <cmath>

#define PI 3.1415962

struct matrix4 {
    float entries[16];
};

struct vector3 {
    float entries[3];
};

matrix4 create_matrix_transform(vector3 translation);

matrix4 rotate_around_Z_axis(float angle);

matrix4 rotate_and_translate_allAtOnce(vector3 pos, float angleX_deg, float angleY_deg, float angleZ_deg);

matrix4 create_look_at(vector3 from, vector3 to);

matrix4 create_perspective_projection(
    float fovy, float aspect, float near, float far
);

float dot(vector3 u, vector3 v);

vector3 normalize(vector3 v);

vector3 cross(vector3 u, vector3 v);

