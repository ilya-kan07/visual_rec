#include <opencv2/opencv.hpp>

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

    cv::Mat mask1, mask2, mask;
    cv::inRange(hsvImage, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsvImage, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), mask2);
    cv::bitwise_or(mask1, mask2, mask);

    cv::imshow("Original image", image);
    cv::imshow("HSV image", hsvImage);
    cv::imshow("Mask", mask1);

    // Ожидаем нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
