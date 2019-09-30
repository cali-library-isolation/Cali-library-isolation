# We don't need any header files.
abi-includes :=

abi-variants := 32 64

abi-32-options := -D__i386__ -U__x86_64__
abi-32-condition := !defined __x86_64__
abi-64-options := -U__i386__ -D__x86_64__ -U__ILP32__ -D__LP64__
abi-64-condition := defined __x86_64__ && defined __LP64__
