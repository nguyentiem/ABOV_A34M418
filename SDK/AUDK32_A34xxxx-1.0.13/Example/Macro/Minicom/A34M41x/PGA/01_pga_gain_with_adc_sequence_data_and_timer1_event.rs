# A. Description
#    A list of commands here configures PGA and generates conversion results of ADC channel 0 and 1.
#
# B. Preparation
#    Connecting target port with a suitable instrument. (such as DAC)
#
# C. Prerequisite Example (abov_example_config.h)
#    1. PGA
#    2. ADC
#    3. Timer1
#
# D. Default Port
#    1. ADC0    : PA0 (AN0)
#
# E. Port Connection 
#    1. Supply Power + ----> PA0 (AN0)
#
# F. Note
#    Should make analog input level to 1V.
#
# For more information, read a user's manual of the target device carefully.
#
# PGA0
# 1. Channel                    : 0
#
# 2. Amplifier                  : [ amp 0 e 0 ]
#    Enable                     : e (Enable)
#    Output Driving Current     : 0 (0%)
#
# 3. Gain                       : [ gain 0 0 ]
#    Enable                     : e (Enable)
#    Gain value                 : 0 (1.200)
#
# ADC0
# 1. Channel                    : 0
#
# 2. Clock                      : [ 0 p 64 ]
#    Source                     : p (Periperhal Clock)
#    Peripheral Clock Divide    : 64
#
# 3. Config                     : [ config 0 i b i 2 1 0 ]
#    Operation                  : i (Interrupt)
#    Mode                       : q (Sequence)
#    Reference Level            : i (Internal VDD)
#    Sequence Count             : 2
#    Base Trigger Source        : 1 (Timer1)
#    Sampling Time              : 0
#
# 4. Seq                        : [ 0 0 0 1 0 ] [ 0 1 1 1 0 ]
#    Sequence Number            : 0
#    Input Channel              : 0 / 1
#    Trigger Source             : 1 (Timer1)
#    Trigger Source Number      : 0 (Timer10)
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
# 3. Config                     : [ 0 i p h 3200 3200 ]
#    Operation                  : i (Interrupt)
#    Mode                       : p (Period)
#    Output Port Polarity       : h (High)
#    A Data                     : 3200
#    B Data                     : 3200

# 4. Adc Trigger Config         : [ 0 e n ]
#    Enable                     : e (Enable)
#    Mode                       : n (Normal)
#

# PGA0
send ""

send "cm pga"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}

send "amp 0 e 0"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}

send "gain 0 0"
expect {
    "<PGA> # "
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

send "config 0 i q i 2 1 0"
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

send "config 0 i p h 3200 3200"
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

send "start 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}


# PGA0
send "cm pga"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}

send "gain 0 18"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 17"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 16"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 15"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 14"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 13"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 12"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 11"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 10"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 9"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 8"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 7"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 6"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 5"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 4"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 3"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 2"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 1"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "gain 0 0"
expect {
    "<PGA> # "
    break
    timeout 5 goto end
}
sleep 1

send "cm adc"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "stop 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
