#include "pixel_awa.h"

#include <math.h>
#include <string.h>

/* ============================================================
 * 鐧界偣鍙傝€冨€硷紙Y = 100锛�
 * D65: sRGB / 鏄剧ず鍣ㄦ爣鍑�
 * D50: 鍗板埛 / 棰勫嵃鏍囧噯
 * ============================================================ */
static const pixel_awa_whitepoint_xyz_t wp_table[2] = {
    /* D65 */ { 95.047, 100.000, 108.883 },
    /* D50 */ { 96.422, 100.000,  82.522 }
};

/* 褰撳墠鐧界偣锛岄粯璁� D65 */
static pixel_awa_whitepoint_t current_wp = PIXEL_AWA_WP_D65;

/* ============================================================
 * sRGB 纬 瑙ｇ爜 / 缂栫爜
 *
 *   C_srgb <= 0.04045  ->  C_lin = C_srgb / 12.92
 *   C_srgb  > 0.04045  ->  C_lin = ((C_srgb + 0.055) / 1.055)^2.4
 *
 * 閫嗗彉鎹紙纬 缂栫爜锛夌敤浜� XYZ -> RGB銆�
 * ============================================================ */
static double srgb_to_linear(double c)
{
    if (c <= 0.04045) {
        return c / 12.92;
    }
    return pow((c + 0.055) / 1.055, 2.4);
}

static double linear_to_srgb(double c)
{
    if (c <= 0.0031308) {
        return 12.92 * c;
    }
    return 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}

/* ============================================================
 * 鐧界偣鐩稿叧鎺ュ彛
 * ============================================================ */
int pixel_awa_set_whitepoint(pixel_awa_whitepoint_t wp)
{
    if (wp != PIXEL_AWA_WP_D65 && wp != PIXEL_AWA_WP_D50) {
        return -1;
    }
    current_wp = wp;
    return 0;
}

pixel_awa_whitepoint_t pixel_awa_get_whitepoint(void)
{
    return current_wp;
}

void pixel_awa_get_whitepoint_xyz(double *x, double *y, double *z)
{
    *x = wp_table[current_wp].x;
    *y = wp_table[current_wp].y;
    *z = wp_table[current_wp].z;
}

/* ============================================================
 * RGB -> XYZ
 * 姝ラ锛氬綊涓€鍖� [0,255] -> [0,1] -> 纬 瑙ｇ爜 -> 鐭╅樀鍙樻崲
 * ============================================================ */
void pixel_awa_rgb_to_xyz(unsigned char r, unsigned char g, unsigned char b,
                          double *x, double *y, double *z)
{
    /* 褰掍竴鍖� */
    double rn = (double)r / 255.0;
    double gn = (double)g / 255.0;
    double bn = (double)b / 255.0;

    /* 纬 瑙ｇ爜锛歴RGB 闈炵嚎鎬� -> 绾挎€� RGB */
    double rl = srgb_to_linear(rn);
    double gl = srgb_to_linear(gn);
    double bl = srgb_to_linear(bn);

    /*
     * sRGB -> XYZ 鍙樻崲鐭╅樀锛圖65 鐧界偣涓嬶級
     *   X = 0.4124564 R + 0.3575761 G + 0.1804375 B
     *   Y = 0.2126729 R + 0.7151522 G + 0.0721750 B
     *   Z = 0.0193339 R + 0.1191920 G + 0.9503041 B
     */
    *x =  0.4124564 * rl + 0.3575761 * gl + 0.1804375 * bl;
    *y =  0.2126729 * rl + 0.7151522 * gl + 0.0721750 * bl;
    *z =  0.0193339 * rl + 0.1191920 * gl + 0.9503041 * bl;

    /* 涔� 100锛屼娇 Y 鍙傝€冧负 100 */
    *x *= 100.0;
    *y *= 100.0;
    *z *= 100.0;
}

/* ============================================================
 * XYZ -> RGB
 * 姝ラ锛氶€嗙煩闃� -> 纬 缂栫爜 -> 鍙嶅綊涓€鍖� -> clamp 鍒� [0,255]
 * ============================================================ */
void pixel_awa_xyz_to_rgb(double x, double y, double z,
                          unsigned char *r, unsigned char *g,
                          unsigned char *b)
{
    double xr = x / 100.0;
    double yr = y / 100.0;
    double zr = z / 100.0;

    /*
     * XYZ -> sRGB 閫嗗彉鎹㈢煩闃�
     *   R =  3.2404542 X - 1.5371385 Y - 0.4985314 Z
     *   G = -0.9692660 X + 1.8760108 Y + 0.0415560 Z
     *   B =  0.0556434 X - 0.2040259 Y + 1.0572252 Z
     */
    double rl =  3.2404542 * xr - 1.5371385 * yr - 0.4985314 * zr;
    double gl = -0.9692660 * xr + 1.8760108 * yr + 0.0415560 * zr;
    double bl =  0.0556434 * xr - 0.2040259 * yr + 1.0572252 * zr;

    /* 纬 缂栫爜锛氱嚎鎬� -> sRGB 闈炵嚎鎬� */
    rl = linear_to_srgb(rl);
    gl = linear_to_srgb(gl);
    bl = linear_to_srgb(bl);

    /* 鍙嶅綊涓€鍖栧苟 clamp 鍒� [0,255] */
    int ri = (int)(rl * 255.0 + 0.5);
    int gi = (int)(gl * 255.0 + 0.5);
    int bi = (int)(bl * 255.0 + 0.5);

    if (ri < 0)   ri = 0;
    if (ri > 255) ri = 255;
    if (gi < 0)   gi = 0;
    if (gi > 255) gi = 255;
    if (bi < 0)   bi = 0;
    if (bi > 255) bi = 255;

    *r = (unsigned char)ri;
    *g = (unsigned char)gi;
    *b = (unsigned char)bi;
}
