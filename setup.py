from setuptools import setup, Extension
import subprocess
import sys
try:
    import pybind11
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pybind11"])
finally:
    from pybind11.setup_helpers import Pybind11Extension


ext_modules = [
    Pybind11Extension(
        "random_events_lib",  # Python module name
        ["export/bindings.cpp", "random_events_lib/src/sigma_algebra.cpp", "random_events_lib/src/set.cpp", "random_events_lib/src/product_algebra.cpp"],  # C++ binding source
        include_dirs=["random_events_lib/include"],  # Include directory for C++ headers
        extra_compile_args=["-std=c++17", "-fPIC"],
    ),
]

setup(
    name="random_events_lib",
    version="0.0.1",
    ext_modules=ext_modules,
    zip_safe=False,
)
