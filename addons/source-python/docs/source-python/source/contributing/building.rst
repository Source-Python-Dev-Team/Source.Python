Building
========

On this page you will learn on how to build the Source.Python binaries
(``source-python.so``, ``source-python.dll``, ``core.so`` and ``core.dll``) on
your own. This allows you to do tests or contribute to the C++ side of
Source.Python by applying fixes, making improvements or exposing useful stuff
to the Python side.


Linux
-----

To build Source.Python on Linux, you need the following requirements:

* `CMake 2.8 or later <https://cmake.org/download/>`_
* `Git <https://git-scm.com/downloads>`_
* `GCC (our buildbot uses 4.8) <https://gcc.gnu.org/>`_

Once you have installed the requirements, follow these steps to build Source.Python:

1. Create a clone of the `Source.Python repository <https://github.com/Source-Python-Dev-Team/Source.Python/>`_.
2. Navigate to the ``src`` directory.
3. Start the ``Build.sh`` script.
4. Choose the game for which you want to build Source.Python.

.. note::

    By default only one CPU core is used to build the binaries. If you wish to
    use more, set the variable ``NUM_CORES`` to the number of core you would
    like to use. This will speed up the build process, but also requires more
    resources.

You can shorten these steps by simply passing the game's name to the script
e.g. ``./Build.sh css``.

If the build finished successfully, it will say something like this:
``[100%] Built target core``.

The compiled binaries are stored in ``src/Builds/Linux/<game>/Release``.


Windows
-------

To build Source.Python on Windows, you need the following requirements:

* `CMake 2.8 or later <https://cmake.org/download/>`_
* `Git <https://git-scm.com/downloads>`_
* `Visual C++ 2010 <https://my.visualstudio.com/Downloads?q=visual%20studio%202010&wt.mc_id=o~msft~vscom~older-downloads>`_
* `Microsoft Build Tools (optional, to build directly via Build.bat) <https://www.microsoft.com/en-us/download/details.aspx?id=48159>`_

Once you have installed the requirements, follow these steps to build Source.Python:

1. Create a clone of the `Source.Python repository <https://github.com/Source-Python-Dev-Team/Source.Python/>`_.
2. Navigate to the ``src`` directory.
3. Start the ``Build.bat`` script.
4. Choose the game for which you want to build Source.Python.
5. Go to ``src/Builds/Windows/<game>``.
6. Open ``source-python.sln``.
7. Switch the build mode to ``Release``.
8. Hit ``F7`` to start the build.

If the build finished successfully, it will say something like that:
``========== Build: 3 succeeded, 0 failed, 0 up-to-date, 1 skipped ========== ``.

You can shorten these steps if you have installed the Microsoft Build Tools.
In that case you can simply start the script and pass the game's name e.g.
``Build.bat css``.

However, in both cases the compiled binaries are stored in ``src/Builds/Windows/<game>/Release``.
