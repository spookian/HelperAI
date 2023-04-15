.global __GOLEM_HOOK_START
__GOLEM_HOOK_START:
	lis %r3, (helperInputHook+0x48000001-0x804ee6e4)@ha
	ori %r3, %r3, (helperInputHook+0x48000001-0x804ee6e4)@l
	lis %r4, 0x804F
	stw %r3, -0x191C (%r4)

	lis %r3, 0x7FC3
	ori %r3, %r3, 0xF378
	stw %r3, -0x1920 (%r4)
	# HelperInputHook branch and argument setup

	rlwinm %r3, %r3, 0, 0xFF000000
	addi %r3, %r3, (piggybackHook-0x805061bc+1)@ha
	lis %r4, 0x8050
	stw %r3, 0x61BC (%r4)
	# piggybackHook

	rlwinm %r3, %r3, 0, 0xFF000000
	addi %r3, %r3, (unlinkHook-0x805058f0+1)@l
	stw %r3, 0x58f0 (%r4)
	# unlinkHook

	b __start_RTDL
	# end