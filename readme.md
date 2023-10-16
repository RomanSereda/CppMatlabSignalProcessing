## [Visual Studio 2022 Project] Processing of the signal created in MatLab

#### ·Import library from matlab: mcc -W 'cpplib:PhaseSignalGen,all' -T link:lib PhaseSignalGen.mlx
#### ·Signal filtering: "embedded-kalman", moving average
#### ·Determination of the signal correlation curve, to convolution kernels (kernels: 2 part of signal)
#### ·Filling in the missing part of the signal, according to the correlation curve
#### ·Calculation of the angle value at point 0 (offset by 250 ticks)


## Semiresult:

<img src="https://github.com/RomanSereda/CppMatlabSignalProcessing/blob/master/screen.png" width="500">
