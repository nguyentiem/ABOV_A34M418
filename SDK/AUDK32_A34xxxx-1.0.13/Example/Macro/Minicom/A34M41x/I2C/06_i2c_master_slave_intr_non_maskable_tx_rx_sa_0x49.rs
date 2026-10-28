# A. Description
#    A list of commands here configures I2C module, and transmits and receives data via I2C.
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
#    2. I2C1    : PD4 (Serial Clock)
#               : PD5 (Serial Data)
#
# E. Port Connection
#    1. PC7 (Serial Clock) <----> PD4 (Serial Clock)
#    2. PC8 (Serial Data)  <----> PD5 (Serial Data)
#
# For more information, read a user's manual of the target device carefully.
#
# I2C0
# 1. Channel                   : 0 (I2C0)
#
# 2. Config                    : [ 0 n m 400000 ]
#    Operation                 : n (Non Maskable Interrupt)
#    Mode                      : m (Master)
#    Baudrate                  : 400000 (400KHz)
#
# 3. Data                      : [ 0 8 0xa5 0x5a 0xa5 0x5a 0x05 0x07 0x09 0xa0 ]
#    Data Length               : 8
#    Data                      : 0xa5 ... (Hexa and space (delimitor))
#
# 4. Tx                        : [ 0 8 -saddr 0x49 ]
#    Receive Data Length       : 8
#    Slave Address             : 0x49 (Hexa Format)
#
# I2C1
# 1. Channel                   : 1 (I2C0)
#
# 2. Config                    : [ 1 n s 400000 -saddr 0x49 ]
#    Operation                 : n (Non Maskable Interrupt)
#    Mode                      : s (Slave)
#    Baudrate                  : 400000 (400KHz)
#    Slave Address             : 0x49 (Hexa Format)
#
# 3. Rx                        : [ 1 8 ]
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
#
# PCU (PDx) for I2C1
# 1. Port Group                : 3 (PCU Port D)
#
# 2. Port                      : [ 3 4 a 1 -pupd p ] [ 3 5 a 1 -pupd p]
#    Pin Number                : 4 / 5 
#    Pin Mode                  : a (Alternative)
#    Alternative               : 1 (SDA/SCL)
#    Pull-up/down              : p (Pull-up)
#
# PCU (PCx/PDx)
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

send "port 3 4 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}

send "port 3 5 a 1 -pupd p"
expect {
    "<PCU> # "
    break
    timeout 5 goto end
}


# I2C1
send "cm i2c"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "uninit 1"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "init 1"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "config 1 n s 400000 -saddr 0x49"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "rx 1 8"
expect {
    "<I2C> # "
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

send "config 0 n m 400000"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "data 0 8 0xa5 0x5a 0xa5 0x5a 0x05 0x07 0x09 0xa0"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

send "tx 0 8 -saddr 0x49"
expect {
    "<I2C> # "
    break
    timeout 5 goto end
}

end:
