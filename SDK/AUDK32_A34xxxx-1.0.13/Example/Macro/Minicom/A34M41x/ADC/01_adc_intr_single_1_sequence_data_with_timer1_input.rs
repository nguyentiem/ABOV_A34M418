# A. Description
#    A list of commands here configures ADC and generates single capture interrupt.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. ADC
#    2. Timer1
#
# D. Default Port
#    1. ADC0    : PA0 (AN0)
#               : PA1 (AN1)
#               : PA2 (AN2)
#    2. Timer10 : PA4 (Output/Capture Input/External Clock Input)
#
# E. Port Connection
#    1. PA4 (Timer10 Output) ----> PA0 (AN0)
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
# 3. Config                     : [ config 0 i s i 1 a 0 ]
#    Operation                  : i (Interrupt)
#    Mode                       : s (Single)
#    Reference Level            : i (Internal VDD)
#    Sequence Count             : 1
#    Base Trigger Source        : a (ADC Instant Start)
#    Sampling Time              : 0
#
# 4. Seq                        : [ 0 0 0 a 0 ]
#    Sequence Number            : 0
#    Input Channel              : 0
#    Trigger Source             : a (ADC Instant Start)
#    Trigger Source Number      : 0 (Ignored)
#
# Timer10
# 1. Channel                    : 0 (Timer10)
#
# 2. Clock                      : [ 0 m h 4 64 -io o ]
#    Source                     : m (MCCR : Misc Clock)
#    MCCR Source                : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide              : 4 
#    Timer1 Pre-Divide          : 64 (MCCR Clock / 4) / 64
#
# 3. Config                     : [ 0 i p h 13 13 ]
#    Operation                  : i (Interrupt)
#    Mode                       : p (Period)
#    Output Port Polarity       : h (High)
#    A Data                     : 13
#    B Data                     : 13
#    Port In/Out (-io)          : o (Output)
#
# Timer10
send ""

send "cm timer1"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "clk 0 m h 4 64"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "config 0 i p h 13 13" 
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "log off"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "start 0"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}


# ADC0
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

send "config 0 i s i 1 a 0"
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

send "start 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

sleep 2
send "dump 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
