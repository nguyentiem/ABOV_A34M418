# A. Description
#    A list of commands here configures QEI module and generates index pulse interrupt.
#
# B. Preparation
#    Make an environment with Infine IFX007T Motor Control Shield board, AMT103 Modular Incremental Encoder and BLDC Motor.
#    - H/W Information
#      AMT103 : https://www.cuidevices.com/product/motion/rotary-encoders/incremental/modular/amt103-v 
#      IFX007T Shield Board : https://www.infineon.com/cms/en/product/evaluation-boards/bldc-shield_ifx007t/  
#      BLDC Motor : https://www.dnj.co.kr/bbs/board.php?bo_table=bldc&wr_id=6
#
# C. Prerequisite Example (abov_example_config.h)
#    1. QEI
#    2. MPWM
#    3. PCU/GPIO
#
# D. Default Port
#    1. QEI : PA13 (QEI0_A)
#           : PA14 (QEI0_B)
#           : PA15 (QEI0_IDX)
#
# E. Port Connection
#    1. PA13 (QEIO_A)   ---> AMT10 B Channel
#       PA14 (QEIO_B)   ---> AMT10 A Channel 
#       PA15 (QEIO_IDX) ---> AMT10 Index Channel
#
# F. Note
#    Should execute the MPWM example script "14_mpwm_intr_normal_uvw_up_counter_bldc_cw_hall_sensor_IFX007T.ttl" at MPWM directory.
#
# Please, read the User Manual of the specific chip for more details.
#
# QEI0
# 1. Channel                      : 0 (QEI0)
#
# 2. Config (cfg)                 : [ 0 i 0x01 q b m d 0xFFFFF ]
#    Operation                    : i (Interrupt)
#    Interrupt Enable             : 0x01 (Index Pulse Rising Edge)
#    Signal Mode                  : q (Quadrature)
#    Capture Edge Mode            : b (Phase-A & Phase-B Edge)
#    Counter Reset                : m (Maximum Reset)
#    Index Count by Ph-A and Ph-B : d (disable)
#    Counter Maximum Value        : 0xFFFFF
#
# PCU (PAx)
# 1. Port Group                   : 0 (PCU Port A)
#
# 2. Port                         : [ 0 13 a 2 ] [ 0 14 a 2 ] [ 0 15 a 2 ]
#    Pin Number                   : 13 / 14 / 15
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 2 (QEI0_A / QEI0_B / QEI0_IDX)
#

# PCU (PA13)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 0 13 a 2"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PA14)
send "port 0 14 a 2"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PA15)
send "port 0 15 a 2"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# QEI0
send "cm qei"
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}

send "cfg 0 i 0x01 q b m d 0xFFFFF" 
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}

send "log on 0"
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}

send "start 0" 
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}

end:
