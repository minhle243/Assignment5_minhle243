#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>

#include"pixel.h"

//sort by x,if the same then -> sort by y
bool sort_pixels(const Pixel& a, const Pixel& b) {
    if (a.x == b.x) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

//calc average of the r, g, b values
void average_colors(std::vector<Pixel>& pixel_list){

    //the sums are doubles instead of floats so we dont lose precision when adding up 130k+ numbers
    double sum_r = 0.0;
    double sum_g = 0.0;
    double sum_b = 0.0;
    // loop through every pixel and add its r, g, b
    for (int i = 0; i < pixel_list.size(); ++i){
        sum_r += pixel_list[i].r;
        sum_g += pixel_list[i].g;
        sum_b += pixel_list[i].b;
    }
    //divide by total number of pixels to get final average
    int n = pixel_list.size();
    std::cout << "Average R : " << sum_r / n << std::endl;
    std::cout << "Average G : " << sum_g / n << std::endl;
    std::cout << "Average B : " << sum_b / n << std::endl;
}
// flip vertically
void flip_vertically(std::vector<Pixel>& pixel_list) {
    if (pixel_list.empty()) return;

    // First, find the maximum y-value to know what we are flipping against
    // The y-length of the image is 256, so max y should be 255
    int max_y = 0;
    for (size_t i = 0; i < pixel_list.size(); ++i) {
        if (pixel_list[i].y > max_y) {
            max_y = pixel_list[i].y;
        }
    }

    // Flip the y-coordinates
    for (size_t i = 0; i < pixel_list.size(); ++i) {
        pixel_list[i].y = max_y - pixel_list[i].y;
    }

    // std::sort (from <algorithm>) puts the vector in order using sort_pixels
    // so flipped.dat is in the same order as pixels.dat (0,0 0,1 0,2 ...)
    std::sort(pixel_list.begin(), pixel_list.end(), sort_pixels);
}

int main(int argc, char* argv[]) {
    // Step 3: Take a string from the command line for the file to read
    // argv[0] is the program name and argv[1] is the file name so if argc < 2 no file name is given
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <pixels.dat>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "Error opening: " << filename << std::endl;
        return 1;
    }

    // Step 2: vector that stores every pixel from the file
    std::vector<Pixel> pixel_list;
    std::string line;

    // Step 3: read each line (x, y, r, g, b) and split it with .find and .substr
    while (std::getline(infile, line)) {
        if (line.empty()) continue; // Skip any empty lines

        // Find positions of the commas
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        //grab the text between the commas and convert to int/float
        Pixel p;
        p.x = std::stoi(line.substr(0, pos1));
        p.y = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        p.r = std::stof(line.substr(pos2 + 1, pos3 - pos2 - 1));
        p.g = std::stof(line.substr(pos3 + 1, pos4 - pos3 - 1));
        p.b = std::stof(line.substr(pos4 + 1));

        //store finished pixel into vector or the list
        pixel_list.push_back(p);
    }

    infile.close();
    std::cout << "Read: " << pixel_list.size() << " pixels\n" << std::endl;

    //step 4 average the colors, then flip the image vertically
    average_colors(pixel_list);
    flip_vertically(pixel_list);

    //step 5:save new pixel list to flipped.dat
    std::ofstream outfile("flipped.dat");
    for (int i = 0; i < pixel_list.size(); i++) {
        outfile << pixel_list[i].x << "," << pixel_list[i].y << ","
                << pixel_list[i].r << "," << pixel_list[i].g << ","
                << pixel_list[i].b << std::endl;
    }
    outfile.close();

    std::cout << "Saved to flipped.dat" << std::endl;
    return 0;
}
