from urllib.request import urlopen
from bs4 import BeautifulSoup
import bs4


class Dictionary(object):

    def search(word):
        soup = BeautifulSoup(urlopen("http://dict.cn/" + word), "lxml")
        basicTag = soup.find_all("ul", class_="dict-basic-ul")[0]

        result = ''

        for child in basicTag.children:
            if not isinstance(child, bs4.NavigableString) and not child.has_attr("style"):
                for s in child.strings:
                    result += s
                result += '\n'

        return result
