# ../

""""""

# =============================================================================
# =============================================================================
from engines.server import engine_server
from memory import get_virtual_function
from memory.hooks import PreHook
from players._entity.orangebox import Player as _Player


# =============================================================================
# =============================================================================
class Player(_Player):
    """"""


# =============================================================================
# >> CALLBACKS
# =============================================================================
@PreHook(get_virtual_function(engine_server, 'ClientCommand'))
def _pre_client_command(args):
    """Block name changes started by the server.

    Pre-hook on IVEngineServer::ClientCommand to block the name changes.
    """
    if args[2] == 'name "%s"':
        return 0
