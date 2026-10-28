# A. Description
#    A list of commands here configures SCU LVD and generates Low Voltage Indicator interrupt.
#
# B. Preparation
#    Supply power to external power port by a suitable instrument or make a environment to generate low voltage.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. SCULVD
#
# For more information, read a user's manual of the target device carefully.
#
# SCULVD
# 1. LVI                       : [ e 8 ]
#    Interrupt Enable          : e (Maskable Interrupt)
#    Detect Level              : 8 (see User Manual's Low Voltage Indicator bit)
#
# SCULVD
send ""

send "cm sculvd"
expect {
    "<SCULVD> # "
    break
    timeout 5 goto end
}

send "log on 0"
expect {
    "<SCULVD> # "
    break
    timeout 5 goto end
}

send "lvi e 8"
expect {
    "<SCULVD> # "
    break
    timeout 5 goto end
}

end:
