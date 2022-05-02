def mk_module_1():
    with open("foo_module.py", 'w') as mdl:
        spc = ' ' * 4
        txt = "def bar():\n{sp}print(\"--This is foo.bar 1--\")\n{sp}return 1\n".format(sp=spc)
        mdl.write(txt)


def mk_module_2():
    with open("foo_module.py", 'w') as mdl:
        spc = ' ' * 4
        txt = "def bar():\n{sp}print(\"--This is foo.bar 2--\")\n{sp}return 2\n".format(sp=spc)
        mdl.write(txt)

def chk_modules():
    print("*** Module 1 ***")
    mk_module_1()
    import foo_module as foo
    foo.bar()

    print("*** Module 2 ***")
    mk_module_2()
    import foo_module as foo
    foo.bar()
