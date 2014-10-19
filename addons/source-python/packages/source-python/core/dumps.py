# ../core/dumps.py

"""Provides dump logging functionalities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Basetypes
from basetypes import SendPropTypes
#   Engines
from engines.server import server_game_dll
#   Paths
from paths import LOG_PATH
#   Stringtables
from stringtables import string_tables


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('dump_server_classes',
           'dump_string_tables',
           )


# =============================================================================
# >> DUMP FUNCTIONS
# =============================================================================
def dump_server_classes(filename):
    """Dump all server class send properties to the given filename."""
    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:

        # Get the starting server class
        server_class = server_game_dll.get_all_server_classes()

        # Use a while statement to loop through all server classes
        while server_class:

            # Print the server class' name to file
            open_file.write('{0}\n'.format(server_class.name))

            # Get all items in the server class' table
            _dump_server_class_table(server_class.table, open_file)

            # Move to the next server class
            server_class = server_class.next

            # Was this not the last server class?
            if server_class is not None:

                # Write a separator line before the next server class output
                open_file.write('\n')


def dump_string_tables(filename):
    """Dump all string tables to the given filename."""
    # Open/close the file
    with LOG_PATH.joinpath(filename + '.txt').open('w') as open_file:

        # Get a list of all current string tables
        all_string_tables = list(string_tables)

        # Loop through the string tables
        for string_table in all_string_tables:

            # Write the string table's name and length to file
            open_file.write('{0} (Length: {1})\n'.format(
                string_table.name, len(string_table)))

            # Loop through all items in the string table
            for item in string_table:

                # Write the item to file
                open_file.write('    {0}\n'.format(item))

            # Is this not the last string table?
            if all_string_tables.index(string_table) != len(string_tables) - 1:

                # Write a separator line before the next string table
                open_file.write('\n')


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _dump_server_class_table(table, open_file, level=1, offset=0):
    """Dump all items in the given table to the given file."""
    # Loop through the send props in the table
    for prop in table:

        # Skip all baseclasses
        if prop.name == 'baseclass':
            continue

        # Get the current offset in case this
        # property is inside an internal table
        new_offset = prop.offset + offset

        # Is the current prop a table?
        if prop.type == SendPropTypes.DATATABLE:

            # Get the new table's instance
            new_table = prop.get_data_table()

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
        elif offset:

            # Write the property and its values to file
            open_file.write('{0}{1} {2} (offset {3} - {4})\n'.format(
                '    ' * level, prop.type, prop.name,
                prop.offset, prop.offset + offset))

        # Was no offset passed?
        else:

            # Write the property and its values to file
            open_file.write('{0}{1} {2} (offset {3})\n'.format(
                '    ' * level, prop.type, prop.name, prop.offset))
