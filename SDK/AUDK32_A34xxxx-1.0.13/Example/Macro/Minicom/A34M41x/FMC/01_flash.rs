# A. Description
#    A list of commands here configures FMC and erase and write code flash
#
# B. Preparation
#    Setup ARM debugger to connect device without reset to see registers and flash memory
#
# C. Prerequisite Example (abov_example_config.h)
#    1. Flash
#
# For more information, read a user's manual of the target device carefully.
#
# 1. Code Flash Size : 0x20000(On REQ)/0x40000(On REQ)/0x80000(MP)
#    0x80000 is taken as flash size to test
# 2. Data Flash Size : 0x8000
# 3. Code Write Protection Size : 0x8000
# 4. Data Write Protection Size : 0x800
# 5. Erase Mode
#    - FMC_CHIP_ERASE_MODE = 0
#    - FMC_PAGE_ERASE_MODE = 1
#    - FMC_1KB_ERASE_MODE  = 2
#    - FMC_2KB_ERASE_MODE  = 4
#    - FMC_4KB_ERASE_MODE  = 8
#    FMC_4KB_ERASE_MODE is taken to test code flash.
#    FMC_1KB_ERASE_MODE is taken to test data flash.

######################  Define Variables ####################### 
# FMC_ID_CFMC = 0
# FMC_ID_DFMC = 1

# FMC_CHIP_ERASE_MODE = 0
# FMC_PAGE_ERASE_MODE = 1
# FMC_1KB_ERASE_MODE  = 2
# FMC_2KB_ERASE_MODE  = 4
# FMC_4KB_ERASE_MODE  = 8

# CODE_FLASH_SIZE  = 524288
# CODE_SECTOR_32KB = 32768
# DATA_FLASH_SIZE  = 32768
# DATA_SECTOR_2KB  = 2048

# CODE_START_TEST_ADR = CODE_FLASH_SIZE - CODE_SECTOR_32KB
# DATA_START_TEST_ADR = DATA_FLASH_SIZE - DATA_SECTOR_2KB
# SIZE_512B  = 512
# SIZE_1KB = 1024
# SIZE_4KB = 4096

# ; 0x0E000000 = 234881024
# DATA_START_PHY_ADR = 234881024 + DATA_START_TEST_ADR
# DUMP_ADDR = CODE_START_TEST_ADR
# DUMP_LEN = DATA_SECTOR_2KB
# DUMP_PAUSE = 5
# DISP_MESSAGE = ""

############## Test commands here for Code Flash ##############
timeout 600

send ""

send "cm fmc"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}

# Print flash geometric information
print "01.Print flash geometric information"
send ""
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
send "info"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 2

# Unprotect all code flash area
print "\n02.Unprotect all code/data flash area"
send ""
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
send "wprot 0 0xFFFFFFFF 0 0"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
send "wprot 1 0xFFFFFFFF 0 0"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 2

# Erase 32KB at CODE FLASH_SIZE
print "\n03.Erase 32KB code flash area at 0x78000"
send ""
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x78000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x79000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x7A000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x7B000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x7C000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x7D000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x7E000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 0 0x7F000 8"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 2

print "\n04.Dump memory (0x78000 ~ 0x7FFFF)"
send "cm scu"
send "rmem 0x78000 8192" 
expect {
    "<SCU> #"
    break
    timeout 300 goto end
}
sleep 2

# Erase 2KB at DATA FLASH_SIZE
print "\n05.Erase 2KB data flash area at 0x7800"
send "cm fmc"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
send "erase 1 0x7800 2"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 0.1
send "erase 1 0x7C00 2"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 2

print "\n06.Dump memory (0xE007800 ~ 0xE007FFF)"
send "cm scu"
send "rmem 0xE007800 512" 
expect {
    "<SCU> #"
    break
    timeout 120 goto end
}
sleep 2

# Write predefined 32B data (0x00 ~ 0x1F) at CODE/DATA_START_TEST_ADR into flash write protected area
# See the result of "info" command
print "\n07.Write 32Byte data (0x00 ~ 0x1F) to code flash write protected area (0x78000 ~ 0x7FFFF)"
send ""
expect {
    "<SCU> # "
    break
    timeout 5 goto end
}
send "cm fmc"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
send "write 0 0x78000 0xFFFFFFFF"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 2

print "\n08.Dump memory (0x78000 ~ 0x7FFFF)"
send "cm scu"
send "rmem 0x78000 8192" 
expect {
    "<SCU> #"
    break
    timeout 300 goto end
}
sleep 2

print "\n09.Write 32Byte data (0x00 ~ 0x1F) to data flash write protected area (0x7800 ~ 0x7FFFF)"
send ""
expect {
    "<SCU> # "
    break
    timeout 5 goto end
}
send "cm fmc"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
send "write 1 0x7800 0xFFFFFFFF"
expect {
    "<FMC> # "
    break
    timeout 5 goto end
}
sleep 2

print "\n10.Dump memory (0xE007800 ~ 0xE007FFF)"
send "cm scu"
send "rmem 0xE007800 512" 
expect {
    "<SCU> #"
    break
    timeout 300 goto end
}
sleep 2

print "\nEnd of Flash Test Sequence"

end:
