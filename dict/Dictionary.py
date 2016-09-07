from urllib.request import urlopen
from bs4 import BeautifulSoup
import bs4


class Dictionary(object):

    def __init__(self, dictType="dict"):
        if dictType == "dict":
            self.url = "http://dict.cn/"
            self.dictType = 0

    def search(self, word):
        if self.dictType == 0:
            return self.__searchDict(word)

    def __searchDict(self, word):
        soup = BeautifulSoup(urlopen(self.url + word), "lxml")
        result = ""

        tagList = soup.find_all("ul", class_="dict-basic-ul")
        if not tagList:
            tagList = soup.find_all("div", class_="basic clearfix")
            basicTag = tagList[0].contents[1]
        else:
            basicTag = tagList[0]

        for child in basicTag.children:
            if (not isinstance(child, bs4.NavigableString) and
                    not child.has_attr("style")):
                tmpList = []
                for s in child.strings:
                    tmpList.append(str(s).replace('；', ", "))
                result += ' '.join(tmpList) + '\n'

        return result
