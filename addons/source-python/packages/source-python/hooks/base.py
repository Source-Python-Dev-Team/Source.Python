# ../hooks/base.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> CLASSES
# =============================================================================
class _HookBase(list):
    '''Base hook class used to store callbacks for the specific hook type'''

    def append(self, callback):
        '''Hook the append method to verify the given callback is callable'''

        # Is the given callback callable?
        if not callable(callback):

            # Raise an exception
            raise ValueError(
                '{0} callbacks must be callable'.format(self.class_name))

        # Is the given callback already registered?
        if callback in self:

            # Raise an exception
            raise ValueError(
                '{0} callback already registered'.format(self.class_name))

        # Add the callback to the list
        super(_HookBase, self).append(callback)

    @property
    def class_name(self):
        '''Returns the name of the class'''
        return self.__class__.__name__


class _HookDecorator(AutoUnload):
    '''Base decorator class used to add/remove hooks on load/unload'''

    def __init__(self, callback):
        '''Store the given callback and register the hook'''
        self.callback = callback
        self._class_instance.append(self.callback)

    def _unload_instance(self):
        '''Unregister the hook'''
        self._class_instance.remove(self.callback)
