1. Getting required drives and SDK
Two things are needed here. First, OpenCL runtime for your graphics card. It can be achieved by simply updating NVIDIA’s graphics card. Secondly, OpenCL SDK is needed for compiling OpenCL code. NVIDIA has hidden them under its CUDA toolkit. So, install CUDA toolkit and you will get OpenCL SDK too.
2. Setting up Visual Studio
I have Visual Studio 2012, hence, the configurations are based on that only. Create a new Visual Studio C++ application (any template). Under src, create a new C file by name of main.c. Similarly, create a kernel file by the name of kernel.cl. main.c will contain the host code. kernel.cl will contain the kernel to be executed.
3. Project configurations
OpenCL is suggested to run on 64-bit configurations. However, the created solution would be in 32–bit only. To fix this, right click on the project > Choose Properties in the context menu. A Property Pages window will open.
Click on Configuration Manager on right. In the Configuration Manager window, select <New..> from Active solution platform dropdown menu. In the New Solution Platform window, choose x64 as new platform and copy settings option as Win32. This will make the project targeted for 64-bit build.
4. Adding code to the project
Since, this is not a OpenCL programming tutorial, hence, I would suggest you to copy main.c code from here and kernel.cl code from here. Since, the original main.c picks up code from vector_add_kernel.cl but we have kernel.cl, hence change the filename in main.c
30: fp = fopen("vector_add_kernel.cl", "r");
5. Running the program
Press F5 to compile and run the program. If no compilation error, it will create a Debug build of the program and launch it.