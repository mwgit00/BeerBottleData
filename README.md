# BeerBottleData

This repo has pictures of brown beer bottles.  So if you want to try to train some kind of brown beer bottle classifier, here is some data for you.  I made a light box with a "green screen" background for photographing the bottles.  Because of the green background, I did not take any pictures of green or clear beer bottles.  I used my phone to take the pictures.  The JPG images have some metadata with the camera settings.  The camera settings may vary from picture to picture.

Data Set 1 has photos of 12 different beer bottles.  They are all vertical but I made slight rotations to the bottle between shots.  Some of the shots in Data Set 1 are slightly blurry.  I think I accidentally selected the wrong focus mode for those pictures.  So I did a reshoot with the same set of beer bottles.  That's why there's Data Set 1 and Data Set 1 Reshoot.  There is also Data Set 2 with 10 more bottles.

The "Grab Cut" is reasonably effective at extracting the bottle from the green background.  See the extractor folder for a Windows console application that goes through Data Set 2, scales down the images by a factor of 0.1, applies Grab Cut, and shows the resulting bottle mask.

I have more bottles and will try to take pictures of them as time permits.  The photos are stored at full resolution and eat up a lot of space.  If the repo gets too big, I may delete the first Data Set 1 with the blurry images.
