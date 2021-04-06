#ifndef INCLUDE_GUARD_H
#define INCLUDE_GUARD_H
#include <cstring>
#include <vector>
#include<cstdio>
#include <cstdlib>
#include <fstream>
#include <bitset>

using namespace std;

class inputMp3{
protected:
    string musicPath;
    ifstream musicifs;
    ifstream lylicsifs;
    ofstream mp3ofs;
    int frameOffset = 10;
    char tagSize[4] = {(int8_t)0x00,(int8_t)0x00,(int8_t)0x15,(int8_t)0x00};
    int intTagSize = 0x1500;
    int musicFileSize;//mp3のファイルのサイズ
    int lylicsFileSize;//歌詞のファイルサイズ
    int8_t frameSize[4];
    int intFrameSize = 0;
    char* musicData;
    char* lylicsData; 
    int8_t encType[2] = {(unsigned int8_t)0x00,(unsigned int8_t)0x01};
    int8_t encUtf8[2] = {(unsigned int8_t)0x00,(unsigned int8_t)0x03};
    int8_t jpn[3] = {(unsigned int8_t)0x6A,(unsigned int8_t)0x70,(unsigned int8_t)0x6E};
    int8_t eng[3] = {(unsigned int8_t)0x65,(unsigned int8_t)0x6E,(unsigned int8_t)0x67};
    unsigned char BOM[2] = {static_cast<unsigned int8_t>(0xFF),static_cast<unsigned int8_t>(0xFE)};
    int8_t varboseBit[2] = {(unsigned int8_t)0x00,(unsigned int8_t)0x00};
    int8_t lylicFrame[4] = {(int8_t)0x55,(int8_t)0x53,(int8_t)0x4C,(int8_t)0x54};
    int8_t artistFrame[4] = {(int8_t)0x54,(int8_t)0x50,(int8_t)0x45,(int8_t)0x31};
    int8_t conposerFrame[4] = {(int8_t)0x54,(int8_t)0x43,(int8_t)0x4F,(int8_t)0x4D};
    int8_t alubmFrame[4] = {(int8_t)0x54,(int8_t)0x41,(int8_t)0x4C,(int8_t)0x42};
    int8_t titleFrame[4] = {(int8_t)0x54,(int8_t)0x49,(int8_t)0x54,(int8_t)0x32};
    int8_t magicFrame[3] = {(int8_t)0x49,(int8_t)0x44,(int8_t)0x33};
    int8_t version[2] = {(int8_t)0x04,(int8_t)0x00};
    int8_t flag = (int8_t)0x00;
    template <typename T>
    int arrLen(T* arr){
        return sizeof(arr)/sizeof(T); 
    } 
    template<typename T>
    void outFile(T* data){
    int arrlen = arrLen(data);
    for (int i = 0; i < arrlen;i++){
        mp3ofs << data[i];
     }
     mp3ofs.flush();
    }
public:
    inputMp3(string mp3FilePath);
    ~inputMp3();
    void getMusicData();
    void getLylicsData();
    bool openMp3(string fileName);
    void inputLilycs();
    void inputAlubum(u16string albumName);
    void inputTitle(u16string titleName);
    void inputConposer(u16string conposerName);
    void inputArtist(u16string artistName);
    void inputMusicdata();
    void deleteBytes();
    int charToInt(char* chArr);
    void intToChar(int num);
    void makeLylicsData(string lylicsPath);
    void tenByteSame();

};
#endif /* !INCLUDE_GUARD_H */


