from HaiciDict import HaiciDict


class Dictionary(object):

    def __init__(self, dictType="dict"):
        if dictType == "dict":
            self.dict = HaiciDict()

    def search(self, word):
        return self.dict.search(word)
