#include <opencv2/opencv.hpp>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>

#define BLUR_SIZE 3
#define CHANNELS 3

using namespace cv;

__global__
void colorToBlurConversion(uc* out, uc* in, int w, int h) 
{
    int Col = blockIdx.x * blockDim.x + threadIdx.x;
    int Row = blockIdx.y * blockDim.y + threadIdx.y;

    if (Col < w && Row < h) {
		int smr = 0;
		int smg = 0;
		int smb = 0;
		int pixels=0;
		int Offset = (Row * w + Col) * CHANNELS;

		for (int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE + 1; ++blurRow) 
        {
			for (int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE + 1; ++blurCol) 
            {
				int curRow = Row + blurRow;
				int curCol = Col + blurCol;
				if (curRow > -1 && curRow < h && curCol > -1 && curCol < w) 
                {
					int current = (curRow * w + curCol) * CHANNELS;
					smr += in[current];
					smg += in[current + 1];
					smb += in[current + 2];
					pixels++;
				}
			}
		}
		out[Offset] = (unsigned char)(smr / pixels);
		out[Offset + 1] = (unsigned char)(smg / pixels);
		out[Offset + 2] = (unsigned char)(smb / pixels);
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

    uc* ptrImageData = NULL;
    uc* ptrImageDataOut = NULL;

    cudaMalloc(&ptrImageDataOut, w * h * CHANNELS);
    cudaMalloc(&ptrImageData, w * h * CHANNELS);
    cudaMemcpy(ptrImageData, image1.data, w * h * CHANNELS, cudaMemcpyHostToDevice);

    dim3 dimGrid(ceil(w / 16.0), ceil(h / 16.0), 1);
    dim3 dimBlock(16, 16, 1);

    colorToBlurConversion<<<dimGrid, dimBlock>>>(ptrImageDataOut, ptrImageData, w, h);
    cudaDeviceSynchronize(); // Esperar a que todos los bloques terminen

    Mat image2(h, w, CV_8UC3);
    cudaMemcpy(image2.data, ptrImageDataOut, w * h * CHANNELS, cudaMemcpyDeviceToHost);

    std::string nuevoNombre = argv[1];
    nuevoNombre = nuevoNombre.substr(0, nuevoNombre.find_last_of('.')) + "_toBlur.jpeg";
    imwrite(nuevoNombre, image2);

    cudaFree(ptrImageData);
    cudaFree(ptrImageDataOut);

    namedWindow("Imagen Procesada", WINDOW_AUTOSIZE);
    imshow("Imagen Procesada", image2);
    waitKey(0);

    return 0;
}