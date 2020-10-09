
# Hardware
```
$ nvidia-smi 
Thu Oct  8 20:11:54 2020       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 450.51.06    Driver Version: 450.51.06    CUDA Version: 11.0     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  GeForce GTX 1080    On   | 00000000:3B:00.0 Off |                  N/A |
| 24%   38C    P8     8W / 180W |      7MiB /  8119MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                                  |
|  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
|        ID   ID                                                   Usage      |
|=============================================================================|
|    0   N/A  N/A      3309      G   /usr/lib/xorg/Xorg                  4MiB |
+-----------------------------------------------------------------------------+
```

# Build
```
$ make -f Makefile.isycl 
/media/disk2/jrhammon/llvm/build/bin/clang++ -O3 -std=c++17 -ferror-limit=3 -fsycl -fsycl-unnamed-lambda -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -L/media/disk2/jrhammon/llvm/build/lib -Wl,-rpath=/media/disk2/jrhammon/llvm/build/lib saxpy.cc -o saxpy.x
clang-12: warning: Unknown CUDA version 11.0. Assuming the latest supported version 10.1 [-Wunknown-cuda-version]
/media/disk2/jrhammon/llvm/build/bin/clang++ -O3 -std=c++17 -ferror-limit=3 -fsycl -fsycl-unnamed-lambda -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -L/media/disk2/jrhammon/llvm/build/lib -Wl,-rpath=/media/disk2/jrhammon/llvm/build/lib saxpy-usm.cc -o saxpy-usm.x
clang-12: warning: Unknown CUDA version 11.0. Assuming the latest supported version 10.1 [-Wunknown-cuda-version]
/media/disk2/jrhammon/llvm/build/bin/clang++ -O3 -std=c++17 -ferror-limit=3 -fsycl -fsycl-unnamed-lambda -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -L/media/disk2/jrhammon/llvm/build/lib -Wl,-rpath=/media/disk2/jrhammon/llvm/build/lib saxpy-usm2.cc -o saxpy-usm2.x
clang-12: warning: Unknown CUDA version 11.0. Assuming the latest supported version 10.1 [-Wunknown-cuda-version]
/media/disk2/jrhammon/llvm/build/bin/clang++ -O3 -std=c++17 -ferror-limit=3 -fsycl -fsycl-unnamed-lambda -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -L/media/disk2/jrhammon/llvm/build/lib -Wl,-rpath=/media/disk2/jrhammon/llvm/build/lib saxpy-usm3.cc -o saxpy-usm3.x
clang-12: warning: Unknown CUDA version 11.0. Assuming the latest supported version 10.1 [-Wunknown-cuda-version]
```

# SYCL buffers
```
$ /usr/local/cuda-11.0/bin/nvprof ./saxpy.x 1000000
SAXPY with 1000000 elements
==17152== NVPROF is profiling process 17152, command: ./saxpy.x 1000000
Program completed without error.
==17152== Profiling application: ./saxpy.x 1000000
==17152== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   75.78%  2.1742ms         3  724.72us  722.59us  727.71us  [CUDA memcpy HtoD]
                   21.86%  627.04us         1  627.04us  627.04us  627.04us  [CUDA memcpy DtoH]
                    2.37%  67.871us         1  67.871us  67.871us  67.871us  _ZTSZZ4mainENKUlRN2cl4sycl7handlerEE35_18clES2_E4axpy
      API calls:   77.61%  242.69ms         1  242.69ms  242.69ms  242.69ms  cuCtxCreate
                   20.77%  64.951ms         1  64.951ms  64.951ms  64.951ms  cuCtxDestroy
                    0.86%  2.6990ms         3  899.68us  896.44us  904.40us  cuMemcpyHtoD
                    0.32%  989.65us         1  989.65us  989.65us  989.65us  cuMemcpyDtoHAsync
                    0.12%  388.17us         3  129.39us  103.64us  149.23us  cuMemFree
                    0.12%  368.50us         3  122.83us  118.06us  127.39us  cuMemAlloc
                    0.07%  218.58us         4  54.644us  4.0900us  72.002us  cuStreamSynchronize
                    0.07%  204.79us         1  204.79us  204.79us  204.79us  cuModuleLoadDataEx
                    0.02%  64.267us         1  64.267us  64.267us  64.267us  cuModuleUnload
                    0.02%  61.043us         6  10.173us     882ns  54.815us  cuEventSynchronize
                    0.01%  23.872us         1  23.872us  23.872us  23.872us  cuLaunchKernel
                    0.00%  12.315us         1  12.315us  12.315us  12.315us  cuStreamCreate
                    0.00%  9.6720us         1  9.6720us  9.6720us  9.6720us  cuStreamDestroy
                    0.00%  7.4340us        26     285ns     168ns     509ns  cuCtxGetCurrent
                    0.00%  6.8750us         3  2.2910us  1.1700us  4.3840us  cuEventRecord
                    0.00%  3.3490us         3  1.1160us     632ns  1.8800us  cuEventCreate
                    0.00%  2.7800us         1  2.7800us  2.7800us  2.7800us  cuDeviceGetPCIBusId
                    0.00%  2.3870us         7     341ns     202ns     775ns  cuDeviceGetAttribute
                    0.00%  2.2470us         3     749ns     460ns     928ns  cuEventDestroy
                    0.00%  1.4040us         3     468ns     214ns     906ns  cuDeviceGetCount
                    0.00%  1.4030us         2     701ns     413ns     990ns  cuModuleGetFunction
                    0.00%  1.3790us         1  1.3790us  1.3790us  1.3790us  cuCtxSynchronize
                    0.00%  1.2220us         1  1.2220us  1.2220us  1.2220us  cuStreamWaitEvent
                    0.00%     830ns         2     415ns     192ns     638ns  cuDeviceGet
                    0.00%     525ns         1     525ns     525ns     525ns  cuCtxPopCurrent
```

# SYCL USM v1
```$ /usr/local/cuda-11.0/bin/nvprof ./saxpy-usm.x 1000000
SAXPY USM with 1000000 elements
==17202== NVPROF is profiling process 17202, command: ./saxpy-usm.x 1000000
Program completed without error.
==17202== Profiling application: ./saxpy-usm.x 1000000
==17202== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:  100.00%  2.8651ms         1  2.8651ms  2.8651ms  2.8651ms  _ZTSZZ4mainENKUlRN2cl4sycl7handlerEE36_18clES2_E5saxpy
      API calls:   71.37%  221.81ms         1  221.81ms  221.81ms  221.81ms  cuCtxCreate
                   20.71%  64.347ms         1  64.347ms  64.347ms  64.347ms  cuCtxDestroy
                    6.67%  20.737ms         3  6.9125ms  49.496us  20.600ms  cuMemAllocManaged
                    0.92%  2.8510ms         1  2.8510ms  2.8510ms  2.8510ms  cuEventSynchronize
                    0.22%  684.28us         3  228.09us  179.14us  284.63us  cuMemFree
                    0.06%  198.92us         1  198.92us  198.92us  198.92us  cuModuleLoadDataEx
                    0.02%  58.695us         1  58.695us  58.695us  58.695us  cuModuleUnload
                    0.01%  24.314us         1  24.314us  24.314us  24.314us  cuLaunchKernel
                    0.01%  17.311us         1  17.311us  17.311us  17.311us  cuStreamCreate
                    0.00%  8.1170us        18     450ns     171ns  1.3550us  cuCtxGetCurrent
                    0.00%  7.1950us         1  7.1950us  7.1950us  7.1950us  cuStreamDestroy
                    0.00%  6.2080us         2  3.1040us  1.1190us  5.0890us  cuEventRecord
                    0.00%  3.2340us         1  3.2340us  3.2340us  3.2340us  cuDeviceGetPCIBusId
                    0.00%  2.9050us         1  2.9050us  2.9050us  2.9050us  cuStreamSynchronize
                    0.00%  2.7730us         2  1.3860us     859ns  1.9140us  cuEventCreate
                    0.00%  2.4230us         7     346ns     201ns     717ns  cuDeviceGetAttribute
                    0.00%  2.2480us         3     749ns     407ns  1.4130us  cuPointerGetAttribute
                    0.00%  1.7850us         2     892ns     777ns  1.0080us  cuEventDestroy
                    0.00%  1.6700us         2     835ns     618ns  1.0520us  cuModuleGetFunction
                    0.00%  1.3730us         3     457ns     217ns     854ns  cuDeviceGetCount
                    0.00%  1.3640us         1  1.3640us  1.3640us  1.3640us  cuCtxSynchronize
                    0.00%     891ns         2     445ns     207ns     684ns  cuDeviceGet
                    0.00%     576ns         1     576ns     576ns     576ns  cuCtxPopCurrent

==17202== Unified Memory profiling result:
Device "GeForce GTX 1080 (0)"
   Count  Avg Size  Min Size  Max Size  Total Size  Total Time  Name
      86  136.33KB  4.0000KB  0.9883MB  11.44922MB  1.084988ms  Host To Device
      24  162.83KB  4.0000KB  0.9961MB  3.816406MB  321.5980us  Device To Host
      12         -         -         -           -  2.807344ms  Gpu page fault groups
Total CPU Page faults: 48
```

# SYCL USM v2
```
$ /usr/local/cuda-11.0/bin/nvprof ./saxpy-usm2.x 1000000
SAXPY USMv2 with 1000000 elements
==17243== NVPROF is profiling process 17243, command: ./saxpy-usm2.x 1000000
Program completed without error.
==17243== Profiling application: ./saxpy-usm2.x 1000000
==17243== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   75.83%  2.1803ms         3  726.78us  719.84us  731.96us  [CUDA memcpy HtoD]
                   21.83%  627.64us         1  627.64us  627.64us  627.64us  [CUDA memcpy DtoH]
                    2.35%  67.487us         1  67.487us  67.487us  67.487us  _ZTSZZ4mainENKUlRN2cl4sycl7handlerEE42_18clES2_E5saxpy
      API calls:   76.85%  223.87ms         1  223.87ms  223.87ms  223.87ms  cuCtxCreate
                   21.49%  62.598ms         1  62.598ms  62.598ms  62.598ms  cuCtxDestroy
                    1.26%  3.6840ms         4  920.99us  886.34us  990.72us  cuMemcpyAsync
                    0.14%  414.14us         3  138.05us  108.56us  154.35us  cuMemFree
                    0.10%  283.42us         3  94.472us  85.328us  110.75us  cuMemAlloc
                    0.06%  189.24us         1  189.24us  189.24us  189.24us  cuModuleLoadDataEx
                    0.04%  121.30us         5  24.260us     813ns  59.020us  cuEventSynchronize
                    0.02%  58.601us         1  58.601us  58.601us  58.601us  cuModuleUnload
                    0.01%  23.433us         1  23.433us  23.433us  23.433us  cuLaunchKernel
                    0.00%  12.885us         1  12.885us  12.885us  12.885us  cuStreamCreate
                    0.00%  10.470us         6  1.7450us     940ns  4.6580us  cuEventRecord
                    0.00%  8.5770us         1  8.5770us  8.5770us  8.5770us  cuStreamDestroy
                    0.00%  8.5740us        34     252ns     171ns     518ns  cuCtxGetCurrent
                    0.00%  5.1960us         6     866ns     422ns  1.7290us  cuEventCreate
                    0.00%  3.5960us         6     599ns     267ns  1.0740us  cuEventDestroy
                    0.00%  3.0590us         1  3.0590us  3.0590us  3.0590us  cuStreamSynchronize
                    0.00%  2.4790us         1  2.4790us  2.4790us  2.4790us  cuDeviceGetPCIBusId
                    0.00%  2.4420us         7     348ns     203ns     691ns  cuDeviceGetAttribute
                    0.00%  2.0130us         3     671ns     376ns  1.2490us  cuPointerGetAttribute
                    0.00%  1.4550us         2     727ns     410ns  1.0450us  cuModuleGetFunction
                    0.00%  1.3830us         3     461ns     210ns     897ns  cuDeviceGetCount
                    0.00%  1.3680us         1  1.3680us  1.3680us  1.3680us  cuCtxSynchronize
                    0.00%     858ns         2     429ns     191ns     667ns  cuDeviceGet
                    0.00%     592ns         1     592ns     592ns     592ns  cuCtxPopCurrent
```

# SYCL USM v3
```
$ /usr/local/cuda-11.0/bin/nvprof ./saxpy-usm3.x 1000000
SAXPY USMv3 with 1000000 elements
==17270== NVPROF is profiling process 17270, command: ./saxpy-usm3.x 1000000
Program completed without error.
==17270== Profiling application: ./saxpy-usm3.x 1000000
==17270== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   75.78%  2.1760ms         3  725.32us  720.38us  728.86us  [CUDA memcpy HtoD]
                   21.85%  627.45us         1  627.45us  627.45us  627.45us  [CUDA memcpy DtoH]
                    2.36%  67.903us         1  67.903us  67.903us  67.903us  _ZTSZ4mainEUlN2cl4sycl2idILi1EEEE42_49
      API calls:   76.33%  218.47ms         1  218.47ms  218.47ms  218.47ms  cuCtxCreate
                   21.99%  62.927ms         1  62.927ms  62.927ms  62.927ms  cuCtxDestroy
                    1.28%  3.6679ms         4  916.99us  885.57us  987.62us  cuMemcpyAsync
                    0.14%  413.35us         3  137.78us  107.36us  155.03us  cuMemFree
                    0.10%  279.47us         3  93.155us  85.231us  106.10us  cuMemAlloc
                    0.07%  189.79us         1  189.79us  189.79us  189.79us  cuModuleLoadDataEx
                    0.04%  120.37us         5  24.073us     773ns  58.506us  cuEventSynchronize
                    0.02%  58.668us         1  58.668us  58.668us  58.668us  cuModuleUnload
                    0.01%  23.203us         1  23.203us  23.203us  23.203us  cuLaunchKernel
                    0.01%  17.429us         1  17.429us  17.429us  17.429us  cuStreamCreate
                    0.00%  10.233us         6  1.7050us     889ns  4.5290us  cuEventRecord
                    0.00%  8.3830us        34     246ns     174ns     482ns  cuCtxGetCurrent
                    0.00%  8.1420us         1  8.1420us  8.1420us  8.1420us  cuStreamDestroy
                    0.00%  5.5670us         6     927ns     386ns  1.8500us  cuEventCreate
                    0.00%  3.5590us         6     593ns     266ns  1.0160us  cuEventDestroy
                    0.00%  3.0860us         1  3.0860us  3.0860us  3.0860us  cuDeviceGetPCIBusId
                    0.00%  2.5830us         1  2.5830us  2.5830us  2.5830us  cuStreamSynchronize
                    0.00%  2.3890us         7     341ns     210ns     679ns  cuDeviceGetAttribute
                    0.00%  2.3120us         3     770ns     442ns  1.4240us  cuPointerGetAttribute
                    0.00%  1.3830us         3     461ns     197ns     925ns  cuDeviceGetCount
                    0.00%  1.3150us         1  1.3150us  1.3150us  1.3150us  cuCtxSynchronize
                    0.00%  1.2360us         2     618ns     370ns     866ns  cuModuleGetFunction
                    0.00%     906ns         2     453ns     207ns     699ns  cuDeviceGet
                    0.00%     771ns         1     771ns     771ns     771ns  cuCtxPopCurrent
                    ```
