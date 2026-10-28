# A. Description
#    A list of commands here configures PCU module and reads a target port.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. PCU/GPIO
#
# D. Default Port
#    1. PCU     : PA0/PA1/PA2/PA3/PA6/PA7 (Input)
#
# For more information, read a user's manual of the target device carefully.
#
# PCU (PAx)
# 1. Port Group                   : 0 (PCU Port A)
#
# 2. Port                         : [ 0 0 i i ] [ 0 1 i i ] [ 0 2 i i ] [ 0 3 i i ] [ 0 6 i i ] [ 0 7 i i ]
#    Pin Number                   : 0 / 1 / 2 / 3 / 6 / 7
#    Pin Mode                     : i (input)
#    Pin Output Mode              : i (input)
#
# 3. Input                        : [ 0 0 ], [ 0 1 ], [ 0 2 ], [ 0 3 ], [ 0 6 ], [ 0 7 ]
#    Pin Number                   : 0, 1, 2, 3, 6, 7
#
# PCU (PA0)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 0 0 i i"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "input 0 0"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}
sleep 0.5

# PCU (PA1)
send "port 0 1 i i"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "input 0 1"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}
sleep 0.5

# PCU (PA2)
send "port 0 2 i i"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "input 0 2"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}
sleep 0.5

# PCU (PA3)
send "port 0 3 i i"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "input 0 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}
sleep 0.5

# PCU (PA6)
send "port 0 6 i i"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "input 0 6"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}
sleep 0.5

# PCU (PA7)
send "port 0 7 i i"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "input 0 7"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}
sleep 0.5

end:
