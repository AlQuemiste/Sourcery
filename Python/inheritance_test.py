class Mother(object):
    a = 2

    def __init__(self, name):
        self.name = name.upper()
        self.id = 1
        self.q = -1
        print("Mother init")

    def say(self):
        print("MOTHER: %s" % self.name)
        print("q = %i" % self.q)


class Daughter(Mother):
    b = -2
    def __init__(self, name):
        #super(Daughter, self).__init__(name)
        Mother.__init__(self, name)
        self.name = name.lower()
        self.id = 11
        self.q += 1
        print("Daughter init")


    def say(self):
        print("DAUGHTER: %s" % self.name)
        print("q = %i" % self.q)

Y = Mother("Lady")
X = Daughter("Ammie")

Y.say()
X.say()
