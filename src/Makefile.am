# 公共头文件路径
AM_CPPFLAGS = -I$(top_srcdir)/include

# 编一个静态库，把算法实现都装进去
lib_LIBRARIES = libpixel_awa.a
libpixel_awa_a_SOURCES = pixel_awa_rgb_xyz.c

# 库和主程序都链数学库
libpixel_awa_a_LIBADD = -lm

# 主程序
# bin_PROGRAMS = rgb2lab
# rgb2lab_SOURCES = pixel_awa_main.c
# rgb2lab_LDADD = libpixel_awa.a -lm
