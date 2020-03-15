#include <filesystem>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

std::string to_string(int i)
{
    std::string res = "0";
    if (i < 10)
        return "000" + std::to_string(i);
    else if (i < 100)
        return "00" + std::to_string(i);

    return "0" + std::to_string(i);
}

int main(int argc, const char** argv)
{
    std::map<std::string, std::pair<float, float>> image_point_mapping;

    std::string path = std::string{"/Users/goksu/Documents/UCSB/2019-2020/Winter/CS291I/project/0.0.Normal"};
    std::cerr << "Image path is: " << path << "\n";

    cv::namedWindow("fundus_image", 1);

    for (int i = 1; i < 483; ++i)
    {
        std::string num = to_string(i);
        std::string image_name = "/Im" + num + "_ORIGA.jpg";

        std::cerr << path + image_name << '\n';

        auto image = cv::imread(path + image_name);

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
            std::make_pair(image_name, std::pair<float, float>{macula_point.first / (float)image.cols,
                                                                            macula_point.second / (float)image.rows}));

        cv::Mat image_labeled(image);
        cv::circle(image_labeled, {macula_point.first, macula_point.second}, 5.f, {200, 200, 50}, -1);

        std::cerr << std::string(image_name) + std::string("_labeled.png") << '\n';
        cv::imwrite(std::string(image_name) + std::string("_labeled.png"), image_labeled);
    }

    std::ofstream labels;
    labels.open("labels.txt");

    for (auto& pair : image_point_mapping)
    {
        labels << pair.first << " " << pair.second.first << ", " << pair.second.second << '\n';
    }

    return 0;
}
