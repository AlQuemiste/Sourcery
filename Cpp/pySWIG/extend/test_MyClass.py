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

print("Python> instantiate Info:")
info = my.Info()
info.i = 42; info.x = -3.14

foo = lambda info_: print("Python> Info (i = {:d}, x = {:f})".format(info_.i, info_.x))

my_class.callPyFunction(foo, info)

print("Python> TEST MyClass: done.")
