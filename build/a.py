class A:
    def add(self,a,b):
        return a + b

class B:
    def add(self,a,b):
        return a + 1

class C(B,A):
    pass

b = C();
print b.add(1,2)
