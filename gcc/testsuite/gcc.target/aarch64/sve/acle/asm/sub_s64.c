/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** sub_s64_m_tied1:
**	sub	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_m_tied1, svint64_t,
		z0 = svsub_s64_m (p0, z0, z1),
		z0 = svsub_m (p0, z0, z1))

/*
** sub_s64_m_tied2:
**	mov	(z[0-9]+\.d), z0\.d
**	movprfx	z0, z1
**	sub	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (sub_s64_m_tied2, svint64_t,
		z0 = svsub_s64_m (p0, z1, z0),
		z0 = svsub_m (p0, z1, z0))

/*
** sub_s64_m_untied:
**	movprfx	z0, z1
**	sub	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_m_untied, svint64_t,
		z0 = svsub_s64_m (p0, z1, z2),
		z0 = svsub_m (p0, z1, z2))

/*
** sub_x0_s64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	sub	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (sub_x0_s64_m_tied1, svint64_t, int64_t,
		 z0 = svsub_n_s64_m (p0, z0, x0),
		 z0 = svsub_m (p0, z0, x0))

/*
** sub_x0_s64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	sub	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (sub_x0_s64_m_untied, svint64_t, int64_t,
		 z0 = svsub_n_s64_m (p0, z1, x0),
		 z0 = svsub_m (p0, z1, x0))

/*
** sub_1_s64_m_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	add	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_1_s64_m_tied1, svint64_t,
		z0 = svsub_n_s64_m (p0, z0, 1),
		z0 = svsub_m (p0, z0, 1))

/*
** sub_1_s64_m_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0, z1
**	add	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_1_s64_m_untied, svint64_t,
		z0 = svsub_n_s64_m (p0, z1, 1),
		z0 = svsub_m (p0, z1, 1))

/*
** sub_m2_s64_m:
**	mov	(z[0-9]+\.d), #2
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m2_s64_m, svint64_t,
		z0 = svsub_n_s64_m (p0, z0, -2),
		z0 = svsub_m (p0, z0, -2))

/*
** sub_s64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	sub	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_z_tied1, svint64_t,
		z0 = svsub_s64_z (p0, z0, z1),
		z0 = svsub_z (p0, z0, z1))

/*
** sub_s64_z_tied2:
**	movprfx	z0\.d, p0/z, z0\.d
**	subr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_z_tied2, svint64_t,
		z0 = svsub_s64_z (p0, z1, z0),
		z0 = svsub_z (p0, z1, z0))

/*
** sub_s64_z_untied:
** (
**	movprfx	z0\.d, p0/z, z1\.d
**	sub	z0\.d, p0/m, z0\.d, z2\.d
** |
**	movprfx	z0\.d, p0/z, z2\.d
**	subr	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_Z (sub_s64_z_untied, svint64_t,
		z0 = svsub_s64_z (p0, z1, z2),
		z0 = svsub_z (p0, z1, z2))

/*
** sub_x0_s64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	sub	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (sub_x0_s64_z_tied1, svint64_t, int64_t,
		 z0 = svsub_n_s64_z (p0, z0, x0),
		 z0 = svsub_z (p0, z0, x0))

/*
** sub_x0_s64_z_untied:
**	mov	(z[0-9]+\.d), x0
** (
**	movprfx	z0\.d, p0/z, z1\.d
**	sub	z0\.d, p0/m, z0\.d, \1
** |
**	movprfx	z0\.d, p0/z, \1
**	subr	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_ZX (sub_x0_s64_z_untied, svint64_t, int64_t,
		 z0 = svsub_n_s64_z (p0, z1, x0),
		 z0 = svsub_z (p0, z1, x0))

/*
** sub_1_s64_z_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0\.d, p0/z, z0\.d
**	add	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_1_s64_z_tied1, svint64_t,
		z0 = svsub_n_s64_z (p0, z0, 1),
		z0 = svsub_z (p0, z0, 1))

/*
** sub_1_s64_z_untied:
**	mov	(z[0-9]+)\.b, #-1
** (
**	movprfx	z0\.d, p0/z, z1\.d
**	add	z0\.d, p0/m, z0\.d, \1\.d
** |
**	movprfx	z0\.d, p0/z, \1\.d
**	add	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_Z (sub_1_s64_z_untied, svint64_t,
		z0 = svsub_n_s64_z (p0, z1, 1),
		z0 = svsub_z (p0, z1, 1))

/*
** sub_s64_x_tied1:
**	sub	z0\.d, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_x_tied1, svint64_t,
		z0 = svsub_s64_x (p0, z0, z1),
		z0 = svsub_x (p0, z0, z1))

/*
** sub_s64_x_tied2:
**	sub	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_x_tied2, svint64_t,
		z0 = svsub_s64_x (p0, z1, z0),
		z0 = svsub_x (p0, z1, z0))

/*
** sub_s64_x_untied:
**	sub	z0\.d, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (sub_s64_x_untied, svint64_t,
		z0 = svsub_s64_x (p0, z1, z2),
		z0 = svsub_x (p0, z1, z2))

/*
** sub_x0_s64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	sub	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (sub_x0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svsub_n_s64_x (p0, z0, x0),
		 z0 = svsub_x (p0, z0, x0))

/*
** sub_x0_s64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	sub	z0\.d, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (sub_x0_s64_x_untied, svint64_t, int64_t,
		 z0 = svsub_n_s64_x (p0, z1, x0),
		 z0 = svsub_x (p0, z1, x0))

/*
** sub_1_s64_x_tied1:
**	sub	z0\.d, z0\.d, #1
**	ret
*/
TEST_UNIFORM_Z (sub_1_s64_x_tied1, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 1),
		z0 = svsub_x (p0, z0, 1))

/*
** sub_1_s64_x_untied:
**	movprfx	z0, z1
**	sub	z0\.d, z0\.d, #1
**	ret
*/
TEST_UNIFORM_Z (sub_1_s64_x_untied, svint64_t,
		z0 = svsub_n_s64_x (p0, z1, 1),
		z0 = svsub_x (p0, z1, 1))

/*
** sub_127_s64_x:
**	sub	z0\.d, z0\.d, #127
**	ret
*/
TEST_UNIFORM_Z (sub_127_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 127),
		z0 = svsub_x (p0, z0, 127))

/*
** sub_128_s64_x:
**	sub	z0\.d, z0\.d, #128
**	ret
*/
TEST_UNIFORM_Z (sub_128_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 128),
		z0 = svsub_x (p0, z0, 128))

/*
** sub_255_s64_x:
**	sub	z0\.d, z0\.d, #255
**	ret
*/
TEST_UNIFORM_Z (sub_255_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 255),
		z0 = svsub_x (p0, z0, 255))

/*
** sub_256_s64_x:
**	sub	z0\.d, z0\.d, #256
**	ret
*/
TEST_UNIFORM_Z (sub_256_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 256),
		z0 = svsub_x (p0, z0, 256))

/*
** sub_511_s64_x:
**	mov	(z[0-9]+\.d), #-511
**	add	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (sub_511_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 511),
		z0 = svsub_x (p0, z0, 511))

/*
** sub_512_s64_x:
**	sub	z0\.d, z0\.d, #512
**	ret
*/
TEST_UNIFORM_Z (sub_512_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 512),
		z0 = svsub_x (p0, z0, 512))

/*
** sub_65280_s64_x:
**	sub	z0\.d, z0\.d, #65280
**	ret
*/
TEST_UNIFORM_Z (sub_65280_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 0xff00),
		z0 = svsub_x (p0, z0, 0xff00))

/*
** sub_65535_s64_x:
**	mov	(z[0-9]+\.d), #-65535
**	add	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (sub_65535_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 65535),
		z0 = svsub_x (p0, z0, 65535))

/*
** sub_65536_s64_x:
**	mov	(z[0-9]+\.d), #-65536
**	add	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (sub_65536_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, 65536),
		z0 = svsub_x (p0, z0, 65536))

/*
** sub_m1_s64_x:
**	add	z0\.d, z0\.d, #1
**	ret
*/
TEST_UNIFORM_Z (sub_m1_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -1),
		z0 = svsub_x (p0, z0, -1))

/*
** sub_m127_s64_x:
**	add	z0\.d, z0\.d, #127
**	ret
*/
TEST_UNIFORM_Z (sub_m127_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -127),
		z0 = svsub_x (p0, z0, -127))

/*
** sub_m128_s64_x:
**	add	z0\.d, z0\.d, #128
**	ret
*/
TEST_UNIFORM_Z (sub_m128_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -128),
		z0 = svsub_x (p0, z0, -128))

/*
** sub_m255_s64_x:
**	add	z0\.d, z0\.d, #255
**	ret
*/
TEST_UNIFORM_Z (sub_m255_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -255),
		z0 = svsub_x (p0, z0, -255))

/*
** sub_m256_s64_x:
**	add	z0\.d, z0\.d, #256
**	ret
*/
TEST_UNIFORM_Z (sub_m256_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -256),
		z0 = svsub_x (p0, z0, -256))

/*
** sub_m511_s64_x:
**	mov	(z[0-9]+\.d), #511
**	add	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (sub_m511_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -511),
		z0 = svsub_x (p0, z0, -511))

/*
** sub_m512_s64_x:
**	add	z0\.d, z0\.d, #512
**	ret
*/
TEST_UNIFORM_Z (sub_m512_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -512),
		z0 = svsub_x (p0, z0, -512))

/*
** sub_m32768_s64_x:
**	add	z0\.d, z0\.d, #32768
**	ret
*/
TEST_UNIFORM_Z (sub_m32768_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -0x8000),
		z0 = svsub_x (p0, z0, -0x8000))

/*
** sub_m65280_s64_x:
**	add	z0\.d, z0\.d, #65280
**	ret
*/
TEST_UNIFORM_Z (sub_m65280_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -0xff00),
		z0 = svsub_x (p0, z0, -0xff00))

/*
** sub_m65535_s64_x:
**	mov	(z[0-9]+\.d), #65535
**	add	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (sub_m65535_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -65535),
		z0 = svsub_x (p0, z0, -65535))

/*
** sub_m65536_s64_x:
**	mov	(z[0-9]+\.d), #65536
**	add	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (sub_m65536_s64_x, svint64_t,
		z0 = svsub_n_s64_x (p0, z0, -65536),
		z0 = svsub_x (p0, z0, -65536))
