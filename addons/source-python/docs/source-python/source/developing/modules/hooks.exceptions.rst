hooks.exceptions module
========================

.. automodule:: hooks.exceptions
    :members:
    :undoc-members:
    :show-inheritance:
    :exclude-members: ExceptHook, except_hooks

.. decorator:: ExceptHook

    Bases: :class:`hooks.base._HookDecorator`

    Fired when an exception is raised.

    The decorated function is passed the following parameters whe
    an exception is raised:

    :param Exception exc_type: The type of :class:`Exception` that was raised.
    :param Exception value: The instance of the :class:`Exception`
        that was raised.
    :param types.TracebackType trace_back: The traceback info for
        the raised exception.

    .. code-block:: python

        from hooks.exceptions import ExceptHook

        @ExceptHook
        def exception_callback(exc_type, value, trace_back):
            # Code...

    .. seealso:: :obj:`sys.excepthook`

.. data:: except_hooks

    The singleton object of the :class:`_ExceptHooks` class.
