#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//enum class State { CALCULATING, DISPLAYING };

const int SCR_Width = 1920;
const int SCR_Height = 1200;    //1080 for normal

class PicDispInfo
{
public:
    PicDispInfo(string fileName, sf::Vector2i origin, float resize, float duration)
        : fileName(fileName), origin(origin), scale(resize), duration(duration) {
    }

    string getFilename() { return fileName; }
    sf::Vector2i getOrigin() { return origin; }
    float getScale() { return scale; }
    float getDuration() { return duration; }
    
    void SetOrigin(int x, int y)
    {
        origin = { x, y };
    }
    string ToCSVString()
    {
        ostringstream oss;
        oss << fileName << "," << origin.x << "," << origin.y << "," << scale << "," << duration;
        return oss.str();
    }
private:
    string fileName;
    sf::Vector2i origin;
    float scale;
    float duration;
};

//---------------------
// 單一固定清單(內部載圖)
//---------------------
class PicListToPlay
{
public:
    PicListToPlay(string listFile, string sourcePath, vector<PicDispInfo>& playlist)
    {
        listFile = listFile;
        srcPath = sourcePath;
        photolist = playlist;
        //m_state = State::CALCULATING;   //not in use!
    }

    int AppendPicInfo(PicDispInfo picDispInfo)
    {
        //if (photolist.size() == 0) return -1;
        photolist.push_back(picDispInfo);
        return photolist.size();
    }

    sf::Image RenderNextPic(bool forward, float& scale, sf::Vector2i& org, float& dur);
private:
    //清單檔案: 絕對路徑+完整檔名; (混合清單檔案s時 不使用(設為String.Empty))
    string listFile;
    //檔案共同路徑 - null時 代表 (混合清單檔案s)採用PicDispInfo.FileName(為完整檔名!)
    string srcPath;
    //圖檔list,包含所有檔案與顯示資訊~
    vector<PicDispInfo> photolist;
    //--------------
    //pure private:
    //State m_state;
    int cur_img_idx = -1;
};