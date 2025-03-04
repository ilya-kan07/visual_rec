#include <opencv2/opencv.hpp>
#include <iostream>

void labelShape(cv::Mat& image, std::vector<cv::Point>& contour) {
    double area = cv::contourArea(contour);
    if (area < 100) return;

    std::string shape;
    cv::Rect boundingBox = cv::boundingRect(contour);
    cv::Point textPosition = boundingBox.tl() + cv::Point(50, 70);

    std::vector<cv::Point> approx;
    cv::approxPolyDP(contour, approx, 0.02 * cv::arcLength(contour, true), true);

    if (approx.size() == 3) {
        shape = "triangle";
    } else if (approx.size() == 4) {
        shape = "square";
    } else {
        shape = "circle";
    }

    cv::putText(image, shape, textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
}

int main() {
    cv::Mat src = cv::imread("../../src/image.png");
    if (src.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    cv::Mat gray, blurred, edges, hsv, mask, combined;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);
    cv::Canny(blurred, edges, 50, 150);

    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(100, 100, 50), cv::Scalar(140, 255, 255), mask);
    cv::bitwise_or(edges, mask, combined);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(combined, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat contourImage = cv::Mat::zeros(src.size(), CV_8UC3);
    cv::drawContours(contourImage, contours, -1, cv::Scalar(0, 255, 0), 2);

    for (auto& contour : contours) {
        labelShape(contourImage, contour);
    }

    cv::imshow("original image", src);
    cv::imshow("countours", contourImage);

    cv::imwrite("../../res/result.png", contourImage);

    cv::waitKey(0);

    return 0;
}
