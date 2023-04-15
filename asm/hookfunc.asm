__GOLEM_HOOK_START:
	lis %r3, helperInputHook+0x48000001-0x804ee6e4
	ori %r3, %r3, helperInputHook+0x48000001-0x804ee6e4
	lis %r4, 0x804F
	stw %r3, -0x1944 (%r4)
	# HelperInputHook branch

	li %r3, 1
	addi %r3, %r3, piggybackHook-0x805061bc
	lis %r4, 0x8050
	stw %r3, 0x61BC (%r4)
	# piggybackHook

	# unlinkHook