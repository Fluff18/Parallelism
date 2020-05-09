#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <string.h>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)
#define SIZE_1 512 
#define SIZE_2 1024 
#define SIZE_3 2048 

#define TILE_SIZE_1 8 
#define TILE_SIZE_2 16 

cl_uint ret_num_platforms = 0;

cl_uint getPlatformIndex(cl_platform_id* platforms, bool platform_type) {

    char* required_platform_subname = (char*) malloc(5);
    cl_uint selected_platform_index = 3; //Start at max
    if(platform_type) {
        strcpy(required_platform_subname, "CPU");
    } else {
        strcpy(required_platform_subname, "Graphics"); //Names as per CapsBasic
    }
    std::cout << "Reqd name = " << required_platform_subname << std::endl;
    for(cl_uint i = 0; i < ret_num_platforms; ++i)
    {
        // Get the length for the i-th platform name
        size_t platform_name_length = 0;
        clGetPlatformInfo(
            platforms[i],
            CL_PLATFORM_NAME,
            0,
            0,
            &platform_name_length
            );

        // Get the name itself for the i-th platform
        char* platform_name = new char[platform_name_length];
        clGetPlatformInfo(
            platforms[i],
            CL_PLATFORM_NAME,
            platform_name_length,
            platform_name,
            0
            );

        if(
            strstr(platform_name, required_platform_subname) 
//&&            selected_platform_index == num_of_platforms // have not selected yet
            )
        {
            std::cout << " [Selected] " << i << std::endl;
            selected_platform_index = i;
            delete [] platform_name;
            return selected_platform_index;
            // return the first match
        }

    }
    return -1;
}
void matrix_mul_sequence (int *A_mat,
                          int *B_mat,
                          int *C_mat,
              size_t SIZE)
{
    for (size_t j=0; j<SIZE; j++) {
        for (size_t i=0; i<SIZE; i++)
            for (size_t k=0; k<SIZE; k++)
                C_mat[j*SIZE + i] += A_mat[j*SIZE + k] * B_mat[k*SIZE + i];
    }
}


int runForSize(size_t SIZE, size_t TILE_SIZE, bool platform_select) {

    cl_device_type platformType;

    if(platform_select) {
        platformType = CL_DEVICE_TYPE_CPU;
    } else {
        platformType = CL_DEVICE_TYPE_GPU;
    }


    std::cout << "Platform " << platform_select << " Matrix size " << SIZE << "x" << SIZE << " Tile size " << TILE_SIZE << std::endl;
    
    int *A = new int[SIZE*SIZE];
    int *B = new int[SIZE*SIZE];
    int *C = new int[SIZE*SIZE];
    int *C_seq = new int[SIZE*SIZE];

    //Initialize matrix
    for(size_t j=0; j<SIZE; j++) {
        for(size_t i=0; i<SIZE; i++) {
            A[j*SIZE + i] = 1;
            B[j*SIZE + i] = i+1;
            C[j*SIZE + i] = 0;
            C_seq[j*SIZE + i] = 0;
        }
    }

    std::chrono::high_resolution_clock::time_point t1, t2;
    t1 = std::chrono::high_resolution_clock::now();
    matrix_mul_sequence(A, B, C_seq, SIZE);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Reference C matrix multiplication: "
        << (float)(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count())/1000000
        << " sec"
        << std::endl;
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("matrix_mul.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp); 
    fclose( fp );


    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;

    cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);


    cl_platform_id* platform_id = new cl_platform_id[ret_num_platforms]; 

    std::cout << "clGetPlatformIDs " << ret_num_platforms << std::endl;
    ret = clGetPlatformIDs(ret_num_platforms, platform_id, 0); 
    std::cout << "clGetPlatformIDs List Ret = " << ret << std::endl;
    cl_uint selected_platform_index = getPlatformIndex(platform_id, platform_select);
    std::cout << "getPlatformIndex " << selected_platform_index << std::endl;
    cl_platform_id platformCPU = platform_id[selected_platform_index];
    ret = clGetDeviceIDs(platformCPU, platformType, 1, &device_id, &ret_num_devices); 
    std::cout << "clGetDeviceIDs " << ret << std::endl;
     cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE*sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE*sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE*sizeof(int), NULL, &ret);

    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE*sizeof(int), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE*sizeof(int), B, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE*sizeof(int), C, 0, NULL, NULL);


    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    std::string build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
    ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
    if (ret == CL_BUILD_PROGRAM_FAILURE) { 
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char *) malloc(log_size);


        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("%s\n", log);
    }


    cl_kernel kernel;
    kernel = clCreateKernel(program, "matrix_mul", &ret);


    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

    int dimention = 2;
    size_t global_item_size[] = {SIZE, SIZE, 1};
    size_t local_item_size[] = {TILE_SIZE, TILE_SIZE, 1};

    cl_event perf_event;
    cl_ulong start, end;

 
    ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size, local_item_size, 0, NULL, &perf_event);
  
    ret = clWaitForEvents(1, &perf_event);
    ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
    ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
    std::cout << "OpenCL matrix multiplication: " << (float)(end - start)/1000000000 << " sec" << std::endl;
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE*sizeof(int), C, 0, NULL, NULL);
    ret = clFinish(command_queue);

    
    kernel = clCreateKernel(program, "matrix_mul_tile", &ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

    ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size, local_item_size, 0, NULL, &perf_event);
    ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
    ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
    std::cout << "OpenCL matrix tiled: " << (float)(end - start)/1000000000 << " sec" << std::endl;
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE*sizeof(int), C, 0, NULL, NULL);
    ret = clFinish(command_queue);

   
    bool validate = true;
    for(size_t j=0; j<SIZE; j++) {
        for(size_t i=0; i<SIZE; i++) {
            if (C[j*SIZE + i] != C_seq[j*SIZE + i])
                validate = false;
        }
    }

    if (validate == false)
        std::cout << "The results are mismatched !!" << std::endl;

    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    std::cout << "Press Enter to finish..." << std::endl;
    getchar();
    return 0;
}

int main(void)
{
        bool isCPU = false; 
    runForSize(SIZE_1, TILE_SIZE_1, isCPU); runForSize(SIZE_1, TILE_SIZE_2, isCPU); 
    runForSize(SIZE_2, TILE_SIZE_1, isCPU); runForSize(SIZE_2, TILE_SIZE_2, isCPU); 
    runForSize(SIZE_3, TILE_SIZE_1, isCPU); runForSize(SIZE_3, TILE_SIZE_2, isCPU); 
    isCPU = true;
    runForSize(SIZE_1, TILE_SIZE_1, isCPU); runForSize(SIZE_1, TILE_SIZE_2, isCPU); 
    runForSize(SIZE_2, TILE_SIZE_1, isCPU); runForSize(SIZE_2, TILE_SIZE_2, isCPU);
    runForSize(SIZE_3, TILE_SIZE_1, isCPU); runForSize(SIZE_3, TILE_SIZE_2, isCPU);
    return 0;
}