# A. Description
#    A list of commands here configures SPI module and receives data via SPI.
#
# B. Preparation
#    Connecting ports with an external SPI device should be correct.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. SPI
#
# D. Default Port
#    1. SPI0    : PA12 (Slave Select)
#               : PA13 (Serial Clock)
#               : PA14 (Master Out Slave In)
#               : PA15 (Master In Slave Out)
#
# For more information, read a user's manual of the target device carefully.
#
# SPI0
# 1. Channel                   : 0 (SPI0)
#
# 2. Config                    : [ 0 i s 8 0 m h 5 -delay 1 1 1 ]
#    Operation                 : i (Interrupt)
#    Mode                      : s (Slave)
#    Data Length               : 8 (8bit)
#    Polarity/Phase            : 0 (Polarity : Low, Phase : Low)
#    Bit Order                 : m (MSB)
#    Slave Select Pin Polarity : h (High)
#    Baudrate                  : 5 (PCLK / (Baudrate + 1))
#    Delay Start               : 1
#    Delay Stop                : 1
#    Delay Burst               : 1
#
# 3. Rx                        : [ 0 8 ]
#    Receive Data Length       : 8
#
# SPI0
send ""

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

send "config 0 i s 8 0 m h 29 -delay 1 1 1"
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

end:
