#include <opencv2/opencv.hpp>

#include <cstdio>

int main(void)
{
    cv::Mat img;
    img = cv::imread("Lenna.png");
    if(img.empty()){
        printf("Image load failed!\n");
        return -1;
    }

    cv::namedWindow("image");

    cv::imshow("image", img);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}
