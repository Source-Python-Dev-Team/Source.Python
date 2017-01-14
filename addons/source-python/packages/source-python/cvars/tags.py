# ../cvars/tags.py

"""Provides an easy way to add/remove values from sv_tags."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from cvars import ConVar


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_SVTags',
           'sv_tags',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _SVTags(ConVar):
    """Class used to interact wit the sv_tags ConVar."""

    def add(self, tag):
        """Add the given tag to sv_tags.

        :param str tag:
            The tag to add to the sv_tags ConVar value.
        """
        tags = set(self.get_string().split(','))
        tags.add(tag)
        self.set_string(','.join(tags))

    def remove(self, tag):
        """Remove the given tag from sv_tags.

        :param str tag:
            The tag to remove from the sv_tags ConVar value.
        """
        tags = set(self.get_string().split(','))
        tags.discard(tag)
        self.set_string(','.join(tags))

# The singleton object of the :class:`_SVTags` class
sv_tags = _SVTags('sv_tags')
