# A. Description
#    A list of commands here configures timer1 and generates a capture interrupt.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. Timer1
#    2. PCU/GPIO
#
# D. Default Port
#    1. Timer10 : PA4 (Output/Capture Input/External Clock Input)
#    2. Timer11 : PC5 (Output/Capture Input/External Clock Input)
#
# E. Port Connection
#    1. PC5 (Timer11 Output) ----> PA4 (Timer10 Capture Input)
#
# For more information, read a user's manual of the target device carefully.
#
# Timer10
# 1. Channel                   : 0 (Timer10)
#
# 2. Clock                     : [ 0 m h 10 1000 ]
#    Source                    : m (MCCR : Misc Clock)
#    MCCR Source               : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide             : 10 
#    Timer1 Pre-Divide         : 1000 (MCCR Clock / 10) / 1000
#
# 3. Config                    : [ 0 i c b h 0 0 ]
#    Operation                 : i (Interrupt)
#    Mode                      : c (Capture)
#    Capture Egde              : b (Both)
#    Output Port Polarity      : h (High)
#    A Data                    : 0
#    B Data                    : 0
#
# Timer11
# 1. Channel                   : 1 (Timer11)
#
# 2. Clock                     : [ 1 m h 10 1000 ]
#    Source                    : m (MCCR : Misc Clock)
#    MCCR Source               : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide             : 10 
#    Timer1 Pre-Divide         : 1000 (MCCR Clock / 10) / 1000
#
# 3. Config                    : [ 1 i p h 3200 3200 -io o ]
#    Operation                 : i (Interrupt)
#    Mode                      : p (Period)
#    Output Port Polarity      : h (High)
#    A Data                    : 3200
#    B Data                    : 3200
#    Port In/Out (-io)         : o (Output)
#
# PCU (PC5) for Timer11
# 1. Port Group                   : 2 (PCU Port C)
#
# 2. Port                         : [ 2 5 a 2 ]
#    Pin Number                   : 5
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 2 (T1IO)

# PCU (PC5)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 2 5 a 2"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# Timer11
send "cm timer1"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "uninit 1"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "init 1"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "clk 1 m h 10 1000"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "config 1 i p h 3200 3200 -io o"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "start 1"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}


# Timer10
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

send "config 0 i c b h 0 0"
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

