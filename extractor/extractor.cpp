// extractor.cpp : Uses GrabCut to extract beer bottles from green backgrounds in the data set images.
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


bool do_grab_cut(const std::string& rs, const double scale, const cv::Rect& rfgd, const int iter_ct = 1)
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

    // initialize scaled foreground rectangle
    int fac = static_cast<int>(1.0 / scale);
    Rect scaled_fgd = Rect(rfgd.x / fac, rfgd.y / fac, rfgd.width / fac, rfgd.height / fac);

    // BGR -> XYZ* YCrCb* HSV Lab Luv HLS YUV*
    // some of the BGR images have "blooming" in the green background because they're overexposed
    // sometimes that blooming was being miscategorized as a foreground pixel
    // switching to different a color space seems to provide better segmentation
    // asterisks indicate color conversions that work well (YUV may be best overall)
    Mat new_color_img;
    cvtColor(new_img, new_color_img, COLOR_BGR2YUV);

    // attempt Grab Cut
    try
    {
        // do at least one iteration initialized from mask around probable bottle
        grabCut(new_color_img, mask, scaled_fgd, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

        // perform optional iterations of GrabCut
        // keep going until limit reached or mask stops changing
        int n;
        for (n = 1; n < iter_ct; n++)
        {
            Mat mask_copy;
            mask.copyTo(mask_copy);
            grabCut(new_color_img, mask, scaled_fgd, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);

            cv::Mat D;
            absdiff(mask, mask_copy, D);
            if (cv::sum(D) == cv::Scalar::all(0))
            {
                break;
            }
        }

        std::cout << rs << " " << n << std::endl;
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        result = false;
    }

    // show mask if all is well
    if (result)
    {
        // modify mask so it only has foreground pixels
        mask = (mask >= GC_PR_FGD);

        // TODO -- use some morphology operators to clean up the mask (fill holes, remove speckles, etc.)
#if 0
        int morph_size = 2;
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
        morphologyEx(new_img, mg, MORPH_GRADIENT, element);
#endif

        if (false)
        {
            // a normalization is required if displaying just the mask
            normalize(mask, mask, 0, 255, cv::NORM_MINMAX);
            imshow("Mask", mask);
        }
        else
        {
            // create a new blank color image and copy masked bottle into it
            Mat x = Mat::zeros(newsz, CV_8UC3);
            new_img.copyTo(x, mask);
            imshow("Mask", x);
        }
        result = wait_for_keypress();
    }

    return result;
}


int main(int argc, char * argv[])
{
    double scale;
    Rect bottle_mask;
    std::list<std::string> listOfDataSets;

    listOfDataSets.push_back("C:\\work\\BeerBottleData\\Beer Bottle Data Set 1 Reshoot");
    listOfDataSets.push_back("C:\\work\\BeerBottleData\\Beer Bottle Data Set 2");
    listOfDataSets.push_back("C:\\work\\BeerBottleData\\Beer Bottle Data Set 3");

    // all bottles should be in this region in full-sized images
    bottle_mask = Rect(Point(690, 20), Point(1770, 3230));

    // scale things down for quicker processing
    // use a scale factor whose reciprocal is an integer
    // some "blooming" boundaries may be seen with larger scale factors
    scale = 0.1;

    bool is_halted = false;
    for (const auto& rsdata : listOfDataSets)
    {
        std::list<std::string> listOfFiles;
        get_dir_list(rsdata, "*.jpg", listOfFiles);
        for (const auto& rs : listOfFiles)
        {
            if (!do_grab_cut(rs, scale, bottle_mask))
            {
                is_halted = true;
                break;
            }
        }

        if (is_halted)
        {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}
