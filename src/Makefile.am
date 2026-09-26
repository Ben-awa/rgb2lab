ARFLAGS = crs

# 公共头文件路径
AM_CPPFLAGS = -I$(top_srcdir)/include

# 编一个静态库，仅构建期使用，不安装到系统
noinst_LIBRARIES = libpixel_awa.a
libpixel_awa_a_SOURCES = pixel_awa_rgb_xyz.c
libpixel_awa_a_LIBADD = -lm
