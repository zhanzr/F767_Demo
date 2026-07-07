# Nucleo F767 board test.

## coremark
Puting code in ITCM ram seems does improve scores.

216 MHz, Standard Lib

### AXIM Flash
ARMCLANG 6.24

#### With FPU-Single
```
-Omax
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 52552
Total time (secs): 52.552000
Iterations/Sec   : 380.575430
Iterations       : 20000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 380.575430 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar / STACK
```


### AXIM Flash + ITCM Code
ARMCLANG 6.24

#### With FPU-Single
```
-Omax
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 17658
Total time (secs): 17.658000
Iterations/Sec   : 1132.631102
Iterations       : 20000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 1132.631102 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar / STACK
```

### ITCM Flash
ARMCLANG 6.24
#### With FPU-Single
```
-Omax
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 33424
Total time (secs): 33.424000
Iterations/Sec   : 598.372427
Iterations       : 20000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 598.372427 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar / STACK
```

### ITCM Flash + ITCM Code
ARMCLANG 6.24
#### With FPU-Single
```
-Omax
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 17658
Total time (secs): 17.658000
Iterations/Sec   : 1132.631102
Iterations       : 20000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 1132.631102 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fno-rtti -flto -funsigned-char -fshort-enums -fshort-wchar / STACK
```