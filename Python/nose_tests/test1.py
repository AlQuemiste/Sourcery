# https://stackoverflow.com/a/30764766

def setup_module():
    print("common_setup")

def teardown_module():
    print("common_teardown")

def test_1():
    print("test_1")

def test_2():
    print("test_2")
