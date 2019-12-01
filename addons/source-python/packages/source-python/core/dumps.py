# ../core/dumps.py

"""Provides dump logging functionalities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Inspect
from inspect import ismethod

# Source.Python Imports
#   Cvars
from cvars import cvar
from cvars.flags import ConVarFlags
#   Entities
from entities import ServerClassGenerator
from entities.datamaps import FieldType
from entities.entity import BaseEntity
from entities.props import SendPropFlags
from entities.props import SendPropType
#   Memory
from memory import CLASS_INFO
from memory import Pointer
from memory.helpers import MemberFunction
#   Paths
from paths import LOG_PATH
#   Stringtables
from stringtables import string_tables


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('dump_class_info',
           'dump_convars',
           'dump_datamaps',
           'dump_server_classes',
           'dump_string_tables',
           'dump_weapon_scripts',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_convar_types = {
    True: 'CMD',
    False: 'VAR',
}


# =============================================================================
# >> CLASS INFO
# =============================================================================
def dump_class_info(filename):
    """Dump the CLASS_INFO dictionary to the given file name."""
    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:
        for classname, class_info in sorted(CLASS_INFO.items()):
            open_file.write('{0}\n'.format(classname))
            for function_name, overloads in sorted(class_info.items()):
                open_file.write('\t{0}:\n'.format(function_name))
                for index, info in enumerate(overloads):
                    open_file.write('\t\tOverload {0}:\n'.format(index))
                    _dump_function_info_attribute(
                        open_file, 'is_virtual', info.is_virtual)
                    _dump_function_info_attribute(
                        open_file, 'this_pointer_offset',
                        info.this_pointer_offset)
                    _dump_function_info_attribute(
                        open_file, 'vtable_index', info.vtable_index)
                    _dump_function_info_attribute(
                        open_file, 'vtable_offset', info.vtable_offset)
                    _dump_function_info_attribute(
                        open_file, 'return_type', info.return_type)
                    _dump_function_info_attribute(
                        open_file, 'argument_types',
                        tuple(map(str, info.argument_types)))
                    _dump_function_info_attribute(
                        open_file, 'calling_convention',
                        info.calling_convention)


def _dump_function_info_attribute(open_file, attr_name, value):
    """Dump an attribute of a FunctionInfo object."""
    open_file.write('\t\t\t{0}: {1}\n'.format(attr_name.ljust(20), value))


# =============================================================================
# >> CONVARS
# =============================================================================
def dump_convars(filename):
    """Dump all convars to the given file name."""
    # Create a dictionary to store the convars
    convars = dict()

    # Get the first convar
    convar = cvar.commands

    # Loop through all convars
    while convar is not None:

        # Store the convar in the dictionary
        convars[convar.name] = convar

        # Move to the next convar
        convar = convar.next

    # Get the number of commands
    command_count = len([
        convar_name for convar_name in convars
        if convars[convar_name].is_command()])

    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:

        # Write the header
        open_file.write(
            'Commands: {0} - Variables: {1} - Total: {2}\n\n'.format(
                command_count, len(convars) - command_count, len(convars)))

        # Loop through all convars in alphabetic order
        for convar_name, convar in sorted(convars.items()):

            # Get the type (CMD/VAR) of convar
            convar_type = _convar_types[convars[convar_name].is_command()]

            # Get the convar's flags
            convar_flags = [
                flag.name for flag in ConVarFlags if flag & convar.flags]

            # Get the convar's help text
            convar_text = convar.help_text

            # Write the convar with its values to file
            open_file.write('{0} - {1}{2}\n{3}\n\n'.format(
                convar_name, convar_type,
                ' - (' + ','.join(convar_flags) + ')' if convar_flags else '',
                '\t' + convar_text if convar_text else ''))


# =============================================================================
# >> DATA MAPS
# =============================================================================
def dump_datamaps(filename):
    """Dump all entity data maps to the given file name."""
    # Create a dict to get rid of all duplicates
    datamaps = dict(_get_datamaps())
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:
        for class_name, datamap in sorted(datamaps.items()):
            _dump_datamap(open_file, class_name, datamap)


def _get_datamaps():
    """Create a generator to loop through all entity DataMap objects.

    The yielded values are two-tuples, which contain the data class name of
    the data map and the actual DataMap object.
    """
    from entities.factories import factory_dictionary
    for classname in factory_dictionary:
        datamap = _get_datamap(classname)
        while datamap:
            yield datamap.class_name, datamap
            datamap = datamap.base


def _get_datamap(classname):
    """Return the DataMap object for the given entity classname."""
    # Check existing entities at first
    entity = BaseEntity.find(classname)
    if entity is not None:
        return entity.datamap

    # We haven't found an entity. Let's create it temporarily
    entity = BaseEntity.create(classname)
    datamap = entity.datamap
    entity.remove()
    return datamap


def _dump_datamap(open_file, class_name, datamap):
    """Dump a DataMap object to the given file object."""
    base = datamap.base
    open_file.write('{} -> {}\n'.format(
        class_name, base and base.class_name))
    for desc in datamap:
        _dump_type_description(open_file, desc)

    open_file.write('\n')


def _dump_type_description(open_file, desc, indent=1, offset=0):
    """Dump a TypeDescription object to the given file object."""
    offset += desc.offset
    open_file.write('{0}{1} {2} (offset {3})'.format(
        '\t'*indent, desc.type, desc.name, offset))

    if desc.type == FieldType.EMBEDDED:
        open_file.write(
            ' [{0} properties]:\n'.format(len(desc.embedded_datamap)))

        # Dump the embedded data map
        for desc in desc.embedded_datamap:
            _dump_type_description(open_file, desc, indent+1, offset)
    else:
        open_file.write('\n')


# =============================================================================
# >> SERVER CLASSES
# =============================================================================
def dump_server_classes(filename):
    """Dump all server class send properties to the given file name."""
    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:

        # Loop through all server classes
        for server_class in ServerClassGenerator():

            table = server_class.table

            # Print the server class' name to file
            open_file.write('{} -> {}\n'.format(
                server_class.name, _find_base_server_class_name(table)))

            # Get all items in the server class' table
            _dump_server_class_table(table, open_file)

            # Move to the next server class
            server_class = server_class.next

            # Was this not the last server class?
            if server_class is not None:

                # Write a separator line before the next server class output
                open_file.write('\n')


def _find_base_server_class_name(table):
    """Return the name of table's base server class."""
    for prop in table:
        if prop.name != 'baseclass':
            continue

        base_name = prop.data_table.name
        for server_class in ServerClassGenerator():
            if server_class.table.name == base_name:
                return server_class.name

        return None

    return None


def _dump_server_class_table(table, open_file, level=1, offset=0):
    """Dump all items in the given table to the given file."""
    # Loop through the send props in the table
    for prop in table:

        # Skip all baseclasses
        if (prop.name == 'baseclass' or
            prop.is_exclude_prop() or
            prop.flags & SendPropFlags.COLLAPSIBLE
        ):
            continue

        # Get the current offset in case this
        # property is inside an internal table
        new_offset = prop.offset + offset

        # Is the current prop a table?
        if prop.type == SendPropType.DATATABLE:

            # Get the new table's instance
            new_table = prop.data_table

            # Was there an offset passed?
            if offset:

                # Write the property and its values to file
                open_file.write(
                    '{0}{1} {2} (offset {3} - {4}) [{5} properties]:\n'.format(
                        '    ' * level, prop.type, prop.name,
                        prop.offset, new_offset, len(new_table)))

            # Was no offset passed?
            else:

                # Write the property and its values to file
                open_file.write(
                    '{0}{1} {2} (offset {3}) [{4} properties]:\n'.format(
                        '    ' * level, prop.type, prop.name,
                        prop.offset, len(new_table)))

            # Dump all items in the table
            _dump_server_class_table(
                new_table, open_file, level + 1, new_offset)

        # Was there an offset passed?
        else:
            real_prop_type = _get_real_prop_type(prop)

            if offset:

                # Write the property and its values to file
                open_file.write('{0}{1} {2} (offset {3} - {4})\n'.format(
                    '    ' * level, real_prop_type, prop.name,
                    prop.offset, prop.offset + offset))

            # Was no offset passed?
            else:

                # Write the property and its values to file
                open_file.write('{0}{1} {2} (offset {3})\n'.format(
                    '    ' * level, real_prop_type, prop.name, prop.offset))

def _get_real_prop_type(prop):
    """Return the real property type name of the given prop."""
    if prop.type != SendPropType.INT:
        return str(prop.type)

    bit_count = prop.bits
    if bit_count < 1:
        return str(prop.type)

    if bit_count >= 17:
        return 'INT'

    if bit_count >= 9:
        return '{}SHORT'.format('' if prop.is_signed() else 'U')

    if bit_count >= 2:
        return '{}CHAR'.format('' if prop.is_signed() else 'U')

    return 'BOOL'


# =============================================================================
# >> STRING TABLES
# =============================================================================
def dump_string_tables(filename):
    """Dump all string tables to the given file name."""
    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:

        # Loop through the string tables
        for current_index, string_table in enumerate(string_tables):

            # Is the current index not zero?
            if current_index:

                # If so, Write a separator line before the next string table
                open_file.write('\n')

            # Get a filtered list of the table's strings skipping all blank
            #   ones...
            items = list(filter(None, string_table))

            # Write the string table's name and length to file
            open_file.write('{0} (Length: {1})\n'.format(
                string_table.name, len(items)))

            # Loop through all items in the string table
            for item in items:

                # Write the item to file
                open_file.write('    {0}\n'.format(item))


# =============================================================================
# >> WEAPON SCRIPTS
# =============================================================================
def dump_weapon_scripts(filename):
    """Dump all WeaponInfo instances to the given file name."""
    # Import weapon_scripts
    # This was moved here due to issues with the bms branch
    from weapons.scripts import weapon_scripts

    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:

        # Loop through all WeaponInfo instances...
        for info in weapon_scripts:

            # Is the current script not parsed yet?
            if not info.is_script_parsed:

                # If so, skip the current weapon...
                continue

            # Write the current weapon class name...
            open_file.write('{0}\n'.format('=' * 80))
            open_file.write('{0}\n'.format(info.class_name))
            open_file.write('{0}\n'.format('=' * 80))

            # Loop through all WeaponInfo's attributes...
            for attr in dir(info):

                # Is the current attribute private or inherited from
                #   Pointer?
                if attr.startswith('_') or hasattr(Pointer, attr):

                    # If so, skip it...
                    continue

                # Get the current attribute value...
                value = getattr(info, attr)

                # Is the current attribute a method or inehrited from Pointer?
                if ismethod(value) or isinstance(
                        value, (MemberFunction, Pointer)):

                    # If so, skip it...
                    continue

                # Write the current attribute...
                open_file.write('{0} = {1}\n'.format(attr, value))
