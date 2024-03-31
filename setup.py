from skbuild import setup


setup(
    name="fral",
    version="1.0.0",
    packages=["fral"],
    author="David Maaghul",
    author_email="dmaaghul95@gmail.com",
    package_dir={"fral": "_fral/python/fral"},
    cmake_install_dir="_fral/python/"
)
