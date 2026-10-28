# A. Description
#    A list of commands here configures SPI module and receives data via SPI.
#
# B. Preparation
#    Connecting ports with an external SPI device should be correct.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. SPI
#    2. PCU/GPIO
#
# D. Default Port
#    1. SPI0    : PA12 (Slave Select)
#               : PA13 (Serial Clock)
#               : PA14 (Master Out Slave In)
#               : PA15 (Master In Slave Out)
#    2. SPI1    : PD0 (Slave Select)
#               : PD1 (Serial Clock)
#               : PD2 (Master Out Slave In)
#               : PD3 (Master In Slave Out)
#
# E. Port Connection
#    1. PA12 (Slave Select)        <----> PD0 (Slave Select)
#    2. PA13 (Serial Clock)        <----> PD1 (Serial Clock)
#    3. PA14 (Master Out Slave In) <----> PD2 (Master Out Slave In)
#    4. PA15 (Master In Slave Out) <----> PD3 (Master In Slave Out)
#
# For more information, read a user's manual of the target device carefully.
#
# SPI0
# 1. Channel                   : 0 (SPI0)
#
# 2. Config                    : [ 0 i s 16 0 m h 91 -delay 1 1 1 ]
#    Operation                 : i (Interrupt)
#    Mode                      : s (Slave)
#    Data Length               : 16 (16bit)
#    Polarity/Phase            : 0 (Polarity : Low, Phase : Low)
#    Bit Order                 : m (MSB)
#    Slave Select Pin Polarity : h (High)
#    Baudrate                  : 91 (PCLK / (Baudrate + 1))
#    Delay Start               : 1
#    Delay Stop                : 1
#    Delay Burst               : 1
#
# 3. Rx                        : [ 0 8 ]
#    Receive Data Length       : 8
#
# SPI1
# 1. Channel                   : 1 (SPI1)
#
# 2. Config                    : [ 1 i m 16 0 m h 91 -delay 1 1 1 ]
#    Operation                 : i (Interrupt)
#    Mode                      : m (Master)
#    Data Length               : 16 (16bit)
#    Polarity/Phase            : 0 (Polarity : Low, Phase : Low)
#    Bit Order                 : m (MSB)
#    Slave Select Pin Polarity : h (High)
#    Baudrate                  : 91 (PCLK / (Baudrate + 1))
#    Delay Start               : 1
#    Delay Stop                : 1
#    Delay Burst               : 1
#
# 3. Data                      : [ 1 16 0x01 0x02 0x03 0x04 0x05 0x06 0xa5 0x5a 0x00 0xff 0xff 0x00 0x00 0xff 0x55 0xaa ]
#    Data Length               : 16
#    Data                      : 0x01 ... (Hexa and space (delimitor))
#
# 4. Tx                        : [ 1 8 ]
#    Transmit Data Length      : 8
#
# PCU (PDx)
# 1. Port Group                : 3 (PCU Port D)
#
# 2. Port                      : [ 3 0 a 1 ] [ 3 1 a 1 ] [ 3 2 a 1 ] [ 3 3 a 1 ]
#    Pin Number                : 0 / 1 / 2 / 3
#    Pin Mode                  : a (Alternative)
#    Alternative               : 1 (SS1/SCK1/MOSI1/MISO1)
# 
# PCU (PDx)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 0 a 1"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 1 a 1"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 2 a 1"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 3 a 1"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# SPI0
send "cm spi"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "config 0 i s 16 0 m h 91 -delay 1 1 1"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "rx 0 8"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

# SPI1
send "cm spi"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "uninit 1"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "init 1"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "config 1 i m 16 0 m h 91 -delay 1 1 1"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "data 1 16 0x01 0x02 0x03 0x04 0x05 0x06 0xa5 0x5a 0x00 0xff 0xff 0x00 0x00 0xff 0x55 0xaa"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

send "tx 1 8"
expect {
    "<SPI> # "
    break
    timeout 5 goto end
}

end:
