# A. Description
#    A list of commands here configures MPWM module and generates high signal-UVW and low signal-UVW.
#
# B. Preparation
#    Make an environment with Infine IFX007T Motor Control Shield board and BLDC Motor.
#    - H/W Information
#      IFX007T Shield Board : https://www.infineon.com/cms/en/product/evaluation-boards/bldc-shield_ifx007t/  
#      BLDC Motor : https://www.dnj.co.kr/bbs/board.php?bo_table=bldc&wr_id=6
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
# E. Port Connection
#    1. MPWM0 : PB0 (MPWM0UH) ---> INU  (IFX007T D11)
#               PB1 (MPWM0UL) ---> INHU (IFX007T D6)
#               PB2 (MPWM0VH) ---> INV  (IFX007T D10)
#               PB3 (MPWM0VL) ---> INHV (IFX007T D5)
#               PB4 (MPWM0WH) ---> INW  (IFX007T D9)
#               PB5 (MPWM0WL) ---> INHW (IFX007T D3)
#    2. GPIO  : BLDC Hall A ---> PA4 (Input)
#               BLDC Hall B ---> PA3 (Input)
#               BLDC Hall C ---> PA2 (Input)
#
# F. Note
#    Should apply 20V to IFX007T Board by Power Supply.
#
# Please, read the User Manual of the specific chip for more details.
#
# MPWM0
# 1. Channel                      : 0 (MWPM0)
#
# 2. Clock                        : [ 0 m h 5 ]
#    Source                       : m (MCCR : Misc Clock)
#    MCCR Source                  : h (HSI : High Speed Internal Clock - 32MHz)
#    Source Divide                : 5
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
# 5. PWM                          : [ 0 -ped 640 -intr 0xff -sdp u 0 0 l l l l l l ]
#    Period (-ped)                : 640 (PWM Frequency : 10KHz)
#    Interrupt Enable (-intr)     : 0xff (0x1(UL) / 0x2(VL) / 0x04(WL) / 0x8(UH) / 0x10(VH) / 0x20(WH) / 0x40 (BU) / 0x80 (PU) )
#    Signal Configuration (-sdp)
#      Signal                     : u (Signal-U)
#      High Duty                  : 0 
#      Low Duty                   : 0
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
# 
# 6. PWM                          : [ 0 -sdp v 0 l l l l l l ]
#    Signal Configuration (-sdp)
#      Signal                     : v (Signal-V)
#      High Duty                  : 0
#      Low Duty                   : 0
#      High Output Level          : l (Low)
#      High Start Level           : l (Low)
#      High Force Level           : l (Low)
#      Low Output Level           : l (Low)
#      Low Start Level            : l (Low)
#      Low Force Level            : l (Low)
#
# 7. PWM                          : [ 0 -sdp w 0 0 l l l l l l -apply ]
#    Signal Configuration (-sdp)
#      Signal                     : w (Signal-W)
#      High Duty                  : 0
#      Low Duty                   : 0
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
# 9. BLDC                              : [ 0 -ref e -mode h -hall 0 2 0 3 0 4 -dir b -duty 600 640 600 640 600 640 0 640 ]
#    Reference Board Enable (-ref)     : e (Enable)
#    Mode (-mode)                      : h (Hall Sensor Commutation)
#    Hall Sensor Port (-hall)
#      C port                          : 0 (Group A) / 2 (Pin Number)
#      B port                          : 0 (Group A) / 3 (Pin Number)
#      A port                          : 0 (Group A) / 4 (Pin Number)
#    Direction (-dir)                  : b (Forward : Clockwise)
#    UVW Duty (-duty)
#      U-High Duty                     : 600
#      U-Low Duty                      : 640
#      V-High Duty                     : 600
#      V-Low Duty                      : 640
#      W-High Duty                     : 600
#      W-Low Duty                      : 640
#      IFX007T On                      : 0 (Must enable -ref option)
#      IFX007T Off                     : 640 (Must enable -ref option)
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

send "clk 0 m h 5"
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

send "pwm 0 -ped 640 -intr 0x1ff -sdp u 0 0 l l l l l l"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -sdp v 0 0 l l l l l l"
expect {
    "<MPWM> # "
    break
    timeout 5 goto end
}

send "pwm 0 -sdp w 0 0 l l l l l l -apply"
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

send "bldc 0 -ref e -mode h -hall 0 2 0 3 0 4 -dir b -duty 600 640 600 640 600 640 0 640"
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
