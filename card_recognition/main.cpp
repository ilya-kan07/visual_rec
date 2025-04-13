#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

// Структура для хранения информации о карте
struct Card {
    string name;
    Mat image;
    vector<KeyPoint> keypoints;
    Mat descriptors;
};

// Функция для фильтрации совпадений
vector<DMatch> ratioTest(const vector<vector<DMatch>>& matches12, double ratio) {
    vector<DMatch> goodMatches;
    for (int i = 0; i < matches12.size(); i++) {
        if (matches12[i].size() == 2 && matches12[i][0].distance < ratio * matches12[i][1].distance) {
            goodMatches.push_back(matches12[i][0]);
        }
    }
    sort(goodMatches.begin(), goodMatches.end(), [](const DMatch& a, const DMatch& b) { return a.distance < b.distance; });
    if (goodMatches.size() > 5) {
        goodMatches.resize(5);
    }
    return goodMatches;
}

// Загрузка эталонных карт
vector<Card> loadReferenceCards() {
    vector<Card> referenceCards;
    vector<string> cardNames = {"2chervi", "3vinni", "5kresti", "9bybovii", "9kresti", "damachervi", "korolkresti", "tysbybovii", "valetvinni"};
    for (const auto& name : cardNames) {
        string path = "../../res/" + name + ".png";
        Mat image = imread(path);
        if (image.empty()) {
            cout << "Failed to load " << path << endl;
            continue;
        }

        Ptr<SIFT> sift = SIFT::create();
        vector<KeyPoint> keypoints;
        Mat descriptors;
        sift->detectAndCompute(image, noArray(), keypoints, descriptors);

        Card card;
        card.name = name;
        card.image = image;
        card.keypoints = keypoints;
        card.descriptors = descriptors;
        referenceCards.push_back(card);
        cout << "Loaded card: " << name << ", keypoints: " << keypoints.size() << endl;
    }
    return referenceCards;
}

int main() {
    Mat image = imread("../../res/image.png");
    if (image.empty()) {
        cout << "Failed to load the main image ../../res/image.png" << endl;
        return -1;
    }

    vector<Card> referenceCards = loadReferenceCards();
    if (referenceCards.empty()) {
        cout << "Failed to load reference cards" << endl;
        return -1;
    }

    Mat imageGray;
    cvtColor(image, imageGray, COLOR_BGR2GRAY);

    Ptr<SIFT> sift = SIFT::create();
    vector<KeyPoint> imageKeypoints;
    Mat imageDescriptors;
    sift->detectAndCompute(image, noArray(), imageKeypoints, imageDescriptors);

    Mat blurred;
    GaussianBlur(imageGray, blurred, Size(7, 7), 2);

    Mat edges;
    Canny(blurred, edges, 50, 200);

    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat resultImage = image.clone();
    drawContours(resultImage, contours, -1, Scalar(255, 0, 0), 3);

    BFMatcher matcher(NORM_L2);
    for (const auto& refCard : referenceCards) {
        if (refCard.descriptors.empty() || imageDescriptors.empty()) continue;

        vector<vector<DMatch>> knnMatches;
        matcher.knnMatch(imageDescriptors, refCard.descriptors, knnMatches, 2);

        auto goodMatches = ratioTest(knnMatches, 0.5);

        vector<Point2f> points;
        for (const auto& match : goodMatches) {
            points.push_back(imageKeypoints[match.queryIdx].pt);
        }

        if (!points.empty()) {
            putText(resultImage, refCard.name, points[0], FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
        }
    }

    imwrite("../../out/output.png", resultImage);
    cout << "Result saved to ../../out/output.png" << endl;

    imshow("Matches", resultImage);
    cv::waitKey(0);

    return 0;
}
