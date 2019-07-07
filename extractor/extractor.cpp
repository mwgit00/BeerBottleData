// extractor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Windows.h"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include <list>
#include <vector>
#include <iostream>
#include <sstream>

using namespace cv;



void get_dir_list(
    const std::string& rsdir,
    const std::string& rspattern,
    std::list<std::string>& listOfFiles)
{
    std::string s = rsdir + "\\" + rspattern;

    WIN32_FIND_DATA search_data;
    memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

    HANDLE handle = FindFirstFile(s.data(), &search_data);

    while (handle != INVALID_HANDLE_VALUE)
    {
        std::string sfile(search_data.cFileName);
        listOfFiles.push_back(rsdir + "\\" + sfile);
        if (FindNextFile(handle, &search_data) == FALSE)
        {
            break;
        }
    }

    FindClose(handle);
}


bool wait_for_keypress()
{
    bool result = true;
    bool is_looping = true;
    int c;
    
    while (is_looping)
    {
        c = (char)waitKey(0);
    
        if ((c == 'n') || (c == 'q'))
        {
            is_looping = false;
        }
    }

    return (c == 'n');
}


bool do_grab_cut(const std::string& rs, const double scale)
{
    bool result = true;
    
    Mat img = imread(rs);
    Size imgsz = img.size();
    double img_scale = scale;
    cv::Size newsz = cv::Size(
        static_cast<int>(imgsz.width * img_scale),
        static_cast<int>(imgsz.height * img_scale));

    Mat new_img;
    resize(img, new_img, newsz);
    Mat mask = Mat::zeros(newsz, CV_8U);

    int fac = static_cast<int>(1.0 / scale);

    // good settings for Data Set 2 at scale 0.1
    Rect bottle_mask = Rect(Point(710 / fac, 20 / fac), Point(1730 / fac, 3230 / fac));
    Mat bgdModel;
    Mat fgdModel;

    try
    {
        grabCut(new_img, mask, bottle_mask, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        result = false;
    }

    normalize(mask, mask, 0, 255, cv::NORM_MINMAX);

    imshow("Mask", mask);
    result = wait_for_keypress();
    return result;
}


int main(int argc, char * argv[])
{
    double scale = 0.1;
    std::list<std::string> listOfFiles;
    get_dir_list("C:\\work\\BeerBottleData\\Beer Bottle Data Set 2", "*.jpg", listOfFiles);
    
    for (const auto& rs : listOfFiles)
    {
        if (!do_grab_cut(rs, scale))
        {
            break;
        }
    }

    destroyWindow("hey2");
    destroyWindow("Mask");
    return 0;
}

