#include <opencv2/opencv.hpp>
#include <vector>

int main() {
    // Загружаем изображение
    cv::Mat image = cv::imread("../../../images/img.png");

    // Проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // линия
    cv::line(image, cv::Point(0, 0), cv::Point(100, 100), cv::Scalar(255, 0, 0));

    // прямоугольник
    cv::rectangle(image, cv::Point(100, 0), cv::Point(200, 100), cv::Scalar(255, 0, 0), -1);

    // круг
    cv::circle(image, cv::Point(250, 50), 50, cv::Scalar(255, 0, 0), -1);

    // эллипс
    cv::ellipse(image, cv::Point(400, 50), cv::Size(100, 50), 0, 0, 360, cv::Scalar(255, 0, 0), -1);

    // многоугольник
    std::vector<cv::Point> points = {{500, 100}, {550, 0}, {600, 100}};
    cv::polylines(image, points, true, cv::Scalar(255, 0, 0));

    // текст
    cv::putText(image, "Hello, OpenCV!", cv::Point(50, 500), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0));

    // Отображаем изображение в окне
    cv::imshow("Hello OpenCV", image);

    // Ожидаем нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
