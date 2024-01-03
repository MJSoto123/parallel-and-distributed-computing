#include <opencv2/opencv.hpp>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>

#define BLUR_SIZE 3
#define CHANNELS 3

using namespace cv;

__global__
void colorToGreyscaleConversion(unsigned char* out, unsigned char* in, int w, int h) 
{	
	int Col = threadIdx.x + blockIdx.x * blockDim.x;
	int Row = threadIdx.y + blockIdx.y * blockDim.y;
	if (Col < w && Row < h) 
    {
		int greyOffset = Row * w + Col;
		
		int offset = greyOffset * CHANNELS;
		unsigned char smr = in[offset];  
		unsigned char smg = in[offset + 1]; 
		unsigned char smb = in[offset + 2]; 

		out[offset] = 0.21f * smr + 0.71f * smg + 0.07f * smb;
		out[offset+1] = 0.21f * smr + 0.71f * smg + 0.07f * smb;
		out[offset+2] = 0.21f * smr + 0.71f * smg + 0.07f * smb;
	}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <nombre_de_archivo_imagen>" << std::endl;
        return -1;
    }

    int w, h;
    Mat image1 = imread(argv[1], IMREAD_COLOR);

    if (image1.empty()) {
        std::cerr << "Error al cargar la imagen." << std::endl;
        return -1;
    }

    namedWindow("Imagen Original", WINDOW_AUTOSIZE);
    imshow("Imagen Original", image1);
    waitKey(0);

    Size imageSize = image1.size();
    w = imageSize.width;
    h = imageSize.height;

    std::cout<<w<<" "<<h<<"\n";

    unsigned char* ptrImageData = NULL;
    unsigned char* ptrImageDataOut = NULL;

    cudaMalloc(&ptrImageDataOut, w * h * CHANNELS);
    cudaMalloc(&ptrImageData, w * h * CHANNELS);
    cudaMemcpy(ptrImageData, image1.data, w * h * CHANNELS, cudaMemcpyHostToDevice);

    dim3 dimGrid(ceil(w / 16.0), ceil(h / 16.0), 1);
    dim3 dimBlock(16, 16, 1);

    colorToGreyscaleConversion<<<dimGrid, dimBlock>>>(ptrImageDataOut, ptrImageData, w, h);
    cudaDeviceSynchronize(); // Esperar a que todos los bloques terminen

    Mat image2(h, w, CV_8UC3);
    cudaMemcpy(image2.data, ptrImageDataOut, w * h * CHANNELS, cudaMemcpyDeviceToHost);

    std::string nuevoNombre = argv[1];
    nuevoNombre = nuevoNombre.substr(0, nuevoNombre.find_last_of('.')) + "_toGray.png";
    imwrite(nuevoNombre, image2);

    cudaFree(ptrImageData);
    cudaFree(ptrImageDataOut);

    namedWindow("Imagen Procesada", WINDOW_AUTOSIZE);
    imshow("Imagen Procesada", image2);
    waitKey(0);

    return 0;
}
