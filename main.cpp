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
    if (pixel_list.empty()){
        std::cout << "Pixel list is empty." << std::endl;
        return;
    }
	//use double = don't lose precision when adding up to 130k+ numbers
    double sum_r = 0.0;
    double sum_g = 0.0;
    double sum_b = 0.0;
    
    for (size_t i = 0; i < pixel_list.size(); ++i){
        sum_r += pixel_list[i].r;
        sum_g += pixel_list[i].g;
        sum_b += pixel_list[i].b;
    }
    //divide by total number of pixels to get final average
    std::cout << "Average R : " << sum_r / pixel_list.size() << std::endl;
    std::cout << "Average G : " << sum_g / pixel_list.size() << std::endl;
    std::cout << "Average B : " << sum_b / pixel_list.size() << std::endl;
}

void flip_vertically(std::vector<Pixel>& pixel_list) {
    if (pixel_list.empty()) return;

    // First, find the maximum y-value to know what we are flipping against
    // (For a 256 height image, max_y should be 255)
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
}

int main(int argc, char* argv[]) {
    // Step 3: Take a string from the command line for the file to read
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    std::ifstream infile(filename);
    
    if (!infile.is_open()) {
        std::cerr << "Error opening input file: " << filename << std::endl;
        return 1;
    }
    
    // Step 2: Create a vector called pixel_list
    std::vector<Pixel> pixel_list;
    std::string line;
    
    // Step 3: Parse using .find() and .substr()
    while (std::getline(infile, line)) {
        if (line.empty()) continue; // Skip any empty lines
        
        Pixel p;
        
        // Find positions of the commas
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);
        
        //extract the text between the commas and turn them into numbers
        p.x = std::stoi(line.substr(0, pos1));
        p.y = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        p.r = std::stof(line.substr(pos2 + 1, pos3 - pos2 - 1));
        p.g = std::stof(line.substr(pos3 + 1, pos4 - pos3 - 1));
        p.b = std::stof(line.substr(pos4 + 1));
        
        //store finished pixel into vector or the list
        pixel_list.push_back(p);
    }
    
    infile.close();
    std::cout << "\nSuccessfully read : " << pixel_list.size() << " pixels\n" << std::endl;
    
    //step 4
    average_colors(pixel_list);
    flip_vertically(pixel_list);
    
    std::sort(pixel_list.begin(), pixel_list.end(), sort_pixels);
    
    //step 5:save new pixel list to flipped.dat
    std::ofstream outfile("flipped.dat");
    if (!outfile.is_open()){
        std::cerr << "Error opening flipped.dat for writing" << std::endl;
        return 1;
    }
    
    for (size_t i = 0; i < pixel_list.size(); ++i) {
        outfile << pixel_list[i].x << ","
                << pixel_list[i].y << ","
                << pixel_list[i].r << ","
                << pixel_list[i].g << ","
                << pixel_list[i].b << "\n";
    }
    
    outfile.close();
    std::cout << "\nFlipped image saved to flipped.dat!" << std::endl;
    
    return 0;
}
