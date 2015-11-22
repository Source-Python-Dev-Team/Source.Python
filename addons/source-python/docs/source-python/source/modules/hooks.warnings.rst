hooks.warnings module
======================

.. automodule:: hooks.warnings
    :members:
    :undoc-members:
    :show-inheritance:
    :exclude-members: WarningHook, warning_hooks

.. decorator:: WarningHook

    Bases: :class:`hooks.base._HookDecorator`

    Fired when a warning is encountered.

    The decorated function is passed the following parameters when
    a warning is fired:

    :param Warning message: The :class:`Warning` instance.
    :param Warning category: The type of :class:`Warning` that was encountered.
    :param str filename: The name of the file where
        the warning was encountered.
    :param int lineno: The line number of the file
        where the warning was encountered.

    .. code-block:: python

        from hooks.warnings import WarningHook

        @WarningHook
        def warning_callback(message, category, filename, lineno):
            # Code...

    .. seealso:: :obj:`warnings.showwarning`

.. data:: warning_hooks

    The singleton object of the :class:`_WarningHooks` class.
