# A. Description
#    A list of commands here configures SCU PWR and enters deep-sleep mode.
#    And it wakes a target device up from Low Voltage Indicator interrupt by SCU LVD module.
#
# B. Preparation
#    Supply power to external power port by a suitable instrument or make a environment to generate low voltage.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. SCULVD
#    2. SCUPWR
#
# For more information, read a user's manual of the target device carefully.
#
# SCULVD
# 1. LVI                        : [ e 8 -wake e ]
#    Interrupt Enable           : e (Maskable Interrupt)
#    Detect Level               : 8 (see User Manual's Low Voltage Indicator bit)
#    Wake-up Enable             : e
#
# 2. LVR                        : [ d ]
#    Interrupt Enable           : d (Disable)
#
# SCUPWR
# 1. AON                        : [ v:e b:e l:e ]
#    Always on Block Enable     : v (VDC) : e (enable) / b (BGR) : e (enable) / l (LSI) / e (enable)
#
# 2. PWR                        : [ d l 0 h 0 ]
#    Power Mode                 : d (Deep Sleep 1)
#    Sleep Clock Source         : l (LSI)
#    Sleep Clock Source Divide  : 0
#    Wakeup Clock Source        : h (HSI)
#    Wakeup Clock Source Divide : 0
#
# SCULVD
send ""

send "cm sculvd"
expect {
    "<SCULVD> # "
    break
    timeout 5 goto end
}

send "lvr d"
expect {
    "<SCULVD> # "
    break
    timeout 5 goto end
}

send "lvi e 8 -wake e"
expect {
    "<SCULVD> # "
    break
    timeout 5 goto end
}

# SCUPWR
send "cm scupwr"
expect {
    "<SCUPWR> # "
    break
    timeout 5 goto end
}

send "aon v:e b:e l:e"
expect {
    "<SCUPWR> # "
    break
    timeout 5 goto end
}

send "pwr d l 0 h 0"
expect {
    "<SCUPWR> # "
    break
    timeout 5 goto end
}

end:
