import MyClass as my

print("Python> TEST MyClass:")

def pyfunction():
    print("Hello from Python!")

print("Python> instantiate MyClass:")
my_class = my.MyClass()
print()

print("Python> configure MyClass:")
my_class.configure(pyfunction, 42)
print()

print("Python> call MyClass.name:")
my_class.name()
print()

print("Python> print MyClass repr:")
print(my_class)

print("Python> MyClass SWIG init:")
my_class_2 = my.MyClass(1, 2)
print(my_class_2)

print("Python> instantiate Info:")
info = my.Info()
info.i = 42; info.x = -3.14

foo = lambda info_: print("Python> Info (i = {:d}, x = {:f})".format(info_.i, info_.x))

my_class.callPyFunction(foo, info)

print()
print("Python> test FooWrapper")

foo = my.Foo(1)
r1 = foo.calc(-1)
print("Python> result_1 =", r1)

r2 = my_class.calcFoo(foo, -1)
print("Python> result_2 =", r2)

class MyFoo(my.Foo):
    def calc(i:int):
        return i + 2

r3 = my_class.calcFoo(MyFoo, -2)
print("Python> result_3 =", r3)

print("Python> TEST MyClass: done.")
