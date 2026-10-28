# A. Description
#    A list of commands here configures SCU CLK and generates HSE Clock Monitor Failure interrupt.
#
# B. Preparation
#    N/A
#
# C. Prerequisite Example (abov_example_config.h)
#    1. SCUCLK
#
# For more information, read a user's manual of the target device carefully.
#
# SCUCLK
# 1. MONITOR             : [ e e m 3 ]
#    Clock Source        : e (HSE : High Speed External Clock)
#    Monitor Enable      : e (Enable)
#    Interrupt Mode      : m (Maskable Interrupt)
#    Priority            : 3
#
# 2. CLKEN               : [ e e ] [ e d ]
#    Clock Source        : e (HSE : High Speed External Clock)
#    Clock Enable        : e (Enable) / d (Disable)
#
# SCUCLK
send ""

send "cm scuclk"
expect {
    "<SCUCLK> # "
    break
    timeout 5 goto end
}

send "clken e e"
expect {
    "<SCUCLK> # "
    break
    timeout 5 goto end
}

send "monitor e e m 3"
expect {
    "<SCUCLK> # "
    break
    timeout 5 goto end
}

sleep 5
send "clken e d"
expect {
    "<SCUCLK> # "
    break
    timeout 5 goto end
}

end:
