/*
	Vec2 Struct
*/
typedef union {
    float nums[2];
    struct { float x, y; };
} Vec2;

/*
	Vec4 Struct
*/
typedef union {
    float nums[4];
    struct { float x, y, z, w; };
} Vec4;

/*
	Mat4 Struct
*/
typedef struct {
    float nums[4][4];
} Mat4;

/* Vec4 Functions */
Vec4 mul4x44(Mat4 m, Vec4 v) {
    Vec4 res;
    for(int x = 0; x < 4; ++x) {
        float sum = 0;
        for(int y = 0; y < 4; ++y)
            sum += m.nums[y][x] * v.nums[y];

        res.nums[x] = sum;
    }
    return res;
}

Vec4 vec4(float x, float y, float z, float w) {
	return (Vec4) {
		x, y, z, w
	};
}

/* Vec2 Functions */
static inline Vec2 vec2(float x, float y) {
    return (Vec2){ x, y };
}

static inline Vec2 vec2_f(float f) {
    return vec2(f, f);
}

static inline Vec2 vec2_rot(float rot) {
    return vec2(cosf(rot), sinf(rot));
}

static inline float rot_vec2(Vec2 rot) {
    return atan2f(rot.y, rot.x);
}

static inline Vec2 add2(Vec2 a, Vec2 b) {
    return vec2(a.x + b.x,
                a.y + b.y);
}

static inline Vec2 add2_f(Vec2 a, float f) {
    return vec2(a.x + f,
                a.y + f);
}

static inline Vec2 sub2(Vec2 a, Vec2 b) {
    return vec2(a.x - b.x,
                a.y - b.y);
}

static inline Vec2 sub2_f(Vec2 a, float f) {
    return vec2(a.x - f,
                a.y - f);
}

static inline Vec2 div2(Vec2 a, Vec2 b) {
    return vec2(a.x / b.x,
                a.y / b.y);
}

static inline Vec2 div2_f(Vec2 a, float f) {
    return vec2(a.x / f,
                a.y / f);
}

static inline Vec2 mul2(Vec2 a, Vec2 b) {
    return vec2(a.x * b.x,
                a.y * b.y);
}

static inline Vec2 mul2_f(Vec2 a, float f) {
    return vec2(a.x * f,
                a.y * f);
}

static inline float dot2(Vec2 a, Vec2 b) {
    return a.x*b.x + a.y*b.y;
}

static inline Vec2 lerp2(Vec2 a, Vec2 b, float t) {
    return add2(mul2_f(a, 1.0f - t), mul2_f(b, t));
}

static inline float mag2(Vec2 a) {
    return sqrtf(dot2(a, a));
}

static inline float magmag2(Vec2 a) {
    return dot2(a, a);
}

static inline Vec2 norm2(Vec2 a) {
    return div2_f(a, mag2(a));
}

/* Mat4 Functions */
static inline Mat4 scale4x4(Vec2 s) {
    return (Mat4) {
         s.x, 0.0f, 0.0f, 0.0f,
        0.0f,  s.y, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

static inline Mat4 rotate_z_4x4(float angle) {
    return (Mat4) {
        cosf(angle), -sinf(angle), 0.0f, 0.0f,
        sinf(angle),  cosf(angle), 0.0f, 0.0f,
               0.0f,        0.0f, 1.0f, 0.0f,
               0.0f,        0.0f, 0.0f, 1.0f,
    };
}

static inline Mat4 translate4x4(Vec2 t) {
    return (Mat4) {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
         t.x,  t.y, 0.0f, 1.0f,
    };
}

Mat4 mul4x4(Mat4 a, Mat4 b) {
    Mat4 out = {0};
    int k, r, c;
    for (c = 0; c < 4; ++c)
        for (r = 0; r < 4; ++r) {
            out.nums[c][r] = 0.0f;
            for (k = 0; k < 4; ++k)
                out.nums[c][r] += a.nums[k][r] * b.nums[c][k];
        }
    return out;
}

Mat4 invert4x4(Mat4 a) {
    float s[6], c[6];
    s[0] = a.nums[0][0]*a.nums[1][1] - a.nums[1][0]*a.nums[0][1];
    s[1] = a.nums[0][0]*a.nums[1][2] - a.nums[1][0]*a.nums[0][2];
    s[2] = a.nums[0][0]*a.nums[1][3] - a.nums[1][0]*a.nums[0][3];
    s[3] = a.nums[0][1]*a.nums[1][2] - a.nums[1][1]*a.nums[0][2];
    s[4] = a.nums[0][1]*a.nums[1][3] - a.nums[1][1]*a.nums[0][3];
    s[5] = a.nums[0][2]*a.nums[1][3] - a.nums[1][2]*a.nums[0][3];

    c[0] = a.nums[2][0]*a.nums[3][1] - a.nums[3][0]*a.nums[2][1];
    c[1] = a.nums[2][0]*a.nums[3][2] - a.nums[3][0]*a.nums[2][2];
    c[2] = a.nums[2][0]*a.nums[3][3] - a.nums[3][0]*a.nums[2][3];
    c[3] = a.nums[2][1]*a.nums[3][2] - a.nums[3][1]*a.nums[2][2];
    c[4] = a.nums[2][1]*a.nums[3][3] - a.nums[3][1]*a.nums[2][3];
    c[5] = a.nums[2][2]*a.nums[3][3] - a.nums[3][2]*a.nums[2][3];
    
    /* Assumes it is invertible */
    float idet = 1.0f/( s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0] );
    
    Mat4 res;
    res.nums[0][0] = ( a.nums[1][1] * c[5] - a.nums[1][2] * c[4] + a.nums[1][3] * c[3]) * idet;
    res.nums[0][1] = (-a.nums[0][1] * c[5] + a.nums[0][2] * c[4] - a.nums[0][3] * c[3]) * idet;
    res.nums[0][2] = ( a.nums[3][1] * s[5] - a.nums[3][2] * s[4] + a.nums[3][3] * s[3]) * idet;
    res.nums[0][3] = (-a.nums[2][1] * s[5] + a.nums[2][2] * s[4] - a.nums[2][3] * s[3]) * idet;

    res.nums[1][0] = (-a.nums[1][0] * c[5] + a.nums[1][2] * c[2] - a.nums[1][3] * c[1]) * idet;
    res.nums[1][1] = ( a.nums[0][0] * c[5] - a.nums[0][2] * c[2] + a.nums[0][3] * c[1]) * idet;
    res.nums[1][2] = (-a.nums[3][0] * s[5] + a.nums[3][2] * s[2] - a.nums[3][3] * s[1]) * idet;
    res.nums[1][3] = ( a.nums[2][0] * s[5] - a.nums[2][2] * s[2] + a.nums[2][3] * s[1]) * idet;

    res.nums[2][0] = ( a.nums[1][0] * c[4] - a.nums[1][1] * c[2] + a.nums[1][3] * c[0]) * idet;
    res.nums[2][1] = (-a.nums[0][0] * c[4] + a.nums[0][1] * c[2] - a.nums[0][3] * c[0]) * idet;
    res.nums[2][2] = ( a.nums[3][0] * s[4] - a.nums[3][1] * s[2] + a.nums[3][3] * s[0]) * idet;
    res.nums[2][3] = (-a.nums[2][0] * s[4] + a.nums[2][1] * s[2] - a.nums[2][3] * s[0]) * idet;

    res.nums[3][0] = (-a.nums[1][0] * c[3] + a.nums[1][1] * c[1] - a.nums[1][2] * c[0]) * idet;
    res.nums[3][1] = ( a.nums[0][0] * c[3] - a.nums[0][1] * c[1] + a.nums[0][2] * c[0]) * idet;
    res.nums[3][2] = (-a.nums[3][0] * s[3] + a.nums[3][1] * s[1] - a.nums[3][2] * s[0]) * idet;
    res.nums[3][3] = ( a.nums[2][0] * s[3] - a.nums[2][1] * s[1] + a.nums[2][2] * s[0]) * idet;
    return res;
}