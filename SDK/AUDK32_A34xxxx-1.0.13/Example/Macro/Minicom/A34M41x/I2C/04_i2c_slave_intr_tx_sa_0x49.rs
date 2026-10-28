# A. Description
#    A list of commands here configures I2C module as a slave and transmits data via I2C.
#
# B. Preparation
#    Connecting ports with an external I2C device should be correct.
#
# C. Prerequisite Example (abov_example_config.h)
#    1. I2C
#    2. PCU/GPIO
#
# D. Default Port
#    1. I2C0    : PC7 (Serial Clock)
#               : PC8 (Serial Data)
#
# For more information, read a user's manual of the target device carefully.
#
# I2C0
# 1. Channel                   : 0 (I2C0)
#
# 2. Config                    : [ 0 i s 400000 -saddr 0x49 ]
#    Operation                 : i (Interrupt)
#    Mode                      : m (Master)
#    Baudrate                  : 400000 (400KHz)
#    Slave Address             : 0x49 (Hexa Format)
#
# 3. Data                      : [ 0 8 0xa0 0x09 0x07 0x05 0x5a 0xa5 0x5a 0xa5 ]
#    Data Length               : 8
#    Data                      : 0xa5 ... (Hexa and space (delimitor))
#
# 4. Tx                        : [ 0 8 ]
#    Receive Data Length       : 8
#
# PCU (PCx) for I2C0
# 1. Port Group                : 2 (PCU Port C)
#
# 2. Port                      : [ 2 7 a 1 -pupd p ] [ 2 8 a 1 -pupd p ]
#    Pin Number                : 7 / 8 
#    Pin Mode                  : a (Alternative)
#    Alternative               : 1 (SCL/SDA)
#    Pull-up/down              : p (Pull-up)

# PCU (PCx)
send ""

send "cm pcu"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 2 7 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 2 8 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

# I2C0
send "cm i2c"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "uninit 0"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "init 0"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "config 0 i s 400000 -saddr 0x49"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "data 0 8 0xa0 0x09 0x07 0x05 0x5a 0xa5 0x5a 0xa5"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "tx 0 8"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

end:
