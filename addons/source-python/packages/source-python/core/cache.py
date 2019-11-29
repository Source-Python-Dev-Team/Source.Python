# ../core/cache.py

"""Provides caching functionality.

.. data:: cached_property
    An alias of :class:`core.cache.CachedProperty`.
"""


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from _core._cache import CachedGenerator
from _core._cache import CachedProperty
from _core._cache import cached_property


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'CachedGenerator',
    'CachedProperty',
    'cached_property'
]
