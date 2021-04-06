#include <cmath>
#include<iostream>
#include "inputMp3.h"

using namespace std;

//ファイルを開くためのパスを受け取るコンストラクタ
inputMp3::inputMp3(string mp3FilePath){
    musicPath = mp3FilePath;
    musicifs.open(musicPath,ios_base::binary);
    if (musicifs.fail()){
        cout << "Cannot open file" << endl;
        return;
    }
    //mp3ファイルのサイズ取得
    musicifs.seekg(0,ios::end);
    musicFileSize = musicifs.tellg();//バイト数獲得
    musicifs.seekg(0,ios::beg);//先頭にアドレスをもどす
    musicData = new char[musicFileSize];
    getMusicData();
}

//破棄するときはファイルクローズ
inputMp3::~inputMp3(){
    lylicsifs.close();
    musicifs.close();
    delete[] musicData;
    delete[] lylicsData;
}

//mp3情報を動的配列に格納
void inputMp3::getMusicData(){
    memset(musicData,(int8_t)0x00,sizeof(musicData));//0クリア
    musicifs.read(musicData,musicFileSize);//バイナリデータを配列に格納
    version[0] = musicData[3]; //バージョン情報を取得
    version[1] = musicData[4]; 
    deleteBytes();
}

//テキスト情報を動的配列に格納
void inputMp3::getLylicsData(){
    memset(lylicsData,0x00,sizeof(lylicsData));//0クリア
    lylicsifs.read(lylicsData,lylicsFileSize);//バイナリデータを配列に格納 
    intFrameSize = lylicsFileSize + 10;//フレームサイズを取得
}

void inputMp3::makeLylicsData(string lylicsPath) {
    //歌詞ファイルのサイズ取得
    lylicsifs.open(lylicsPath,ios_base::binary);
    if(lylicsifs.fail()){
        cout << "Cannot open file" << endl;
        return;
    }
    lylicsifs.seekg(0,ios_base::end);
    lylicsFileSize = lylicsifs.tellg();//バイト数獲得
    lylicsifs.seekg(0,ios_base::beg);//先頭にアドレスをもどす
    lylicsData = new char[lylicsFileSize];
    getLylicsData();
}

void inputMp3::deleteBytes(){
    int deleteNum;
    int binaryDataLen = sizeof(musicData)/sizeof(char);
    for (int i = 0;i < intTagSize - 3;i++){
        if (lylicFrame[0] == musicData[i] && lylicFrame[1] == musicData[i+1]
            && lylicFrame[2] == musicData[i+2] && lylicFrame[3] == musicData[i+3]){
                int offset = i;
                unsigned int dataSize = (musicData[offset+4] >> 24) + (musicData[offset+5] >> 16) + (musicData[offset+6] >> 8) + (musicData[offset+7]);
                for (int j = 0;j < dataSize + 8;j++){
                    musicData[offset + j] = (int8_t)0x00;
                }
                return;
            } else {
                musicData[i] = (int8_t)0x00;
            }
    }
}

//タグの生成
void inputMp3::tenByteSame(){
    //タグの１０バイト
//    mp3ofs << magicFrame << version << flag << tagSize ;//3+2+1+4=10
    for(int i = 0;i < 3; i++){
        mp3ofs << magicFrame[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << version[i];
    }
    mp3ofs << flag;
    for(int i = 0;i < 4; i++){
        mp3ofs << tagSize[i];
    }
//    mp3ofs << lylicFrame << intToChar(intFrameSize) << flag << encType << jpn << BOM << varboseBit << lylicsData << musicData;
}

//ファイル開く
bool inputMp3::openMp3(string fileName) {
    mp3ofs.open(fileName,ios_base::binary);
    if (mp3ofs.fail()){
        cout << "Cannot open File" << endl;
        return false;
    }
    tenByteSame();
    return true;
}
//ファイルに出力
//歌詞ファイル
void inputMp3::inputLilycs(){
    //タグの１０バイト
//    mp3ofs << magicFrame << version << flag << tagSize ;//3+2+1+4=10
    for(int i = 0;i < 4; i++){
        mp3ofs << lylicFrame[i];
    }
    intToChar(intFrameSize);
    for(int i = 0;i < 4; i++){
        mp3ofs << frameSize[i];
    }
    mp3ofs << flag;
    for(int i = 0;i < 2; i++){
        mp3ofs << encType[i];
    }
    for(int i = 0;i < 3; i++){
        mp3ofs << jpn[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << BOM[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << varboseBit[i];
    }
    for(int i = 0;i < lylicsFileSize; i++){
        mp3ofs << lylicsData[i];
    }
    for(int i = 0;i < 2; i++){
       mp3ofs << varboseBit[i];
    }
}
//アルバムファイル
void inputMp3::inputAlubum(u16string alubumName){
    for(int i = 0;i < 4; i++){
        mp3ofs << alubmFrame[i];
    }
    intFrameSize = alubumName.length() * 2 + 5;
    intToChar(intFrameSize);
    for(int i = 0;i < 4; i++){
        mp3ofs << frameSize[i];
    }
    mp3ofs << flag;
    for(int i = 0;i < 2; i++){
        mp3ofs << encType[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << BOM[i];
    }
    for(int i = 0;i < alubumName.length(); i++){
        int buffer = 0;
        int num = alubumName[i];
        int8_t littleEndian[2];
        for(int j = 1;j >= 0;j--){
            buffer = num / pow(256,j);
            littleEndian[j] = buffer;
            num -= buffer*pow(256,j); 
        }
       littleEndian[0] > littleEndian[1] ? mp3ofs << littleEndian[0] << littleEndian[1] : mp3ofs << littleEndian[1] << littleEndian[0];
    }
    for(int i = 0;i < 2; i++){
       mp3ofs << varboseBit[i];
    }
}
//タイトル
void inputMp3::inputTitle(u16string titleName){
    for(int i = 0;i < 4; i++){
        mp3ofs << titleFrame[i];
    }
    intFrameSize = titleName.length() * 2 + 5;
    intToChar(intFrameSize);
    for(int i = 0;i < 4; i++){
        mp3ofs << frameSize[i];
    }
    mp3ofs << flag;
    for(int i = 0;i < 2; i++){
        mp3ofs << encType[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << BOM[i];
    }
    for(int i = 0;i <titleName.length(); i++){
        int buffer = 0;
        int num = titleName[i];
        int8_t littleEndian[2];
        for(int j = 1;j >= 0;j--){
            buffer = num / pow(256,j);
            littleEndian[j] = buffer;
            num -= buffer*pow(256,j); 
        }
       littleEndian[0] > littleEndian[1] ? mp3ofs << littleEndian[0] << littleEndian[1] : mp3ofs << littleEndian[1] << littleEndian[0];
    }
    for(int i = 0;i < 2; i++){
       mp3ofs << varboseBit[i];
    }
}
//アーティスト名
  void inputMp3::inputArtist(u16string artistName){
    for(int i = 0;i < 4; i++){
        mp3ofs << artistFrame[i];
    }
    int count = 0;
    intFrameSize = artistName.length() * 2+ 5;
    intToChar(intFrameSize);
    for(int i = 0;i < 4; i++){
        mp3ofs << frameSize[i];
    }
    mp3ofs << flag;
    for(int i = 0;i < 2; i++){
        mp3ofs << encType[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << BOM[i];
    }
    for(int i = 0;i < artistName.length(); i++){
        int buffer = 0;
        int num = artistName[i];
        int8_t littleEndian[2];
        for(int j = 1;j >= 0;j--){
            buffer = num / pow(256,j);
            littleEndian[j] = buffer;
            num -= buffer*pow(256,j); 
        }
       littleEndian[0] > littleEndian[1] ? mp3ofs << littleEndian[0] << littleEndian[1] : mp3ofs << littleEndian[1] << littleEndian[0];
    }
    for(int i = 0;i < 2; i++){
       mp3ofs << varboseBit[i];
    }
  }
  //作曲者
  void inputMp3::inputConposer(u16string conposerName){
    for(int i = 0;i < 4; i++){
        mp3ofs << conposerFrame[i];
    }
    int count = 0;
    intFrameSize = conposerName.length() * 2+ 5;
    intToChar(intFrameSize);
    for(int i = 0;i < 4; i++){
        mp3ofs << frameSize[i];
    }
    mp3ofs << flag;
    for(int i = 0;i < 2; i++){
        mp3ofs << encType[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << BOM[i];
    }
    for(int i = 0;i < conposerName.length(); i++){
        int buffer = 0;
        int num = conposerName[i];
        int8_t littleEndian[2];
        for(int j = 1;j >= 0;j--){
            buffer = num / pow(256,j);
            littleEndian[j] = buffer;
            num -= buffer*pow(256,j); 
        }
       littleEndian[0] > littleEndian[1] ? mp3ofs << littleEndian[0] << littleEndian[1] : mp3ofs << littleEndian[1] << littleEndian[0];
    }
    for(int i = 0;i < 2; i++){
       mp3ofs << varboseBit[i];
    }
  }

void inputMp3::inputMusicdata(){
    for(int i = 0;i < musicFileSize; i++){
        mp3ofs << musicData[i];
    }
    for(int i = 0;i < 2; i++){
        mp3ofs << varboseBit[i];
    }
    mp3ofs.close();
}

int inputMp3::charToInt(char* chArr){
    int chArrLen = sizeof(chArr)/sizeof(char*);
    int size= 0;
    for (int i = 0;i < chArrLen; i++){
        size += chArr[i] >> 8*(chArrLen-1-i);
    }
    return size;
}

void inputMp3::intToChar(int num){
    int insertNum;
    for(int i = 3; i >= 0; i--){
        insertNum = num/pow(256,i);
        frameSize[3-i] = (unsigned int8_t)(insertNum);
        num -= insertNum*pow(256,i);
    }
}
