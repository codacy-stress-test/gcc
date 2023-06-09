/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** pnext_b64_tied1:
**	pnext	p0\.d, p3, p0\.d
**	ret
*/
TEST_UNIFORM_P (pnext_b64_tied1,
		p0 = svpnext_b64 (p3, p0),
		p0 = svpnext_b64 (p3, p0))

/*
** pnext_b64_untied:
** (
**	mov	p0\.b, p1\.b
**	pnext	p0\.d, p3, p0\.d
** |
**	pnext	p1\.d, p3, p1\.d
**	mov	p0\.b, p1\.b
** )
**	ret
*/
TEST_UNIFORM_P (pnext_b64_untied,
		p0 = svpnext_b64 (p3, p1),
		p0 = svpnext_b64 (p3, p1))
