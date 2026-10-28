# A. Description
#    A list of commands here configures timer1 and generates a oneshot interrupt.
#
# B. Preparation
#    N/A
#
# C. Prerequisite Example (abov_example_config.h)
#    1. Timer1
#
# D. Default Port
#    1. Timer10 : PA4 (Output/Capture Input/External Clock Input)
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
# 3. Config                    : [ 0 i o h 3200 3200 -io o ]
#    Operation                 : i (Interrupt)
#    Mode                      : o (One-Shot) 
#    Output Port Polarity      : h (High)
#    A Data                    : 3200
#    B Data                    : 3200
#    Port In/Out (-io)         : o (Output)
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

send "clk 0 m h 10 1000"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "config 0 i o h 3200 3200 -io o"
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
