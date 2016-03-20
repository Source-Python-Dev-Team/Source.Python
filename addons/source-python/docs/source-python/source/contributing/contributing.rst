Contributing
============

There are several topics in Source.Python you can contribute to and we
appreciate every contribution! To contribute to Source.Python, please use
`Github's pull request functionality <https://github.com/Source-Python-Dev-Team/Source.Python/pulls>`_.
You can read more about pull request on the
`Github help page <https://help.github.com/articles/using-pull-requests/>`_.

When creating a pull request, please always provide a reason why you made that
change. We will then review, discuss and merge or close it.

If you successfully contributed to Source.Python we will gladly add you to our
:doc:`credits page <../general/credits>` (if you want).


Contributing to the source code
-------------------------------

If you want to contribute to the source code you can simply create a pull
request. But before creating a new pull request, please consider these two
points.

1. Test the changes you have made.
2. Try to roughly meet our :doc:`coding convention <coding-conventions>`.


Contributing to the wiki
------------------------

Contributing to this wiki will also be done via pull requests. We are using
`Sphinx <http://sphinx-doc.org/>`_ to create the wiki. Therefore, you can find
rst-files in Source.Python's documentation directory (``../addons/source-python/docs/source-python/source``).
rst-files in that directory are usually simple wiki pages like this one.

The source code is documented in the ``modules`` sub-directory. It's mostly
auto-documented, so these files only contain a few directives. E.g.

.. code::

    bitbuffers module
    ==================

    .. automodule:: bitbuffers
        :members:
        :undoc-members:
        :show-inheritance:

The ``automodule`` directive tells Sphinx to parse the ``bitbuffers`` module
and look for Sphinx directives in the doc strings of the Python objects in
that module (e.g. functions and classes). However, in some cases you have to
manually document specific things in a module. In that case you can simply
extend or replace the rst-file of the module you want to edit.

Before creating a pull request you should also test if the HTML output of your
change is correct and looks good. To do so you can build an offline copy of
the wiki. Building an offline copy is very simple.

1. Install a game server.
2. Install Source.Python.
3. Run ``sp docs build source-python``.
4. Open ``../addons/source-python/docs/source-python/build/index.html`` and review your changes.

.. warning::

    If you have made a change to the source code documentation directly in the
    source code, you have to restart your server.

.. note::

    Always restarting and typing the long build command can get very annoying.
    But luckily you can create server command aliases. Just add
    ``alias b sp docs build source-python`` to your ``autoexec.cfg``. Then you
    only need to enter ``b`` in your server console to build the offline wiki.

If we have merged your pull request our buildbot will be triggered and a new
build will be created. As soon as the new build has been finished it's used to
build a new wiki that gets published automatically. The whole process usually
takes 20-30 minutes.
