
import setuptools

setuptools.setup(
    name = 'protobpl',
    version = '1.0',
    description='Protoc plugin for generation of boost::python bindings',
    author = 'Johnny Graettinger',
    author_email = 'johnny531@gmail.com',
    url = 'http://www.github.com/dademurphy/protobpl',
    packages = ['protobpl'],
    entry_points = {
        'console_scripts': [
            'protoc-gen-bplbindings = protobpl.bindings_generator:main']},
)

