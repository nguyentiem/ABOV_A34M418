# A. Description
#    A list of commands here configures ADC and generates multiple capture interrupt through mpwm0 trigger source.
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
#               : PA3 (AN3)
#               : PA4 (AN4)
#               : PA5 (AN5)
#    2. MPWM0   : PB0 (MPWM0UH)
#               : PB1 (MPWM0UL)
#               : PB2 (MPWM0VH)
#               : PB3 (MPWM0VL)
#               : PB4 (MPWM0WH)
#               : PB5 (MPWM0WL)
#
# E. Port Connection
#    1. PB0 (MPWM0UH) ----> PA0 (AN0)
#       PB2 (MPWMOVH) ----> PA2 (AN2)
#       PB4 (MPWMOWH) ----> PA4 (AN4)
#
# For more information, read a user's manual of the target device carefully.
#
# ADC0
# 1. Channel                      : 0
#
# 2. Clock                        : [ 0 m h 128 ]
#    Source                       : m (MCCR : Misc Clock)
#    MCCR Source                  : h (HSI : High Speed Internal Clock - 32MHz)
#    MCCR Source Divide           : 128
#
# 3. Config                       : [ config 0 i m i 8 3 0 ]
#    Operation                    : i (Interrupt)
#    Mode                         : m (Multiple)
#    Reference Level              : i (Internal VDD)
#    Sequence Count               : 8
#    Base Trigger Source          : 1 (Timer1)
#    Sampling Time                : 0
#
# 4. Seq                          : [ 0 0 0 5 0 ] [ 0 1 2 5 1 ] [ 0 2 4 5 2]
#    Sequence Number              : 0 / 1 / 2
#    Input Channel                : 0 / 2 / 4
#    Trigger Source               : 5 (MPWM0)
#    Trigger Source Number        : 0 / 1 / 2 (ATR0 / ATR1 / ATR2)
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
# 4. IRQ                          : [ 0 i u 15 ] [ 0 i v 15 ] [ 0 i w 15 ]
#    Operation                    : i (Interrupt)
#    Handler                      : u (Signal-U) / v (Signal-V) / w (Signal-W)
#    Priority                     : 15
#
# 5. PWM                          : [ 0 -ped 65535 -intr 0xff -sdp u 10922 10922 l l l l l l ]
#    Period (-ped)                : 65535
#    Interrupt Enable (-intr)     : 0xff (0x1(UL) / 0x2(VL) / 0x04(WL) / 0x8(UH) / 0x10(VH) / 0x20(WH) / 0x40 (BU) / 0x80 (PU) )
#    Signal Configuration (-sdp)
#      Signal                     : u (Signal-U)
#      High Duty                  : 10922
#      Low Duty                   : 10922
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
# 
# 6. PWM                          : [ 0 -sdp v 21844 21844 l l l l l l ]
#    Signal Configuration (-sdp)
#      Signal                     : v (Signal-V)
#      High Duty                  : 21844
#      Low Duty                   : 21844
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#
# 7. PWM                          : [ 0 -sdp w 43688 43688 l l l l l l -apply ]
#    Signal Configuration (-sdp)
#      Signal                     : w (Signal-W)
#      High Duty                  : 43688
#      Low Duty                   : 43688
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#    Update Configuration (-apply) 
#
# 8. AdcTrg                       : [ 0 0 u u e e 10992 ] [ 0 1 v u e e 21844 ] [ 0 2 w u e e 43688 ]
#    Channel Number               : 0 / 1 / 2
#    Signal                       : u (Signal-U) / v (Signal-V) / w (Signal-W)
#    Match Counter Mode           : u (Up Counter Mode)
#    Interrupt                    : e (Enable)
#    Trigger Update Mode          : e (Update counter value to compare block after two PWM clock cycle)
#    Trigger Counter Value        : 10992 / 21844 / 43688 (Must be under Period value)
#
# 9. Start                        : [ 0 c 0x1 ]
#    Start Mode                   : c (recount)
#    Signal                       : 0x1 (PWM)
#
# PCU (PAx) for ADC
# 1. Port Group                   : 0 (PCU Port A)
#
# 2. Port                         : [ 0 4 a 3 ]
#    Pin Number                   : 4
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 3 (AN4)
#
# PCU (PBx) for MPWM
# 1. Port Group                   : 1 (PCU Port B)
#
# 2. Port                         : [ 1 0 a 3 ] [ 1 2 a 3 ] [ 1 4 a 3 ]
#    Pin Number                   : 0 / 2 / 4
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 3 (MP0UH / MP0VH / MP0WH)
#
# PCU (PA4)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 0 4 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PB0)
send "port 1 0 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PB2)
send "port 1 2 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PB4)
send "port 1 4 a 3"
expect {
    "<PCU> # "
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

send "config 0 i m i 8 5 0"
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

send "seq 0 1 2 5 1"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 2 4 5 2"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 3 0 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 4 2 5 1"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 5 4 5 2"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 6 0 5 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

send "seq 0 7 2 5 1"
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

send "irq 0 i v 15"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "irq 0 i w 15"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -ped 65535 -intr 0xc9 -sdp u 10922 10922 l l l l l l"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -sdp v 21844 21844 l l l l l l"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -sdp w 43688 43688 l l l l l l -apply"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "adctrg 0 0 u u e e 10922"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "adctrg 0 1 v u e e 21844"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "adctrg 0 2 w u e e 43688"
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

sleep 7
send "dump 0"
expect {
    "<ADC> # "
    break
    timeout 5 goto end
}

end:
