# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

# COMPILER TO BE USED
hip_compiler=/opt/rocm/hip/bin/hipcc

# HIPRNG NORMAL DEVICE API TESTS
cd $current_work_dir/../../../build/test/unit-hip/device/device_normal && $hip_compiler -I$current_work_dir/../../../lib/include/ -I/usr/local/cuda/include/ -I/opt/rocm/hip/include -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -o $current_work_dir/../../../build/test/unit-hip/device/device_normal/bin/mrg32k3aSingle_test_device_normal -L/usr/local/cuda/lib64 -L$current_work_dir/../../../build/lib/src -lcudart -lhiprng_nvcc $current_work_dir/device_normal/mrg32k3aSingle_test_device_normal.cpp $current_work_dir/../gtest_main.cpp $current_work_dir/../gtest-all.cpp

cd $current_work_dir/../../../build/test/unit-hip/device/device_normal && $hip_compiler -I$current_work_dir/../../../lib/include/ -I/usr/local/cuda/include/ -I/opt/rocm/hip/include -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -o $current_work_dir/../../../build/test/unit-hip/device/device_normal/bin/philox432Single_test_device_normal -L/usr/local/cuda/lib64 -L$current_work_dir/../../../build/lib/src -lcudart -lhiprng_nvcc $current_work_dir/device_normal/philox432Single_test_device_normal.cpp $current_work_dir/../gtest_main.cpp $current_work_dir/../gtest-all.cpp

cd $current_work_dir/../../../build/test/unit-hip/device/device_normal && $hip_compiler -I$current_work_dir/../../../lib/include/ -I/usr/local/cuda/include/ -I/opt/rocm/hip/include -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -o $current_work_dir/../../../build/test/unit-hip/device/device_normal/bin/xorwowSingle_test_device_normal -L/usr/local/cuda/lib64 -L$current_work_dir/../../../build/lib/src -lcudart -lhiprng_nvcc $current_work_dir/device_normal/xorwowSingle_test_device_normal.cpp $current_work_dir/../gtest_main.cpp $current_work_dir/../gtest-all.cpp

# HIPRNG UNIFORM DEVICE API TESTS
cd $current_work_dir/../../../build/test/unit-hip/device/device_uniform && $hip_compiler -I$current_work_dir/../../../lib/include/ -I/usr/local/cuda/include/ -I/opt/rocm/hip/include -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -o $current_work_dir/../../../build/test/unit-hip/device/device_uniform/bin/mrg32k3aSingle_test_device_uniform -L/usr/local/cuda/lib64 -L$current_work_dir/../../../build/lib/src -lcudart -lhiprng_nvcc $current_work_dir/device_uniform/mrg32k3aSingle_test_device_uniform.cpp $current_work_dir/../gtest_main.cpp $current_work_dir/../gtest-all.cpp

cd $current_work_dir/../../../build/test/unit-hip/device/device_uniform && $hip_compiler -I$current_work_dir/../../../lib/include/ -I/usr/local/cuda/include/ -I/opt/rocm/hip/include -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -o $current_work_dir/../../../build/test/unit-hip/device/device_uniform/bin/philox432Single_test_device_uniform -L/usr/local/cuda/lib64 -L$current_work_dir/../../../build/lib/src -lcudart -lhiprng_nvcc $current_work_dir/device_uniform/philox432Single_test_device_uniform.cpp $current_work_dir/../gtest_main.cpp $current_work_dir/../gtest-all.cpp

cd $current_work_dir/../../../build/test/unit-hip/device/device_uniform && $hip_compiler -I$current_work_dir/../../../lib/include/ -I/usr/local/cuda/include/ -I/opt/rocm/hip/include -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -o $current_work_dir/../../../build/test/unit-hip/device/device_uniform/bin/xorwowSingle_test_device_uniform -L/usr/local/cuda/lib64 -L$current_work_dir/../../../build/lib/src -lcudart -lhiprng_nvcc $current_work_dir/device_uniform/xorwowSingle_test_device_uniform.cpp $current_work_dir/../gtest_main.cpp $current_work_dir/../gtest-all.cpp
