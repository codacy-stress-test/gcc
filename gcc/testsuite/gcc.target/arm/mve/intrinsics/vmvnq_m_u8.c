/* { dg-require-effective-target arm_v8_1m_mve_ok } */
/* { dg-add-options arm_v8_1m_mve } */
/* { dg-additional-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_mve.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
**foo:
**	...
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vmvnt	q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
uint8x16_t
foo (uint8x16_t inactive, uint8x16_t a, mve_pred16_t p)
{
  return vmvnq_m_u8 (inactive, a, p);
}


/*
**foo1:
**	...
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vmvnt	q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
uint8x16_t
foo1 (uint8x16_t inactive, uint8x16_t a, mve_pred16_t p)
{
  return vmvnq_m (inactive, a, p);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */
