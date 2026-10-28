# A. Description
#    A list of commands here configures ADC and generates single capture and comparator matched interrupt by Timer10 output signal.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
# 
# C. Prerequisite Example (abov_example_config.h)
#    1. ADC
#    2. Timer1
#    3. PCU/GPIO
#
# D. Default Port
#    1. ADC0    : PA0 (AN0)
#               : PA1 (AN1)
#               : PA2 (AN2)
#    2. Timer10 : PA4 (Output/Capture Input/External Clock Input)
#
# E. Port Connection
#    1. PA4 (Timer10 Output) ---> PA0 (AN0)
#
# For more information, read a user's manual of the target device carefully.
#
# ADC0
# 1. Channel                    : 0
#
# 2. Clock                      : [ 0 p 64 ]
#    Source                     : p (Periperhal Clock)
#    Peripheral Clock Divide    : 64
#
# 3. Config                     : [ config 0 i s i 1 1 0 ]
#    Operation                  : i (Interrupt)
#    Mode                       : s (Single)
#    Reference Level            : i (Internal VDD)
#    Sequence Count             : 1
#    Base Trigger Source        : 1 (Timer1)
#    Sampling Time              : 0
#
# 4. Seq                        : [ 0 0 0 1 1 ]
#    Sequence Number            : 0
#    Input Channel              : 0
#    Trigger Source             : 1 (Timer1)
#    Trigger Source Number      : 1 (Timer11)
#
# 5. Cmp                        : [ 0 e 0 -intr e u ]
#    Comparator Enable          : e (Enable)
#    Input Channel              : 0 
#    Comparator Data            : 1024
#    Interrupt Enable           : e (Enable)
#    Interrupt Trigger          : u (Input Channel Sampling Data > Comparator Data)
#
# Timer10
# 1. Channel                    : 0 (Timer10)
#
# 2. Clock                      : [ 0 m h 10 1000 ]
#    Source                     : m (MCCR : Misc Clock)
#    MCCR Source                : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide              : 10
#    Timer1 Pre-Divide          : 1000 (MCCR Clock / 10) / 1000
#
# 3. Config                     : [ 0 i p h 1600 1600 -io o ]
#    Operation                  : i (Interrupt)
#    Mode                       : p (Period)
#    Output Port Polarity       : h (High)
#    A Data                     : 1600
#    B Data                     : 1600
#    Port In/Out (-io)          : o (Output)
#
# Timer11
# 1. Channel                    : 1 (Timer11)
#
# 2. Clock                      : [ 1 m h 10 1000 ]
#    Source                     : m (MCCR : Misc Clock)
#    MCCR Source                : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide              : 10
#    Timer1 Pre-Divide          : 1000 (MCCR Clock / 10) / 1000
#
# 3. Config                     : [ 1 i p h 1600 1600 ]
#    Operation                  : i (Interrupt)
#    Mode                       : p (Period)
#    Output Port Polarity       : h (High)
#    A Data                     : 1600
#    B Data                     : 1600

# 4. Adc Trigger Config         : [ 1 e n ]
#    Enable                     : e (Enable)
#    Mode                       : n (Normal)
#

# Timer10
send ""

send "cm timer1"
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

send "config 0 i p h 1600 1600"
expect {
    "<TIMER1> # "
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

send "log off"
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

send "config 1 i p h 1600 1600"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "adctrgcfg 1 e n"
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

send "clk 0 p 64"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "config 0 i s i 1 1 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 0 0 1 1"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "cmp 0 e 0 1024 -intr e u"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "log on"
expect {
    "<ADC> # "
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

send "start 0"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

# Timer11
send "start 1"
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

send "start 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
