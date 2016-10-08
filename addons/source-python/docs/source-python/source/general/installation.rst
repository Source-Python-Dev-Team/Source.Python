Installation
============

Requirements
------------

1. On Linux GLIBC 2.17 or later is required. Check your version by typing ``ld -version``.
2. On Windows `Visual C++ Redistributable for Visual Studio 2015 <https://www.microsoft.com/en-us/download/details.aspx?id=48145>`_ is required. Most machines already have a redistributable installed, so you only need to install it if Source.Python didn't load properly due to the missing redistributable.


Installation steps
------------------

1. Download the latest Source.Python version from our `buildbot <http://builds.sourcepython.com/job/Source.Python/lastSuccessfulBuild/>`_.
2. Extract the archive in your server's game folder. E.g. ``../my_servers/css/cstrike/``
3. Start/restart your server and validate the installation:

   1. Enter the server command ``plugin_print``. It should show the following entry: ``Source.Python, (C) 2012-2015, Source Python Team.``
   2. Enter the server variable ``sp_version``. It should show the Source.Python version that is installed.

4. If you validated the installation, you successfully installed Source.Python!
5. Edit your server's ``../cfg/source-python/core_settings.ini`` file to your liking.
6. If you are having problems installing Source.Python, please let us know on our `forums <http://forums.sourcepython.com/>`_!
