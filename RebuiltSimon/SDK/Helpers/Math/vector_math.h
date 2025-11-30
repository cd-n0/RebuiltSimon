// TODO: metaprogram
#ifndef VECTOR_MATH_H_
#define VECTOR_MATH_H_

#include <math.h>

#define VMDEF static inline

VMDEF float vec2_dot(vec2_t lhs, vec2_t rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1];
}

VMDEF float vec2_sqrlen(vec2_t v)
{
    return vec2_dot(v, v);
}

VMDEF float vec2_len(vec2_t v)
{
    return sqrtf(vec2_sqrlen(v));
}

VMDEF float vec3_dot(vec3_t a, vec3_t b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

VMDEF float vec3_sqrlen(vec3_t v)
{
    return vec3_dot(v, v);
}

VMDEF float vec3_len(vec3_t v)
{
    return sqrtf(vec3_sqrlen(v));
}

#undef VMDEF

#endif /* VECTOR_MATH_H_ */