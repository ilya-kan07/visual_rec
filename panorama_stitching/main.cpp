#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// Функция для обрезки черных областей
Mat cropBlackBorders(const Mat& image) {
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    vector<Point> nonZeroPoints;
    findNonZero(gray, nonZeroPoints);

    if (nonZeroPoints.empty()) {
        return image;
    }

    int minX = image.cols, maxX = 0, minY = image.rows, maxY = 0;
    for (const auto& point : nonZeroPoints) {
        if (point.x < minX) minX = point.x;
        if (point.x > maxX) maxX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.y > maxY) maxY = point.y;
    }

    Rect cropRect(minX, minY, maxX - minX + 1, maxY - minY + 1);
    return image(cropRect).clone();
}

int main() {
    Mat img1 = imread("../../res/image.png");
    Mat img2 = imread("../../res/image1.png");

    if (img1.empty() || img2.empty()) {
        cout << "Ошибка: Не удалось загрузить одно или оба изображения!" << endl;
        return -1;
    }

    Ptr<SIFT> sift = SIFT::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    sift->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    sift->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

    Ptr<DescriptorMatcher> matcher = FlannBasedMatcher::create();
    vector<vector<DMatch>> knn_matches;
    matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);

    // Применяем тест отношения для фильтрации хороших соответствий
    const float ratio_thresh = 0.75f;
    vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches);
    imshow("Matches", img_matches);
    imwrite("../../out/matches.jpg", img_matches);

    vector<Point2f> points1, points2;
    for (size_t i = 0; i < good_matches.size(); i++) {
        points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
        points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }

    Mat H = findHomography(points2, points1, RANSAC);

    if (H.empty()) {
        cout << "Ошибка: Не удалось вычислить гомографию!" << endl;
        return -1;
    }

    Mat warped_img2;
    warpPerspective(img2, warped_img2, H, Size(img1.cols + img2.cols, img1.rows));

    Mat panorama = Mat::zeros(warped_img2.rows, warped_img2.cols, warped_img2.type());
    img1.copyTo(panorama(Rect(0, 0, img1.cols, img1.rows)));
    warped_img2.copyTo(panorama, warped_img2 != 0);

    Mat cropped_panorama = cropBlackBorders(panorama);

    imshow("Panorama", cropped_panorama);
    imwrite("../../out/panorama.jpg", cropped_panorama);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
