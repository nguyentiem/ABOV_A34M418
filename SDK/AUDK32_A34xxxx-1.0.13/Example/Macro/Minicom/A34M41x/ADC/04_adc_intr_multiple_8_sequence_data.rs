# A. Description
#    A list of commands here configures ADC and generates multiple capture interrupt.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. ADC
#
# D. Default Port
#    1. ADC0    : PA0 (AN0)
#               : PA1 (AN1)
#               : PA2 (AN2)
#
# For more information, read a user's manual of the target device carefully.
#
# ADC0
# 1. Channel                    : 0
#
# 2. Clock                      : [ 0 m h 128 ]
#    Source                     : m (MCCR : Misc Clock)
#    MCCR Source                : h (HSI : High Speed Internal Clock - 32MHz)
#    MCCR Source Divide         : 128
#
# 3. Config                     : [ config 0 i m i 8 1 0 ]
#    Operation                  : i (Interrupt)
#    Mode                       : m (Multiple)
#    Reference Level            : i (Internal VDD)
#    Sequence Count             : 8
#    Base Trigger Source        : 1 (Timer1/ADC Instant Start)
#    Sampling Time              : 0
#
# 4. Seq                        : [ 0 0 0 a 0 ]
#    Sequence Number            : 0
#    Input Channel              : 0
#    Trigger Source             : a (ADC Instant Start)
#    Trigger Source Number      : 0 (Ignored)
#
# ADC0
send ""

send "cm adc"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "clk 0 m h 128"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "config 0 i m i 8 1 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 0 0 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 1 1 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 2 2 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 3 3 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 4 4 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 5 5 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 6 6 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 7 7 a 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "start 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

sleep 3
send "dump 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
