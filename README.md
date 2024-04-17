Using object detection, creating a plugin that automatically masks images or videos. Very useful for asset generation; and saves a lot of time. 


Though this product already exists for Adobe Premiere Pro; it is locked behind a $100+ paywall. This will be a free plugin for all.  

This project was inspired from the following websites:
remove.bg
runway.ml

These are good free alternatives to use, but costs time:
  -1. Export asset
  -2. Import into one of the above websites
  -3. Wait, and then download the output
  -4. Import into premiere pro

This project will cut out the "middleman", no need to export or import any new assets
-1. Right click on asset
-2. Generate mask
-3. Done 

Plan:
  -Exporting RGB value data from Premiere Pro to get format of data. 
  -With the format of the data, we can now train our Object Detection Model 
  -Integrate Plugin to Premiere pro 

  I am starting with automatic image masking; then once that is completed, I will start automatically masking the video.
