# A. Description
#    A list of commands here configures QEI module and display a forward direction information.
#
# B. Preparation
#    Connecting target port with a suitable instrument.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. QEI
#    2. Timer1
#    3. PCU/GPIO
#
# D. Default Port
#    1. QEI     : PA13 (QEI0_A)
#               : PA14 (QEI0_B)
#               : PA15 (QEI0_IDX)
#    2. Timer10 : PC4 (T0IO)
#    3. Timer11 : PC5 (T1IO)
#
# E. Port Connection 
#    1. PC4 (T0IO) ----> PA13 (QEI0_A)
#       PC5 (T1IO) ----> PA14 (QEI0_B) 
#
# Please, read the User Manual of the specific chip for more details.
#
# QEI0
# 1. Channel                    : 0 (QEI0)
#
# 2. Config (cfg)                 : [ 0 n 0x02 q b m d 0xFFFFF ]
#    Operation                    : n (Non Maskable Interrupt)
#    Interrupt Enable             : 0x02 (Direction Change)
#    Signal Mode                  : q (Quadrature)
#    Capture Edge Mode            : b (Phase-A & Phase-B Edge)
#    Counter Reset                : m (Maximum Reset)
#    Index Count by Ph-A and Ph-B : d (disable)
#    Counter Maximum Value        : 0xFFFFF
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
# 3. Config                     : [ 0 i p l 320 320 -io o ]
#    Operation                  : i (Interrupt)
#    Mode                       : p (Period)
#    Output Port Polarity       : h (High)
#    A Data                     : 320
#    B Data                     : 320
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
# 3. Config                     : [ 1 i p l 320 320 -io o ]
#    Operation                  : i (Interrupt)
#    Mode                       : p (Period)
#    Output Port Polarity       : h (High)
#    A Data                     : 320
#    B Data                     : 320
#    Port In/Out (-io)          : o (Output)
#
# PCU (PAx)
# 1. Port Group                   : 0 (PCU Port A)
#
# 2. Port                         : [ 0 13 a 2 ] [ 0 14 a 2 ] [ 0 15 a 2 ]
#    Pin Number                   : 13 / 14 / 15
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 2 (QEI0_A / QEI0_B / QEI0_IDX)
#
# PCU (PCx)
# 1. Port Group                   : 2 (PCU Port C)
#
# 2. Port                         : [ 2 4 a 2 ] [ 2 5 a 2 ]
#    Pin Number                   : 4 / 5
#    Pin Mode                     : a (Alternative)
#    Alternative                  : 2 (T0IO / T1IO)
#

# PCU (PA13)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 0 13 a 2 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# PCU (PA14)
send "port 0 14 a 2 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# PCU (PA15)
send "port 0 15 a 2 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# PCU (PC4)
send "port 2 4 a 2"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# PCU (PC5)
send "port 2 5 a 2"
expect {
    "<PCU> # "
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

send "config 0 i p l 320 320 -io o" 
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

send "config 1 i p l 320 320 -io o" 
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

send "start 1"
expect {
    "<TIMER1> # "
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

send "cfg 0 n 0x02 q b m d 0xFFFFF" 
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

set a 0
set b 10

loop:
inc a
send "info 0"
expect {
    "<QEI> # "
    break
    timeout 5 goto end
}
sleep 1

if a < b goto loop

end:
