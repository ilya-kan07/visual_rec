#include <opencv2/opencv.hpp>

int main() {
    // Загружаем изображение
    cv::Mat image = cv::imread("../../../images/img.png");

    // Проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    int height = image.rows;
    int width = image.cols;

    cv::Rect topLeftRect(0, 0, width / 2, height / 2);
    cv::Rect topRightRect(width / 2, 0, width / 2, height / 2);
    cv::Rect bottomLeftRect(0, height / 2, width / 2, height / 2);
    cv::Rect bottomRightRect(width / 2, height / 2, width / 2, height / 2);

    cv::Mat topLeft = image(topLeftRect).clone();
    cv::Mat topRight = image(topRightRect).clone();
    cv::Mat bottomLeft = image(bottomLeftRect).clone();
    cv::Mat bottomRight = image(bottomRightRect).clone();

    // Верхний правый угол
    cv::bitwise_not(topRight, topRight);

    // Нижний левый угол
    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_BGR2GRAY);
    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_GRAY2BGR);

    // Нижний правый угол
    bottomRight.setTo(cv::Scalar(0, 255, 0));

    cv::Mat result;
    image.copyTo(result);

    topLeft.copyTo(result(topLeftRect));
    topRight.copyTo(result(topRightRect));
    bottomLeft.copyTo(result(bottomLeftRect));
    bottomRight.copyTo(result(bottomRightRect));

    cv::imshow("Original image", image);
    cv::imshow("Processed image", result);

    // Ожидаем нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
