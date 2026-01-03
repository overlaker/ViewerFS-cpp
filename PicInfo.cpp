#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "PicInfo.h"

using namespace std;

sf::Image PicListToPlay::RenderNextPic(bool forward, float& ratio, sf::Vector2i& org, float& dur)
{
    forward ? cur_img_idx++ : cur_img_idx--;
    if (cur_img_idx == photolist.size()) cur_img_idx = 0;
    else if (cur_img_idx == -1) cur_img_idx = photolist.size() - 1;
    PicDispInfo pic = photolist.at(cur_img_idx);

    string img_file = srcPath.substr(1) + "\\" + pic.getFilename();
    cout << "displaying file: " << img_file << endl;
    
    bool _extra_enable = false;  //為比較基準一致,暫且關閉此功能...
    int _extra_h = SCR_Height - 1080;

    int width, height, num_channels;
    unsigned char* image_data = stbi_load(img_file.c_str(), &width, &height, &num_channels, 0);

    if (image_data) {
        std::cout << "Image loaded successfully!" << std::endl;
        std::cout << "Width: " << width << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Channels: " << num_channels << std::endl;

        ratio = pic.getScale();
        org = pic.getOrigin();
        dur = pic.getDuration();

        float new_x = org.x * ratio;
        if (org.x > 0) new_x = SCR_Width - width * ratio;   //靠右對齊; picList檔中設定為<0(上面已變號!)~
        if (width * ratio < SCR_Width)
        {
            //水平置中
            new_x = (SCR_Width - width * ratio) / 2.0;
            cout << "Middle_X: " << new_x << endl;
        }

        float new_y = org.y * ratio;
        if (org.y > 0) new_y = SCR_Height - height * ratio;  //靠底(下)對齊; picList檔中設定為<0(上面已變號!)~
        if (height * ratio <= SCR_Height)
        {
            //垂直置中
            new_y = (SCR_Height - height * ratio) / 2;
            cout << "Middle_Y: " << new_x << endl;
        }
        else if (_extra_h > 0 && org.y < 0 && _extra_enable)
        {
            //特高螢幕,可(盡量)降低圖片
            new_y = (org.y + _extra_h) * ratio;
            if (new_y > 0) new_y = 0;
        }

        org = { (int)new_x, (int)new_y };

        // Example: Accessing a pixel (assuming RGBA and 4 channels)
        // Note: Pixel data is typically row-major, so (y * width + x) * num_channels
        // if (num_channels == 4) {
        //     unsigned char red = image_data[0]; // First pixel, red component
        //     std::cout << "Red of first pixel: " << (int)red << std::endl;
        // }
        unsigned char* img_disp_data;
        if (num_channels == 3)
        {
            //convert from RGB to RGBA
            const int rgba_size = width * height * 4;
            img_disp_data = new unsigned char[rgba_size]();
            //iterate through each pixel:
            for (int i = 0; i < width * height; ++i) {
                // Calculate indices for RGB and RGBA arrays
                int rgb_index = i * 3;
                int rgba_index = i * 4;

                // Copy R, G, B values
                img_disp_data[rgba_index] = image_data[rgb_index];         // Red
                img_disp_data[rgba_index + 1] = image_data[rgb_index + 1]; // Green
                img_disp_data[rgba_index + 2] = image_data[rgb_index + 2]; // Blue

                // Add the alpha value
                img_disp_data[rgba_index + 3] = 255;
            }
        }
        else
        {
            cout << "++ A " << num_channels << " channels file! ++" << endl;
            img_disp_data = image_data;
        }

        //Create an sf::Image from the loaded data
        sf::Image image;
        image.create(width, height, img_disp_data);

        //Free the stb_image allocated memory.
        stbi_image_free(image_data);

        return image;
    }
    else {
        std::cerr << "Failed to load image." << std::endl;
        return sf::Image();
    }
}
