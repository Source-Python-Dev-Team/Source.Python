Known issues
============

EventScripts/Source.Python incompatibility on Linux
---------------------------------------------------

Unfortunately, it's not possible to run EventScripts and Source.Python at the
same time on a Linux server. There are some issues with the symbols of the
shared Python libraries (``libpython2.5.so.1.0`` and ``libpython3.4m.so.1.0``),
which causes either EventScripts to be loaded with Source.Python's Python
version or vice versa. This doesn't work well and results in a crash on startup.

SourceMod's Accelerator incompatibility
---------------------------------------
If you are running `SourceMod's Accelerator <https://forums.alliedmods.net/showthread.php?t=277703&>`_
with Source.Python, you may experience random crashes that would normally be caught since this extension
prevents us from catching and preventing them.

Hibernation issues
------------------
Some features (such as tick listeners, delays, Python threads, etc.) do not work on some games (e.g. CS:GO)
while the server is hibernating. If you require these features at all time, please disable hibernation.
