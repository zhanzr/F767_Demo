# Nucleo F767 board test.

## dhrystone

216 MHz, Standard Lib

### AXIM Flash
ARMCLANG 6.24

Execution starts, 4000000 runs through Dhrystone
#### With FPU-Single
```
-Omax
MicroSecond for one run through Dhrystone[23-4598]:      1.144
Dhrystones per Second:  874316.938
DMIPS/MHz:      2.304
```

### ITCM Flash
ARMCLANG 6.24
Execution starts, 4000000 runs through Dhrystone
#### With FPU-Single
```
-Omax
MicroSecond for one run through Dhrystone[23-3171]:      0.787
Dhrystones per Second:  1270648.000
DMIPS/MHz:      3.348
```

