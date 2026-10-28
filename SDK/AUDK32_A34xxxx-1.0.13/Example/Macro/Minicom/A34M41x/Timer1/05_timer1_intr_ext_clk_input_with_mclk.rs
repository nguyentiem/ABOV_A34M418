# A. Description
#    A list of commands here configures timer1 and generates a periodic interrupt based on external clock source.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. Timer1
#    2. SCUCLK
#    3. PCU/GPIO
#
# D. Default Port
#    1. Timer10 : PA4 (Output/Capture Input/External Clock Input)
#    2. PCU     : PC9 (Clock Output)
#
# E. Port Connection
#    1. PC9 (Clock Output) ----> PA4 (Timer10 External Clock Input)
#
# For more information, read a user's manual of the target device carefully.
#
# Timer10
# 1. Channel                   : 0 (Timer10)
#
# 2. Clock                     : [ 0 e f 64 ]
#    Source                    : e (External Clock Input) 
#    External Clock Input Edge : f (Falling) 
#    Timer1 Pre-Divide         : 64
#
# 3. Config                    : [ 0 i p h 64000 64000 -io i ]
#    Operation                 : i (Interrupt)
#    Mode                      : p (Periodic)
#    Output Port Polarity      : h (High)
#    A Data                    : 64000
#    B Data                    : 64000
#
# PCU (PC9)
# 1. Port Group                : 2 (PCU Port C)
#
# 2. Port                      : [ 2 9 a 3 ]
#    Pin Number                : 9
#    Pin Mode                  : a (Alternative)
#    Alternative               : 3 (Clock Output)
#
# SCUCLK
# 1. Clock Out                 : [ clkout m 15 e ]  
#    Clock Source              : m (MCLK - HSI 32MHz)
#    Clock Source Divide       : 15 (Clock Source / (2 * (Divide + 1))
#    Clock Output Enable       : e (Enable)
#
# PCU (PC9)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 2 9 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# SCUCLK
send "cm scuclk"
expect {
    "<SCUCLK> # "
    break
    timeout 5 goto end
}

send "clkout m 15 e"
expect {
    "<SCUCLK> # "
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

send "clk 0 e f 64"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "config 0 i p h 64000 64000 -io i"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "log on"
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
