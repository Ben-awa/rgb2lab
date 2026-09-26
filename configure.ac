AC_INIT([rgb2lab], [0.1.0], [lubancat@local])
AC_CONFIG_SRCDIR([src/pixel_awa_rgb_xyz.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])

# 编译器
AC_PROG_CC
AC_PROG_CC_C99

# 静态库工具（如果以后要编静态库需要）
AC_PROG_RANLIB
AM_PROG_AR
AC_SUBST([ARFLAGS], [crs])

# 检查数学库 libm
AC_CHECK_LIB([m], [pow], [], [
    AC_MSG_ERROR([libm (math library) is required but was not found])
])

# 检查头文件
AC_CHECK_HEADERS([stdio.h stdlib.h math.h string.h], [], [
    AC_MSG_ERROR([required header files are missing])
])

# 让 automake 知道要处理子目录
AC_CONFIG_FILES([
    Makefile
    include/Makefile
    src/Makefile
    tests/Makefile
])

AC_OUTPUT
