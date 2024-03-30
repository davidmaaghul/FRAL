from skbuild import setup


setup(
    name="fral",
    version="0.0.1",
    packages=["fral"],
    author="David Maaghul",
    author_email="your.email@example.com",
    package_dir={"fral": "_fral/python/fral"},
    cmake_install_dir="_fral/python/"
)
