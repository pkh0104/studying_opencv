#include <opencv2/opencv.hpp>

#include <iostream>
#include <cstdio>

typedef cv::Vec<uchar, 3> Vec3b;
typedef cv::Vec<uchar, 4> Vec4b;
typedef cv::Vec<float, 4> Vec4f;

int main(void)
{
    int rows = 9, cols = 16;
    cv::Mat mat1(rows, cols, CV_8U);
    printf("mat1 Size(%d, %d) Chs(%d) Elems(%zu) Dims(%d)\n", 
            mat1.cols, mat1.rows, 
            mat1.channels(), mat1.elemSize(), mat1.dims);
    for(int step = 0; step < mat1.dims; ++step){
        printf("step[%d] = %zu\n", step, mat1.step[step]);
    }
    for(int y = 0; y < mat1.rows; ++y){
        for(int x = 0; x < mat1.cols; ++x){
            uchar* p = mat1.ptr<uchar>(y, x);
            *p =  mat1.step[0] * y + mat1.step[1] * x;
            printf("%d,", mat1.at<uchar>(y, x));
        }
        printf("\n");
    }
    printf("\n");

    cv::Mat mat2(rows, cols, CV_8UC3);
    printf("mat2 Size(%d, %d) Chs(%d) Elems(%zu) Dims(%d)\n", 
            mat2.cols, mat2.rows, 
            mat2.channels(), mat2.elemSize(), mat2.dims);
    for(int step = 0; step < mat2.dims; ++step){
        printf("step[%d] = %zu\n", step, mat2.step[step]);
    }
    for(int y = 0; y < mat2.rows; ++y){
        for(int x = 0; x < mat2.cols; ++x){
            Vec3b* p = mat2.ptr<Vec3b>(y, x);
            (*p)[0] =  y;
            (*p)[1] =  x;
            (*p)[2] =  0;
            for(int ch = 0; ch < mat2.channels(); ++ch){
                printf("%d,", mat2.at<Vec3b>(y, x)[ch]);
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");

    cv::Mat mat3(rows, cols, CV_32FC2, cv::Scalar(1, 3));
    printf("mat3 Size(%d, %d) Chs(%d) Elems(%zu) Dims(%d)\n", 
            mat3.cols, mat3.rows, mat3.channels(), mat3.elemSize(), mat3.dims);
    for(int step = 0; step < mat3.dims; ++step){
        printf("step[%d] = %zu\n", step, mat3.step[step]);
    }

    cv::Mat mat4(rows, cols, CV_32FC4, cv::Scalar(1, 3, 2, 4));
    printf("mat4 Size(%d, %d) Chs(%d) Elems(%zu) Dims(%d)\n", 
            mat4.cols, mat4.rows, mat4.channels(), mat4.elemSize(), mat4.dims);
    for(int step = 0; step < mat4.dims; ++step){
        printf("step[%d] = %zu\n", step, mat4.step[step]);
    }
    for(int y = 0; y < mat4.rows; ++y){
        for(int x = 0; x < mat4.cols; ++x){
            for(int c = 0; c < mat4.channels(); ++c){
                printf("%.1f,", mat4.at<Vec4f>(y, x)[c]);
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");

    int sz[] = {2, 3, 4, 5};
    cv::Mat mat5(4, sz, CV_8UC4, cv::Scalar::all(0));
    printf("mat5 Size(%d, %d) Chs(%d) Elems(%zu) Dims(%d)\n", 
            mat5.cols, mat5.rows, mat5.channels(), mat5.elemSize(), mat5.dims);
    for(int step = 0; step < mat5.dims; ++step){
        printf("step[%d] = %zu\n", step, mat5.step[step]);
    }
    for(int d0 = 0; d0 < sz[0]; ++d0){
        for(int d1 = 0; d1 < sz[1]; ++d1){
            for(int d2 = 0; d2 < sz[2]; ++d2){
                for(int d3 = 0; d3 < sz[3]; ++d3){
                    cv::Vec<int, 4> idx(d0, d1, d2, d3);
                    mat5.at<Vec4b>(idx)[0] = d0;
                    mat5.at<Vec4b>(idx)[1] = d1;
                    mat5.at<Vec4b>(idx)[2] = d2;
                    mat5.at<Vec4b>(idx)[3] = d3;
                }
            }
        }
    }
    for(int d0 = 0; d0 < sz[0]; ++d0){
        for(int d1 = 0; d1 < sz[1]; ++d1){
            for(int d2 = 0; d2 < sz[2]; ++d2){
                for(int d3 = 0; d3 < sz[3]; ++d3){
                    int idx[] = {d0, d1, d2, d3};
                    Vec4b* p = mat5.ptr<Vec4b>(idx);
                    for(int ch = 0; ch < mat5.channels(); ++ch){
                        printf("%d,", (*p)[ch]);
                    }
                    printf(" ");
                }
                printf("\n");
            }
            printf("\n\n");
        }
        printf("\n\n\n");
    }
    printf("\n");

    return 0;
}
