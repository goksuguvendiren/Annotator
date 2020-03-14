#include <filesystem>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main(int argc, const char** argv)
{
    std::cerr << "hi\n";

    if (argc != 2)
    {
        std::cout << "Usage is : ./Annotater path_to_image_folder \n";
        return 0;
    }

    std::map<std::string, std::pair<float, float>> image_point_mapping;

    std::string path = std::string{argv[1]};
    std::cerr << "Image path is: " << path << "\n";

    cv::namedWindow("fundus_image", 1);

    for (const auto& entry : fs::directory_iterator(path))
    {
        std::cout << entry.path().filename() << std::endl;
        auto image = cv::imread(entry.path());

        std::pair<int, int> macula_point;
        cv::setMouseCallback(
            "fundus_image",
            [](int event, int x, int y, int flags, void* userdata) -> void {
                if (event == cv::EVENT_LBUTTONDBLCLK)
                {
                    auto point = (std::pair<int, int>*)(userdata);
                    point->first = x;
                    point->second = y;

                    std::cerr << "double clicked on : " << x << ", " << y << '\n';
                }
            },
            &macula_point);

        cv::imshow("fundus_image", image);
        auto key = cv::waitKey(0);

        std::cerr << "Saving point : " << (macula_point.first / (float)image.cols) << ", "
                  << (macula_point.second / (float)image.rows) << '\n';

        image_point_mapping.insert(
            std::make_pair(entry.path().filename(), std::pair<float, float>{macula_point.first / (float)image.cols,
                                                                            macula_point.second / (float)image.rows}));

        cv::Mat image_labeled(image);
        cv::circle(image_labeled, {macula_point.first, macula_point.second}, 5.f, {200, 200, 50}, -1);

        std::cerr << std::string(entry.path().filename()) + std::string("_labeled.png") << '\n';
        cv::imwrite(std::string(entry.path().filename()) + std::string("_labeled.png"), image_labeled);
    }

    std::ofstream labels;
    labels.open("labels.txt");

    for (auto& pair : image_point_mapping)
    {
        labels << pair.first << " " << pair.second.first << ", " << pair.second.second << '\n';
    }

    return 0;
}
