# A. Description
#    A list of commands here configures AES module and generates encrypted data (ECB : electronic codebook).
#
# B. Preparation
#    N/A
#
# C. Prerequisite Example (abov_example_config.h)
#    1. AES
#
# Please, read the User Manual of the specific chip for more details.
#
# AES0
# 1. Channel                   : 0 (AES0)
#
# 2. Config                    : [ 0 n e s s ] [ 0 n d s s ]
#    Operation                 : n (Non Maskable Interrupt)
#    Cipher Mode               : e (Encryption) / d (Decryption)
#    In Fifo Alignment         : s (swap word)
#    Out Fifo Alignment        : s (swap word)
#
# 3. Key                       : [ 0 0x00000000 0x00000001 0x00000002 0x00000003 ]
#
# 4. Buf                       : [ 0 0 4 0x00000000 0x00000001 0x00000002 0x00000003 ]
#                              : [ 0 4 4 0x00000004 0x00000005 0x00000006 0x00000007 ]
#                              : [ 0 8 4 0x00000008 0x00000009 0x0000000a 0x0000000b ]
#                              : [ 0 12 4 0x0000000c 0x0000000d 0x0000000e 0x0000000f ]
#                              : [ 0 16 4 0x00000000 0x00000001 0x00000002 0x00000003 ]
#                              : [ 0 20 4 0x00000004 0x00000005 0x00000006 0x00000007 ]
#                              : [ 0 24 4 0x00000008 0x00000009 0x0000000a 0x0000000b ]
#                              : [ 0 28 4 0x0000000c 0x0000000d 0x0000000e 0x0000000f ]
#    Start Buffer Position     : 0 / 4 / 8 / 12 / 16 / 20 / 24 / 28
#    Data Length               : 4 / 4 / 4 / 4 / 4 / 4 / 4 / 4
#
# 5. Cmpt                      : [ 0 32 ] [ 0 32 -cp]
#    Computation Buffer Length : 32
#    Copy out buffer to in buffer : -cp
#
# AES0
send ""

send "cm aes"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "config 0 n e s s"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "key 0 0x00000000 0x00000001 0x00000002 0x00000003"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 0 4 0x00000000 0x00000001 0x00000002 0x00000003"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 4 4 0x00000004 0x00000005 0x00000006 0x00000007"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 8 4 0x00000008 0x00000009 0x0000000A 0x0000000B"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 12 4 0x0000000C 0x0000000D 0x0000000E 0x0000000F"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 16 4 0x00000000 0x00000001 0x00000002 0x00000003"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 20 4 0x00000004 0x00000005 0x00000006 0x00000007"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 24 4 0x00000008 0x00000009 0x0000000a 0x0000000b"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "buf 0 28 4 0x0000000c 0x0000000d 0x0000000e 0x0000000f"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "cmpt 0 32"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}
sleep 1

send "dump"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}
sleep 1

send "config 0 n d s s"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}

send "cmpt 0 32 -cp"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}
sleep 1

send "dump"
expect {
    "<AES> # "
    break
    timeout 5 goto end
}
