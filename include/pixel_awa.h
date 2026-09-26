#ifndef PIXEL_AWA_H
#define PIXEL_AWA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * libpixel_awa 鈥斺€� RGB / XYZ / LAB 鑹插僵绌洪棿杞崲搴�
 *
 * 杞崲閾撅細RGB 鈬� XYZ 鈬� LAB
 *
 *   RGB 鈹€鈹€纬瑙ｇ爜鈹€鈹€鈻� linearRGB 鈹€鈹€鐭╅樀鈹€鈹€鈻� XYZ 鈹€鈹€f(t)鈹€鈹€鈻� LAB
 *   LAB 鈹€鈹€f鈦宦光攢鈹€鈻� XYZ 鈹€鈹€閫嗙煩闃碘攢鈹€鈻� linearRGB 鈹€鈹€纬缂栫爜鈹€鈹€鈻� RGB
 *
 * 鏀寔鐧界偣锛欴65锛堥粯璁わ級銆丏50
 */

/* ============================================================
 * 1. 鏁版嵁绫诲瀷
 * ============================================================ */

/* 鐧界偣鏋氫妇 */
typedef enum {
    PIXEL_AWA_WP_D65 = 0,   /* 鏃ュ厜锛宻RGB / 鏄剧ず鍣ㄦ爣鍑� */
    PIXEL_AWA_WP_D50 = 1    /* 鍗板埛 / 棰勫嵃鏍囧噯 */
} pixel_awa_whitepoint_t;

/* 鐧界偣鍙傝€冨€硷紙XYZ锛孻=100锛� */
typedef struct {
    double x;
    double y;
    double z;
} pixel_awa_whitepoint_xyz_t;

/* LAB 棰滆壊 */
typedef struct {
    double l;   /* 0 - 100 */
    double a;   /* 閫氬父 -128 ~ +127 */
    double b;   /* 閫氬父 -128 ~ +127 */
} pixel_awa_lab_t;

/* RGB锛屽垎閲忚寖鍥� 0-255 */
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel_awa_rgb_t;

/* 鑹插樊绠楁硶 */
typedef enum {
    PIXEL_AWA_DELTA_E_CIE76 = 0,
    PIXEL_AWA_DELTA_E_CIE94 = 1,
    PIXEL_AWA_DELTA_E_CMC   = 2
} pixel_awa_delta_e_method_t;

/* 搴旂敤绫诲瀷锛圕IE94 鐢級 */
typedef enum {
    PIXEL_AWA_APPLICATION_GRAPHIC  = 0,   /* 鍗板埛 / 骞抽潰 */
    PIXEL_AWA_APPLICATION_TEXTILE  = 1    /* 绾虹粐 */
} pixel_awa_application_t;

/* ============================================================
 * 2. 鍒濆鍖栦笌鍏ㄥ眬璁剧疆
 * ============================================================ */

/*
 * 璁剧疆鐧界偣銆傛墍鏈夎浆鎹㈠嚱鏁伴兘浣跨敤褰撳墠鐧界偣銆�
 * 榛樿 D65銆�
 * 杩斿洖 0 鎴愬姛锛岄潪 0 琛ㄧず鏈煡鐧界偣銆�
 */
int  pixel_awa_set_whitepoint(pixel_awa_whitepoint_t wp);

/* 鑾峰彇褰撳墠鐧界偣 */
pixel_awa_whitepoint_t pixel_awa_get_whitepoint(void);

/* 鑾峰彇褰撳墠鐧界偣鐨� XYZ 鍙傝€冨€� */
void pixel_awa_get_whitepoint_xyz(double *x, double *y, double *z);

/* 搴撶増鏈瓧绗︿覆锛屽舰濡� "1.0.0" */
const char *pixel_awa_version(void);

/* ============================================================
 * 3. 鏍稿績杞崲鍑芥暟
 * ============================================================ */

/* RGB (0-255) 鈫� XYZ锛屽寘鍚� sRGB 纬 瑙ｇ爜 */
void pixel_awa_rgb_to_xyz(unsigned char r, unsigned char g, unsigned char b,
                          double *x, double *y, double *z);

/* XYZ 鈫� RGB (0-255)锛屽惈 纬 缂栫爜锛岀粨鏋� clamp 鍒� [0,255] */
void pixel_awa_xyz_to_rgb(double x, double y, double z,
                          unsigned char *r, unsigned char *g,
                          unsigned char *b);

/* RGB 鈫� LAB锛屼竴姝ュ埌浣� */
void pixel_awa_rgb_to_lab(unsigned char r, unsigned char g, unsigned char b,
                          double *l, double *a, double *b_out);

/* LAB 鈫� RGB锛屼竴姝ュ埌浣� */
void pixel_awa_lab_to_rgb(double l, double a, double b_out,
                          unsigned char *r, unsigned char *g,
                          unsigned char *b);

/*
 * XYZ 鈫� LAB
 * xyz_ref 涓虹櫧鐐瑰弬鑰冨€硷紙Xr, Yr, Zr锛夛紱浼� NULL 鍒欎娇鐢ㄥ綋鍓嶇櫧鐐广€�
 */
void pixel_awa_xyz_to_lab(double x, double y, double z,
                          double *l, double *a, double *b_out,
                          const double *xyz_ref);

/* LAB 鈫� XYZ */
void pixel_awa_lab_to_xyz(double l, double a, double b_out,
                          double *x, double *y, double *z,
                          const double *xyz_ref);

/* ============================================================
 * 4. 鑹插樊璁＄畻
 * ============================================================ */

/*
 * 璁＄畻涓や釜 LAB 棰滆壊涔嬮棿鐨勮壊宸� 螖E銆�
 *
 * method: CIE76 / CIE94 / CMC
 * app:    浠� CIE94 浣跨敤锛圙RAPHIC 鎴� TEXTILE锛�
 * l_c:    CMC 绠楁硶鐨� lightness 鏉冮噸锛堥粯璁� 2.0锛�
 * c_c:    CMC 绠楁硶鐨� chroma 鏉冮噸锛堥粯璁� 1.0锛�
 */
double pixel_awa_delta_e(const pixel_awa_lab_t *lab1,
                         const pixel_awa_lab_t *lab2,
                         pixel_awa_delta_e_method_t method,
                         pixel_awa_application_t app,
                         double l_c, double c_c);

/* CIE76 蹇嵎鏂瑰紡 */
double pixel_awa_delta_e_cie76(const pixel_awa_lab_t *lab1,
                               const pixel_awa_lab_t *lab2);

/* CIE94 蹇嵎鏂瑰紡 */
double pixel_awa_delta_e_cie94(const pixel_awa_lab_t *lab1,
                               const pixel_awa_lab_t *lab2,
                               pixel_awa_application_t app);

/* CMC 蹇嵎鏂瑰紡 */
double pixel_awa_delta_e_cmc(const pixel_awa_lab_t *lab1,
                             const pixel_awa_lab_t *lab2,
                             double l_c, double c_c);

/* ============================================================
 * 5. 宸ュ叿鍑芥暟
 * ============================================================ */

/* 鎶� RGB 鎵撳嵃鎴愬彲璇诲瓧绗︿覆鍒� buf锛堥渶 鈮� 32 瀛楄妭锛� */
void pixel_awa_rgb_to_string(unsigned char r, unsigned char g,
                             unsigned char b, char *buf, int buflen);

/* 鎶� LAB 鎵撳嵃鎴愬彲璇诲瓧绗︿覆鍒� buf锛堥渶 鈮� 64 瀛楄妭锛� */
void pixel_awa_lab_to_string(const pixel_awa_lab_t *lab,
                             char *buf, int buflen);

/*
 * 鍙屽悜浜掕瘉娴嬭瘯锛�
 *   瀵圭粰瀹� RGB 鍋� RGB鈫扡AB鈫扲GB锛岃繑鍥炲悇閫氶亾鏈€澶х粷瀵硅宸€�
 */
double pixel_awa_roundtrip_error(unsigned char r, unsigned char g,
                                 unsigned char b);

#ifdef __cplusplus
}
#endif

#endif /* PIXEL_AWA_H */
