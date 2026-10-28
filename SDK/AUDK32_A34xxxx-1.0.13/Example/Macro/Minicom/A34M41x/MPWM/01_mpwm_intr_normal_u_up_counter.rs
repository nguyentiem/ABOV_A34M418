# A. Description
#    A list of commands here configures MPWM module and generates high signal-U and low signal-U.
#
# B. Preparation
#    Make an environment to measure signal-U high/low output by D.default output port.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. MPWM
#    2. PCU/GPIO
#
# D. Default Port
#    1. MPWM0 : PB0 (MPWM0UH)
#             : PB1 (MPWM0UL)
#
# Please, read the User Manual of the specific chip for more details.
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
# 5. PWM                          : [ 0 -ped 1024 -intr 0xc9 -sdp u 512 1024 l l l l l l -apply ]
#    Period (-ped)                : 1024
#    Interrupt Enable (-intr)     : 0xc9 (0x1(UL) / 0x8(UH) / 0x40 (BU) / 0x80 (PU))
#    Signal Configuration (-sdp)
#      Signal                     : u (Signal-U)
#      High Duty                  : 512
#      Low Duty                   : 1024
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#    Update Configuration (-apply) 
#
# 6. Start                        : [ 0 c 0x1 ]
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

send "pwm 0 -ped 1024 -intr 0xc9 -sdp u 512 1024 l l l l l l -apply"
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

send "start 0 c 0x1" 
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

end:
