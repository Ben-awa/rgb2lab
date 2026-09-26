#ifndef PIXEL_AWA_H
#define PIXEL_AWA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * libpixel_awa —— RGB / XYZ / LAB 色彩空间转换库
 *
 * 转换链：RGB ⇄ XYZ ⇄ LAB
 *
 *   RGB ──γ解码──▶ linearRGB ──矩阵──▶ XYZ ──f(t)──▶ LAB
 *   LAB ──f⁻¹──▶ XYZ ──逆矩阵──▶ linearRGB ──γ编码──▶ RGB
 *
 * 支持白点：D65（默认）、D50
 */

/* ============================================================
 * 1. 数据类型
 * ============================================================ */

/* 白点枚举 */
typedef enum {
    PIXEL_AWA_WP_D65 = 0,   /* 日光，sRGB / 显示器标准 */
    PIXEL_AWA_WP_D50 = 1    /* 印刷 / 预印标准 */
} pixel_awa_whitepoint_t;

/* 白点参考值（XYZ，Y=100） */
typedef struct {
    double x;
    double y;
    double z;
} pixel_awa_whitepoint_xyz_t;

/* LAB 颜色 */
typedef struct {
    double l;   /* 0 - 100 */
    double a;   /* 通常 -128 ~ +127 */
    double b;   /* 通常 -128 ~ +127 */
} pixel_awa_lab_t;

/* RGB，分量范围 0-255 */
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel_awa_rgb_t;

/* 色差算法 */
typedef enum {
    PIXEL_AWA_DELTA_E_CIE76 = 0,
    PIXEL_AWA_DELTA_E_CIE94 = 1,
    PIXEL_AWA_DELTA_E_CMC   = 2
} pixel_awa_delta_e_method_t;

/* 应用类型（CIE94 用） */
typedef enum {
    PIXEL_AWA_APPLICATION_GRAPHIC  = 0,   /* 印刷 / 平面 */
    PIXEL_AWA_APPLICATION_TEXTILE  = 1    /* 纺织 */
} pixel_awa_application_t;

/* ============================================================
 * 2. 初始化与全局设置
 * ============================================================ */

/*
 * 设置白点。所有转换函数都使用当前白点。
 * 默认 D65。
 * 返回 0 成功，非 0 表示未知白点。
 */
int  pixel_awa_set_whitepoint(pixel_awa_whitepoint_t wp);

/* 获取当前白点 */
pixel_awa_whitepoint_t pixel_awa_get_whitepoint(void);

/* 获取当前白点的 XYZ 参考值 */
void pixel_awa_get_whitepoint_xyz(double *x, double *y, double *z);

/* 库版本字符串，形如 "1.0.0" */
const char *pixel_awa_version(void);

/* ============================================================
 * 3. 核心转换函数
 * ============================================================ */

/* RGB (0-255) → XYZ，包含 sRGB γ 解码 */
void pixel_awa_rgb_to_xyz(unsigned char r, unsigned char g, unsigned char b,
                          double *x, double *y, double *z);

/* XYZ → RGB (0-255)，含 γ 编码，结果 clamp 到 [0,255] */
void pixel_awa_xyz_to_rgb(double x, double y, double z,
                          unsigned char *r, unsigned char *g,
                          unsigned char *b);

/* RGB → LAB，一步到位 */
void pixel_awa_rgb_to_lab(unsigned char r, unsigned char g, unsigned char b,
                          double *l, double *a, double *b_out);

/* LAB → RGB，一步到位 */
void pixel_awa_lab_to_rgb(double l, double a, double b_out,
                          unsigned char *r, unsigned char *g,
                          unsigned char *b);

/*
 * XYZ → LAB
 * xyz_ref 为白点参考值（Xr, Yr, Zr）；传 NULL 则使用当前白点。
 */
void pixel_awa_xyz_to_lab(double x, double y, double z,
                          double *l, double *a, double *b_out,
                          const double *xyz_ref);

/* LAB → XYZ */
void pixel_awa_lab_to_xyz(double l, double a, double b_out,
                          double *x, double *y, double *z,
                          const double *xyz_ref);

/* ============================================================
 * 4. 色差计算
 * ============================================================ */

/*
 * 计算两个 LAB 颜色之间的色差 ΔE。
 *
 * method: CIE76 / CIE94 / CMC
 * app:    仅 CIE94 使用（GRAPHIC 或 TEXTILE）
 * l_c:    CMC 算法的 lightness 权重（默认 2.0）
 * c_c:    CMC 算法的 chroma 权重（默认 1.0）
 */
double pixel_awa_delta_e(const pixel_awa_lab_t *lab1,
                         const pixel_awa_lab_t *lab2,
                         pixel_awa_delta_e_method_t method,
                         pixel_awa_application_t app,
                         double l_c, double c_c);

/* CIE76 快捷方式 */
double pixel_awa_delta_e_cie76(const pixel_awa_lab_t *lab1,
                               const pixel_awa_lab_t *lab2);

/* CIE94 快捷方式 */
double pixel_awa_delta_e_cie94(const pixel_awa_lab_t *lab1,
                               const pixel_awa_lab_t *lab2,
                               pixel_awa_application_t app);

/* CMC 快捷方式 */
double pixel_awa_delta_e_cmc(const pixel_awa_lab_t *lab1,
                             const pixel_awa_lab_t *lab2,
                             double l_c, double c_c);

/* ============================================================
 * 5. 工具函数
 * ============================================================ */

/* 把 RGB 打印成可读字符串到 buf（需 ≥ 32 字节） */
void pixel_awa_rgb_to_string(unsigned char r, unsigned char g,
                             unsigned char b, char *buf, int buflen);

/* 把 LAB 打印成可读字符串到 buf（需 ≥ 64 字节） */
void pixel_awa_lab_to_string(const pixel_awa_lab_t *lab,
                             char *buf, int buflen);

/*
 * 双向互证测试：
 *   对给定 RGB 做 RGB→LAB→RGB，返回各通道最大绝对误差。
 */
double pixel_awa_roundtrip_error(unsigned char r, unsigned char g,
                                 unsigned char b);

#ifdef __cplusplus
}
#endif

#endif /* PIXEL_AWA_H */
