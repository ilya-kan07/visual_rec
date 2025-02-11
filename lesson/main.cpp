#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main()
{
    std::string image_path = "C:/cv/cvProject/cvProject/img.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);

    imshow("Display window", img);
    int k = waitKey(0);
    return 0;
}
