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

	lis %r3, (piggybackHook+0x48000001-0x805061bc)@h
	ori %r3, %r3, (piggybackHook+0x48000001-0x805061bc)@l
	lis %r4, 0x8050
	stw %r3, 0x61BC (%r4)
	# piggybackHook

	lis %r3, (unlinkParent_PiggyHook+0x48000001-0x805058f0)@ha
	addi %r3, %r3, (unlinkParent_PiggyHook+0x48000001-0x805058f0)@l
	stw %r3, 0x58f0 (%r4)
	# unlinkParent_PiggyHook

	lis %r3, (unlinkChild_PiggyHook+0x48000001-0x80505930)@ha
	addi %r3, %r3, (unlinkChild_PiggyHook+0x48000001-0x80505930)@l
	stw %r3, 0x5930 (%r4)
	# unlinkChild_PiggyHook
	
	b __start_RTDL
	# end