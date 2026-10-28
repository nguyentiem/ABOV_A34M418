# A. Description
#    A list of commands here configures CMP and generates non-maskable both edge detected interrupt.
#
# B. Preparation
#    Supply reference and source signal to specific port by a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. CMP
#    2. Timer1
#    3. PCU/GPIO
#
# D. Default Port
#    1. CMP0    : PA4 (CP0A)
#               : PA7 (CREF0)
#    2. Timer11 : PC5 (Output/Capture Input/External Clock Input)
#
# E. Port Connection
#    1. PC5 (Timer11 Output) ----> PA4 (CP0A)
#
# For more information, read a user's manual of the target device carefully.
#
# CMP0
# 1. Config                     : [ 0 n 0 0 h b -dbc 0 15 0 ]
#    Operation                  : n (Non Maskable Interrupt)
#    Source                     : 0 (CP0A)
#    Reference                  : 0 (CREF0)
#    Hysteresis                 : h (High)
#    Trigger Mode               : b (both edge)
#    Debounce/Noise Filter (-dbc)     
#    Clock Divide               : 0 (No Clock Divider available so default 0)
#    Debounce Count             : 15
#    LSI Clock Enable           : d (No LSI Clock available so default d)
#
# Timer11
# 1. Channel                   : 1 (Timer11)
#
# 2. Clock                     : [ 0 m h 10 1000 ]
#    Source                    : m ( MCCR : Misc Clock )
#    MCCR Source               : h ( HSI : High Speed Internal Clock - 32MHz )
#    Source Divide             : 10 
#    Timer1 Pre-Divide         : 1000 ( MCCR Clock / 10 ) / 1000
#
# 3. Config                    : [ 0 i p h 3200 3200 -io o ]
#    Operation                 : i (Interrupt)
#    Mode                      : p (Period)
#    Output Port Polarity      : h (High)
#    A Data                    : 3200
#    B Data                    : 3200
#    Port In/Out (-io)         : o (Output)
#
# PCU (PC5)
# 1. Port Group                : 2 (PCU Port C)
#
# 2. Port                      : [ 2 5 a 2 ]
#    Pin Number                : 5
#    Pin Mode                  : a (Alternative)
#    Alternative               : 2 (T1IO)
#

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

send "config 1 i p h 3200 3200"
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

send "start 1"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}


# CMP0
send "cm cmp"
expect {
    "<CMP> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<CMP> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<CMP> # "
    break
    timeout 5 goto end
}

send "config 0 n 0 0 h b -dbc 0 15 d"
expect {
    "<CMP> # "
    break
    timeout 5 goto end
}

send "log on 0"
expect {
    "<CMP> # "
    break
    timeout 5 goto end
}

send "start 0"
expect {
    "<CMP> # "
    break
    timeout 5 goto end
}

end:
