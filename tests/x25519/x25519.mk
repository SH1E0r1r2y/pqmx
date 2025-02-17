
TESTS += x25519

X25519_PLATFORMS += m7-an500
X25519_PLATFORMS += nucleo-f767zi
X25519_PLATFORMS += stm32f4discovery

# C sources required for this test
X25519_SOURCES += main.c


X25519_ASMDIR = ../../asm/manual/x25519
X25519_ASMS += $(X25519_ASMDIR)/fe25519_add.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_sub.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_mul.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_sqr.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_add_opt_m7.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_sub_opt_m7.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_mul_opt_m7.s
X25519_ASMS += $(X25519_ASMDIR)/fe25519_sqr_opt_m7.s
X25519_ASMS += $(X25519_ASMDIR)/curve25519_scalarmult.s