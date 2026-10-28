# A. Description
#    A list of commands here configures RNG module and generates a random number.
#
# B. Preparation
#    N/A
#
# C. Prerequisite Example (abov_example_config.h)
#    1. RNG
#
# Please, read the User Manual of the specific chip for more details.
#
# RNG0
# 1. Channel                   : 0 (RNG0)
#
# 2. Config                    : [ 0 p l o 65535 ]
#    Operation                 : p (Polling)
#    LFS Clock                 : l (LSI - Low Speed Internal Clock)
#    CAS Clock                 : o (OSC - Internal RNG OSC for CAS)
#    Generation Time           : 65535
#
# 3. Seed                      : [ 0 0xf0f0f0f0 ]
#    Seed value                : 0xf0f0f0f0

# 4. Gen                       : [ 0 e ]
#    Generation Enable         : e (Enable)
#
# RNG0
send ""

send "cm rng"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

send "config 0 p l o 65535"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

send "seed 0 0xf0f0f0f0"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

send "log on 5"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

set a 0
set b 16

loop:
inc a
send "gen 0 e"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}
if a < 16 goto loop

send "gen 0 d"
expect {
    "<RNG> # "
    break
    timeout 5 goto end
}

end:
