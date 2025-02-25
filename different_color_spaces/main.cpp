#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

int main() {
    // Загружаем изображение
    cv::Mat image = cv::imread("../../../images/img.png");

    // Проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat labImage;
    cv::cvtColor(image, labImage, cv::COLOR_BGR2Lab);

    cv::Mat yuvImage;
    cv::cvtColor(image, yuvImage, cv::COLOR_BGR2YUV);

    cv::Mat xyzImage;
    cv::cvtColor(image, xyzImage, cv::COLOR_BGR2XYZ);

    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    cv::Mat smoothedGrayImage;
    cv::GaussianBlur(grayImage, smoothedGrayImage, cv::Size(5, 5), 1.5);

    cv::imshow("Original image", image);
    cv::imshow("HSV image", hsvImage);
    cv::imshow("LAB image", labImage);
    cv::imshow("YUV image", yuvImage);
    cv::imshow("XYZ image", xyzImage);
    cv::imshow("GRAY image", grayImage);
    cv::imshow("Smoothed GRAY image", smoothedGrayImage);

    //cv::imwrite("../../results/hsv_image.png", hsvImage);
    //cv::imwrite("../../results/lab_image.png", labImage);
    //cv::imwrite("../../results/yuv_image.png", yuvImage);
    //cv::imwrite("../../results/xyz_image.png", xyzImage);
    //cv::imwrite("../../results/gray_image.png", grayImage);
    //cv::imwrite("../../results/smoothed_gray_image.png", smoothedGrayImage);

    // обнаружение линий
    cv::Mat edges;
    cv::Canny(grayImage, edges, 80, 180, 3);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(edges, lines, 1, CV_PI / 180, 150);

    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;

        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(image, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(smoothedGrayImage, circles, cv::HOUGH_GRADIENT, 1, smoothedGrayImage.rows / 8, 100, 30, 5, 100);

    for (size_t i = 0; i < circles.size(); i++) {
        cv::Vec3i c = circles[i];
        cv::Point center(c[0], c[1]);
        int radius = c[2];

        cv::circle(image, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
    }

    cv::imshow("Lines and Circles", image);
    //cv::imwrite("../../results/lines_and_circles.png", image);

    // Ожидаем нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
