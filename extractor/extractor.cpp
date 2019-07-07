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


bool wait_for_keypress(const int ct = 1)
{
    bool result = true;
    bool is_looping = true;
    int k = ct;

    // loop until 'n' (next) or 'q' (quit) or timeout
    while (is_looping && k--)
    {
        int c = (char)waitKey(1);
    
        if ((c == 'n') || (c == 'q'))
        {
            is_looping = false;
            result = (c == 'n');
        }
    }

    return result;
}


bool do_grab_cut(const std::string& rs, const double scale, const cv::Rect& rfgd)
{
    bool result = true;
    Mat bgdModel;
    Mat fgdModel;

    // read input image and resize it
    Mat img = imread(rs);
    Size imgsz = img.size();
    double img_scale = scale;
    cv::Size newsz = cv::Size(
        static_cast<int>(imgsz.width * img_scale),
        static_cast<int>(imgsz.height * img_scale));

    // make output mask image of same size as resized input image
    Mat new_img;
    resize(img, new_img, newsz);
    Mat mask = Mat::zeros(newsz, CV_8U);

    // initialize foreground rectangle
    int fac = static_cast<int>(1.0 / scale);
    Rect scaled_fgd = Rect(rfgd.x / fac, rfgd.y / fac, rfgd.width / fac, rfgd.height / fac);

    // attempt Grab Cut
    try
    {
        grabCut(new_img, mask, scaled_fgd, bgdModel, fgdModel, 3, GC_INIT_WITH_RECT);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        result = false;
    }

    // show mask if all is well
    if (result)
    {
        normalize(mask, mask, 0, 255, cv::NORM_MINMAX);
        imshow("Mask", mask);
        result = wait_for_keypress();
    }

    return result;
}


int main(int argc, char * argv[])
{
    double scale;
    Rect bottle_mask;
    std::list<std::string> listOfFiles;

    // good settings for Data Set 2 at scale 0.1
    bottle_mask = Rect(Point(710, 20), Point(1730, 3230));
    scale = 0.1;

    get_dir_list("C:\\work\\BeerBottleData\\Beer Bottle Data Set 2", "*.jpg", listOfFiles);
    
    for (const auto& rs : listOfFiles)
    {
        if (!do_grab_cut(rs, scale, bottle_mask))
        {
            break;
        }
    }

    destroyWindow("Mask");
    return 0;
}

