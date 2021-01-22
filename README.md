# BeerBottleData

This repo has pictures of brown beer bottles.  So if you want to try to train some kind of brown beer bottle classifier, here is some data for you.  Nearly all of the bottle images have no cap.  That will probably bias any training to favor capless bottles.  But I like to take the cap off and drink my beer!  In the few images with a cap, I put a twist-off cap back on the bottle.

I made a light box with a "green screen" background for photographing the bottles.  Because of the green background, I did not take any pictures of green or clear beer bottles.  It would be difficult to tell the difference between empty bottle pixels and background pixels.  I used my phone to take the pictures.  The JPG images have some metadata with the camera settings.  The camera settings may vary from picture to picture.

Data Set 1 has photos of 12 different beer bottles.  Some of the shots in Data Set 1 are slightly blurry.  I think I accidentally selected the wrong focus mode for those pictures.  So I did a reshoot with the same set of beer bottles.  That's why there's **Data Set 1** and **Data Set 1 Reshoot**.  There is also **Data Set 2** with 10 bottles and **Data Set 3** with 8 bottles.  All of the images are of a vertical bottle but I made slight rotations to the bottle between shots.  There are at least eight photos of every bottle.  The photos are *mostly* ordered by rotation:  front first, then roughly a 45 degree rotation counter-clockwise for each successive shot.  There are several breaks in that pattern.

The "Grab Cut" algorithm is reasonably effective at extracting the bottle from the green background.  See the **extractor** folder for a Windows console application that goes through Data Set 1 Reshoot, Data Set 2, and Data Set 3.  It scales down the images by a factor of 0.1, applies Grab Cut, and shows the resulting masked bottle image.

The **extractor** project was built and tested in Visual Studio 2015 on a Windows 7 machine with OpenCV 4.0.1.

The **extractor_vs2019** project was built and tested in Visual Studio 2019 on a Windows 10 machine with OpenCV 4.3.0.

Drink responsibly!

## Videos

Click the images for YouTube videos showing beer bottle data sets:

[![Gradient Template Demo Video](http://img.youtube.com/vi/T1v3UY_k9lg/0.jpg)](http://www.youtube.com/watch?v=T1v3UY_k9lg)

[![Gradient Template Demo Video](http://img.youtube.com/vi/eB1OTePfVxk/0.jpg)](http://www.youtube.com/watch?v=eB1OTePfVxk)

[![Gradient Template Demo Video](http://img.youtube.com/vi/r4XwJhrDc2U/0.jpg)](http://www.youtube.com/watch?v=r4XwJhrDc2U)
