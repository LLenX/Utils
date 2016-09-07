import bs4
from urllib.request import urlopen


class HaiciDict:

    def __init__(self):
        self.url = "http://dict.cn/"

    def search(self, word):
        self.soup = bs4.BeautifulSoup(urlopen(self.url + word), "lxml")
        self.resultList = []

        if self.__resultNotFound(word):
            return self.resultList[0]

        self.__getPhonetic()
        self.__getDescription()

        return '\n'.join(self.resultList)

    def __resultNotFound(self, word, count=5):
        rootList = self.soup.find_all("div", class_="unfind")
        if not rootList:
            return False

        rootTag = rootList[0]

        result = "未找到" + word + " 你要找的是不是:\n"
        suggTag = rootTag.contents[3]
        suggList = []
        for child in suggTag.children:
            # pick out the '\n's
            if not isinstance(child, bs4.NavigableString):
                entry = ""
                for s in child.strings:
                    entry += s + ' '
                suggList.append(entry)
                if len(suggList) == count:
                    break

        result += '\n'.join(suggList) + '\n'
        self.resultList.append(result)

        return True

    def __getDescription(self):
        result = ""

        # normal search
        tagList = self.soup.find_all("ul", class_="dict-basic-ul")
        if tagList:
            basicTag = tagList[0]
        else:
            # search like some abbrieviation
            tagList = self.soup.find_all("div", class_="basic clearfix")
            basicTag = tagList[0].contents[1]

        for child in basicTag.children:
            # pick the translation list part
            if (not isinstance(child, bs4.NavigableString) and
                    not child.has_attr("style")):
                tmpList = []
                for s in child.strings:
                    tmpList.append(str(s).replace('；', ", "))
                result += ' '.join(tmpList) + '\n'

        self.resultList.append(result)

    def __getPhonetic(self):
        divTag = self.soup.find_all("div", class_="phonetic")[0]
        result = "BrE: " + divTag.contents[1].contents[1].string + \
            "\nAmE: " + divTag.contents[3].contents[1].string
        self.resultList.append(result)
