stringtables
============

This page contains tutorials about the :mod:`stringtables` package.


Printing out all available string tables
----------------------------------------

.. code-block:: python

    from stringtables import string_tables

    # Print out all the string table names and their current size
    for table in string_tables:
        print(table.name.ljust(25) + str(len(table)))

The output:

.. code-block:: none

    downloadables            58
    modelprecache            151
    genericprecache          1
    soundprecache            1121
    decalprecache            227
    instancebaseline         13
    lightstyles              64
    userinfo                 24
    DynamicModels            0
    server_query_info        2
    ParticleEffectNames      20
    EffectDispatch           0
    VguiScreen               2
    Materials                2
    InfoPanel                2
    Scenes                   0
    ServerMapCycle           1
    GameRulesCreation        1
    BlackMarketTable         0