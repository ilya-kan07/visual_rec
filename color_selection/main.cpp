#include <opencv2/opencv.hpp>

int main() {
    // Загружаем изображение
    cv::Mat image = cv::imread("../../../images/img.png");

    // Проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    cv::Mat hsvImg;
    cv::cvtColor(image, hsvImg, cv::COLOR_BGR2HSV);

    cv::Scalar lower_green(35, 40, 40); // Нижняя граница (H, S, V)
    cv::Scalar upper_green(85, 255, 255); // Верхняя граница (H, S, V)

    cv::Mat mask;
    cv::inRange(hsvImg, lower_green, upper_green, mask);

    // Применяем маску к изображению
    cv::Mat result;
    cv::bitwise_and(image, image, result, mask);

    // Сохраняем результаты
    cv::imwrite("../../results/original_image.png", image);
    cv::imwrite("../../results/mask.png", mask);
    cv::imwrite("../../results/segmentation_result.png", result);

    // Отображаем результаты
    cv::imshow("Оригинальное изображение", image);
    cv::imshow("Маска", mask);
    cv::imshow("Результат сегментации", result);

    // Ожидаем нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
