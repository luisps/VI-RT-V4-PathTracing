//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>

void ImagePPM::ToneMap () {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].R) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].G) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].B) * 255);
        }
    }

}

// Details and code on PPM files available at:
// https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/reading-writing-images.html

bool ImagePPM::Save (std::string filename) {
    if (W == 0 || H == 0) { fprintf(stderr, "Can't save an empty image\n"); return false; }
    
    // convert from float to {0,1,..., 255}
    ToneMap();
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);  //need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << W << " " << H << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < W * H; ++i) {
            r = imageToSave[i].val[0];
            g = imageToSave[i].val[1];
            b = imageToSave[i].val[2];
            ofs << r << g << b;
        }
        ofs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return  false;
    }
}

bool ImagePPM::Load (std::string filename) {
    std::ifstream ifs;
     ifs.open(filename, std::ios::binary);
     // need to spec. binary mode for Windows users
     //Image src;
     try {
         if (ifs.fail()) {
             throw("Can't open input file");
             return false;
         }
         std::string header;
         int w, h, b;
         ifs >> header;
         if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
         ifs >> w >> h >> b;
         W = w;
         H = h;
         imagePlane =  new RGB[W*H];
         ifs.ignore(256, '\n');  //skip empty lines in necessary until we get to the binary data
         unsigned char pix[3];  //read each pixel one by one and convert bytes to floats
         for (int i = 0; i < w * h; ++i) {
             ifs.read(reinterpret_cast<char *>(pix), 3);
             imagePlane[i].R = pix[0] / 255.f;
             imagePlane[i].G = pix[1] / 255.f;
             imagePlane[i].B = pix[2] / 255.f;
         }
         ifs.close();
     }
     catch (const char *err) {
         fprintf(stderr, "%s\n", err);
         ifs.close();
         return false;
     }
  
     return true;
}
