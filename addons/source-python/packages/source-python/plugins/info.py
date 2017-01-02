# ../plugins/info.py

"""Provides plugin information storing."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from cvars.public import PublicConVar


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PluginInfo',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PluginInfo(dict):
    """Store information for a plugin."""

    def __init__(self, name, verbose_name=None, author=None, description=None,
            version=None, url=None, permissions=None, public_convar=True,
            display_in_listing=None, **kwargs):
        """Initialize the instance.

        :param str name:
            Name of the plugin on the file system.
        :param str verbose_name:
            A verbose name for the plugin (e.g. GunGame).
        :param str author:
            Name of the author.
        :param str description:
            A short description of what the plugin does.
        :param str version:
            Current version of the plugin.
        :param str url:
            A link to a thread in the 'Plugin Releases' forum section or the
            plugin's SPPM link.
        :param list permissions:
            A list of permissions defined or used by the plugin. The list
            should contain tuples that define the permission and a short
            description of the permission.
        :param public_convar:
            If set to ``True``, a public convar will be generated based on the
            plugin name, verbose name and version. Set it to ``False`` if you
            don't want a public convar or set it to a dictionary containing
            the parameters to create a :class:`cvars.public.PublicConvar`
            instance.
        :param list display_in_listing:
            A list that contains custom attributes that should appear in the
            plugin listing (e.g. sp plugin list).
        :param kwargs:
            Any additional attributes you want to set. If you want those
            attributes to appear in the plugin listing, update
            :attr:`display_in_listing`.
        """
        super().__init__(**kwargs)
        self.name = name
        self._verbose_name = verbose_name
        self.author = author
        self.description = description
        self._version = version
        self.url = url

        # All permissions defined by this plugin
        # A list that contains tuples:
        # Example:
        # [('test1.kick', 'Permission to kick players.'),
        #  ('test1.ban', 'Permission to ban players.'),
        #  ('test1.start_vote', 'Permission to start a vote.')]
        self.permissions = [] if permissions is None else permissions
        self.public_convar = public_convar

        self.display_in_listing = [] if display_in_listing is None else display_in_listing

    def _create_public_convar(self):
        """Create a public convar if :attr:`public_convar` is set to True."""
        name = '{}_version'.format(self.name)
        description = '{} version.'.format(self.verbose_name)
        if self.public_convar is True:
            self.public_convar = PublicConVar(
                name,
                self.version,
                description
            )
        elif isinstance(self.public_convar, dict):
            self.public_convar = PublicConVar(
                self.public_convar.pop('name', name),
                self.public_convar.pop('value', self.version),
                self.public_convar.pop('description', description),
                **self.public_convar)

    def get_verbose_name(self):
        """Return the verbose name of the plugin.

        If no verbose name has been set, the plugin name will be titled.

        :rtype: str
        """
        if self._verbose_name is None:
            return self.name.replace('_', ' ').title()

        return self._verbose_name

    def set_verbose_name(self, value):
        """Set the verbose name of the plugin."""
        self._verbose_name = value

    verbose_name = property(get_verbose_name, set_verbose_name)

    def get_version(self):
        """Return the plugin's version.

        :rtype: str
        """
        if self._version is None:
            return 'unversioned'

        return self._version

    def set_version(self, value):
        """Set the plugin's version."""
        self._version = value

    version = property(get_version, set_version)

    # Redirect __getitem__ and __setitem__ to __getattr__ and __setattr__
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__
