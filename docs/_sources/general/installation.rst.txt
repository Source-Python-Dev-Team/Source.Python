Installation
============

Requirements
------------

Linux
^^^^^

1. GLIBC 2.29 or later is required. Check your version by typing ``ld -version``.
2. The 32 bit version of ``libz.so.1`` and ``libffi.so.7`` is required. See the table below to install the proper packages.

================================= ================================== ======================================
Distribution                      libz.so.1                          libffi.so.7
================================= ================================== ======================================
Debian/Ubuntu                     apt-get install zlib1g:i386        apt-get install libffi7:i386
================================= ================================== ======================================



Windows
^^^^^^^

There are no special requirements.


Installation steps
------------------

1. Download the latest Source.Python version from our `download page <http://downloads.sourcepython.com/>`_.
2. Extract the archive in your server's game folder. E.g. ``../my_servers/css/cstrike/``
3. Start/restart your server and validate the installation by typing ``sp info`` into the server console (use ``rcon`` if you are not working directly on your server). It should print something like this:

.. code-block:: none

    IMPORTANT: Please copy the full output.
    --------------------------------------------------------
    Checksum      : b2471f6d7fbd05548c831307f1903218
    Date          : 2018-03-04 20:35:02.237858
    OS            : Windows-10-10.0.16299
    Game          : css
    SP version    : 646
    Github commit : 3aee743f47727579a0bf2b013d54b4d4ba0aff4c
    Server plugins:
       00: Source.Python, (C) 2012-2018, Source.Python Team.
    SP plugins:
    --------------------------------------------------------

4. If you validated the installation, you successfully installed Source.Python!
5. You can now edit your server's ``../cfg/source-python/core_settings.ini`` file to your liking.

If you are having problems installing Source.Python (e. g. ``Unknown command "sp"``), please let us know on our `forums <http://forums.sourcepython.com/>`_!
