# ../

""""""

# =============================================================================
# =============================================================================
# Source.Python Imports
#   Bitbuffers
from bitbuffers import BitBufferWrite
#   Engines
from engines.server import engine_server
#   Memory
from memory import get_virtual_function
from memory.hooks import PreHook
#   Messages
from _messages import ProtobufMessage
#   Players
from players._entity import Player as _Player


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(_Player):

    def send_convar_value(self, cvar_name, value):
        """Send a convar value.

        :param str cvar_name: Name of the convar.
        :param str value: Value to send.
        """
        buffer_size = 256
        buffer = BitBufferWrite(buffer_size)
        msg = ProtobufMessage('CNETMsg_SetConVar')
        cvar = msg.mutable_message('convars').add_message('cvars')
        cvar.set_string('name', cvar_name)
        cvar.set_string('value', str(value))

        msg_size = msg.byte_size
        buffer.write_var_int32(6)
        buffer.write_var_int32(msg_size)
        msg.serialize_to_array(
            buffer.data + buffer.num_bytes_written, buffer_size)
        buffer.seek_to_bit((buffer.num_bytes_written + msg_size) * 8)
        self.client.net_channel.send_data(buffer)

    def get_projectile_ammo(self, projectile):
        """"""

    def set_projectile_ammo(self, projectile, value):
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
