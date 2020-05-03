
class A:
    def add(self,a,b):
        return a + b

class B(A):
    pass

a = B()
print a.add(1,2)
