#include "linAlgebra.h"

matrix4 create_matrix_transform(vector3 translation){
    matrix4 matrix;

    matrix.entries[0] = 1.0f;
    matrix.entries[1] = 0.0f;
    matrix.entries[2] = 0.0f;
    matrix.entries[3] = 0.0f;

    matrix.entries[4] = 0.0f;
    matrix.entries[5] = 1.0f;
    matrix.entries[6] = 0.0f;
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = 0.0f;
    matrix.entries[9] = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = translation.entries[0];
    matrix.entries[13] = translation.entries[1];
    matrix.entries[14] = translation.entries[2];
    matrix.entries[15] = 1.0f;

    return matrix;
}   

matrix4 rotate_around_Z_axis(float angle){

    angle = angle * PI / 180.0f;

    matrix4 matrix;

    float cos = cosf(angle);
    float sin = sinf(angle);

    matrix.entries[0] = cos;
    matrix.entries[1] = sin;
    matrix.entries[2] = 0.0f;
    matrix.entries[3] = 0.0f;

    matrix.entries[4] = -sin;
    matrix.entries[5] = cos;
    matrix.entries[6] = 0.0f;
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = 0.0f;
    matrix.entries[9] = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = 0.0f;
    matrix.entries[13] = 0.0f;
    matrix.entries[14] = 0.0f;
    matrix.entries[15] = 1.0f;

    return matrix;
}


matrix4 rotate_and_translate_allAtOnce(vector3 pos, float angleX_deg, float angleY_deg, float angleZ_deg) {
    float ax = angleX_deg * PI / 180.0f;
    float ay = angleY_deg * PI / 180.0f;
    float az = angleZ_deg * PI / 180.0f;

    float cosX = cosf(ax), sinX = sinf(ax);
    float cosY = cosf(ay), sinY = sinf(ay);
    float cosZ = cosf(az), sinZ = sinf(az);

    matrix4 matrix;

    // Rotation combined: Z * Y * X (common convention)
    matrix.entries[0]  = cosY * cosZ;
    matrix.entries[1]  = cosX * sinZ + sinX * sinY * cosZ;
    matrix.entries[2]  = sinX * sinZ - cosX * sinY * cosZ;
    matrix.entries[3]  = 0.0f;

    matrix.entries[4]  = -cosY * sinZ;
    matrix.entries[5]  = cosX * cosZ - sinX * sinY * sinZ;
    matrix.entries[6]  = sinX * cosZ + cosX * sinY * sinZ;
    matrix.entries[7]  = 0.0f;

    matrix.entries[8]  = sinY;
    matrix.entries[9]  = -sinX * cosY;
    matrix.entries[10] = cosX * cosY;
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = pos.entries[0];
    matrix.entries[13] = pos.entries[1];
    matrix.entries[14] = pos.entries[2];
    matrix.entries[15] = 1.0f;

    return matrix;
}




matrix4 create_look_at(vector3 from, vector3 to){
    vector3 global_up = {0.0f, 0.0f, 1.0f};

    vector3 forward_vec = {
        to.entries[0]-from.entries[0],
        to.entries[1]-from.entries[1],
        to.entries[2]-from.entries[2]
    };
    forward_vec = normalize(forward_vec);

    vector3 right_vec = normalize(cross(forward_vec, global_up));
    vector3 up_vec = normalize(cross(right_vec, forward_vec));
    
    matrix4 matrix;

    matrix.entries[0] = right_vec.entries[0];
    matrix.entries[1] = up_vec.entries[0];
    matrix.entries[2] = -forward_vec.entries[0];
    matrix.entries[3] = 0.0f;

    matrix.entries[4] = right_vec.entries[1];
    matrix.entries[5] = up_vec.entries[1];
    matrix.entries[6] = -forward_vec.entries[1];
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = right_vec.entries[2];
    matrix.entries[9] = up_vec.entries[2];
    matrix.entries[10] = -forward_vec.entries[2];
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = -dot(right_vec,from);
    matrix.entries[13] = -dot(up_vec,from);
    matrix.entries[14] = dot(forward_vec, from);
    matrix.entries[15] = 1.0f;

    return matrix;
}

matrix4 create_perspective_projection(float fovy, float aspect,
    float near, float far
){
    fovy = fovy * PI /(2.0f*180.0f);
    float tan = tanf(fovy);
    float n = -near;
    float f = -far;

    matrix4 matrix;
    for(int i = 0; i<16; ++i){
        matrix.entries[i]=0.0f;
    };

    matrix.entries[0] = 1.0f/(aspect*tan);
    matrix.entries[5]= 1.0f/tan;
    matrix.entries[10]= -(n+f)/(n-f);
    matrix.entries[11]= -1.0f;
    matrix.entries[14]= 2*n*f/(n-f);

    return matrix;

}

float dot(vector3 u, vector3 v){
    return u.entries[0]*v.entries[0]
            + u.entries[1] * v.entries[1]
            + u.entries[2] *v.entries[2];
}

vector3 normalize(vector3 v){
    vector3 u;

    float magnitude = sqrtf(dot(v,v));

    u.entries[0]=v.entries[0] / magnitude;
    u.entries[1]=v.entries[1] / magnitude;
    u.entries[2]=v.entries[2] / magnitude;

    return u;

}

vector3 cross(vector3 u, vector3 v){

    vector3 w;

    w.entries[0] = u.entries[1] * v.entries[2] - u.entries[2] * v.entries[1];
    w.entries[1] = -(u.entries[0]*v.entries[2] -u.entries[2]*v.entries[0]);
    w.entries[2] = u.entries[0]*v.entries[1]-u.entries[1]*v.entries[0];

    return w;
}
