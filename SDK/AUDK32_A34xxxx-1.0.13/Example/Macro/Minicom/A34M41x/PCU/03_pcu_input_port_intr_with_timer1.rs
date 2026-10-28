# A. Description
#    A list of commands here configures PCU module and generates input interrupt via the signal from timer1.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. PCU/GPIO
#    2. Timer1
#
# D. Default Port
#    1. PCU     : PA0 (Input)
#                 PA1 (Input)
#                 PB2 (Input)
#    2. Timer10 : PA4 (Output/Capture Input/External Clock Input)
#
# E. Port Connection
#    1. PA4 (Timer10 Output) ----> PA0 (Input)
#                            ----> PA1 (Input)
#                            ----> PB2 (Input)
#
# For more information, read a user's manual of the target device carefully.
#
# PCU (PA0/PA1/PB2)
# 1. Port Group                   : 0 (PCU Port A)
#                                 : 1 (PCU Port B)
#
# 2. Port                         : [ 0 0 i i -intr i e r ] [ 0 1 i i -intr i e r ] [ 1 2 i i -intr i e r ]
#    Pin Number                   : 0 / 1 / 2
#    Pin Mode                     : i (Input)
#    Pin Input Mode               : i (Normal Input)
#    Pin Operation Mode           : i (Interrupt)
#    Pin Interrupt Mode           : e (Edge)
#    Pin Interrupt Trigger Edge   : r (Rising)
#
# Timer10
# 1. Channel                      : 0 (Timer10)
#
# 2. Clock                        : [ 0 m h 10 1000 ]
#    Source                       : m (MCCR : Misc Clock)
#    MCCR Source                  : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide                : 10 
#    Timer1 Pre-Divide            : 1000 (MCCR Clock / 10) / 1000
#
# 3. Config                       : [ 0 i p h 3200 3200 -io o ]
#    Operation                    : i (Interrupt)
#    Mode                         : p (Period)
#    Output Port Polarity         : h (High)
#    A Data                       : 3200
#    B Data                       : 3200
#    Port In/Out (-io)            : o (Output)
#
# PCU (PA0)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 0 0 i i -intr i e r"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PA1)
send "port 0 1 i i -intr i e r"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PB2)
send "port 1 2 i i -intr i e r"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# Timer10 
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
send "clk 0 m h 10 1000"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}
send "config 0 i p h 3200 3200 -io o"
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

end:
