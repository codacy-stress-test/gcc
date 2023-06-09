/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** qadd_u8_tied1:
**	uqadd	z0\.b, (z0\.b, z1\.b|z1\.b, z0\.b)
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_tied1, svuint8_t,
		z0 = svqadd_u8 (z0, z1),
		z0 = svqadd (z0, z1))

/*
** qadd_u8_tied2:
**	uqadd	z0\.b, (z0\.b, z1\.b|z1\.b, z0\.b)
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_tied2, svuint8_t,
		z0 = svqadd_u8 (z1, z0),
		z0 = svqadd (z1, z0))

/*
** qadd_u8_untied:
**	uqadd	z0\.b, (z1\.b, z2\.b|z2\.b, z1\.b)
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_untied, svuint8_t,
		z0 = svqadd_u8 (z1, z2),
		z0 = svqadd (z1, z2))

/*
** qadd_w0_u8_tied1:
**	mov	(z[0-9]+\.b), w0
**	uqadd	z0\.b, (z0\.b, \1|\1, z0\.b)
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_tied1, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8 (z0, x0),
		 z0 = svqadd (z0, x0))

/*
** qadd_w0_u8_untied:
**	mov	(z[0-9]+\.b), w0
**	uqadd	z0\.b, (z1\.b, \1|\1, z1\.b)
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_untied, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8 (z1, x0),
		 z0 = svqadd (z1, x0))

/*
** qadd_1_u8_tied1:
**	uqadd	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_tied1, svuint8_t,
		z0 = svqadd_n_u8 (z0, 1),
		z0 = svqadd (z0, 1))

/*
** qadd_1_u8_untied:
**	movprfx	z0, z1
**	uqadd	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_untied, svuint8_t,
		z0 = svqadd_n_u8 (z1, 1),
		z0 = svqadd (z1, 1))

/*
** qadd_127_u8:
**	uqadd	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (qadd_127_u8, svuint8_t,
		z0 = svqadd_n_u8 (z0, 127),
		z0 = svqadd (z0, 127))

/*
** qadd_128_u8:
**	uqadd	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (qadd_128_u8, svuint8_t,
		z0 = svqadd_n_u8 (z0, 128),
		z0 = svqadd (z0, 128))

/*
** qadd_255_u8:
**	uqadd	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (qadd_255_u8, svuint8_t,
		z0 = svqadd_n_u8 (z0, 255),
		z0 = svqadd (z0, 255))

/*
** qadd_m1_u8:
**	uqadd	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (qadd_m1_u8, svuint8_t,
		z0 = svqadd_n_u8 (z0, -1),
		z0 = svqadd (z0, -1))

/*
** qadd_m127_u8:
**	uqadd	z0\.b, z0\.b, #129
**	ret
*/
TEST_UNIFORM_Z (qadd_m127_u8, svuint8_t,
		z0 = svqadd_n_u8 (z0, -127),
		z0 = svqadd (z0, -127))

/*
** qadd_m128_u8:
**	uqadd	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (qadd_m128_u8, svuint8_t,
		z0 = svqadd_n_u8 (z0, -128),
		z0 = svqadd (z0, -128))

/*
** qadd_u8_m_tied1:
**	uqadd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_m_tied1, svuint8_t,
		z0 = svqadd_u8_m (p0, z0, z1),
		z0 = svqadd_m (p0, z0, z1))

/*
** qadd_u8_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	uqadd	z0\.b, p0/m, z0\.b, \1\.b
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_m_tied2, svuint8_t,
		z0 = svqadd_u8_m (p0, z1, z0),
		z0 = svqadd_m (p0, z1, z0))

/*
** qadd_u8_m_untied:
**	movprfx	z0, z1
**	uqadd	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_m_untied, svuint8_t,
		z0 = svqadd_u8_m (p0, z1, z2),
		z0 = svqadd_m (p0, z1, z2))

/*
** qadd_w0_u8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_m_tied1, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8_m (p0, z0, x0),
		 z0 = svqadd_m (p0, z0, x0))

/*
** qadd_w0_u8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_m_untied, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8_m (p0, z1, x0),
		 z0 = svqadd_m (p0, z1, x0))

/*
** qadd_1_u8_m_tied1:
**	mov	(z[0-9]+\.b), #1
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_m_tied1, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, 1),
		z0 = svqadd_m (p0, z0, 1))

/*
** qadd_1_u8_m_untied:
**	mov	(z[0-9]+\.b), #1
**	movprfx	z0, z1
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_m_untied, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z1, 1),
		z0 = svqadd_m (p0, z1, 1))

/*
** qadd_127_u8_m:
**	mov	(z[0-9]+\.b), #127
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_127_u8_m, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, 127),
		z0 = svqadd_m (p0, z0, 127))

/*
** qadd_128_u8_m:
**	mov	(z[0-9]+\.b), #-128
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_128_u8_m, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, 128),
		z0 = svqadd_m (p0, z0, 128))

/*
** qadd_255_u8_m:
**	mov	(z[0-9]+\.b), #-1
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_255_u8_m, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, 255),
		z0 = svqadd_m (p0, z0, 255))

/*
** qadd_m1_u8_m:
**	mov	(z[0-9]+\.b), #-1
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_m1_u8_m, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, -1),
		z0 = svqadd_m (p0, z0, -1))

/*
** qadd_m127_u8_m:
**	mov	(z[0-9]+\.b), #-127
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_m127_u8_m, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, -127),
		z0 = svqadd_m (p0, z0, -127))

/*
** qadd_m128_u8_m:
**	mov	(z[0-9]+\.b), #-128
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_m128_u8_m, svuint8_t,
		z0 = svqadd_n_u8_m (p0, z0, -128),
		z0 = svqadd_m (p0, z0, -128))

/*
** qadd_u8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_z_tied1, svuint8_t,
		z0 = svqadd_u8_z (p0, z0, z1),
		z0 = svqadd_z (p0, z0, z1))

/*
** qadd_u8_z_tied2:
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_z_tied2, svuint8_t,
		z0 = svqadd_u8_z (p0, z1, z0),
		z0 = svqadd_z (p0, z1, z0))

/*
** qadd_u8_z_untied:
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	uqadd	z0\.b, p0/m, z0\.b, z2\.b
** |
**	movprfx	z0\.b, p0/z, z2\.b
**	uqadd	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_z_untied, svuint8_t,
		z0 = svqadd_u8_z (p0, z1, z2),
		z0 = svqadd_z (p0, z1, z2))

/*
** qadd_w0_u8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_z_tied1, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8_z (p0, z0, x0),
		 z0 = svqadd_z (p0, z0, x0))

/*
** qadd_w0_u8_z_untied:
**	mov	(z[0-9]+\.b), w0
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	uqadd	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8_z (p0, z1, x0),
		 z0 = svqadd_z (p0, z1, x0))

/*
** qadd_1_u8_z_tied1:
**	mov	(z[0-9]+\.b), #1
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_z_tied1, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, 1),
		z0 = svqadd_z (p0, z0, 1))

/*
** qadd_1_u8_z_untied:
**	mov	(z[0-9]+\.b), #1
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	uqadd	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_z_untied, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z1, 1),
		z0 = svqadd_z (p0, z1, 1))

/*
** qadd_127_u8_z:
**	mov	(z[0-9]+\.b), #127
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_127_u8_z, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, 127),
		z0 = svqadd_z (p0, z0, 127))

/*
** qadd_128_u8_z:
**	mov	(z[0-9]+\.b), #-128
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_128_u8_z, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, 128),
		z0 = svqadd_z (p0, z0, 128))

/*
** qadd_255_u8_z:
**	mov	(z[0-9]+\.b), #-1
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_255_u8_z, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, 255),
		z0 = svqadd_z (p0, z0, 255))

/*
** qadd_m1_u8_z:
**	mov	(z[0-9]+\.b), #-1
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_m1_u8_z, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, -1),
		z0 = svqadd_z (p0, z0, -1))

/*
** qadd_m127_u8_z:
**	mov	(z[0-9]+\.b), #-127
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_m127_u8_z, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, -127),
		z0 = svqadd_z (p0, z0, -127))

/*
** qadd_m128_u8_z:
**	mov	(z[0-9]+\.b), #-128
**	movprfx	z0\.b, p0/z, z0\.b
**	uqadd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (qadd_m128_u8_z, svuint8_t,
		z0 = svqadd_n_u8_z (p0, z0, -128),
		z0 = svqadd_z (p0, z0, -128))

/*
** qadd_u8_x_tied1:
**	uqadd	z0\.b, (z0\.b, z1\.b|z1\.b, z0\.b)
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_x_tied1, svuint8_t,
		z0 = svqadd_u8_x (p0, z0, z1),
		z0 = svqadd_x (p0, z0, z1))

/*
** qadd_u8_x_tied2:
**	uqadd	z0\.b, (z0\.b, z1\.b|z1\.b, z0\.b)
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_x_tied2, svuint8_t,
		z0 = svqadd_u8_x (p0, z1, z0),
		z0 = svqadd_x (p0, z1, z0))

/*
** qadd_u8_x_untied:
**	uqadd	z0\.b, (z1\.b, z2\.b|z2\.b, z1\.b)
**	ret
*/
TEST_UNIFORM_Z (qadd_u8_x_untied, svuint8_t,
		z0 = svqadd_u8_x (p0, z1, z2),
		z0 = svqadd_x (p0, z1, z2))

/*
** qadd_w0_u8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	uqadd	z0\.b, (z0\.b, \1|\1, z0\.b)
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_x_tied1, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8_x (p0, z0, x0),
		 z0 = svqadd_x (p0, z0, x0))

/*
** qadd_w0_u8_x_untied:
**	mov	(z[0-9]+\.b), w0
**	uqadd	z0\.b, (z1\.b, \1|\1, z1\.b)
**	ret
*/
TEST_UNIFORM_ZX (qadd_w0_u8_x_untied, svuint8_t, uint8_t,
		 z0 = svqadd_n_u8_x (p0, z1, x0),
		 z0 = svqadd_x (p0, z1, x0))

/*
** qadd_1_u8_x_tied1:
**	uqadd	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_x_tied1, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, 1),
		z0 = svqadd_x (p0, z0, 1))

/*
** qadd_1_u8_x_untied:
**	movprfx	z0, z1
**	uqadd	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (qadd_1_u8_x_untied, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z1, 1),
		z0 = svqadd_x (p0, z1, 1))

/*
** qadd_127_u8_x:
**	uqadd	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (qadd_127_u8_x, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, 127),
		z0 = svqadd_x (p0, z0, 127))

/*
** qadd_128_u8_x:
**	uqadd	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (qadd_128_u8_x, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, 128),
		z0 = svqadd_x (p0, z0, 128))

/*
** qadd_255_u8_x:
**	uqadd	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (qadd_255_u8_x, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, 255),
		z0 = svqadd_x (p0, z0, 255))

/*
** qadd_m1_u8_x:
**	uqadd	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (qadd_m1_u8_x, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, -1),
		z0 = svqadd_x (p0, z0, -1))

/*
** qadd_m127_u8_x:
**	uqadd	z0\.b, z0\.b, #129
**	ret
*/
TEST_UNIFORM_Z (qadd_m127_u8_x, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, -127),
		z0 = svqadd_x (p0, z0, -127))

/*
** qadd_m128_u8_x:
**	uqadd	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (qadd_m128_u8_x, svuint8_t,
		z0 = svqadd_n_u8_x (p0, z0, -128),
		z0 = svqadd_x (p0, z0, -128))
