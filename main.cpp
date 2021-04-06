#include<iostream>
#include "inputMp3.h"
#include <cstdint>
#include<iostream>
#include<windows.h>
#include<codecvt>
#include<locale>
using namespace std;

int main(int argc,char *argv[]){
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> converter;
    string a ;
    cout << "歌詞を入力したいmp3ファイルのパスを入力してください";
    cin >> a; 
    string b ;
    cout << "歌詞ファイルのパスを入力してください";
    cin >> b; 
    u16string str1 = u"あいうえおかきくけこ";
    inputMp3 sample(a);
//    string c ;
//    cout << "歌詞ファイルのパスを入力してください";
//    cin >> c; 
//    converter.from_bytes(c);
    sample.makeLylicsData(b);
    sample.openMp3("new.mp3");
    sample.inputLilycs();
//    sample.inputAlubum(str1);
//    sample.inputConposer(str1);
//    sample.inputTitle(str1);
//    sample.inputArtist(str1);
    sample.inputMusicdata();
    return 0;
}
