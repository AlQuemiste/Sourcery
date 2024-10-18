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

print("Python> TEST MyClass: done.")
