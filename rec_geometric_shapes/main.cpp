#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap("../../res/video.mkv");
    if (!cap.isOpened()) {
        std::cerr << "Error: couldn't open the file!" << std::endl;
        return -1;
    }

    cv::Mat frame, hsv, blurred, edges, contoursImg;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    cv::VideoWriter outputVideo("../../out/output.avi", cv::VideoWriter::fourcc('M','J','P','G'), fps, cv::Size(frame_width, frame_height));
    if (!outputVideo.isOpened()) {
        std::cerr << "Error: couldn't create video file!" << std::endl;
        return -1;
    }

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: empty frame!" << std::endl;
            break;
        }

        cv::cvtColor(frame, hsv, cv::COLOR_HSV2BGR);
        cv::GaussianBlur(hsv, blurred, cv::Size(7, 7), 1.5);
        cv::Canny(blurred, edges, 50, 150);

        // Поиск контуров
        contoursImg = frame.clone();
        cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double perimeter = cv::arcLength(contour, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contour, approx, 0.02 * perimeter, true);

            cv::drawContours(contoursImg, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 255, 0), 2);
            //cv::putText(contoursImg, "square", approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

        }

        outputVideo.write(contoursImg);

        //cv::imshow("Original Video", frame);
        cv::imshow("Contours", contoursImg);

        if (cv::waitKey(30) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
