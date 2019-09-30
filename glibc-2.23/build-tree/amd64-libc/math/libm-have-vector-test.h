#if defined TEST_VECTOR_acos && TEST_VECTOR_acos
# define HAVE_VECTOR_acos 1
# define acos_VEC_SUFF WRAPPER_NAME (acos)
WRAPPER_DECL (WRAPPER_NAME (acos))
#else
# define HAVE_VECTOR_acos 0
# define acos_VEC_SUFF acos
#endif

#if defined TEST_VECTOR_acosf && TEST_VECTOR_acosf
# define HAVE_VECTOR_acosf 1
# define acosf_VEC_SUFF WRAPPER_NAME (acosf)
WRAPPER_DECL (WRAPPER_NAME (acosf))
#else
# define HAVE_VECTOR_acosf 0
# define acosf_VEC_SUFF acosf
#endif

#if defined TEST_VECTOR_acosl && TEST_VECTOR_acosl
# define HAVE_VECTOR_acosl 1
# define acosl_VEC_SUFF WRAPPER_NAME (acosl)
WRAPPER_DECL (WRAPPER_NAME (acosl))
#else
# define HAVE_VECTOR_acosl 0
# define acosl_VEC_SUFF acosl
#endif

#if defined TEST_VECTOR_acosh && TEST_VECTOR_acosh
# define HAVE_VECTOR_acosh 1
# define acosh_VEC_SUFF WRAPPER_NAME (acosh)
WRAPPER_DECL (WRAPPER_NAME (acosh))
#else
# define HAVE_VECTOR_acosh 0
# define acosh_VEC_SUFF acosh
#endif

#if defined TEST_VECTOR_acoshf && TEST_VECTOR_acoshf
# define HAVE_VECTOR_acoshf 1
# define acoshf_VEC_SUFF WRAPPER_NAME (acoshf)
WRAPPER_DECL (WRAPPER_NAME (acoshf))
#else
# define HAVE_VECTOR_acoshf 0
# define acoshf_VEC_SUFF acoshf
#endif

#if defined TEST_VECTOR_acoshl && TEST_VECTOR_acoshl
# define HAVE_VECTOR_acoshl 1
# define acoshl_VEC_SUFF WRAPPER_NAME (acoshl)
WRAPPER_DECL (WRAPPER_NAME (acoshl))
#else
# define HAVE_VECTOR_acoshl 0
# define acoshl_VEC_SUFF acoshl
#endif

#if defined TEST_VECTOR_asin && TEST_VECTOR_asin
# define HAVE_VECTOR_asin 1
# define asin_VEC_SUFF WRAPPER_NAME (asin)
WRAPPER_DECL (WRAPPER_NAME (asin))
#else
# define HAVE_VECTOR_asin 0
# define asin_VEC_SUFF asin
#endif

#if defined TEST_VECTOR_asinf && TEST_VECTOR_asinf
# define HAVE_VECTOR_asinf 1
# define asinf_VEC_SUFF WRAPPER_NAME (asinf)
WRAPPER_DECL (WRAPPER_NAME (asinf))
#else
# define HAVE_VECTOR_asinf 0
# define asinf_VEC_SUFF asinf
#endif

#if defined TEST_VECTOR_asinl && TEST_VECTOR_asinl
# define HAVE_VECTOR_asinl 1
# define asinl_VEC_SUFF WRAPPER_NAME (asinl)
WRAPPER_DECL (WRAPPER_NAME (asinl))
#else
# define HAVE_VECTOR_asinl 0
# define asinl_VEC_SUFF asinl
#endif

#if defined TEST_VECTOR_asinh && TEST_VECTOR_asinh
# define HAVE_VECTOR_asinh 1
# define asinh_VEC_SUFF WRAPPER_NAME (asinh)
WRAPPER_DECL (WRAPPER_NAME (asinh))
#else
# define HAVE_VECTOR_asinh 0
# define asinh_VEC_SUFF asinh
#endif

#if defined TEST_VECTOR_asinhf && TEST_VECTOR_asinhf
# define HAVE_VECTOR_asinhf 1
# define asinhf_VEC_SUFF WRAPPER_NAME (asinhf)
WRAPPER_DECL (WRAPPER_NAME (asinhf))
#else
# define HAVE_VECTOR_asinhf 0
# define asinhf_VEC_SUFF asinhf
#endif

#if defined TEST_VECTOR_asinhl && TEST_VECTOR_asinhl
# define HAVE_VECTOR_asinhl 1
# define asinhl_VEC_SUFF WRAPPER_NAME (asinhl)
WRAPPER_DECL (WRAPPER_NAME (asinhl))
#else
# define HAVE_VECTOR_asinhl 0
# define asinhl_VEC_SUFF asinhl
#endif

#if defined TEST_VECTOR_atan && TEST_VECTOR_atan
# define HAVE_VECTOR_atan 1
# define atan_VEC_SUFF WRAPPER_NAME (atan)
WRAPPER_DECL (WRAPPER_NAME (atan))
#else
# define HAVE_VECTOR_atan 0
# define atan_VEC_SUFF atan
#endif

#if defined TEST_VECTOR_atanf && TEST_VECTOR_atanf
# define HAVE_VECTOR_atanf 1
# define atanf_VEC_SUFF WRAPPER_NAME (atanf)
WRAPPER_DECL (WRAPPER_NAME (atanf))
#else
# define HAVE_VECTOR_atanf 0
# define atanf_VEC_SUFF atanf
#endif

#if defined TEST_VECTOR_atanl && TEST_VECTOR_atanl
# define HAVE_VECTOR_atanl 1
# define atanl_VEC_SUFF WRAPPER_NAME (atanl)
WRAPPER_DECL (WRAPPER_NAME (atanl))
#else
# define HAVE_VECTOR_atanl 0
# define atanl_VEC_SUFF atanl
#endif

#if defined TEST_VECTOR_atanh && TEST_VECTOR_atanh
# define HAVE_VECTOR_atanh 1
# define atanh_VEC_SUFF WRAPPER_NAME (atanh)
WRAPPER_DECL (WRAPPER_NAME (atanh))
#else
# define HAVE_VECTOR_atanh 0
# define atanh_VEC_SUFF atanh
#endif

#if defined TEST_VECTOR_atanhf && TEST_VECTOR_atanhf
# define HAVE_VECTOR_atanhf 1
# define atanhf_VEC_SUFF WRAPPER_NAME (atanhf)
WRAPPER_DECL (WRAPPER_NAME (atanhf))
#else
# define HAVE_VECTOR_atanhf 0
# define atanhf_VEC_SUFF atanhf
#endif

#if defined TEST_VECTOR_atanhl && TEST_VECTOR_atanhl
# define HAVE_VECTOR_atanhl 1
# define atanhl_VEC_SUFF WRAPPER_NAME (atanhl)
WRAPPER_DECL (WRAPPER_NAME (atanhl))
#else
# define HAVE_VECTOR_atanhl 0
# define atanhl_VEC_SUFF atanhl
#endif

#if defined TEST_VECTOR_cabs && TEST_VECTOR_cabs
# define HAVE_VECTOR_cabs 1
# define cabs_VEC_SUFF WRAPPER_NAME (cabs)
WRAPPER_DECL (WRAPPER_NAME (cabs))
#else
# define HAVE_VECTOR_cabs 0
# define cabs_VEC_SUFF cabs
#endif

#if defined TEST_VECTOR_cabsf && TEST_VECTOR_cabsf
# define HAVE_VECTOR_cabsf 1
# define cabsf_VEC_SUFF WRAPPER_NAME (cabsf)
WRAPPER_DECL (WRAPPER_NAME (cabsf))
#else
# define HAVE_VECTOR_cabsf 0
# define cabsf_VEC_SUFF cabsf
#endif

#if defined TEST_VECTOR_cabsl && TEST_VECTOR_cabsl
# define HAVE_VECTOR_cabsl 1
# define cabsl_VEC_SUFF WRAPPER_NAME (cabsl)
WRAPPER_DECL (WRAPPER_NAME (cabsl))
#else
# define HAVE_VECTOR_cabsl 0
# define cabsl_VEC_SUFF cabsl
#endif

#if defined TEST_VECTOR_cacos && TEST_VECTOR_cacos
# define HAVE_VECTOR_cacos 1
# define cacos_VEC_SUFF WRAPPER_NAME (cacos)
WRAPPER_DECL (WRAPPER_NAME (cacos))
#else
# define HAVE_VECTOR_cacos 0
# define cacos_VEC_SUFF cacos
#endif

#if defined TEST_VECTOR_cacosf && TEST_VECTOR_cacosf
# define HAVE_VECTOR_cacosf 1
# define cacosf_VEC_SUFF WRAPPER_NAME (cacosf)
WRAPPER_DECL (WRAPPER_NAME (cacosf))
#else
# define HAVE_VECTOR_cacosf 0
# define cacosf_VEC_SUFF cacosf
#endif

#if defined TEST_VECTOR_cacosl && TEST_VECTOR_cacosl
# define HAVE_VECTOR_cacosl 1
# define cacosl_VEC_SUFF WRAPPER_NAME (cacosl)
WRAPPER_DECL (WRAPPER_NAME (cacosl))
#else
# define HAVE_VECTOR_cacosl 0
# define cacosl_VEC_SUFF cacosl
#endif

#if defined TEST_VECTOR_cacosh && TEST_VECTOR_cacosh
# define HAVE_VECTOR_cacosh 1
# define cacosh_VEC_SUFF WRAPPER_NAME (cacosh)
WRAPPER_DECL (WRAPPER_NAME (cacosh))
#else
# define HAVE_VECTOR_cacosh 0
# define cacosh_VEC_SUFF cacosh
#endif

#if defined TEST_VECTOR_cacoshf && TEST_VECTOR_cacoshf
# define HAVE_VECTOR_cacoshf 1
# define cacoshf_VEC_SUFF WRAPPER_NAME (cacoshf)
WRAPPER_DECL (WRAPPER_NAME (cacoshf))
#else
# define HAVE_VECTOR_cacoshf 0
# define cacoshf_VEC_SUFF cacoshf
#endif

#if defined TEST_VECTOR_cacoshl && TEST_VECTOR_cacoshl
# define HAVE_VECTOR_cacoshl 1
# define cacoshl_VEC_SUFF WRAPPER_NAME (cacoshl)
WRAPPER_DECL (WRAPPER_NAME (cacoshl))
#else
# define HAVE_VECTOR_cacoshl 0
# define cacoshl_VEC_SUFF cacoshl
#endif

#if defined TEST_VECTOR_carg && TEST_VECTOR_carg
# define HAVE_VECTOR_carg 1
# define carg_VEC_SUFF WRAPPER_NAME (carg)
WRAPPER_DECL (WRAPPER_NAME (carg))
#else
# define HAVE_VECTOR_carg 0
# define carg_VEC_SUFF carg
#endif

#if defined TEST_VECTOR_cargf && TEST_VECTOR_cargf
# define HAVE_VECTOR_cargf 1
# define cargf_VEC_SUFF WRAPPER_NAME (cargf)
WRAPPER_DECL (WRAPPER_NAME (cargf))
#else
# define HAVE_VECTOR_cargf 0
# define cargf_VEC_SUFF cargf
#endif

#if defined TEST_VECTOR_cargl && TEST_VECTOR_cargl
# define HAVE_VECTOR_cargl 1
# define cargl_VEC_SUFF WRAPPER_NAME (cargl)
WRAPPER_DECL (WRAPPER_NAME (cargl))
#else
# define HAVE_VECTOR_cargl 0
# define cargl_VEC_SUFF cargl
#endif

#if defined TEST_VECTOR_casin && TEST_VECTOR_casin
# define HAVE_VECTOR_casin 1
# define casin_VEC_SUFF WRAPPER_NAME (casin)
WRAPPER_DECL (WRAPPER_NAME (casin))
#else
# define HAVE_VECTOR_casin 0
# define casin_VEC_SUFF casin
#endif

#if defined TEST_VECTOR_casinf && TEST_VECTOR_casinf
# define HAVE_VECTOR_casinf 1
# define casinf_VEC_SUFF WRAPPER_NAME (casinf)
WRAPPER_DECL (WRAPPER_NAME (casinf))
#else
# define HAVE_VECTOR_casinf 0
# define casinf_VEC_SUFF casinf
#endif

#if defined TEST_VECTOR_casinl && TEST_VECTOR_casinl
# define HAVE_VECTOR_casinl 1
# define casinl_VEC_SUFF WRAPPER_NAME (casinl)
WRAPPER_DECL (WRAPPER_NAME (casinl))
#else
# define HAVE_VECTOR_casinl 0
# define casinl_VEC_SUFF casinl
#endif

#if defined TEST_VECTOR_casinh && TEST_VECTOR_casinh
# define HAVE_VECTOR_casinh 1
# define casinh_VEC_SUFF WRAPPER_NAME (casinh)
WRAPPER_DECL (WRAPPER_NAME (casinh))
#else
# define HAVE_VECTOR_casinh 0
# define casinh_VEC_SUFF casinh
#endif

#if defined TEST_VECTOR_casinhf && TEST_VECTOR_casinhf
# define HAVE_VECTOR_casinhf 1
# define casinhf_VEC_SUFF WRAPPER_NAME (casinhf)
WRAPPER_DECL (WRAPPER_NAME (casinhf))
#else
# define HAVE_VECTOR_casinhf 0
# define casinhf_VEC_SUFF casinhf
#endif

#if defined TEST_VECTOR_casinhl && TEST_VECTOR_casinhl
# define HAVE_VECTOR_casinhl 1
# define casinhl_VEC_SUFF WRAPPER_NAME (casinhl)
WRAPPER_DECL (WRAPPER_NAME (casinhl))
#else
# define HAVE_VECTOR_casinhl 0
# define casinhl_VEC_SUFF casinhl
#endif

#if defined TEST_VECTOR_catan && TEST_VECTOR_catan
# define HAVE_VECTOR_catan 1
# define catan_VEC_SUFF WRAPPER_NAME (catan)
WRAPPER_DECL (WRAPPER_NAME (catan))
#else
# define HAVE_VECTOR_catan 0
# define catan_VEC_SUFF catan
#endif

#if defined TEST_VECTOR_catanf && TEST_VECTOR_catanf
# define HAVE_VECTOR_catanf 1
# define catanf_VEC_SUFF WRAPPER_NAME (catanf)
WRAPPER_DECL (WRAPPER_NAME (catanf))
#else
# define HAVE_VECTOR_catanf 0
# define catanf_VEC_SUFF catanf
#endif

#if defined TEST_VECTOR_catanl && TEST_VECTOR_catanl
# define HAVE_VECTOR_catanl 1
# define catanl_VEC_SUFF WRAPPER_NAME (catanl)
WRAPPER_DECL (WRAPPER_NAME (catanl))
#else
# define HAVE_VECTOR_catanl 0
# define catanl_VEC_SUFF catanl
#endif

#if defined TEST_VECTOR_catanh && TEST_VECTOR_catanh
# define HAVE_VECTOR_catanh 1
# define catanh_VEC_SUFF WRAPPER_NAME (catanh)
WRAPPER_DECL (WRAPPER_NAME (catanh))
#else
# define HAVE_VECTOR_catanh 0
# define catanh_VEC_SUFF catanh
#endif

#if defined TEST_VECTOR_catanhf && TEST_VECTOR_catanhf
# define HAVE_VECTOR_catanhf 1
# define catanhf_VEC_SUFF WRAPPER_NAME (catanhf)
WRAPPER_DECL (WRAPPER_NAME (catanhf))
#else
# define HAVE_VECTOR_catanhf 0
# define catanhf_VEC_SUFF catanhf
#endif

#if defined TEST_VECTOR_catanhl && TEST_VECTOR_catanhl
# define HAVE_VECTOR_catanhl 1
# define catanhl_VEC_SUFF WRAPPER_NAME (catanhl)
WRAPPER_DECL (WRAPPER_NAME (catanhl))
#else
# define HAVE_VECTOR_catanhl 0
# define catanhl_VEC_SUFF catanhl
#endif

#if defined TEST_VECTOR_cbrt && TEST_VECTOR_cbrt
# define HAVE_VECTOR_cbrt 1
# define cbrt_VEC_SUFF WRAPPER_NAME (cbrt)
WRAPPER_DECL (WRAPPER_NAME (cbrt))
#else
# define HAVE_VECTOR_cbrt 0
# define cbrt_VEC_SUFF cbrt
#endif

#if defined TEST_VECTOR_cbrtf && TEST_VECTOR_cbrtf
# define HAVE_VECTOR_cbrtf 1
# define cbrtf_VEC_SUFF WRAPPER_NAME (cbrtf)
WRAPPER_DECL (WRAPPER_NAME (cbrtf))
#else
# define HAVE_VECTOR_cbrtf 0
# define cbrtf_VEC_SUFF cbrtf
#endif

#if defined TEST_VECTOR_cbrtl && TEST_VECTOR_cbrtl
# define HAVE_VECTOR_cbrtl 1
# define cbrtl_VEC_SUFF WRAPPER_NAME (cbrtl)
WRAPPER_DECL (WRAPPER_NAME (cbrtl))
#else
# define HAVE_VECTOR_cbrtl 0
# define cbrtl_VEC_SUFF cbrtl
#endif

#if defined TEST_VECTOR_ccos && TEST_VECTOR_ccos
# define HAVE_VECTOR_ccos 1
# define ccos_VEC_SUFF WRAPPER_NAME (ccos)
WRAPPER_DECL (WRAPPER_NAME (ccos))
#else
# define HAVE_VECTOR_ccos 0
# define ccos_VEC_SUFF ccos
#endif

#if defined TEST_VECTOR_ccosf && TEST_VECTOR_ccosf
# define HAVE_VECTOR_ccosf 1
# define ccosf_VEC_SUFF WRAPPER_NAME (ccosf)
WRAPPER_DECL (WRAPPER_NAME (ccosf))
#else
# define HAVE_VECTOR_ccosf 0
# define ccosf_VEC_SUFF ccosf
#endif

#if defined TEST_VECTOR_ccosl && TEST_VECTOR_ccosl
# define HAVE_VECTOR_ccosl 1
# define ccosl_VEC_SUFF WRAPPER_NAME (ccosl)
WRAPPER_DECL (WRAPPER_NAME (ccosl))
#else
# define HAVE_VECTOR_ccosl 0
# define ccosl_VEC_SUFF ccosl
#endif

#if defined TEST_VECTOR_ccosh && TEST_VECTOR_ccosh
# define HAVE_VECTOR_ccosh 1
# define ccosh_VEC_SUFF WRAPPER_NAME (ccosh)
WRAPPER_DECL (WRAPPER_NAME (ccosh))
#else
# define HAVE_VECTOR_ccosh 0
# define ccosh_VEC_SUFF ccosh
#endif

#if defined TEST_VECTOR_ccoshf && TEST_VECTOR_ccoshf
# define HAVE_VECTOR_ccoshf 1
# define ccoshf_VEC_SUFF WRAPPER_NAME (ccoshf)
WRAPPER_DECL (WRAPPER_NAME (ccoshf))
#else
# define HAVE_VECTOR_ccoshf 0
# define ccoshf_VEC_SUFF ccoshf
#endif

#if defined TEST_VECTOR_ccoshl && TEST_VECTOR_ccoshl
# define HAVE_VECTOR_ccoshl 1
# define ccoshl_VEC_SUFF WRAPPER_NAME (ccoshl)
WRAPPER_DECL (WRAPPER_NAME (ccoshl))
#else
# define HAVE_VECTOR_ccoshl 0
# define ccoshl_VEC_SUFF ccoshl
#endif

#if defined TEST_VECTOR_ceil && TEST_VECTOR_ceil
# define HAVE_VECTOR_ceil 1
# define ceil_VEC_SUFF WRAPPER_NAME (ceil)
WRAPPER_DECL (WRAPPER_NAME (ceil))
#else
# define HAVE_VECTOR_ceil 0
# define ceil_VEC_SUFF ceil
#endif

#if defined TEST_VECTOR_ceilf && TEST_VECTOR_ceilf
# define HAVE_VECTOR_ceilf 1
# define ceilf_VEC_SUFF WRAPPER_NAME (ceilf)
WRAPPER_DECL (WRAPPER_NAME (ceilf))
#else
# define HAVE_VECTOR_ceilf 0
# define ceilf_VEC_SUFF ceilf
#endif

#if defined TEST_VECTOR_ceill && TEST_VECTOR_ceill
# define HAVE_VECTOR_ceill 1
# define ceill_VEC_SUFF WRAPPER_NAME (ceill)
WRAPPER_DECL (WRAPPER_NAME (ceill))
#else
# define HAVE_VECTOR_ceill 0
# define ceill_VEC_SUFF ceill
#endif

#if defined TEST_VECTOR_cexp && TEST_VECTOR_cexp
# define HAVE_VECTOR_cexp 1
# define cexp_VEC_SUFF WRAPPER_NAME (cexp)
WRAPPER_DECL (WRAPPER_NAME (cexp))
#else
# define HAVE_VECTOR_cexp 0
# define cexp_VEC_SUFF cexp
#endif

#if defined TEST_VECTOR_cexpf && TEST_VECTOR_cexpf
# define HAVE_VECTOR_cexpf 1
# define cexpf_VEC_SUFF WRAPPER_NAME (cexpf)
WRAPPER_DECL (WRAPPER_NAME (cexpf))
#else
# define HAVE_VECTOR_cexpf 0
# define cexpf_VEC_SUFF cexpf
#endif

#if defined TEST_VECTOR_cexpl && TEST_VECTOR_cexpl
# define HAVE_VECTOR_cexpl 1
# define cexpl_VEC_SUFF WRAPPER_NAME (cexpl)
WRAPPER_DECL (WRAPPER_NAME (cexpl))
#else
# define HAVE_VECTOR_cexpl 0
# define cexpl_VEC_SUFF cexpl
#endif

#if defined TEST_VECTOR_cimag && TEST_VECTOR_cimag
# define HAVE_VECTOR_cimag 1
# define cimag_VEC_SUFF WRAPPER_NAME (cimag)
WRAPPER_DECL (WRAPPER_NAME (cimag))
#else
# define HAVE_VECTOR_cimag 0
# define cimag_VEC_SUFF cimag
#endif

#if defined TEST_VECTOR_cimagf && TEST_VECTOR_cimagf
# define HAVE_VECTOR_cimagf 1
# define cimagf_VEC_SUFF WRAPPER_NAME (cimagf)
WRAPPER_DECL (WRAPPER_NAME (cimagf))
#else
# define HAVE_VECTOR_cimagf 0
# define cimagf_VEC_SUFF cimagf
#endif

#if defined TEST_VECTOR_cimagl && TEST_VECTOR_cimagl
# define HAVE_VECTOR_cimagl 1
# define cimagl_VEC_SUFF WRAPPER_NAME (cimagl)
WRAPPER_DECL (WRAPPER_NAME (cimagl))
#else
# define HAVE_VECTOR_cimagl 0
# define cimagl_VEC_SUFF cimagl
#endif

#if defined TEST_VECTOR_clog && TEST_VECTOR_clog
# define HAVE_VECTOR_clog 1
# define clog_VEC_SUFF WRAPPER_NAME (clog)
WRAPPER_DECL (WRAPPER_NAME (clog))
#else
# define HAVE_VECTOR_clog 0
# define clog_VEC_SUFF clog
#endif

#if defined TEST_VECTOR_clogf && TEST_VECTOR_clogf
# define HAVE_VECTOR_clogf 1
# define clogf_VEC_SUFF WRAPPER_NAME (clogf)
WRAPPER_DECL (WRAPPER_NAME (clogf))
#else
# define HAVE_VECTOR_clogf 0
# define clogf_VEC_SUFF clogf
#endif

#if defined TEST_VECTOR_clogl && TEST_VECTOR_clogl
# define HAVE_VECTOR_clogl 1
# define clogl_VEC_SUFF WRAPPER_NAME (clogl)
WRAPPER_DECL (WRAPPER_NAME (clogl))
#else
# define HAVE_VECTOR_clogl 0
# define clogl_VEC_SUFF clogl
#endif

#if defined TEST_VECTOR_clog10 && TEST_VECTOR_clog10
# define HAVE_VECTOR_clog10 1
# define clog10_VEC_SUFF WRAPPER_NAME (clog10)
WRAPPER_DECL (WRAPPER_NAME (clog10))
#else
# define HAVE_VECTOR_clog10 0
# define clog10_VEC_SUFF clog10
#endif

#if defined TEST_VECTOR_clog10f && TEST_VECTOR_clog10f
# define HAVE_VECTOR_clog10f 1
# define clog10f_VEC_SUFF WRAPPER_NAME (clog10f)
WRAPPER_DECL (WRAPPER_NAME (clog10f))
#else
# define HAVE_VECTOR_clog10f 0
# define clog10f_VEC_SUFF clog10f
#endif

#if defined TEST_VECTOR_clog10l && TEST_VECTOR_clog10l
# define HAVE_VECTOR_clog10l 1
# define clog10l_VEC_SUFF WRAPPER_NAME (clog10l)
WRAPPER_DECL (WRAPPER_NAME (clog10l))
#else
# define HAVE_VECTOR_clog10l 0
# define clog10l_VEC_SUFF clog10l
#endif

#if defined TEST_VECTOR_conj && TEST_VECTOR_conj
# define HAVE_VECTOR_conj 1
# define conj_VEC_SUFF WRAPPER_NAME (conj)
WRAPPER_DECL (WRAPPER_NAME (conj))
#else
# define HAVE_VECTOR_conj 0
# define conj_VEC_SUFF conj
#endif

#if defined TEST_VECTOR_conjf && TEST_VECTOR_conjf
# define HAVE_VECTOR_conjf 1
# define conjf_VEC_SUFF WRAPPER_NAME (conjf)
WRAPPER_DECL (WRAPPER_NAME (conjf))
#else
# define HAVE_VECTOR_conjf 0
# define conjf_VEC_SUFF conjf
#endif

#if defined TEST_VECTOR_conjl && TEST_VECTOR_conjl
# define HAVE_VECTOR_conjl 1
# define conjl_VEC_SUFF WRAPPER_NAME (conjl)
WRAPPER_DECL (WRAPPER_NAME (conjl))
#else
# define HAVE_VECTOR_conjl 0
# define conjl_VEC_SUFF conjl
#endif

#if defined TEST_VECTOR_cos && TEST_VECTOR_cos
# define HAVE_VECTOR_cos 1
# define cos_VEC_SUFF WRAPPER_NAME (cos)
WRAPPER_DECL (WRAPPER_NAME (cos))
#else
# define HAVE_VECTOR_cos 0
# define cos_VEC_SUFF cos
#endif

#if defined TEST_VECTOR_cosf && TEST_VECTOR_cosf
# define HAVE_VECTOR_cosf 1
# define cosf_VEC_SUFF WRAPPER_NAME (cosf)
WRAPPER_DECL (WRAPPER_NAME (cosf))
#else
# define HAVE_VECTOR_cosf 0
# define cosf_VEC_SUFF cosf
#endif

#if defined TEST_VECTOR_cosl && TEST_VECTOR_cosl
# define HAVE_VECTOR_cosl 1
# define cosl_VEC_SUFF WRAPPER_NAME (cosl)
WRAPPER_DECL (WRAPPER_NAME (cosl))
#else
# define HAVE_VECTOR_cosl 0
# define cosl_VEC_SUFF cosl
#endif

#if defined TEST_VECTOR_cosh && TEST_VECTOR_cosh
# define HAVE_VECTOR_cosh 1
# define cosh_VEC_SUFF WRAPPER_NAME (cosh)
WRAPPER_DECL (WRAPPER_NAME (cosh))
#else
# define HAVE_VECTOR_cosh 0
# define cosh_VEC_SUFF cosh
#endif

#if defined TEST_VECTOR_coshf && TEST_VECTOR_coshf
# define HAVE_VECTOR_coshf 1
# define coshf_VEC_SUFF WRAPPER_NAME (coshf)
WRAPPER_DECL (WRAPPER_NAME (coshf))
#else
# define HAVE_VECTOR_coshf 0
# define coshf_VEC_SUFF coshf
#endif

#if defined TEST_VECTOR_coshl && TEST_VECTOR_coshl
# define HAVE_VECTOR_coshl 1
# define coshl_VEC_SUFF WRAPPER_NAME (coshl)
WRAPPER_DECL (WRAPPER_NAME (coshl))
#else
# define HAVE_VECTOR_coshl 0
# define coshl_VEC_SUFF coshl
#endif

#if defined TEST_VECTOR_cpow && TEST_VECTOR_cpow
# define HAVE_VECTOR_cpow 1
# define cpow_VEC_SUFF WRAPPER_NAME (cpow)
WRAPPER_DECL (WRAPPER_NAME (cpow))
#else
# define HAVE_VECTOR_cpow 0
# define cpow_VEC_SUFF cpow
#endif

#if defined TEST_VECTOR_cpowf && TEST_VECTOR_cpowf
# define HAVE_VECTOR_cpowf 1
# define cpowf_VEC_SUFF WRAPPER_NAME (cpowf)
WRAPPER_DECL (WRAPPER_NAME (cpowf))
#else
# define HAVE_VECTOR_cpowf 0
# define cpowf_VEC_SUFF cpowf
#endif

#if defined TEST_VECTOR_cpowl && TEST_VECTOR_cpowl
# define HAVE_VECTOR_cpowl 1
# define cpowl_VEC_SUFF WRAPPER_NAME (cpowl)
WRAPPER_DECL (WRAPPER_NAME (cpowl))
#else
# define HAVE_VECTOR_cpowl 0
# define cpowl_VEC_SUFF cpowl
#endif

#if defined TEST_VECTOR_cproj && TEST_VECTOR_cproj
# define HAVE_VECTOR_cproj 1
# define cproj_VEC_SUFF WRAPPER_NAME (cproj)
WRAPPER_DECL (WRAPPER_NAME (cproj))
#else
# define HAVE_VECTOR_cproj 0
# define cproj_VEC_SUFF cproj
#endif

#if defined TEST_VECTOR_cprojf && TEST_VECTOR_cprojf
# define HAVE_VECTOR_cprojf 1
# define cprojf_VEC_SUFF WRAPPER_NAME (cprojf)
WRAPPER_DECL (WRAPPER_NAME (cprojf))
#else
# define HAVE_VECTOR_cprojf 0
# define cprojf_VEC_SUFF cprojf
#endif

#if defined TEST_VECTOR_cprojl && TEST_VECTOR_cprojl
# define HAVE_VECTOR_cprojl 1
# define cprojl_VEC_SUFF WRAPPER_NAME (cprojl)
WRAPPER_DECL (WRAPPER_NAME (cprojl))
#else
# define HAVE_VECTOR_cprojl 0
# define cprojl_VEC_SUFF cprojl
#endif

#if defined TEST_VECTOR_creal && TEST_VECTOR_creal
# define HAVE_VECTOR_creal 1
# define creal_VEC_SUFF WRAPPER_NAME (creal)
WRAPPER_DECL (WRAPPER_NAME (creal))
#else
# define HAVE_VECTOR_creal 0
# define creal_VEC_SUFF creal
#endif

#if defined TEST_VECTOR_crealf && TEST_VECTOR_crealf
# define HAVE_VECTOR_crealf 1
# define crealf_VEC_SUFF WRAPPER_NAME (crealf)
WRAPPER_DECL (WRAPPER_NAME (crealf))
#else
# define HAVE_VECTOR_crealf 0
# define crealf_VEC_SUFF crealf
#endif

#if defined TEST_VECTOR_creall && TEST_VECTOR_creall
# define HAVE_VECTOR_creall 1
# define creall_VEC_SUFF WRAPPER_NAME (creall)
WRAPPER_DECL (WRAPPER_NAME (creall))
#else
# define HAVE_VECTOR_creall 0
# define creall_VEC_SUFF creall
#endif

#if defined TEST_VECTOR_csin && TEST_VECTOR_csin
# define HAVE_VECTOR_csin 1
# define csin_VEC_SUFF WRAPPER_NAME (csin)
WRAPPER_DECL (WRAPPER_NAME (csin))
#else
# define HAVE_VECTOR_csin 0
# define csin_VEC_SUFF csin
#endif

#if defined TEST_VECTOR_csinf && TEST_VECTOR_csinf
# define HAVE_VECTOR_csinf 1
# define csinf_VEC_SUFF WRAPPER_NAME (csinf)
WRAPPER_DECL (WRAPPER_NAME (csinf))
#else
# define HAVE_VECTOR_csinf 0
# define csinf_VEC_SUFF csinf
#endif

#if defined TEST_VECTOR_csinl && TEST_VECTOR_csinl
# define HAVE_VECTOR_csinl 1
# define csinl_VEC_SUFF WRAPPER_NAME (csinl)
WRAPPER_DECL (WRAPPER_NAME (csinl))
#else
# define HAVE_VECTOR_csinl 0
# define csinl_VEC_SUFF csinl
#endif

#if defined TEST_VECTOR_csinh && TEST_VECTOR_csinh
# define HAVE_VECTOR_csinh 1
# define csinh_VEC_SUFF WRAPPER_NAME (csinh)
WRAPPER_DECL (WRAPPER_NAME (csinh))
#else
# define HAVE_VECTOR_csinh 0
# define csinh_VEC_SUFF csinh
#endif

#if defined TEST_VECTOR_csinhf && TEST_VECTOR_csinhf
# define HAVE_VECTOR_csinhf 1
# define csinhf_VEC_SUFF WRAPPER_NAME (csinhf)
WRAPPER_DECL (WRAPPER_NAME (csinhf))
#else
# define HAVE_VECTOR_csinhf 0
# define csinhf_VEC_SUFF csinhf
#endif

#if defined TEST_VECTOR_csinhl && TEST_VECTOR_csinhl
# define HAVE_VECTOR_csinhl 1
# define csinhl_VEC_SUFF WRAPPER_NAME (csinhl)
WRAPPER_DECL (WRAPPER_NAME (csinhl))
#else
# define HAVE_VECTOR_csinhl 0
# define csinhl_VEC_SUFF csinhl
#endif

#if defined TEST_VECTOR_csqrt && TEST_VECTOR_csqrt
# define HAVE_VECTOR_csqrt 1
# define csqrt_VEC_SUFF WRAPPER_NAME (csqrt)
WRAPPER_DECL (WRAPPER_NAME (csqrt))
#else
# define HAVE_VECTOR_csqrt 0
# define csqrt_VEC_SUFF csqrt
#endif

#if defined TEST_VECTOR_csqrtf && TEST_VECTOR_csqrtf
# define HAVE_VECTOR_csqrtf 1
# define csqrtf_VEC_SUFF WRAPPER_NAME (csqrtf)
WRAPPER_DECL (WRAPPER_NAME (csqrtf))
#else
# define HAVE_VECTOR_csqrtf 0
# define csqrtf_VEC_SUFF csqrtf
#endif

#if defined TEST_VECTOR_csqrtl && TEST_VECTOR_csqrtl
# define HAVE_VECTOR_csqrtl 1
# define csqrtl_VEC_SUFF WRAPPER_NAME (csqrtl)
WRAPPER_DECL (WRAPPER_NAME (csqrtl))
#else
# define HAVE_VECTOR_csqrtl 0
# define csqrtl_VEC_SUFF csqrtl
#endif

#if defined TEST_VECTOR_ctan && TEST_VECTOR_ctan
# define HAVE_VECTOR_ctan 1
# define ctan_VEC_SUFF WRAPPER_NAME (ctan)
WRAPPER_DECL (WRAPPER_NAME (ctan))
#else
# define HAVE_VECTOR_ctan 0
# define ctan_VEC_SUFF ctan
#endif

#if defined TEST_VECTOR_ctanf && TEST_VECTOR_ctanf
# define HAVE_VECTOR_ctanf 1
# define ctanf_VEC_SUFF WRAPPER_NAME (ctanf)
WRAPPER_DECL (WRAPPER_NAME (ctanf))
#else
# define HAVE_VECTOR_ctanf 0
# define ctanf_VEC_SUFF ctanf
#endif

#if defined TEST_VECTOR_ctanl && TEST_VECTOR_ctanl
# define HAVE_VECTOR_ctanl 1
# define ctanl_VEC_SUFF WRAPPER_NAME (ctanl)
WRAPPER_DECL (WRAPPER_NAME (ctanl))
#else
# define HAVE_VECTOR_ctanl 0
# define ctanl_VEC_SUFF ctanl
#endif

#if defined TEST_VECTOR_ctanh && TEST_VECTOR_ctanh
# define HAVE_VECTOR_ctanh 1
# define ctanh_VEC_SUFF WRAPPER_NAME (ctanh)
WRAPPER_DECL (WRAPPER_NAME (ctanh))
#else
# define HAVE_VECTOR_ctanh 0
# define ctanh_VEC_SUFF ctanh
#endif

#if defined TEST_VECTOR_ctanhf && TEST_VECTOR_ctanhf
# define HAVE_VECTOR_ctanhf 1
# define ctanhf_VEC_SUFF WRAPPER_NAME (ctanhf)
WRAPPER_DECL (WRAPPER_NAME (ctanhf))
#else
# define HAVE_VECTOR_ctanhf 0
# define ctanhf_VEC_SUFF ctanhf
#endif

#if defined TEST_VECTOR_ctanhl && TEST_VECTOR_ctanhl
# define HAVE_VECTOR_ctanhl 1
# define ctanhl_VEC_SUFF WRAPPER_NAME (ctanhl)
WRAPPER_DECL (WRAPPER_NAME (ctanhl))
#else
# define HAVE_VECTOR_ctanhl 0
# define ctanhl_VEC_SUFF ctanhl
#endif

#if defined TEST_VECTOR_erf && TEST_VECTOR_erf
# define HAVE_VECTOR_erf 1
# define erf_VEC_SUFF WRAPPER_NAME (erf)
WRAPPER_DECL (WRAPPER_NAME (erf))
#else
# define HAVE_VECTOR_erf 0
# define erf_VEC_SUFF erf
#endif

#if defined TEST_VECTOR_erff && TEST_VECTOR_erff
# define HAVE_VECTOR_erff 1
# define erff_VEC_SUFF WRAPPER_NAME (erff)
WRAPPER_DECL (WRAPPER_NAME (erff))
#else
# define HAVE_VECTOR_erff 0
# define erff_VEC_SUFF erff
#endif

#if defined TEST_VECTOR_erfl && TEST_VECTOR_erfl
# define HAVE_VECTOR_erfl 1
# define erfl_VEC_SUFF WRAPPER_NAME (erfl)
WRAPPER_DECL (WRAPPER_NAME (erfl))
#else
# define HAVE_VECTOR_erfl 0
# define erfl_VEC_SUFF erfl
#endif

#if defined TEST_VECTOR_erfc && TEST_VECTOR_erfc
# define HAVE_VECTOR_erfc 1
# define erfc_VEC_SUFF WRAPPER_NAME (erfc)
WRAPPER_DECL (WRAPPER_NAME (erfc))
#else
# define HAVE_VECTOR_erfc 0
# define erfc_VEC_SUFF erfc
#endif

#if defined TEST_VECTOR_erfcf && TEST_VECTOR_erfcf
# define HAVE_VECTOR_erfcf 1
# define erfcf_VEC_SUFF WRAPPER_NAME (erfcf)
WRAPPER_DECL (WRAPPER_NAME (erfcf))
#else
# define HAVE_VECTOR_erfcf 0
# define erfcf_VEC_SUFF erfcf
#endif

#if defined TEST_VECTOR_erfcl && TEST_VECTOR_erfcl
# define HAVE_VECTOR_erfcl 1
# define erfcl_VEC_SUFF WRAPPER_NAME (erfcl)
WRAPPER_DECL (WRAPPER_NAME (erfcl))
#else
# define HAVE_VECTOR_erfcl 0
# define erfcl_VEC_SUFF erfcl
#endif

#if defined TEST_VECTOR_exp && TEST_VECTOR_exp
# define HAVE_VECTOR_exp 1
# define exp_VEC_SUFF WRAPPER_NAME (exp)
WRAPPER_DECL (WRAPPER_NAME (exp))
#else
# define HAVE_VECTOR_exp 0
# define exp_VEC_SUFF exp
#endif

#if defined TEST_VECTOR_expf && TEST_VECTOR_expf
# define HAVE_VECTOR_expf 1
# define expf_VEC_SUFF WRAPPER_NAME (expf)
WRAPPER_DECL (WRAPPER_NAME (expf))
#else
# define HAVE_VECTOR_expf 0
# define expf_VEC_SUFF expf
#endif

#if defined TEST_VECTOR_expl && TEST_VECTOR_expl
# define HAVE_VECTOR_expl 1
# define expl_VEC_SUFF WRAPPER_NAME (expl)
WRAPPER_DECL (WRAPPER_NAME (expl))
#else
# define HAVE_VECTOR_expl 0
# define expl_VEC_SUFF expl
#endif

#if defined TEST_VECTOR_exp10 && TEST_VECTOR_exp10
# define HAVE_VECTOR_exp10 1
# define exp10_VEC_SUFF WRAPPER_NAME (exp10)
WRAPPER_DECL (WRAPPER_NAME (exp10))
#else
# define HAVE_VECTOR_exp10 0
# define exp10_VEC_SUFF exp10
#endif

#if defined TEST_VECTOR_exp10f && TEST_VECTOR_exp10f
# define HAVE_VECTOR_exp10f 1
# define exp10f_VEC_SUFF WRAPPER_NAME (exp10f)
WRAPPER_DECL (WRAPPER_NAME (exp10f))
#else
# define HAVE_VECTOR_exp10f 0
# define exp10f_VEC_SUFF exp10f
#endif

#if defined TEST_VECTOR_exp10l && TEST_VECTOR_exp10l
# define HAVE_VECTOR_exp10l 1
# define exp10l_VEC_SUFF WRAPPER_NAME (exp10l)
WRAPPER_DECL (WRAPPER_NAME (exp10l))
#else
# define HAVE_VECTOR_exp10l 0
# define exp10l_VEC_SUFF exp10l
#endif

#if defined TEST_VECTOR_pow10 && TEST_VECTOR_pow10
# define HAVE_VECTOR_pow10 1
# define pow10_VEC_SUFF WRAPPER_NAME (pow10)
WRAPPER_DECL (WRAPPER_NAME (pow10))
#else
# define HAVE_VECTOR_pow10 0
# define pow10_VEC_SUFF pow10
#endif

#if defined TEST_VECTOR_pow10f && TEST_VECTOR_pow10f
# define HAVE_VECTOR_pow10f 1
# define pow10f_VEC_SUFF WRAPPER_NAME (pow10f)
WRAPPER_DECL (WRAPPER_NAME (pow10f))
#else
# define HAVE_VECTOR_pow10f 0
# define pow10f_VEC_SUFF pow10f
#endif

#if defined TEST_VECTOR_pow10l && TEST_VECTOR_pow10l
# define HAVE_VECTOR_pow10l 1
# define pow10l_VEC_SUFF WRAPPER_NAME (pow10l)
WRAPPER_DECL (WRAPPER_NAME (pow10l))
#else
# define HAVE_VECTOR_pow10l 0
# define pow10l_VEC_SUFF pow10l
#endif

#if defined TEST_VECTOR_exp2 && TEST_VECTOR_exp2
# define HAVE_VECTOR_exp2 1
# define exp2_VEC_SUFF WRAPPER_NAME (exp2)
WRAPPER_DECL (WRAPPER_NAME (exp2))
#else
# define HAVE_VECTOR_exp2 0
# define exp2_VEC_SUFF exp2
#endif

#if defined TEST_VECTOR_exp2f && TEST_VECTOR_exp2f
# define HAVE_VECTOR_exp2f 1
# define exp2f_VEC_SUFF WRAPPER_NAME (exp2f)
WRAPPER_DECL (WRAPPER_NAME (exp2f))
#else
# define HAVE_VECTOR_exp2f 0
# define exp2f_VEC_SUFF exp2f
#endif

#if defined TEST_VECTOR_exp2l && TEST_VECTOR_exp2l
# define HAVE_VECTOR_exp2l 1
# define exp2l_VEC_SUFF WRAPPER_NAME (exp2l)
WRAPPER_DECL (WRAPPER_NAME (exp2l))
#else
# define HAVE_VECTOR_exp2l 0
# define exp2l_VEC_SUFF exp2l
#endif

#if defined TEST_VECTOR_expm1 && TEST_VECTOR_expm1
# define HAVE_VECTOR_expm1 1
# define expm1_VEC_SUFF WRAPPER_NAME (expm1)
WRAPPER_DECL (WRAPPER_NAME (expm1))
#else
# define HAVE_VECTOR_expm1 0
# define expm1_VEC_SUFF expm1
#endif

#if defined TEST_VECTOR_expm1f && TEST_VECTOR_expm1f
# define HAVE_VECTOR_expm1f 1
# define expm1f_VEC_SUFF WRAPPER_NAME (expm1f)
WRAPPER_DECL (WRAPPER_NAME (expm1f))
#else
# define HAVE_VECTOR_expm1f 0
# define expm1f_VEC_SUFF expm1f
#endif

#if defined TEST_VECTOR_expm1l && TEST_VECTOR_expm1l
# define HAVE_VECTOR_expm1l 1
# define expm1l_VEC_SUFF WRAPPER_NAME (expm1l)
WRAPPER_DECL (WRAPPER_NAME (expm1l))
#else
# define HAVE_VECTOR_expm1l 0
# define expm1l_VEC_SUFF expm1l
#endif

#if defined TEST_VECTOR_fabs && TEST_VECTOR_fabs
# define HAVE_VECTOR_fabs 1
# define fabs_VEC_SUFF WRAPPER_NAME (fabs)
WRAPPER_DECL (WRAPPER_NAME (fabs))
#else
# define HAVE_VECTOR_fabs 0
# define fabs_VEC_SUFF fabs
#endif

#if defined TEST_VECTOR_fabsf && TEST_VECTOR_fabsf
# define HAVE_VECTOR_fabsf 1
# define fabsf_VEC_SUFF WRAPPER_NAME (fabsf)
WRAPPER_DECL (WRAPPER_NAME (fabsf))
#else
# define HAVE_VECTOR_fabsf 0
# define fabsf_VEC_SUFF fabsf
#endif

#if defined TEST_VECTOR_fabsl && TEST_VECTOR_fabsl
# define HAVE_VECTOR_fabsl 1
# define fabsl_VEC_SUFF WRAPPER_NAME (fabsl)
WRAPPER_DECL (WRAPPER_NAME (fabsl))
#else
# define HAVE_VECTOR_fabsl 0
# define fabsl_VEC_SUFF fabsl
#endif

#if defined TEST_VECTOR_floor && TEST_VECTOR_floor
# define HAVE_VECTOR_floor 1
# define floor_VEC_SUFF WRAPPER_NAME (floor)
WRAPPER_DECL (WRAPPER_NAME (floor))
#else
# define HAVE_VECTOR_floor 0
# define floor_VEC_SUFF floor
#endif

#if defined TEST_VECTOR_floorf && TEST_VECTOR_floorf
# define HAVE_VECTOR_floorf 1
# define floorf_VEC_SUFF WRAPPER_NAME (floorf)
WRAPPER_DECL (WRAPPER_NAME (floorf))
#else
# define HAVE_VECTOR_floorf 0
# define floorf_VEC_SUFF floorf
#endif

#if defined TEST_VECTOR_floorl && TEST_VECTOR_floorl
# define HAVE_VECTOR_floorl 1
# define floorl_VEC_SUFF WRAPPER_NAME (floorl)
WRAPPER_DECL (WRAPPER_NAME (floorl))
#else
# define HAVE_VECTOR_floorl 0
# define floorl_VEC_SUFF floorl
#endif

#if defined TEST_VECTOR_fma && TEST_VECTOR_fma
# define HAVE_VECTOR_fma 1
# define fma_VEC_SUFF WRAPPER_NAME (fma)
WRAPPER_DECL (WRAPPER_NAME (fma))
#else
# define HAVE_VECTOR_fma 0
# define fma_VEC_SUFF fma
#endif

#if defined TEST_VECTOR_fmaf && TEST_VECTOR_fmaf
# define HAVE_VECTOR_fmaf 1
# define fmaf_VEC_SUFF WRAPPER_NAME (fmaf)
WRAPPER_DECL (WRAPPER_NAME (fmaf))
#else
# define HAVE_VECTOR_fmaf 0
# define fmaf_VEC_SUFF fmaf
#endif

#if defined TEST_VECTOR_fmal && TEST_VECTOR_fmal
# define HAVE_VECTOR_fmal 1
# define fmal_VEC_SUFF WRAPPER_NAME (fmal)
WRAPPER_DECL (WRAPPER_NAME (fmal))
#else
# define HAVE_VECTOR_fmal 0
# define fmal_VEC_SUFF fmal
#endif

#if defined TEST_VECTOR_fpclassify && TEST_VECTOR_fpclassify
# define HAVE_VECTOR_fpclassify 1
# define fpclassify_VEC_SUFF WRAPPER_NAME (fpclassify)
WRAPPER_DECL (WRAPPER_NAME (fpclassify))
#else
# define HAVE_VECTOR_fpclassify 0
# define fpclassify_VEC_SUFF fpclassify
#endif

#if defined TEST_VECTOR_fpclassifyf && TEST_VECTOR_fpclassifyf
# define HAVE_VECTOR_fpclassifyf 1
# define fpclassifyf_VEC_SUFF WRAPPER_NAME (fpclassifyf)
WRAPPER_DECL (WRAPPER_NAME (fpclassifyf))
#else
# define HAVE_VECTOR_fpclassifyf 0
# define fpclassifyf_VEC_SUFF fpclassifyf
#endif

#if defined TEST_VECTOR_fpclassifyl && TEST_VECTOR_fpclassifyl
# define HAVE_VECTOR_fpclassifyl 1
# define fpclassifyl_VEC_SUFF WRAPPER_NAME (fpclassifyl)
WRAPPER_DECL (WRAPPER_NAME (fpclassifyl))
#else
# define HAVE_VECTOR_fpclassifyl 0
# define fpclassifyl_VEC_SUFF fpclassifyl
#endif

#if defined TEST_VECTOR_frexp && TEST_VECTOR_frexp
# define HAVE_VECTOR_frexp 1
# define frexp_VEC_SUFF WRAPPER_NAME (frexp)
WRAPPER_DECL (WRAPPER_NAME (frexp))
#else
# define HAVE_VECTOR_frexp 0
# define frexp_VEC_SUFF frexp
#endif

#if defined TEST_VECTOR_frexpf && TEST_VECTOR_frexpf
# define HAVE_VECTOR_frexpf 1
# define frexpf_VEC_SUFF WRAPPER_NAME (frexpf)
WRAPPER_DECL (WRAPPER_NAME (frexpf))
#else
# define HAVE_VECTOR_frexpf 0
# define frexpf_VEC_SUFF frexpf
#endif

#if defined TEST_VECTOR_frexpl && TEST_VECTOR_frexpl
# define HAVE_VECTOR_frexpl 1
# define frexpl_VEC_SUFF WRAPPER_NAME (frexpl)
WRAPPER_DECL (WRAPPER_NAME (frexpl))
#else
# define HAVE_VECTOR_frexpl 0
# define frexpl_VEC_SUFF frexpl
#endif

#if defined TEST_VECTOR_ilogb && TEST_VECTOR_ilogb
# define HAVE_VECTOR_ilogb 1
# define ilogb_VEC_SUFF WRAPPER_NAME (ilogb)
WRAPPER_DECL (WRAPPER_NAME (ilogb))
#else
# define HAVE_VECTOR_ilogb 0
# define ilogb_VEC_SUFF ilogb
#endif

#if defined TEST_VECTOR_ilogbf && TEST_VECTOR_ilogbf
# define HAVE_VECTOR_ilogbf 1
# define ilogbf_VEC_SUFF WRAPPER_NAME (ilogbf)
WRAPPER_DECL (WRAPPER_NAME (ilogbf))
#else
# define HAVE_VECTOR_ilogbf 0
# define ilogbf_VEC_SUFF ilogbf
#endif

#if defined TEST_VECTOR_ilogbl && TEST_VECTOR_ilogbl
# define HAVE_VECTOR_ilogbl 1
# define ilogbl_VEC_SUFF WRAPPER_NAME (ilogbl)
WRAPPER_DECL (WRAPPER_NAME (ilogbl))
#else
# define HAVE_VECTOR_ilogbl 0
# define ilogbl_VEC_SUFF ilogbl
#endif

#if defined TEST_VECTOR_isfinite && TEST_VECTOR_isfinite
# define HAVE_VECTOR_isfinite 1
# define isfinite_VEC_SUFF WRAPPER_NAME (isfinite)
WRAPPER_DECL (WRAPPER_NAME (isfinite))
#else
# define HAVE_VECTOR_isfinite 0
# define isfinite_VEC_SUFF isfinite
#endif

#if defined TEST_VECTOR_isfinitef && TEST_VECTOR_isfinitef
# define HAVE_VECTOR_isfinitef 1
# define isfinitef_VEC_SUFF WRAPPER_NAME (isfinitef)
WRAPPER_DECL (WRAPPER_NAME (isfinitef))
#else
# define HAVE_VECTOR_isfinitef 0
# define isfinitef_VEC_SUFF isfinitef
#endif

#if defined TEST_VECTOR_isfinitel && TEST_VECTOR_isfinitel
# define HAVE_VECTOR_isfinitel 1
# define isfinitel_VEC_SUFF WRAPPER_NAME (isfinitel)
WRAPPER_DECL (WRAPPER_NAME (isfinitel))
#else
# define HAVE_VECTOR_isfinitel 0
# define isfinitel_VEC_SUFF isfinitel
#endif

#if defined TEST_VECTOR_finite && TEST_VECTOR_finite
# define HAVE_VECTOR_finite 1
# define finite_VEC_SUFF WRAPPER_NAME (finite)
WRAPPER_DECL (WRAPPER_NAME (finite))
#else
# define HAVE_VECTOR_finite 0
# define finite_VEC_SUFF finite
#endif

#if defined TEST_VECTOR_finitef && TEST_VECTOR_finitef
# define HAVE_VECTOR_finitef 1
# define finitef_VEC_SUFF WRAPPER_NAME (finitef)
WRAPPER_DECL (WRAPPER_NAME (finitef))
#else
# define HAVE_VECTOR_finitef 0
# define finitef_VEC_SUFF finitef
#endif

#if defined TEST_VECTOR_finitel && TEST_VECTOR_finitel
# define HAVE_VECTOR_finitel 1
# define finitel_VEC_SUFF WRAPPER_NAME (finitel)
WRAPPER_DECL (WRAPPER_NAME (finitel))
#else
# define HAVE_VECTOR_finitel 0
# define finitel_VEC_SUFF finitel
#endif

#if defined TEST_VECTOR_isgreater && TEST_VECTOR_isgreater
# define HAVE_VECTOR_isgreater 1
# define isgreater_VEC_SUFF WRAPPER_NAME (isgreater)
WRAPPER_DECL (WRAPPER_NAME (isgreater))
#else
# define HAVE_VECTOR_isgreater 0
# define isgreater_VEC_SUFF isgreater
#endif

#if defined TEST_VECTOR_isgreaterf && TEST_VECTOR_isgreaterf
# define HAVE_VECTOR_isgreaterf 1
# define isgreaterf_VEC_SUFF WRAPPER_NAME (isgreaterf)
WRAPPER_DECL (WRAPPER_NAME (isgreaterf))
#else
# define HAVE_VECTOR_isgreaterf 0
# define isgreaterf_VEC_SUFF isgreaterf
#endif

#if defined TEST_VECTOR_isgreaterl && TEST_VECTOR_isgreaterl
# define HAVE_VECTOR_isgreaterl 1
# define isgreaterl_VEC_SUFF WRAPPER_NAME (isgreaterl)
WRAPPER_DECL (WRAPPER_NAME (isgreaterl))
#else
# define HAVE_VECTOR_isgreaterl 0
# define isgreaterl_VEC_SUFF isgreaterl
#endif

#if defined TEST_VECTOR_isgreaterequal && TEST_VECTOR_isgreaterequal
# define HAVE_VECTOR_isgreaterequal 1
# define isgreaterequal_VEC_SUFF WRAPPER_NAME (isgreaterequal)
WRAPPER_DECL (WRAPPER_NAME (isgreaterequal))
#else
# define HAVE_VECTOR_isgreaterequal 0
# define isgreaterequal_VEC_SUFF isgreaterequal
#endif

#if defined TEST_VECTOR_isgreaterequalf && TEST_VECTOR_isgreaterequalf
# define HAVE_VECTOR_isgreaterequalf 1
# define isgreaterequalf_VEC_SUFF WRAPPER_NAME (isgreaterequalf)
WRAPPER_DECL (WRAPPER_NAME (isgreaterequalf))
#else
# define HAVE_VECTOR_isgreaterequalf 0
# define isgreaterequalf_VEC_SUFF isgreaterequalf
#endif

#if defined TEST_VECTOR_isgreaterequall && TEST_VECTOR_isgreaterequall
# define HAVE_VECTOR_isgreaterequall 1
# define isgreaterequall_VEC_SUFF WRAPPER_NAME (isgreaterequall)
WRAPPER_DECL (WRAPPER_NAME (isgreaterequall))
#else
# define HAVE_VECTOR_isgreaterequall 0
# define isgreaterequall_VEC_SUFF isgreaterequall
#endif

#if defined TEST_VECTOR_isinf && TEST_VECTOR_isinf
# define HAVE_VECTOR_isinf 1
# define isinf_VEC_SUFF WRAPPER_NAME (isinf)
WRAPPER_DECL (WRAPPER_NAME (isinf))
#else
# define HAVE_VECTOR_isinf 0
# define isinf_VEC_SUFF isinf
#endif

#if defined TEST_VECTOR_isinff && TEST_VECTOR_isinff
# define HAVE_VECTOR_isinff 1
# define isinff_VEC_SUFF WRAPPER_NAME (isinff)
WRAPPER_DECL (WRAPPER_NAME (isinff))
#else
# define HAVE_VECTOR_isinff 0
# define isinff_VEC_SUFF isinff
#endif

#if defined TEST_VECTOR_isinfl && TEST_VECTOR_isinfl
# define HAVE_VECTOR_isinfl 1
# define isinfl_VEC_SUFF WRAPPER_NAME (isinfl)
WRAPPER_DECL (WRAPPER_NAME (isinfl))
#else
# define HAVE_VECTOR_isinfl 0
# define isinfl_VEC_SUFF isinfl
#endif

#if defined TEST_VECTOR_isless && TEST_VECTOR_isless
# define HAVE_VECTOR_isless 1
# define isless_VEC_SUFF WRAPPER_NAME (isless)
WRAPPER_DECL (WRAPPER_NAME (isless))
#else
# define HAVE_VECTOR_isless 0
# define isless_VEC_SUFF isless
#endif

#if defined TEST_VECTOR_islessf && TEST_VECTOR_islessf
# define HAVE_VECTOR_islessf 1
# define islessf_VEC_SUFF WRAPPER_NAME (islessf)
WRAPPER_DECL (WRAPPER_NAME (islessf))
#else
# define HAVE_VECTOR_islessf 0
# define islessf_VEC_SUFF islessf
#endif

#if defined TEST_VECTOR_islessl && TEST_VECTOR_islessl
# define HAVE_VECTOR_islessl 1
# define islessl_VEC_SUFF WRAPPER_NAME (islessl)
WRAPPER_DECL (WRAPPER_NAME (islessl))
#else
# define HAVE_VECTOR_islessl 0
# define islessl_VEC_SUFF islessl
#endif

#if defined TEST_VECTOR_islessequal && TEST_VECTOR_islessequal
# define HAVE_VECTOR_islessequal 1
# define islessequal_VEC_SUFF WRAPPER_NAME (islessequal)
WRAPPER_DECL (WRAPPER_NAME (islessequal))
#else
# define HAVE_VECTOR_islessequal 0
# define islessequal_VEC_SUFF islessequal
#endif

#if defined TEST_VECTOR_islessequalf && TEST_VECTOR_islessequalf
# define HAVE_VECTOR_islessequalf 1
# define islessequalf_VEC_SUFF WRAPPER_NAME (islessequalf)
WRAPPER_DECL (WRAPPER_NAME (islessequalf))
#else
# define HAVE_VECTOR_islessequalf 0
# define islessequalf_VEC_SUFF islessequalf
#endif

#if defined TEST_VECTOR_islessequall && TEST_VECTOR_islessequall
# define HAVE_VECTOR_islessequall 1
# define islessequall_VEC_SUFF WRAPPER_NAME (islessequall)
WRAPPER_DECL (WRAPPER_NAME (islessequall))
#else
# define HAVE_VECTOR_islessequall 0
# define islessequall_VEC_SUFF islessequall
#endif

#if defined TEST_VECTOR_islessgreater && TEST_VECTOR_islessgreater
# define HAVE_VECTOR_islessgreater 1
# define islessgreater_VEC_SUFF WRAPPER_NAME (islessgreater)
WRAPPER_DECL (WRAPPER_NAME (islessgreater))
#else
# define HAVE_VECTOR_islessgreater 0
# define islessgreater_VEC_SUFF islessgreater
#endif

#if defined TEST_VECTOR_islessgreaterf && TEST_VECTOR_islessgreaterf
# define HAVE_VECTOR_islessgreaterf 1
# define islessgreaterf_VEC_SUFF WRAPPER_NAME (islessgreaterf)
WRAPPER_DECL (WRAPPER_NAME (islessgreaterf))
#else
# define HAVE_VECTOR_islessgreaterf 0
# define islessgreaterf_VEC_SUFF islessgreaterf
#endif

#if defined TEST_VECTOR_islessgreaterl && TEST_VECTOR_islessgreaterl
# define HAVE_VECTOR_islessgreaterl 1
# define islessgreaterl_VEC_SUFF WRAPPER_NAME (islessgreaterl)
WRAPPER_DECL (WRAPPER_NAME (islessgreaterl))
#else
# define HAVE_VECTOR_islessgreaterl 0
# define islessgreaterl_VEC_SUFF islessgreaterl
#endif

#if defined TEST_VECTOR_isnan && TEST_VECTOR_isnan
# define HAVE_VECTOR_isnan 1
# define isnan_VEC_SUFF WRAPPER_NAME (isnan)
WRAPPER_DECL (WRAPPER_NAME (isnan))
#else
# define HAVE_VECTOR_isnan 0
# define isnan_VEC_SUFF isnan
#endif

#if defined TEST_VECTOR_isnanf && TEST_VECTOR_isnanf
# define HAVE_VECTOR_isnanf 1
# define isnanf_VEC_SUFF WRAPPER_NAME (isnanf)
WRAPPER_DECL (WRAPPER_NAME (isnanf))
#else
# define HAVE_VECTOR_isnanf 0
# define isnanf_VEC_SUFF isnanf
#endif

#if defined TEST_VECTOR_isnanl && TEST_VECTOR_isnanl
# define HAVE_VECTOR_isnanl 1
# define isnanl_VEC_SUFF WRAPPER_NAME (isnanl)
WRAPPER_DECL (WRAPPER_NAME (isnanl))
#else
# define HAVE_VECTOR_isnanl 0
# define isnanl_VEC_SUFF isnanl
#endif

#if defined TEST_VECTOR_isnormal && TEST_VECTOR_isnormal
# define HAVE_VECTOR_isnormal 1
# define isnormal_VEC_SUFF WRAPPER_NAME (isnormal)
WRAPPER_DECL (WRAPPER_NAME (isnormal))
#else
# define HAVE_VECTOR_isnormal 0
# define isnormal_VEC_SUFF isnormal
#endif

#if defined TEST_VECTOR_isnormalf && TEST_VECTOR_isnormalf
# define HAVE_VECTOR_isnormalf 1
# define isnormalf_VEC_SUFF WRAPPER_NAME (isnormalf)
WRAPPER_DECL (WRAPPER_NAME (isnormalf))
#else
# define HAVE_VECTOR_isnormalf 0
# define isnormalf_VEC_SUFF isnormalf
#endif

#if defined TEST_VECTOR_isnormall && TEST_VECTOR_isnormall
# define HAVE_VECTOR_isnormall 1
# define isnormall_VEC_SUFF WRAPPER_NAME (isnormall)
WRAPPER_DECL (WRAPPER_NAME (isnormall))
#else
# define HAVE_VECTOR_isnormall 0
# define isnormall_VEC_SUFF isnormall
#endif

#if defined TEST_VECTOR_issignaling && TEST_VECTOR_issignaling
# define HAVE_VECTOR_issignaling 1
# define issignaling_VEC_SUFF WRAPPER_NAME (issignaling)
WRAPPER_DECL (WRAPPER_NAME (issignaling))
#else
# define HAVE_VECTOR_issignaling 0
# define issignaling_VEC_SUFF issignaling
#endif

#if defined TEST_VECTOR_issignalingf && TEST_VECTOR_issignalingf
# define HAVE_VECTOR_issignalingf 1
# define issignalingf_VEC_SUFF WRAPPER_NAME (issignalingf)
WRAPPER_DECL (WRAPPER_NAME (issignalingf))
#else
# define HAVE_VECTOR_issignalingf 0
# define issignalingf_VEC_SUFF issignalingf
#endif

#if defined TEST_VECTOR_issignalingl && TEST_VECTOR_issignalingl
# define HAVE_VECTOR_issignalingl 1
# define issignalingl_VEC_SUFF WRAPPER_NAME (issignalingl)
WRAPPER_DECL (WRAPPER_NAME (issignalingl))
#else
# define HAVE_VECTOR_issignalingl 0
# define issignalingl_VEC_SUFF issignalingl
#endif

#if defined TEST_VECTOR_isunordered && TEST_VECTOR_isunordered
# define HAVE_VECTOR_isunordered 1
# define isunordered_VEC_SUFF WRAPPER_NAME (isunordered)
WRAPPER_DECL (WRAPPER_NAME (isunordered))
#else
# define HAVE_VECTOR_isunordered 0
# define isunordered_VEC_SUFF isunordered
#endif

#if defined TEST_VECTOR_isunorderedf && TEST_VECTOR_isunorderedf
# define HAVE_VECTOR_isunorderedf 1
# define isunorderedf_VEC_SUFF WRAPPER_NAME (isunorderedf)
WRAPPER_DECL (WRAPPER_NAME (isunorderedf))
#else
# define HAVE_VECTOR_isunorderedf 0
# define isunorderedf_VEC_SUFF isunorderedf
#endif

#if defined TEST_VECTOR_isunorderedl && TEST_VECTOR_isunorderedl
# define HAVE_VECTOR_isunorderedl 1
# define isunorderedl_VEC_SUFF WRAPPER_NAME (isunorderedl)
WRAPPER_DECL (WRAPPER_NAME (isunorderedl))
#else
# define HAVE_VECTOR_isunorderedl 0
# define isunorderedl_VEC_SUFF isunorderedl
#endif

#if defined TEST_VECTOR_j0 && TEST_VECTOR_j0
# define HAVE_VECTOR_j0 1
# define j0_VEC_SUFF WRAPPER_NAME (j0)
WRAPPER_DECL (WRAPPER_NAME (j0))
#else
# define HAVE_VECTOR_j0 0
# define j0_VEC_SUFF j0
#endif

#if defined TEST_VECTOR_j0f && TEST_VECTOR_j0f
# define HAVE_VECTOR_j0f 1
# define j0f_VEC_SUFF WRAPPER_NAME (j0f)
WRAPPER_DECL (WRAPPER_NAME (j0f))
#else
# define HAVE_VECTOR_j0f 0
# define j0f_VEC_SUFF j0f
#endif

#if defined TEST_VECTOR_j0l && TEST_VECTOR_j0l
# define HAVE_VECTOR_j0l 1
# define j0l_VEC_SUFF WRAPPER_NAME (j0l)
WRAPPER_DECL (WRAPPER_NAME (j0l))
#else
# define HAVE_VECTOR_j0l 0
# define j0l_VEC_SUFF j0l
#endif

#if defined TEST_VECTOR_j1 && TEST_VECTOR_j1
# define HAVE_VECTOR_j1 1
# define j1_VEC_SUFF WRAPPER_NAME (j1)
WRAPPER_DECL (WRAPPER_NAME (j1))
#else
# define HAVE_VECTOR_j1 0
# define j1_VEC_SUFF j1
#endif

#if defined TEST_VECTOR_j1f && TEST_VECTOR_j1f
# define HAVE_VECTOR_j1f 1
# define j1f_VEC_SUFF WRAPPER_NAME (j1f)
WRAPPER_DECL (WRAPPER_NAME (j1f))
#else
# define HAVE_VECTOR_j1f 0
# define j1f_VEC_SUFF j1f
#endif

#if defined TEST_VECTOR_j1l && TEST_VECTOR_j1l
# define HAVE_VECTOR_j1l 1
# define j1l_VEC_SUFF WRAPPER_NAME (j1l)
WRAPPER_DECL (WRAPPER_NAME (j1l))
#else
# define HAVE_VECTOR_j1l 0
# define j1l_VEC_SUFF j1l
#endif

#if defined TEST_VECTOR_jn && TEST_VECTOR_jn
# define HAVE_VECTOR_jn 1
# define jn_VEC_SUFF WRAPPER_NAME (jn)
WRAPPER_DECL (WRAPPER_NAME (jn))
#else
# define HAVE_VECTOR_jn 0
# define jn_VEC_SUFF jn
#endif

#if defined TEST_VECTOR_jnf && TEST_VECTOR_jnf
# define HAVE_VECTOR_jnf 1
# define jnf_VEC_SUFF WRAPPER_NAME (jnf)
WRAPPER_DECL (WRAPPER_NAME (jnf))
#else
# define HAVE_VECTOR_jnf 0
# define jnf_VEC_SUFF jnf
#endif

#if defined TEST_VECTOR_jnl && TEST_VECTOR_jnl
# define HAVE_VECTOR_jnl 1
# define jnl_VEC_SUFF WRAPPER_NAME (jnl)
WRAPPER_DECL (WRAPPER_NAME (jnl))
#else
# define HAVE_VECTOR_jnl 0
# define jnl_VEC_SUFF jnl
#endif

#if defined TEST_VECTOR_lgamma && TEST_VECTOR_lgamma
# define HAVE_VECTOR_lgamma 1
# define lgamma_VEC_SUFF WRAPPER_NAME (lgamma)
WRAPPER_DECL (WRAPPER_NAME (lgamma))
#else
# define HAVE_VECTOR_lgamma 0
# define lgamma_VEC_SUFF lgamma
#endif

#if defined TEST_VECTOR_lgammaf && TEST_VECTOR_lgammaf
# define HAVE_VECTOR_lgammaf 1
# define lgammaf_VEC_SUFF WRAPPER_NAME (lgammaf)
WRAPPER_DECL (WRAPPER_NAME (lgammaf))
#else
# define HAVE_VECTOR_lgammaf 0
# define lgammaf_VEC_SUFF lgammaf
#endif

#if defined TEST_VECTOR_lgammal && TEST_VECTOR_lgammal
# define HAVE_VECTOR_lgammal 1
# define lgammal_VEC_SUFF WRAPPER_NAME (lgammal)
WRAPPER_DECL (WRAPPER_NAME (lgammal))
#else
# define HAVE_VECTOR_lgammal 0
# define lgammal_VEC_SUFF lgammal
#endif

#if defined TEST_VECTOR_gamma && TEST_VECTOR_gamma
# define HAVE_VECTOR_gamma 1
# define gamma_VEC_SUFF WRAPPER_NAME (gamma)
WRAPPER_DECL (WRAPPER_NAME (gamma))
#else
# define HAVE_VECTOR_gamma 0
# define gamma_VEC_SUFF gamma
#endif

#if defined TEST_VECTOR_gammaf && TEST_VECTOR_gammaf
# define HAVE_VECTOR_gammaf 1
# define gammaf_VEC_SUFF WRAPPER_NAME (gammaf)
WRAPPER_DECL (WRAPPER_NAME (gammaf))
#else
# define HAVE_VECTOR_gammaf 0
# define gammaf_VEC_SUFF gammaf
#endif

#if defined TEST_VECTOR_gammal && TEST_VECTOR_gammal
# define HAVE_VECTOR_gammal 1
# define gammal_VEC_SUFF WRAPPER_NAME (gammal)
WRAPPER_DECL (WRAPPER_NAME (gammal))
#else
# define HAVE_VECTOR_gammal 0
# define gammal_VEC_SUFF gammal
#endif

#if defined TEST_VECTOR_lrint && TEST_VECTOR_lrint
# define HAVE_VECTOR_lrint 1
# define lrint_VEC_SUFF WRAPPER_NAME (lrint)
WRAPPER_DECL (WRAPPER_NAME (lrint))
#else
# define HAVE_VECTOR_lrint 0
# define lrint_VEC_SUFF lrint
#endif

#if defined TEST_VECTOR_lrintf && TEST_VECTOR_lrintf
# define HAVE_VECTOR_lrintf 1
# define lrintf_VEC_SUFF WRAPPER_NAME (lrintf)
WRAPPER_DECL (WRAPPER_NAME (lrintf))
#else
# define HAVE_VECTOR_lrintf 0
# define lrintf_VEC_SUFF lrintf
#endif

#if defined TEST_VECTOR_lrintl && TEST_VECTOR_lrintl
# define HAVE_VECTOR_lrintl 1
# define lrintl_VEC_SUFF WRAPPER_NAME (lrintl)
WRAPPER_DECL (WRAPPER_NAME (lrintl))
#else
# define HAVE_VECTOR_lrintl 0
# define lrintl_VEC_SUFF lrintl
#endif

#if defined TEST_VECTOR_llrint && TEST_VECTOR_llrint
# define HAVE_VECTOR_llrint 1
# define llrint_VEC_SUFF WRAPPER_NAME (llrint)
WRAPPER_DECL (WRAPPER_NAME (llrint))
#else
# define HAVE_VECTOR_llrint 0
# define llrint_VEC_SUFF llrint
#endif

#if defined TEST_VECTOR_llrintf && TEST_VECTOR_llrintf
# define HAVE_VECTOR_llrintf 1
# define llrintf_VEC_SUFF WRAPPER_NAME (llrintf)
WRAPPER_DECL (WRAPPER_NAME (llrintf))
#else
# define HAVE_VECTOR_llrintf 0
# define llrintf_VEC_SUFF llrintf
#endif

#if defined TEST_VECTOR_llrintl && TEST_VECTOR_llrintl
# define HAVE_VECTOR_llrintl 1
# define llrintl_VEC_SUFF WRAPPER_NAME (llrintl)
WRAPPER_DECL (WRAPPER_NAME (llrintl))
#else
# define HAVE_VECTOR_llrintl 0
# define llrintl_VEC_SUFF llrintl
#endif

#if defined TEST_VECTOR_log && TEST_VECTOR_log
# define HAVE_VECTOR_log 1
# define log_VEC_SUFF WRAPPER_NAME (log)
WRAPPER_DECL (WRAPPER_NAME (log))
#else
# define HAVE_VECTOR_log 0
# define log_VEC_SUFF log
#endif

#if defined TEST_VECTOR_logf && TEST_VECTOR_logf
# define HAVE_VECTOR_logf 1
# define logf_VEC_SUFF WRAPPER_NAME (logf)
WRAPPER_DECL (WRAPPER_NAME (logf))
#else
# define HAVE_VECTOR_logf 0
# define logf_VEC_SUFF logf
#endif

#if defined TEST_VECTOR_logl && TEST_VECTOR_logl
# define HAVE_VECTOR_logl 1
# define logl_VEC_SUFF WRAPPER_NAME (logl)
WRAPPER_DECL (WRAPPER_NAME (logl))
#else
# define HAVE_VECTOR_logl 0
# define logl_VEC_SUFF logl
#endif

#if defined TEST_VECTOR_log10 && TEST_VECTOR_log10
# define HAVE_VECTOR_log10 1
# define log10_VEC_SUFF WRAPPER_NAME (log10)
WRAPPER_DECL (WRAPPER_NAME (log10))
#else
# define HAVE_VECTOR_log10 0
# define log10_VEC_SUFF log10
#endif

#if defined TEST_VECTOR_log10f && TEST_VECTOR_log10f
# define HAVE_VECTOR_log10f 1
# define log10f_VEC_SUFF WRAPPER_NAME (log10f)
WRAPPER_DECL (WRAPPER_NAME (log10f))
#else
# define HAVE_VECTOR_log10f 0
# define log10f_VEC_SUFF log10f
#endif

#if defined TEST_VECTOR_log10l && TEST_VECTOR_log10l
# define HAVE_VECTOR_log10l 1
# define log10l_VEC_SUFF WRAPPER_NAME (log10l)
WRAPPER_DECL (WRAPPER_NAME (log10l))
#else
# define HAVE_VECTOR_log10l 0
# define log10l_VEC_SUFF log10l
#endif

#if defined TEST_VECTOR_log1p && TEST_VECTOR_log1p
# define HAVE_VECTOR_log1p 1
# define log1p_VEC_SUFF WRAPPER_NAME (log1p)
WRAPPER_DECL (WRAPPER_NAME (log1p))
#else
# define HAVE_VECTOR_log1p 0
# define log1p_VEC_SUFF log1p
#endif

#if defined TEST_VECTOR_log1pf && TEST_VECTOR_log1pf
# define HAVE_VECTOR_log1pf 1
# define log1pf_VEC_SUFF WRAPPER_NAME (log1pf)
WRAPPER_DECL (WRAPPER_NAME (log1pf))
#else
# define HAVE_VECTOR_log1pf 0
# define log1pf_VEC_SUFF log1pf
#endif

#if defined TEST_VECTOR_log1pl && TEST_VECTOR_log1pl
# define HAVE_VECTOR_log1pl 1
# define log1pl_VEC_SUFF WRAPPER_NAME (log1pl)
WRAPPER_DECL (WRAPPER_NAME (log1pl))
#else
# define HAVE_VECTOR_log1pl 0
# define log1pl_VEC_SUFF log1pl
#endif

#if defined TEST_VECTOR_log2 && TEST_VECTOR_log2
# define HAVE_VECTOR_log2 1
# define log2_VEC_SUFF WRAPPER_NAME (log2)
WRAPPER_DECL (WRAPPER_NAME (log2))
#else
# define HAVE_VECTOR_log2 0
# define log2_VEC_SUFF log2
#endif

#if defined TEST_VECTOR_log2f && TEST_VECTOR_log2f
# define HAVE_VECTOR_log2f 1
# define log2f_VEC_SUFF WRAPPER_NAME (log2f)
WRAPPER_DECL (WRAPPER_NAME (log2f))
#else
# define HAVE_VECTOR_log2f 0
# define log2f_VEC_SUFF log2f
#endif

#if defined TEST_VECTOR_log2l && TEST_VECTOR_log2l
# define HAVE_VECTOR_log2l 1
# define log2l_VEC_SUFF WRAPPER_NAME (log2l)
WRAPPER_DECL (WRAPPER_NAME (log2l))
#else
# define HAVE_VECTOR_log2l 0
# define log2l_VEC_SUFF log2l
#endif

#if defined TEST_VECTOR_logb && TEST_VECTOR_logb
# define HAVE_VECTOR_logb 1
# define logb_VEC_SUFF WRAPPER_NAME (logb)
WRAPPER_DECL (WRAPPER_NAME (logb))
#else
# define HAVE_VECTOR_logb 0
# define logb_VEC_SUFF logb
#endif

#if defined TEST_VECTOR_logbf && TEST_VECTOR_logbf
# define HAVE_VECTOR_logbf 1
# define logbf_VEC_SUFF WRAPPER_NAME (logbf)
WRAPPER_DECL (WRAPPER_NAME (logbf))
#else
# define HAVE_VECTOR_logbf 0
# define logbf_VEC_SUFF logbf
#endif

#if defined TEST_VECTOR_logbl && TEST_VECTOR_logbl
# define HAVE_VECTOR_logbl 1
# define logbl_VEC_SUFF WRAPPER_NAME (logbl)
WRAPPER_DECL (WRAPPER_NAME (logbl))
#else
# define HAVE_VECTOR_logbl 0
# define logbl_VEC_SUFF logbl
#endif

#if defined TEST_VECTOR_lround && TEST_VECTOR_lround
# define HAVE_VECTOR_lround 1
# define lround_VEC_SUFF WRAPPER_NAME (lround)
WRAPPER_DECL (WRAPPER_NAME (lround))
#else
# define HAVE_VECTOR_lround 0
# define lround_VEC_SUFF lround
#endif

#if defined TEST_VECTOR_lroundf && TEST_VECTOR_lroundf
# define HAVE_VECTOR_lroundf 1
# define lroundf_VEC_SUFF WRAPPER_NAME (lroundf)
WRAPPER_DECL (WRAPPER_NAME (lroundf))
#else
# define HAVE_VECTOR_lroundf 0
# define lroundf_VEC_SUFF lroundf
#endif

#if defined TEST_VECTOR_lroundl && TEST_VECTOR_lroundl
# define HAVE_VECTOR_lroundl 1
# define lroundl_VEC_SUFF WRAPPER_NAME (lroundl)
WRAPPER_DECL (WRAPPER_NAME (lroundl))
#else
# define HAVE_VECTOR_lroundl 0
# define lroundl_VEC_SUFF lroundl
#endif

#if defined TEST_VECTOR_llround && TEST_VECTOR_llround
# define HAVE_VECTOR_llround 1
# define llround_VEC_SUFF WRAPPER_NAME (llround)
WRAPPER_DECL (WRAPPER_NAME (llround))
#else
# define HAVE_VECTOR_llround 0
# define llround_VEC_SUFF llround
#endif

#if defined TEST_VECTOR_llroundf && TEST_VECTOR_llroundf
# define HAVE_VECTOR_llroundf 1
# define llroundf_VEC_SUFF WRAPPER_NAME (llroundf)
WRAPPER_DECL (WRAPPER_NAME (llroundf))
#else
# define HAVE_VECTOR_llroundf 0
# define llroundf_VEC_SUFF llroundf
#endif

#if defined TEST_VECTOR_llroundl && TEST_VECTOR_llroundl
# define HAVE_VECTOR_llroundl 1
# define llroundl_VEC_SUFF WRAPPER_NAME (llroundl)
WRAPPER_DECL (WRAPPER_NAME (llroundl))
#else
# define HAVE_VECTOR_llroundl 0
# define llroundl_VEC_SUFF llroundl
#endif

#if defined TEST_VECTOR_modf && TEST_VECTOR_modf
# define HAVE_VECTOR_modf 1
# define modf_VEC_SUFF WRAPPER_NAME (modf)
WRAPPER_DECL (WRAPPER_NAME (modf))
#else
# define HAVE_VECTOR_modf 0
# define modf_VEC_SUFF modf
#endif

#if defined TEST_VECTOR_modff && TEST_VECTOR_modff
# define HAVE_VECTOR_modff 1
# define modff_VEC_SUFF WRAPPER_NAME (modff)
WRAPPER_DECL (WRAPPER_NAME (modff))
#else
# define HAVE_VECTOR_modff 0
# define modff_VEC_SUFF modff
#endif

#if defined TEST_VECTOR_modfl && TEST_VECTOR_modfl
# define HAVE_VECTOR_modfl 1
# define modfl_VEC_SUFF WRAPPER_NAME (modfl)
WRAPPER_DECL (WRAPPER_NAME (modfl))
#else
# define HAVE_VECTOR_modfl 0
# define modfl_VEC_SUFF modfl
#endif

#if defined TEST_VECTOR_nearbyint && TEST_VECTOR_nearbyint
# define HAVE_VECTOR_nearbyint 1
# define nearbyint_VEC_SUFF WRAPPER_NAME (nearbyint)
WRAPPER_DECL (WRAPPER_NAME (nearbyint))
#else
# define HAVE_VECTOR_nearbyint 0
# define nearbyint_VEC_SUFF nearbyint
#endif

#if defined TEST_VECTOR_nearbyintf && TEST_VECTOR_nearbyintf
# define HAVE_VECTOR_nearbyintf 1
# define nearbyintf_VEC_SUFF WRAPPER_NAME (nearbyintf)
WRAPPER_DECL (WRAPPER_NAME (nearbyintf))
#else
# define HAVE_VECTOR_nearbyintf 0
# define nearbyintf_VEC_SUFF nearbyintf
#endif

#if defined TEST_VECTOR_nearbyintl && TEST_VECTOR_nearbyintl
# define HAVE_VECTOR_nearbyintl 1
# define nearbyintl_VEC_SUFF WRAPPER_NAME (nearbyintl)
WRAPPER_DECL (WRAPPER_NAME (nearbyintl))
#else
# define HAVE_VECTOR_nearbyintl 0
# define nearbyintl_VEC_SUFF nearbyintl
#endif

#if defined TEST_VECTOR_remquo && TEST_VECTOR_remquo
# define HAVE_VECTOR_remquo 1
# define remquo_VEC_SUFF WRAPPER_NAME (remquo)
WRAPPER_DECL (WRAPPER_NAME (remquo))
#else
# define HAVE_VECTOR_remquo 0
# define remquo_VEC_SUFF remquo
#endif

#if defined TEST_VECTOR_remquof && TEST_VECTOR_remquof
# define HAVE_VECTOR_remquof 1
# define remquof_VEC_SUFF WRAPPER_NAME (remquof)
WRAPPER_DECL (WRAPPER_NAME (remquof))
#else
# define HAVE_VECTOR_remquof 0
# define remquof_VEC_SUFF remquof
#endif

#if defined TEST_VECTOR_remquol && TEST_VECTOR_remquol
# define HAVE_VECTOR_remquol 1
# define remquol_VEC_SUFF WRAPPER_NAME (remquol)
WRAPPER_DECL (WRAPPER_NAME (remquol))
#else
# define HAVE_VECTOR_remquol 0
# define remquol_VEC_SUFF remquol
#endif

#if defined TEST_VECTOR_rint && TEST_VECTOR_rint
# define HAVE_VECTOR_rint 1
# define rint_VEC_SUFF WRAPPER_NAME (rint)
WRAPPER_DECL (WRAPPER_NAME (rint))
#else
# define HAVE_VECTOR_rint 0
# define rint_VEC_SUFF rint
#endif

#if defined TEST_VECTOR_rintf && TEST_VECTOR_rintf
# define HAVE_VECTOR_rintf 1
# define rintf_VEC_SUFF WRAPPER_NAME (rintf)
WRAPPER_DECL (WRAPPER_NAME (rintf))
#else
# define HAVE_VECTOR_rintf 0
# define rintf_VEC_SUFF rintf
#endif

#if defined TEST_VECTOR_rintl && TEST_VECTOR_rintl
# define HAVE_VECTOR_rintl 1
# define rintl_VEC_SUFF WRAPPER_NAME (rintl)
WRAPPER_DECL (WRAPPER_NAME (rintl))
#else
# define HAVE_VECTOR_rintl 0
# define rintl_VEC_SUFF rintl
#endif

#if defined TEST_VECTOR_round && TEST_VECTOR_round
# define HAVE_VECTOR_round 1
# define round_VEC_SUFF WRAPPER_NAME (round)
WRAPPER_DECL (WRAPPER_NAME (round))
#else
# define HAVE_VECTOR_round 0
# define round_VEC_SUFF round
#endif

#if defined TEST_VECTOR_roundf && TEST_VECTOR_roundf
# define HAVE_VECTOR_roundf 1
# define roundf_VEC_SUFF WRAPPER_NAME (roundf)
WRAPPER_DECL (WRAPPER_NAME (roundf))
#else
# define HAVE_VECTOR_roundf 0
# define roundf_VEC_SUFF roundf
#endif

#if defined TEST_VECTOR_roundl && TEST_VECTOR_roundl
# define HAVE_VECTOR_roundl 1
# define roundl_VEC_SUFF WRAPPER_NAME (roundl)
WRAPPER_DECL (WRAPPER_NAME (roundl))
#else
# define HAVE_VECTOR_roundl 0
# define roundl_VEC_SUFF roundl
#endif

#if defined TEST_VECTOR_scalbn && TEST_VECTOR_scalbn
# define HAVE_VECTOR_scalbn 1
# define scalbn_VEC_SUFF WRAPPER_NAME (scalbn)
WRAPPER_DECL (WRAPPER_NAME (scalbn))
#else
# define HAVE_VECTOR_scalbn 0
# define scalbn_VEC_SUFF scalbn
#endif

#if defined TEST_VECTOR_scalbnf && TEST_VECTOR_scalbnf
# define HAVE_VECTOR_scalbnf 1
# define scalbnf_VEC_SUFF WRAPPER_NAME (scalbnf)
WRAPPER_DECL (WRAPPER_NAME (scalbnf))
#else
# define HAVE_VECTOR_scalbnf 0
# define scalbnf_VEC_SUFF scalbnf
#endif

#if defined TEST_VECTOR_scalbnl && TEST_VECTOR_scalbnl
# define HAVE_VECTOR_scalbnl 1
# define scalbnl_VEC_SUFF WRAPPER_NAME (scalbnl)
WRAPPER_DECL (WRAPPER_NAME (scalbnl))
#else
# define HAVE_VECTOR_scalbnl 0
# define scalbnl_VEC_SUFF scalbnl
#endif

#if defined TEST_VECTOR_ldexp && TEST_VECTOR_ldexp
# define HAVE_VECTOR_ldexp 1
# define ldexp_VEC_SUFF WRAPPER_NAME (ldexp)
WRAPPER_DECL (WRAPPER_NAME (ldexp))
#else
# define HAVE_VECTOR_ldexp 0
# define ldexp_VEC_SUFF ldexp
#endif

#if defined TEST_VECTOR_ldexpf && TEST_VECTOR_ldexpf
# define HAVE_VECTOR_ldexpf 1
# define ldexpf_VEC_SUFF WRAPPER_NAME (ldexpf)
WRAPPER_DECL (WRAPPER_NAME (ldexpf))
#else
# define HAVE_VECTOR_ldexpf 0
# define ldexpf_VEC_SUFF ldexpf
#endif

#if defined TEST_VECTOR_ldexpl && TEST_VECTOR_ldexpl
# define HAVE_VECTOR_ldexpl 1
# define ldexpl_VEC_SUFF WRAPPER_NAME (ldexpl)
WRAPPER_DECL (WRAPPER_NAME (ldexpl))
#else
# define HAVE_VECTOR_ldexpl 0
# define ldexpl_VEC_SUFF ldexpl
#endif

#if defined TEST_VECTOR_scalbln && TEST_VECTOR_scalbln
# define HAVE_VECTOR_scalbln 1
# define scalbln_VEC_SUFF WRAPPER_NAME (scalbln)
WRAPPER_DECL (WRAPPER_NAME (scalbln))
#else
# define HAVE_VECTOR_scalbln 0
# define scalbln_VEC_SUFF scalbln
#endif

#if defined TEST_VECTOR_scalblnf && TEST_VECTOR_scalblnf
# define HAVE_VECTOR_scalblnf 1
# define scalblnf_VEC_SUFF WRAPPER_NAME (scalblnf)
WRAPPER_DECL (WRAPPER_NAME (scalblnf))
#else
# define HAVE_VECTOR_scalblnf 0
# define scalblnf_VEC_SUFF scalblnf
#endif

#if defined TEST_VECTOR_scalblnl && TEST_VECTOR_scalblnl
# define HAVE_VECTOR_scalblnl 1
# define scalblnl_VEC_SUFF WRAPPER_NAME (scalblnl)
WRAPPER_DECL (WRAPPER_NAME (scalblnl))
#else
# define HAVE_VECTOR_scalblnl 0
# define scalblnl_VEC_SUFF scalblnl
#endif

#if defined TEST_VECTOR_signbit && TEST_VECTOR_signbit
# define HAVE_VECTOR_signbit 1
# define signbit_VEC_SUFF WRAPPER_NAME (signbit)
WRAPPER_DECL (WRAPPER_NAME (signbit))
#else
# define HAVE_VECTOR_signbit 0
# define signbit_VEC_SUFF signbit
#endif

#if defined TEST_VECTOR_signbitf && TEST_VECTOR_signbitf
# define HAVE_VECTOR_signbitf 1
# define signbitf_VEC_SUFF WRAPPER_NAME (signbitf)
WRAPPER_DECL (WRAPPER_NAME (signbitf))
#else
# define HAVE_VECTOR_signbitf 0
# define signbitf_VEC_SUFF signbitf
#endif

#if defined TEST_VECTOR_signbitl && TEST_VECTOR_signbitl
# define HAVE_VECTOR_signbitl 1
# define signbitl_VEC_SUFF WRAPPER_NAME (signbitl)
WRAPPER_DECL (WRAPPER_NAME (signbitl))
#else
# define HAVE_VECTOR_signbitl 0
# define signbitl_VEC_SUFF signbitl
#endif

#if defined TEST_VECTOR_sin && TEST_VECTOR_sin
# define HAVE_VECTOR_sin 1
# define sin_VEC_SUFF WRAPPER_NAME (sin)
WRAPPER_DECL (WRAPPER_NAME (sin))
#else
# define HAVE_VECTOR_sin 0
# define sin_VEC_SUFF sin
#endif

#if defined TEST_VECTOR_sinf && TEST_VECTOR_sinf
# define HAVE_VECTOR_sinf 1
# define sinf_VEC_SUFF WRAPPER_NAME (sinf)
WRAPPER_DECL (WRAPPER_NAME (sinf))
#else
# define HAVE_VECTOR_sinf 0
# define sinf_VEC_SUFF sinf
#endif

#if defined TEST_VECTOR_sinl && TEST_VECTOR_sinl
# define HAVE_VECTOR_sinl 1
# define sinl_VEC_SUFF WRAPPER_NAME (sinl)
WRAPPER_DECL (WRAPPER_NAME (sinl))
#else
# define HAVE_VECTOR_sinl 0
# define sinl_VEC_SUFF sinl
#endif

#if defined TEST_VECTOR_sinh && TEST_VECTOR_sinh
# define HAVE_VECTOR_sinh 1
# define sinh_VEC_SUFF WRAPPER_NAME (sinh)
WRAPPER_DECL (WRAPPER_NAME (sinh))
#else
# define HAVE_VECTOR_sinh 0
# define sinh_VEC_SUFF sinh
#endif

#if defined TEST_VECTOR_sinhf && TEST_VECTOR_sinhf
# define HAVE_VECTOR_sinhf 1
# define sinhf_VEC_SUFF WRAPPER_NAME (sinhf)
WRAPPER_DECL (WRAPPER_NAME (sinhf))
#else
# define HAVE_VECTOR_sinhf 0
# define sinhf_VEC_SUFF sinhf
#endif

#if defined TEST_VECTOR_sinhl && TEST_VECTOR_sinhl
# define HAVE_VECTOR_sinhl 1
# define sinhl_VEC_SUFF WRAPPER_NAME (sinhl)
WRAPPER_DECL (WRAPPER_NAME (sinhl))
#else
# define HAVE_VECTOR_sinhl 0
# define sinhl_VEC_SUFF sinhl
#endif

#if defined TEST_VECTOR_sqrt && TEST_VECTOR_sqrt
# define HAVE_VECTOR_sqrt 1
# define sqrt_VEC_SUFF WRAPPER_NAME (sqrt)
WRAPPER_DECL (WRAPPER_NAME (sqrt))
#else
# define HAVE_VECTOR_sqrt 0
# define sqrt_VEC_SUFF sqrt
#endif

#if defined TEST_VECTOR_sqrtf && TEST_VECTOR_sqrtf
# define HAVE_VECTOR_sqrtf 1
# define sqrtf_VEC_SUFF WRAPPER_NAME (sqrtf)
WRAPPER_DECL (WRAPPER_NAME (sqrtf))
#else
# define HAVE_VECTOR_sqrtf 0
# define sqrtf_VEC_SUFF sqrtf
#endif

#if defined TEST_VECTOR_sqrtl && TEST_VECTOR_sqrtl
# define HAVE_VECTOR_sqrtl 1
# define sqrtl_VEC_SUFF WRAPPER_NAME (sqrtl)
WRAPPER_DECL (WRAPPER_NAME (sqrtl))
#else
# define HAVE_VECTOR_sqrtl 0
# define sqrtl_VEC_SUFF sqrtl
#endif

#if defined TEST_VECTOR_tan && TEST_VECTOR_tan
# define HAVE_VECTOR_tan 1
# define tan_VEC_SUFF WRAPPER_NAME (tan)
WRAPPER_DECL (WRAPPER_NAME (tan))
#else
# define HAVE_VECTOR_tan 0
# define tan_VEC_SUFF tan
#endif

#if defined TEST_VECTOR_tanf && TEST_VECTOR_tanf
# define HAVE_VECTOR_tanf 1
# define tanf_VEC_SUFF WRAPPER_NAME (tanf)
WRAPPER_DECL (WRAPPER_NAME (tanf))
#else
# define HAVE_VECTOR_tanf 0
# define tanf_VEC_SUFF tanf
#endif

#if defined TEST_VECTOR_tanl && TEST_VECTOR_tanl
# define HAVE_VECTOR_tanl 1
# define tanl_VEC_SUFF WRAPPER_NAME (tanl)
WRAPPER_DECL (WRAPPER_NAME (tanl))
#else
# define HAVE_VECTOR_tanl 0
# define tanl_VEC_SUFF tanl
#endif

#if defined TEST_VECTOR_tanh && TEST_VECTOR_tanh
# define HAVE_VECTOR_tanh 1
# define tanh_VEC_SUFF WRAPPER_NAME (tanh)
WRAPPER_DECL (WRAPPER_NAME (tanh))
#else
# define HAVE_VECTOR_tanh 0
# define tanh_VEC_SUFF tanh
#endif

#if defined TEST_VECTOR_tanhf && TEST_VECTOR_tanhf
# define HAVE_VECTOR_tanhf 1
# define tanhf_VEC_SUFF WRAPPER_NAME (tanhf)
WRAPPER_DECL (WRAPPER_NAME (tanhf))
#else
# define HAVE_VECTOR_tanhf 0
# define tanhf_VEC_SUFF tanhf
#endif

#if defined TEST_VECTOR_tanhl && TEST_VECTOR_tanhl
# define HAVE_VECTOR_tanhl 1
# define tanhl_VEC_SUFF WRAPPER_NAME (tanhl)
WRAPPER_DECL (WRAPPER_NAME (tanhl))
#else
# define HAVE_VECTOR_tanhl 0
# define tanhl_VEC_SUFF tanhl
#endif

#if defined TEST_VECTOR_tgamma && TEST_VECTOR_tgamma
# define HAVE_VECTOR_tgamma 1
# define tgamma_VEC_SUFF WRAPPER_NAME (tgamma)
WRAPPER_DECL (WRAPPER_NAME (tgamma))
#else
# define HAVE_VECTOR_tgamma 0
# define tgamma_VEC_SUFF tgamma
#endif

#if defined TEST_VECTOR_tgammaf && TEST_VECTOR_tgammaf
# define HAVE_VECTOR_tgammaf 1
# define tgammaf_VEC_SUFF WRAPPER_NAME (tgammaf)
WRAPPER_DECL (WRAPPER_NAME (tgammaf))
#else
# define HAVE_VECTOR_tgammaf 0
# define tgammaf_VEC_SUFF tgammaf
#endif

#if defined TEST_VECTOR_tgammal && TEST_VECTOR_tgammal
# define HAVE_VECTOR_tgammal 1
# define tgammal_VEC_SUFF WRAPPER_NAME (tgammal)
WRAPPER_DECL (WRAPPER_NAME (tgammal))
#else
# define HAVE_VECTOR_tgammal 0
# define tgammal_VEC_SUFF tgammal
#endif

#if defined TEST_VECTOR_trunc && TEST_VECTOR_trunc
# define HAVE_VECTOR_trunc 1
# define trunc_VEC_SUFF WRAPPER_NAME (trunc)
WRAPPER_DECL (WRAPPER_NAME (trunc))
#else
# define HAVE_VECTOR_trunc 0
# define trunc_VEC_SUFF trunc
#endif

#if defined TEST_VECTOR_truncf && TEST_VECTOR_truncf
# define HAVE_VECTOR_truncf 1
# define truncf_VEC_SUFF WRAPPER_NAME (truncf)
WRAPPER_DECL (WRAPPER_NAME (truncf))
#else
# define HAVE_VECTOR_truncf 0
# define truncf_VEC_SUFF truncf
#endif

#if defined TEST_VECTOR_truncl && TEST_VECTOR_truncl
# define HAVE_VECTOR_truncl 1
# define truncl_VEC_SUFF WRAPPER_NAME (truncl)
WRAPPER_DECL (WRAPPER_NAME (truncl))
#else
# define HAVE_VECTOR_truncl 0
# define truncl_VEC_SUFF truncl
#endif

#if defined TEST_VECTOR_y0 && TEST_VECTOR_y0
# define HAVE_VECTOR_y0 1
# define y0_VEC_SUFF WRAPPER_NAME (y0)
WRAPPER_DECL (WRAPPER_NAME (y0))
#else
# define HAVE_VECTOR_y0 0
# define y0_VEC_SUFF y0
#endif

#if defined TEST_VECTOR_y0f && TEST_VECTOR_y0f
# define HAVE_VECTOR_y0f 1
# define y0f_VEC_SUFF WRAPPER_NAME (y0f)
WRAPPER_DECL (WRAPPER_NAME (y0f))
#else
# define HAVE_VECTOR_y0f 0
# define y0f_VEC_SUFF y0f
#endif

#if defined TEST_VECTOR_y0l && TEST_VECTOR_y0l
# define HAVE_VECTOR_y0l 1
# define y0l_VEC_SUFF WRAPPER_NAME (y0l)
WRAPPER_DECL (WRAPPER_NAME (y0l))
#else
# define HAVE_VECTOR_y0l 0
# define y0l_VEC_SUFF y0l
#endif

#if defined TEST_VECTOR_y1 && TEST_VECTOR_y1
# define HAVE_VECTOR_y1 1
# define y1_VEC_SUFF WRAPPER_NAME (y1)
WRAPPER_DECL (WRAPPER_NAME (y1))
#else
# define HAVE_VECTOR_y1 0
# define y1_VEC_SUFF y1
#endif

#if defined TEST_VECTOR_y1f && TEST_VECTOR_y1f
# define HAVE_VECTOR_y1f 1
# define y1f_VEC_SUFF WRAPPER_NAME (y1f)
WRAPPER_DECL (WRAPPER_NAME (y1f))
#else
# define HAVE_VECTOR_y1f 0
# define y1f_VEC_SUFF y1f
#endif

#if defined TEST_VECTOR_y1l && TEST_VECTOR_y1l
# define HAVE_VECTOR_y1l 1
# define y1l_VEC_SUFF WRAPPER_NAME (y1l)
WRAPPER_DECL (WRAPPER_NAME (y1l))
#else
# define HAVE_VECTOR_y1l 0
# define y1l_VEC_SUFF y1l
#endif

#if defined TEST_VECTOR_yn && TEST_VECTOR_yn
# define HAVE_VECTOR_yn 1
# define yn_VEC_SUFF WRAPPER_NAME (yn)
WRAPPER_DECL (WRAPPER_NAME (yn))
#else
# define HAVE_VECTOR_yn 0
# define yn_VEC_SUFF yn
#endif

#if defined TEST_VECTOR_ynf && TEST_VECTOR_ynf
# define HAVE_VECTOR_ynf 1
# define ynf_VEC_SUFF WRAPPER_NAME (ynf)
WRAPPER_DECL (WRAPPER_NAME (ynf))
#else
# define HAVE_VECTOR_ynf 0
# define ynf_VEC_SUFF ynf
#endif

#if defined TEST_VECTOR_ynl && TEST_VECTOR_ynl
# define HAVE_VECTOR_ynl 1
# define ynl_VEC_SUFF WRAPPER_NAME (ynl)
WRAPPER_DECL (WRAPPER_NAME (ynl))
#else
# define HAVE_VECTOR_ynl 0
# define ynl_VEC_SUFF ynl
#endif

#if defined TEST_VECTOR_significand && TEST_VECTOR_significand
# define HAVE_VECTOR_significand 1
# define significand_VEC_SUFF WRAPPER_NAME (significand)
WRAPPER_DECL (WRAPPER_NAME (significand))
#else
# define HAVE_VECTOR_significand 0
# define significand_VEC_SUFF significand
#endif

#if defined TEST_VECTOR_significandf && TEST_VECTOR_significandf
# define HAVE_VECTOR_significandf 1
# define significandf_VEC_SUFF WRAPPER_NAME (significandf)
WRAPPER_DECL (WRAPPER_NAME (significandf))
#else
# define HAVE_VECTOR_significandf 0
# define significandf_VEC_SUFF significandf
#endif

#if defined TEST_VECTOR_significandl && TEST_VECTOR_significandl
# define HAVE_VECTOR_significandl 1
# define significandl_VEC_SUFF WRAPPER_NAME (significandl)
WRAPPER_DECL (WRAPPER_NAME (significandl))
#else
# define HAVE_VECTOR_significandl 0
# define significandl_VEC_SUFF significandl
#endif

#if defined TEST_VECTOR_atan2 && TEST_VECTOR_atan2
# define HAVE_VECTOR_atan2 1
# define atan2_VEC_SUFF WRAPPER_NAME (atan2)
WRAPPER_DECL_ff (WRAPPER_NAME (atan2))
#else
# define HAVE_VECTOR_atan2 0
# define atan2_VEC_SUFF atan2
#endif

#if defined TEST_VECTOR_atan2f && TEST_VECTOR_atan2f
# define HAVE_VECTOR_atan2f 1
# define atan2f_VEC_SUFF WRAPPER_NAME (atan2f)
WRAPPER_DECL_ff (WRAPPER_NAME (atan2f))
#else
# define HAVE_VECTOR_atan2f 0
# define atan2f_VEC_SUFF atan2f
#endif

#if defined TEST_VECTOR_atan2l && TEST_VECTOR_atan2l
# define HAVE_VECTOR_atan2l 1
# define atan2l_VEC_SUFF WRAPPER_NAME (atan2l)
WRAPPER_DECL_ff (WRAPPER_NAME (atan2l))
#else
# define HAVE_VECTOR_atan2l 0
# define atan2l_VEC_SUFF atan2l
#endif

#if defined TEST_VECTOR_copysign && TEST_VECTOR_copysign
# define HAVE_VECTOR_copysign 1
# define copysign_VEC_SUFF WRAPPER_NAME (copysign)
WRAPPER_DECL_ff (WRAPPER_NAME (copysign))
#else
# define HAVE_VECTOR_copysign 0
# define copysign_VEC_SUFF copysign
#endif

#if defined TEST_VECTOR_copysignf && TEST_VECTOR_copysignf
# define HAVE_VECTOR_copysignf 1
# define copysignf_VEC_SUFF WRAPPER_NAME (copysignf)
WRAPPER_DECL_ff (WRAPPER_NAME (copysignf))
#else
# define HAVE_VECTOR_copysignf 0
# define copysignf_VEC_SUFF copysignf
#endif

#if defined TEST_VECTOR_copysignl && TEST_VECTOR_copysignl
# define HAVE_VECTOR_copysignl 1
# define copysignl_VEC_SUFF WRAPPER_NAME (copysignl)
WRAPPER_DECL_ff (WRAPPER_NAME (copysignl))
#else
# define HAVE_VECTOR_copysignl 0
# define copysignl_VEC_SUFF copysignl
#endif

#if defined TEST_VECTOR_fdim && TEST_VECTOR_fdim
# define HAVE_VECTOR_fdim 1
# define fdim_VEC_SUFF WRAPPER_NAME (fdim)
WRAPPER_DECL_ff (WRAPPER_NAME (fdim))
#else
# define HAVE_VECTOR_fdim 0
# define fdim_VEC_SUFF fdim
#endif

#if defined TEST_VECTOR_fdimf && TEST_VECTOR_fdimf
# define HAVE_VECTOR_fdimf 1
# define fdimf_VEC_SUFF WRAPPER_NAME (fdimf)
WRAPPER_DECL_ff (WRAPPER_NAME (fdimf))
#else
# define HAVE_VECTOR_fdimf 0
# define fdimf_VEC_SUFF fdimf
#endif

#if defined TEST_VECTOR_fdiml && TEST_VECTOR_fdiml
# define HAVE_VECTOR_fdiml 1
# define fdiml_VEC_SUFF WRAPPER_NAME (fdiml)
WRAPPER_DECL_ff (WRAPPER_NAME (fdiml))
#else
# define HAVE_VECTOR_fdiml 0
# define fdiml_VEC_SUFF fdiml
#endif

#if defined TEST_VECTOR_fmax && TEST_VECTOR_fmax
# define HAVE_VECTOR_fmax 1
# define fmax_VEC_SUFF WRAPPER_NAME (fmax)
WRAPPER_DECL_ff (WRAPPER_NAME (fmax))
#else
# define HAVE_VECTOR_fmax 0
# define fmax_VEC_SUFF fmax
#endif

#if defined TEST_VECTOR_fmaxf && TEST_VECTOR_fmaxf
# define HAVE_VECTOR_fmaxf 1
# define fmaxf_VEC_SUFF WRAPPER_NAME (fmaxf)
WRAPPER_DECL_ff (WRAPPER_NAME (fmaxf))
#else
# define HAVE_VECTOR_fmaxf 0
# define fmaxf_VEC_SUFF fmaxf
#endif

#if defined TEST_VECTOR_fmaxl && TEST_VECTOR_fmaxl
# define HAVE_VECTOR_fmaxl 1
# define fmaxl_VEC_SUFF WRAPPER_NAME (fmaxl)
WRAPPER_DECL_ff (WRAPPER_NAME (fmaxl))
#else
# define HAVE_VECTOR_fmaxl 0
# define fmaxl_VEC_SUFF fmaxl
#endif

#if defined TEST_VECTOR_fmin && TEST_VECTOR_fmin
# define HAVE_VECTOR_fmin 1
# define fmin_VEC_SUFF WRAPPER_NAME (fmin)
WRAPPER_DECL_ff (WRAPPER_NAME (fmin))
#else
# define HAVE_VECTOR_fmin 0
# define fmin_VEC_SUFF fmin
#endif

#if defined TEST_VECTOR_fminf && TEST_VECTOR_fminf
# define HAVE_VECTOR_fminf 1
# define fminf_VEC_SUFF WRAPPER_NAME (fminf)
WRAPPER_DECL_ff (WRAPPER_NAME (fminf))
#else
# define HAVE_VECTOR_fminf 0
# define fminf_VEC_SUFF fminf
#endif

#if defined TEST_VECTOR_fminl && TEST_VECTOR_fminl
# define HAVE_VECTOR_fminl 1
# define fminl_VEC_SUFF WRAPPER_NAME (fminl)
WRAPPER_DECL_ff (WRAPPER_NAME (fminl))
#else
# define HAVE_VECTOR_fminl 0
# define fminl_VEC_SUFF fminl
#endif

#if defined TEST_VECTOR_fmod && TEST_VECTOR_fmod
# define HAVE_VECTOR_fmod 1
# define fmod_VEC_SUFF WRAPPER_NAME (fmod)
WRAPPER_DECL_ff (WRAPPER_NAME (fmod))
#else
# define HAVE_VECTOR_fmod 0
# define fmod_VEC_SUFF fmod
#endif

#if defined TEST_VECTOR_fmodf && TEST_VECTOR_fmodf
# define HAVE_VECTOR_fmodf 1
# define fmodf_VEC_SUFF WRAPPER_NAME (fmodf)
WRAPPER_DECL_ff (WRAPPER_NAME (fmodf))
#else
# define HAVE_VECTOR_fmodf 0
# define fmodf_VEC_SUFF fmodf
#endif

#if defined TEST_VECTOR_fmodl && TEST_VECTOR_fmodl
# define HAVE_VECTOR_fmodl 1
# define fmodl_VEC_SUFF WRAPPER_NAME (fmodl)
WRAPPER_DECL_ff (WRAPPER_NAME (fmodl))
#else
# define HAVE_VECTOR_fmodl 0
# define fmodl_VEC_SUFF fmodl
#endif

#if defined TEST_VECTOR_hypot && TEST_VECTOR_hypot
# define HAVE_VECTOR_hypot 1
# define hypot_VEC_SUFF WRAPPER_NAME (hypot)
WRAPPER_DECL_ff (WRAPPER_NAME (hypot))
#else
# define HAVE_VECTOR_hypot 0
# define hypot_VEC_SUFF hypot
#endif

#if defined TEST_VECTOR_hypotf && TEST_VECTOR_hypotf
# define HAVE_VECTOR_hypotf 1
# define hypotf_VEC_SUFF WRAPPER_NAME (hypotf)
WRAPPER_DECL_ff (WRAPPER_NAME (hypotf))
#else
# define HAVE_VECTOR_hypotf 0
# define hypotf_VEC_SUFF hypotf
#endif

#if defined TEST_VECTOR_hypotl && TEST_VECTOR_hypotl
# define HAVE_VECTOR_hypotl 1
# define hypotl_VEC_SUFF WRAPPER_NAME (hypotl)
WRAPPER_DECL_ff (WRAPPER_NAME (hypotl))
#else
# define HAVE_VECTOR_hypotl 0
# define hypotl_VEC_SUFF hypotl
#endif

#if defined TEST_VECTOR_nextafter && TEST_VECTOR_nextafter
# define HAVE_VECTOR_nextafter 1
# define nextafter_VEC_SUFF WRAPPER_NAME (nextafter)
WRAPPER_DECL_ff (WRAPPER_NAME (nextafter))
#else
# define HAVE_VECTOR_nextafter 0
# define nextafter_VEC_SUFF nextafter
#endif

#if defined TEST_VECTOR_nextafterf && TEST_VECTOR_nextafterf
# define HAVE_VECTOR_nextafterf 1
# define nextafterf_VEC_SUFF WRAPPER_NAME (nextafterf)
WRAPPER_DECL_ff (WRAPPER_NAME (nextafterf))
#else
# define HAVE_VECTOR_nextafterf 0
# define nextafterf_VEC_SUFF nextafterf
#endif

#if defined TEST_VECTOR_nextafterl && TEST_VECTOR_nextafterl
# define HAVE_VECTOR_nextafterl 1
# define nextafterl_VEC_SUFF WRAPPER_NAME (nextafterl)
WRAPPER_DECL_ff (WRAPPER_NAME (nextafterl))
#else
# define HAVE_VECTOR_nextafterl 0
# define nextafterl_VEC_SUFF nextafterl
#endif

#if defined TEST_VECTOR_nexttoward && TEST_VECTOR_nexttoward
# define HAVE_VECTOR_nexttoward 1
# define nexttoward_VEC_SUFF WRAPPER_NAME (nexttoward)
WRAPPER_DECL_ff (WRAPPER_NAME (nexttoward))
#else
# define HAVE_VECTOR_nexttoward 0
# define nexttoward_VEC_SUFF nexttoward
#endif

#if defined TEST_VECTOR_nexttowardf && TEST_VECTOR_nexttowardf
# define HAVE_VECTOR_nexttowardf 1
# define nexttowardf_VEC_SUFF WRAPPER_NAME (nexttowardf)
WRAPPER_DECL_ff (WRAPPER_NAME (nexttowardf))
#else
# define HAVE_VECTOR_nexttowardf 0
# define nexttowardf_VEC_SUFF nexttowardf
#endif

#if defined TEST_VECTOR_nexttowardl && TEST_VECTOR_nexttowardl
# define HAVE_VECTOR_nexttowardl 1
# define nexttowardl_VEC_SUFF WRAPPER_NAME (nexttowardl)
WRAPPER_DECL_ff (WRAPPER_NAME (nexttowardl))
#else
# define HAVE_VECTOR_nexttowardl 0
# define nexttowardl_VEC_SUFF nexttowardl
#endif

#if defined TEST_VECTOR_pow && TEST_VECTOR_pow
# define HAVE_VECTOR_pow 1
# define pow_VEC_SUFF WRAPPER_NAME (pow)
WRAPPER_DECL_ff (WRAPPER_NAME (pow))
#else
# define HAVE_VECTOR_pow 0
# define pow_VEC_SUFF pow
#endif

#if defined TEST_VECTOR_powf && TEST_VECTOR_powf
# define HAVE_VECTOR_powf 1
# define powf_VEC_SUFF WRAPPER_NAME (powf)
WRAPPER_DECL_ff (WRAPPER_NAME (powf))
#else
# define HAVE_VECTOR_powf 0
# define powf_VEC_SUFF powf
#endif

#if defined TEST_VECTOR_powl && TEST_VECTOR_powl
# define HAVE_VECTOR_powl 1
# define powl_VEC_SUFF WRAPPER_NAME (powl)
WRAPPER_DECL_ff (WRAPPER_NAME (powl))
#else
# define HAVE_VECTOR_powl 0
# define powl_VEC_SUFF powl
#endif

#if defined TEST_VECTOR_remainder && TEST_VECTOR_remainder
# define HAVE_VECTOR_remainder 1
# define remainder_VEC_SUFF WRAPPER_NAME (remainder)
WRAPPER_DECL_ff (WRAPPER_NAME (remainder))
#else
# define HAVE_VECTOR_remainder 0
# define remainder_VEC_SUFF remainder
#endif

#if defined TEST_VECTOR_remainderf && TEST_VECTOR_remainderf
# define HAVE_VECTOR_remainderf 1
# define remainderf_VEC_SUFF WRAPPER_NAME (remainderf)
WRAPPER_DECL_ff (WRAPPER_NAME (remainderf))
#else
# define HAVE_VECTOR_remainderf 0
# define remainderf_VEC_SUFF remainderf
#endif

#if defined TEST_VECTOR_remainderl && TEST_VECTOR_remainderl
# define HAVE_VECTOR_remainderl 1
# define remainderl_VEC_SUFF WRAPPER_NAME (remainderl)
WRAPPER_DECL_ff (WRAPPER_NAME (remainderl))
#else
# define HAVE_VECTOR_remainderl 0
# define remainderl_VEC_SUFF remainderl
#endif

#if defined TEST_VECTOR_drem && TEST_VECTOR_drem
# define HAVE_VECTOR_drem 1
# define drem_VEC_SUFF WRAPPER_NAME (drem)
WRAPPER_DECL_ff (WRAPPER_NAME (drem))
#else
# define HAVE_VECTOR_drem 0
# define drem_VEC_SUFF drem
#endif

#if defined TEST_VECTOR_dremf && TEST_VECTOR_dremf
# define HAVE_VECTOR_dremf 1
# define dremf_VEC_SUFF WRAPPER_NAME (dremf)
WRAPPER_DECL_ff (WRAPPER_NAME (dremf))
#else
# define HAVE_VECTOR_dremf 0
# define dremf_VEC_SUFF dremf
#endif

#if defined TEST_VECTOR_dreml && TEST_VECTOR_dreml
# define HAVE_VECTOR_dreml 1
# define dreml_VEC_SUFF WRAPPER_NAME (dreml)
WRAPPER_DECL_ff (WRAPPER_NAME (dreml))
#else
# define HAVE_VECTOR_dreml 0
# define dreml_VEC_SUFF dreml
#endif

#if defined TEST_VECTOR_scalb && TEST_VECTOR_scalb
# define HAVE_VECTOR_scalb 1
# define scalb_VEC_SUFF WRAPPER_NAME (scalb)
WRAPPER_DECL_ff (WRAPPER_NAME (scalb))
#else
# define HAVE_VECTOR_scalb 0
# define scalb_VEC_SUFF scalb
#endif

#if defined TEST_VECTOR_scalbf && TEST_VECTOR_scalbf
# define HAVE_VECTOR_scalbf 1
# define scalbf_VEC_SUFF WRAPPER_NAME (scalbf)
WRAPPER_DECL_ff (WRAPPER_NAME (scalbf))
#else
# define HAVE_VECTOR_scalbf 0
# define scalbf_VEC_SUFF scalbf
#endif

#if defined TEST_VECTOR_scalbl && TEST_VECTOR_scalbl
# define HAVE_VECTOR_scalbl 1
# define scalbl_VEC_SUFF WRAPPER_NAME (scalbl)
WRAPPER_DECL_ff (WRAPPER_NAME (scalbl))
#else
# define HAVE_VECTOR_scalbl 0
# define scalbl_VEC_SUFF scalbl
#endif

#if defined TEST_VECTOR_sincos && TEST_VECTOR_sincos
# define HAVE_VECTOR_sincos 1
# define sincos_VEC_SUFF WRAPPER_NAME (sincos)
WRAPPER_DECL_fFF (WRAPPER_NAME (sincos))
#else
# define HAVE_VECTOR_sincos 0
# define sincos_VEC_SUFF sincos
#endif

#if defined TEST_VECTOR_sincosf && TEST_VECTOR_sincosf
# define HAVE_VECTOR_sincosf 1
# define sincosf_VEC_SUFF WRAPPER_NAME (sincosf)
WRAPPER_DECL_fFF (WRAPPER_NAME (sincosf))
#else
# define HAVE_VECTOR_sincosf 0
# define sincosf_VEC_SUFF sincosf
#endif

#if defined TEST_VECTOR_sincosl && TEST_VECTOR_sincosl
# define HAVE_VECTOR_sincosl 1
# define sincosl_VEC_SUFF WRAPPER_NAME (sincosl)
WRAPPER_DECL_fFF (WRAPPER_NAME (sincosl))
#else
# define HAVE_VECTOR_sincosl 0
# define sincosl_VEC_SUFF sincosl
#endif

