# A. Description
#    A list of commands here configures ADC and generates burst capture interrupt through mpwm0 trigger source.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. ADC
#    2. MPWM
#    3. PCU/GPIO
#
# D. Default Port
#    1. ADC0    : PA0 (AN0)
#               : PA1 (AN1)
#               : PA2 (AN2)
#    2. MPWM0   : PB0 (MPWM0UH)
#               : PB1 (MPWM0UL)
#
# E. Port Connection
#    1. PB0 (MPWM0UH) ----> PA0 (AN0)
#       PB1 (MPWMOUL) ----> PA1 (AN1)
#
# For more information, read a user's manual of the target device carefully.
#
# ADC0
# 1. Channel                      : 0
#
# 2. Clock                        : [ 0 p 64 ]
#    Source                       : p (Peripheral Clock)
#    Peripheral Clock Divide      : 64
#
# 3. Config                       : [ config 0 i b i 8 1 0 ]
#    Operation                    : i (Interrupt)
#    Mode                         : b (Burst)
#    Reference Level              : i (Internal VDD)
#    Sequence Count               : 8
#    Base Trigger Source          : 1 (Timer1)
#    Sampling Time                : 0
#
# 4. Seq                          : [ 0 0 0 5 0 ]
#    Sequence Number              : 0
#    Input Channel                : 0
#    Trigger Source               : 5 (MPWM0)
#    Trigger Source Number        : 0 (ATR0)
#
# MPWM0
# 1. Channel                      : 0 (MWPM0)
#
# 2. Clock                        : [ 0 m h 255 ]
#    Source                       : m (MCCR : Misc Clock)
#    MCCR Source                  : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide                : 255
#
# 3. Config                       : [ 0 p s u e e ]
#    Mode                         : p (Normal PWM)
#    Channel Mode                 : s (One Symmetric)
#    Counter Mode                 : u (Up Counter)
#    Period Match                 : e (Enable)
#    Bottom Match                 : e (Enable)
#
# 4. IRQ                          : [ 0 i u 15 ]
#    Operation                    : i (Interrupt)
#    Handler                      : u (Signal-U)
#    Priority                     : 15
#
# 5. PWM                          : [ 0 -ped 65535 -intr 0xc9 -sdp u 32767 32767 l l l l l l -apply ]
#    Period (-ped)                : 1024
#    Interrupt Enable (-intr)     : 0xc9 (0x1(UL) / 0x8(UH) / 0x40 (BU) / 0x80 (PU))
#    Signal Configuration (-sdp)
#      Signal                     : u (Signal-U)
#      High Duty                  : 32767
#      Low Duty                   : 32767
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#    Update Configuration (-apply) 
#
# 6. AdcTrg                       : [ 0 0 u u e e 32767 ]
#    Channel Number               : 0
#    Signal                       : u (Signal-U)
#    Match Counter Mode           : u (Up Counter Mode)
#    Interrupt                    : e (Enable)
#    Trigger Update Mode          : e (Update counter value to compare block after two PWM clock cycle)
#    Trigger Counter Value        : 32767 (Must be under Period value)
#
# 7. Start                        : [ 0 c 0x1 ]
#    Start Mode                   : c (recount)
#    Signal                       : 0x1 (PWM)
# 
# PCU (PBx)
# 1. Port Group                   : 1 (PCU Port B)
#
# 2. Port                         : [ 1 0 a 3 ] [ 1 1 a 3 ]
#    Pin Number                   : 0 / 1
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 3 (MP0UH / MP0UL)
#
# PCU (PB0)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 1 0 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# PCU (PB1)
send "port 1 1 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# MPWM0
send "cm mpwm"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "clk 0 m h 255"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "config 0 p s u e e"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "irq 0 i u 15"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -ped 65535 -intr 0xc9 -sdp u 32767 32767 l l l l l l -apply"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "adctrg 0 0 u u e e 32767"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "log on 480"
expect {
    "<MPWM> # "
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

send "config 0 i b i 8 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 0 0 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 1 1 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 2 2 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 3 3 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 4 4 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 5 5 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 6 6 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 7 7 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}


# MPWM0
send "cm mpwm"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "start 0 c 0x1" 
expect {
    "<MPWM> # "
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

sleep 5
send "dump 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
