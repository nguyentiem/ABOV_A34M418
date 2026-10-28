# A. Description
#    A list of commands here configures ADC and generates non-maskable multiple capture interrupt through timer1 trigger sources.
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
# 1. Channel                   : 0
#
# 2. Clock                     : [ 0 m h 128 ]
#    Source                    : m (MCCR : Misc Clock)
#    MCCR Source               : h (HSI : High Speed Internal Clock - 32MHz)
#    MCCR Source Divide        : 128
#
# 3. Config                    : [ config 0 n m i 8 3 0 ]
#    Operation                 : n (Non Maskable Interrupt)
#    Mode                      : m (Multiple)
#    Reference Level           : i (Internal VDD)
#    Sequence Count            : 8
#    Base Trigger Source       : 1 (Timer1)
#    Sampling Time             : 0
#
# 4. Seq                       : [ 0 0 0 1 0 ]
#    Sequence Number           : 0
#    Input Channel             : 0
#    Trigger Source            : 1 (Timer1)
#    Trigger Source Number     : 0 (Timer10)
#
# Timer1n
# 1. Channel                   : 0 / 1 / 2 (Timer1n)
#
# 2. Clock                     : [ 0 m h 10 1000 ] [ 1 m h 10 1000 ] [ 2 m h 10 1000 ]
#    Source                    : m (MCCR : Misc Clock)
#    MCCR Source               : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide             : 10
#    Timer1 Pre-Divide         : 1000 (MCCR Clock / 10) / 1000
#
# 3. Config                    : [ 0 i p h 320 320 -io o ] [ 1 i p h 960 960 -io o ] [ 2 i p h 640 640 -io o ]
#    Operation                 : i (Interrupt)
#    Mode                      : p (Period)
#    Output Port Polarity      : h (High)
#    A Data                    : 320/960/640
#    B Data                    : 320/960/640
#    Port In/Out (-io)         : o (Output)
#
# 4. Adc Trigger Config        : [ 0 e n ] [ 1 e n ] [ 2 e n ]
#    Enable                    : e (Enable)
#    Mode                      : n (Normal)
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

send "config 0 i p h 320 320" 
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "adctrgcfg 0 e n"
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

# Timer11
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

send "config 1 i p h 960 960" 
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

# Timer12
send "uninit 2"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "init 2"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "clk 2 m h 10 1000"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "config 2 i p h 640 640" 
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

send "adctrgcfg 2 e n"
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

send "config 0 n m i 8 1 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 0 0 1 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 1 1 1 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 2 2 1 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 3 3 1 1"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 4 4 1 1"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 5 5 1 1"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 6 6 1 2"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 7 7 1 2"
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


# Timer12
send "start 2"
expect {
    "<TIMER1> # "
    break
    timeout 5 goto end
}

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

sleep 3 
send "dump 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
