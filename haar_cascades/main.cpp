#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main() {
    // Открываем видеофайл
    cv::VideoCapture cap("../../res/video.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Ошибка: Не удалось открыть видеофайл!" << std::endl;
        return -1;
    }

    // Загружаем каскады Хаара
    cv::CascadeClassifier face_cascade, eye_cascade, smile_cascade;
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        std::cerr << "Ошибка: Не удалось загрузить каскад для лиц!" << std::endl;
        return -1;
    }
    if (!eye_cascade.load("haarcascade_eye.xml")) {
        std::cerr << "Ошибка: Не удалось загрузить каскад для глаз!" << std::endl;
        return -1;
    }
    if (!smile_cascade.load("haarcascade_smile.xml")) {
        std::cerr << "Ошибка: Не удалось загрузить каскад для улыбок!" << std::endl;
        return -1;
    }

    // Получаем параметры видео
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    // Создаем объект для записи видео
    cv::VideoWriter video_writer("../../out/output_video.mp4", cv::VideoWriter::fourcc('M', 'P', '4', 'V'), fps, cv::Size(frame_width, frame_height));
    if (!video_writer.isOpened()) {
        std::cerr << "Ошибка: Не удалось создать файл для записи видео!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) { // Читаем кадры в цикле
        if (frame.empty()) {
            std::cerr << "Ошибка: Кадр пустой!" << std::endl;
            break;
        }

        // Преобразуем кадр в оттенки серого
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray); // Улучшаем контрастность

        // Детекция лиц
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Для каждого найденного лица
        for (size_t i = 0; i < faces.size(); i++) {
            // Рисуем прямоугольник вокруг лица (синий цвет)
            cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2);

            // Область лица для поиска глаз и улыбки
            cv::Mat faceROI = gray(faces[i]);

            // Детекция глаз
            std::vector<cv::Rect> eyes;
            eye_cascade.detectMultiScale(faceROI, eyes, 1.5, 5, 0, cv::Size(20, 20), cv::Size(50, 50));
            for (size_t j = 0; j < eyes.size(); j++) {
                // Корректируем координаты глаз относительно всего кадра
                cv::Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2,
                                     faces[i].y + eyes[j].y + eyes[j].height / 2);
                int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
                // Рисуем прямоугольник вокруг глаз (красный цвет)
                cv::rectangle(frame,
                              cv::Point(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y),
                              cv::Point(faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height),
                              cv::Scalar(0, 0, 255), 2);
            }

            // Детекция улыбки
            std::vector<cv::Rect> smiles;
            smile_cascade.detectMultiScale(faceROI, smiles, 1.5, 20, 0, cv::Size(20, 20));
            for (size_t j = 0; j < smiles.size(); j++) {
                // Корректируем координаты улыбки относительно всего кадра
                cv::rectangle(frame,
                              cv::Point(faces[i].x + smiles[j].x, faces[i].y + smiles[j].y),
                              cv::Point(faces[i].x + smiles[j].x + smiles[j].width, faces[i].y + smiles[j].y + smiles[j].height),
                              cv::Scalar(0, 255, 0), 2); // Зеленый цвет для улыбки
            }
        }

        // Отображаем кадр
        cv::imshow("Обнаружение лиц, глаз и улыбок", frame);

        // Записываем кадр в выходное видео
        video_writer.write(frame);

        // Выход по нажатию клавиши 'q'
        if (cv::waitKey(30) == 'q') {
            break;
        }
    }

    // Освобождаем ресурсы
    cap.release();
    video_writer.release();
    cv::destroyAllWindows();
    return 0;
}
