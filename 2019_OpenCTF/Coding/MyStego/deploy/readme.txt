[Infp]
Title:MyStego
Description:It's just a simple code.
Attach:MyStego.zip

[Deploy]
 Not necessary

[Solving Strategy]
 1. analyze source code working
 2. for each pixel, we can get one data bit as (red^green^blue) ^ 1
 3. concatenate the bits together, then it is a bitmap image file
 4. get the flag from the bitmap image!
