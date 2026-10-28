# A. Description
#    A list of commands here configures MPWM module and generates high signal-UVW and low signal-UVW.
#
# B. Preparation
#    Make an environment to measure signal-UVW high/low output by D.default output port.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. MPWM
#    2. PCU/GPIO
#
# D. Default Port
#    1. MPWM0 : PB0 (MPWM0UH)
#             : PB1 (MPWM0UL)
#             : PB2 (MPWM0VH)
#             : PB3 (MPWM0VL)
#             : PB4 (MPWM0WH)
#             : PB5 (MPWM0WL)
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
# 3. Config                       : [ 0 p s b e e ]
#    Mode                         : p (Normal PWM)
#    Channel Mode                 : s (One Symmetric)
#    Counter Mode                 : b (Up-Down Counter)
#    Period Match                 : e (Enable)
#    Bottom Match                 : e (Enable)
#
# 4. IRQ                          : [ 0 i u 15 ] [ 0 i v 15 ] [ 0 i w 15 ]
#    Operation                    : i (Interrupt)
#    Handler                      : u (Signal-U) / v (Signal-V) / w (Signal-W)
#    Priority                     : 15
#
# 5. PWM                          : [ 0 -ped 1024 -intr 0xff -sdp u 1024 1024 l l l l l l ]
#    Period (-ped)                : 1024
#    Interrupt Enable (-intr)     : 0xff (0x1(UL) / 0x2(VL) / 0x04(WL) / 0x8(UH) / 0x10(VH) / 0x20(WH) / 0x40 (BU) / 0x80 (PU) )
#    Signal Configuration (-sdp)
#      Signal                     : u (Signal-U)
#      High Duty                  : 1024
#      Low Duty                   : 1024
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
# 
# 6. PWM                          : [ 0 -sdp v 1024 1024 l l l l l l ]
#    Signal Configuration (-sdp)
#      Signal                     : v (Signal-V)
#      High Duty                  : 1024
#      Low Duty                   : 1024
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#
# 7. PWM                          : [ 0 -sdp w 1024 1024 l l l l l l -apply ]
#    Signal Configuration (-sdp)
#      Signal                     : w (Signal-W)
#      High Duty                  : 1024
#      Low Duty                   : 1024
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#    Update Configuration (-apply) 
#
# 8. Start                        : [ 0 c 0x1 ]
#    Start Mode                   : c (recount)
#    Signal                       : 0x1 (PWM)
# 
# 9. Duty                         : [ 0 m 512 1024 1024 512 1024 1024 ]
#    Signal                       : m (PWM)
#    U-High Duty                  : 512
#    U-Low Duty                   : 1024
#    V-High Duty                  : 1024
#    V-Low Duty                   : 512
#    W-High Duty                  : 1024
#    W-Low Duty                   : 1024
#
# PCU (PBx)
# 1. Port Group                   : 1 (PCU Port B)
#
# 2. Port                         : [ 1 0 a 3 ] [ 1 1 a 3 ] [ 1 2 a 3 ] [ 1 3 a 3 ] [ 1 4 a 3 ] [ 1 5 a 3 ]
#    Pin Number                   : 0 / 1 / 2 / 3 / 4 / 5
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 3 (MP0UH / MP0UL / MP0VH / MP0VL / MP0WH / MP0WL)
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

# PCU (PB2)
send "port 1 2 a 3"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PB3)
send "port 1 3 a 3"
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


# PCU (PB5)
send "port 1 5 a 3"
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

send "config 0 p s b e e"
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

send "pwm 0 -ped 1024 -intr 0xff -sdp u 1024 1024 l l l l l l"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -sdp v 1024 1024 l l l l l l"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -sdp w 1024 1024 l l l l l l -apply"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "log off"
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

set a 0
set b 50

loop:
inc a

# UH : on UL : off / VH : off VL : on / WH : off WL : off (Phase-UV)
send "duty 0 m 512 1024 1024 512 1024 1024"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

# UH : on UL : off / VH : off VL : off / WH : off WL : on (Phase-UW)
send "duty 0 m 512 1024 1024 1024 1024 512"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

# UH : off UL : off / VH : on VL : off / WH : off WL : on (Phase-VW)
send "duty 0 m 1024 1024 512 1024 1024 512"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

# UH : off UL : on / VH : on VL : off / WH : off WL : off (Phase-VU)
send "duty 0 m 1024 512 512 1024 1024 1024"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

# UH : off UL : on / VH : off VL : off / WH : on WL : off (Phase-WU)
send "duty 0 m 1024 512 1024 1024 512 1024"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

# UH : off UL : off / VH : off VL : on / WH : on WL : off (Phase-WV)
send "duty 0 m 1024 1024 1024 512 512 1024"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

if a < b goto loop

send "duty 0 m 1024 1024 1024 1024 1024 1024"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

end:
