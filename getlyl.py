from bs4 import BeautifulSoup as bs
import requests
import sys

def execute(argv):
    if len(argv) == 1:
        print("few args must two")
        return
    
    url = str(argv[1])
    res = requests.get(url).text
    
    soup = bs(res,"html.parser")
    article = soup.find("article", class_="contentsBox movie_box")
    div = soup.find("div", class_="hiragana")
    div.find("span", class_="rt").extract()
    
    print(div.get_text())
    
    words = div.get_text()
    f = open(argv[2],"w")
    f.write(words)
    f.close()

if __name__ == "__main__":
    execute(sys.argv)