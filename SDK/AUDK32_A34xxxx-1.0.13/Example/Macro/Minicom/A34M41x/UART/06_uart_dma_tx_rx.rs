# A. Description
#    A list of commands here configures UART, and transmits and receives data via UART.
#
# B. Preparation
#    Connecting ports with a suitable instrument should be correct
#
# C. Prerequisite Example (abov_example_config.h)
#    1. UART
#    2. PCU/GPIO
#
# D. Default Port
#    1. UART1   : PC6 (Transmit)
#               : PC5 (Receive)
#    2. UART2   : PD6 (Transmit)
#               : PD7 (Receive)
#
# E. Port Connection
#    1. PC6 (UART1 Transmit) <----> PD7 (UART2 Receive)
#    2. PC5 (UART1 Receive)  <----> PD6 (UART2 Transmit)
#
# For more information, read a user's manual of the target device carefully.
#
# UART1
# 1. Channel                   : 1 (UART1)
#
# 2. CLK                       : [ 1 p ]
#    Source                    : m (MCCR : Misc Clock)
#    MCCR Source               : m (MCLK : Main Clock)
#    Source Divide             : 2
#
# 3. Config                    : [ 1 d 8 o 1 38400 ]
#    Operation                 : d (DMA)
#    Data Bit                  : 8 (8bit)
#    Parity                    : o (odd)
#    Stop Bit                  : 1 (1bit)
#    Baudrate                  : 38400 bps
#
# 4. Data                      : [ 1 8 0xa5 0x5a 0xa5 0x5a 0x05 0x07 0x09 0xa0 ]
#    Data Length               : 8
#    Data                      : 0xa5 ... (Hexa and space (delimitor))
#
# 5. Tx                        : [ 1 8 ]
#    Receive Data Length       : 8
#
# UART2
# 1. Channel                   : 2 (UART2)
#
# 2. CLK                       : [ 2 m m 2 ]
#    Source                    : m (MCCR : Misc Clock)
#    MCCR Source               : m (MCLK : Main Clock)
#    Source Divide             : 2
#
# 3. Config                    : [ 2 d 8 o 1 38400 ]
#    Operation                 : d (DMA)
#    Data Bit                  : 8 (8bit)
#    Parity                    : o (odd)
#    Stop Bit                  : 1 (1bit)
#    Baudrate                  : 38400 bps
#
# 4. Rx                        : [ 2 8 ]
#    Receive Data Length       : 8
#
#
# PCU (PCx)
# 1. Port Group                : 2 (PCU Port C)
#
# 2. Port                      : [ 2 6 a 1 -pupd p ] [ 2 5 a 1 -pupd p ]
#    Pin Number                : 6 / 5 
#    Pin Mode                  : a (Alternative)
#    Alternative               : 1 (TX/RX)
#    Pull-up/down              : p (Pull-up)
#
# PCU (PDx)
# 1. Port Group                : 3 (PCU Port D)
#
# 2. Port                      : [ 3 6 a 1 -pupd p ] [ 3 7 a 1 -pupd p ]
#    Pin Number                : 6 / 7 
#    Pin Mode                  : a (Alternative)
#    Alternative               : 1 (TX/RX)
#    Pull-up/down              : p (Pull-up)
#
# PCU (PCx)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 2 6 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 2 5 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# PCU (PDx)
send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 6 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 7 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# UART2
send "cm uart"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "uninit 2"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "init 2"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "clk 2 m m 2"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "config 2 d 8 o 1 38400"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "rx 2 8"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

# UART1

send "uninit 1"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "init 1"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "clk 1 m m 2"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "config 1 d 8 o 1 38400"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "data 1 8 0xa5 0x5a 0xa5 0x5a 0x05 0x07 0x09 0xa0"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

send "tx 1 8"
expect {
    "<UART> # "
    break
    timeout 5 goto end
}

end:
