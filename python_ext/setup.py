import os
from setuptools import setup, Extension
import numpy as np

module = Extension('game', sources=['main.cpp'], language='c++')

setup(name="game", ext_modules=[module], 
    include_dirs = [np.get_include(), "../src/"]
)