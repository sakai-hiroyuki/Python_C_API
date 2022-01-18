from distutils import extension
from distutils.core import setup, Extension

extensions = [    
    Extension("point", ["src/point.c"])
]

setup(name="point", version="1.0.0", ext_modules=extensions)
