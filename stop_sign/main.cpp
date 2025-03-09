#include <opencv2/opencv.hpp>

int main() {
    // Загружаем изображение
     cv::Mat image = cv::imread("../../res/image.png");

    // Проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    cv::Mat imageWithCircle = image.clone();

    cv::Mat hsvImg;
    cv::cvtColor(image, hsvImg, cv::COLOR_BGR2HSV);

    cv::Mat mask1, mask2, mask;
    cv::inRange(hsvImg, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsvImg, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), mask2);

    cv::bitwise_or(mask1, mask2, mask);

    cv::Mat result;
    cv::bitwise_and(image, image, result, mask);

    cv::imwrite("../../results/original_image.png", image);
    cv::imwrite("../../results/mask.png", mask);
    cv::imwrite("../../results/segmentation_result.png", result);

    cv::imshow("Original image", image);
    cv::imshow("Mask", mask);
    cv::imshow("Segmentation result", result);

    cv::Mat blurredImage;
    cv::cvtColor(result, blurredImage, cv::COLOR_HSV2BGR);
    cv::cvtColor(blurredImage, blurredImage, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(blurredImage, blurredImage, cv::Size(9, 9), 2);
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(blurredImage, circles, cv::HOUGH_GRADIENT, 1.25, blurredImage.rows, 200, 30);

    if (!circles.empty()) {
        cv::Vec3i c = circles[0];
        cv::Point center(c[0], c[1]);
        int radius = static_cast<int>(c[2] * 1.0);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
        cv::circle(image, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
    }

    cv::imwrite("../../results/Image_with_circle.png", image);
    cv::imshow("Image with circle", image);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
