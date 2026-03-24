listeners
=========

This page contains tutorials about the :mod:`listeners` package.


OnClientActive
--------------

Called when a client has passed the loading screen and spawns into the server.

.. code-block:: python

    from listeners import OnClientActive

    @OnClientActive
    def on_client_active(index):
        pass


OnClientConnect
---------------

Called when a client tries to connect to the server. At this point the client
can still be rejected.

.. code-block:: python

    from listeners import OnClientConnect

    @OnClientConnect
    def on_client_connect(allow_connect_ptr, edict, name, address, reject_msg_ptr, reject_msg_len):
        if name != 'something_you_dont_like':
            return

        allow_connect_ptr.set_bool(False)
        reject_msg_ptr.set_string_array('Your name is not allowed'[:reject_msg_len])

.. warning:: The length of the reject message should never exceed the value of ``reject_msg_len``.


OnClientDisconnect
------------------

Called when a client disconnects from the server.

.. code-block:: python

    from listeners import OnClientDisconnect

    @OnClientDisconnect
    def on_client_disconnect(index):
        print(index)


OnClientFullyConnect
--------------------

Called when the client has fully connected to the server.

.. code-block:: python

    from listeners import OnClientFullyConnect

    @OnClientFullyConnect
    def on_client_fully_connect(index):
        pass

.. note:: This listener only gets called in CS:GO and Blade Symphony.


OnClientPutInServer
-------------------

Called when the client is put in the server. It gets called right before
`OnClientActive`_.

.. code-block:: python

    from listeners import OnClientPutInServer

    @OnClientPutInServer
    def on_client_put_in_server(edict, name):
        pass


OnClientSettingsChanged
-----------------------

Called when a client setting has been changed e.g. ``cl_language``.

.. code-block:: python

    from listeners import OnClientSettingsChanged

    @OnClientSettingsChanged
    def on_client_settings_changed(index):
        pass


OnCombinerPreCache
------------------

Called when a combined model gets precached.

.. code-block:: python

    from listeners import OnCombinerPreCache

    @OnCombinerPreCache
    def on_combiner_precache(cache_data_type, model_handle):
        pass

.. note:: This listener only gets called in CS:GO.


OnConVarChanged
---------------

Called when a convar has been changed.

.. code-block:: python

    from listeners import OnConVarChanged

    @OnConVarChanged
    def on_convar_changed(convar, old_value):
        pass


OnDataLoaded
------------

Called when data has been loaded into the model cache.

.. code-block:: python

    from listeners import OnDataLoaded

    @OnDataLoaded
    def on_data_loaded(cache_data_type, model_handle):
        pass


OnDataUnloaded
--------------

Called when data has been unloaded from the model cache.

.. code-block:: python

    from listeners import OnDataUnloaded

    @OnDataUnloaded
    def on_data_unloaded(cache_data_type, model_handle):
        pass


OnEdictAllocated
----------------

Called when an edict has been allocated.

.. code-block:: python

    from listeners import OnEdictAllocated

    @OnEdictAllocated
    def on_edict_allocated(index):
        pass

.. note:: This listener only gets called in CS:GO, Black Mesa: Source and Blade Symphony.


OnEdictFreed
------------

Called when an edict gets freed.

.. code-block:: python

    from listeners import OnEdictFreed

    @OnEdictFreed
    def on_edict_freed(edict):
        pass

.. note:: This listener only gets called in CS:GO, Black Mesa: Source and Blade Symphony.


OnEntityCreated
---------------

Called when an entity has been created.

.. code-block:: python

    from listeners import OnEntityCreated

    @OnEntityCreated
    def on_entity_created(base_entity):
        pass


OnNetworkedEntityCreated
------------------------

Called when a networked entity has been created.

.. code-block:: python

    from listeners import OnNetworkedEntityCreated

    @OnNetworkedEntityCreated
    def on_networked_entity_created(entity):
        pass


OnEntityDeleted
---------------

Called when an entity gets deleted.

.. code-block:: python

    from listeners import OnEntityDeleted

    @OnEntityDeleted
    def on_entity_deleted(base_entity):
        pass


OnNetworkedEntityDeleted
------------------------

Called when a networked entity gets deleted.

.. code-block:: python

    from listeners import OnNetworkedEntityDeleted

    @OnNetworkedEntityDeleted
    def on_networked_entity_deleted(entity):
        pass


OnEntityOutput
--------------

Called when an entity output has been fired.

.. code-block:: python

    from listeners import OnEntityOutput

    @OnEntityOutput
    def on_entity_output(output_name, activator, caller, value, delay):
        pass

.. note::

    ``activator`` and ``caller`` are :class:`entities.entity.BaseEntity`
    instances if they are server-only entities. Otherwise they are
    :class:`entities.entity.Entity` instances.


OnEntityPreSpawned
------------------

Called before an entity has been spawned.

.. code-block:: python

    from listeners import OnEntityPreSpawned

    @OnEntityPreSpawned
    def on_entity_pre_spawned(base_entity):
        pass

.. note:: This listener gets only called in Black Mesa: Source.


OnNetworkedEntityPreSpawned
---------------------------

Called before a networked entity has been spawned.

.. code-block:: python

    from listeners import OnNetworkedEntityPreSpawned

    @OnNetworkedEntityPreSpawned
    def on_networked_entity_pre_spawned(entity):
        pass

.. note:: This listener gets only called in Black Mesa: Source.


OnEntitySpawned
---------------

Called when an entity has been spawned.

.. code-block:: python

    from listeners import OnEntitySpawned

    @OnEntitySpawned
    def on_entity_spawned(base_entity):
        pass


OnNetworkedEntitySpawned
------------------------

Called when a networked entity has been spawned.

.. code-block:: python

    from listeners import OnNetworkedEntitySpawned

    @OnNetworkedEntitySpawned
    def on_networked_entity_spawned(entity):
        pass


OnEntityCollision
-----------------

Called when a non-player entity is about to collide with another entity.

.. note::

    This listener can be extremely noisy. Use :class:`entities.collisions.CollisionHash`,
    :class:`entities.collisions.CollisionMap`, or :class:`entities.collisions.CollisionSet`
    if you don't have dynamic conditions to test for.

.. code-block:: python

    from listeners import OnEntityCollision

    @OnEntityCollision
    def on_entity_collision(entity, other):
        # Disable weapons/projectiles collisions with everything except players
        return not (entity.is_weapon() and not other.is_player())


OnEntityTransmit
----------------

Called when a non-player entity is about to be transmitted to a player.

.. note::

    This listener can be extremely noisy. Use :class:`entities.transmit.TransmitHash`,
    :class:`entities.transmit.TransmitMap`, or :class:`entities.transmit.TransmitSet`
    if you don't have dynamic conditions to test for.

.. code-block:: python

    from listeners import OnEntityTransmit

    @OnEntityTransmit
    def on_entity_transmit(player, entity):
        # Never transmit hostages
        return entity.classname != 'hostage_entity'


OnLevelInit
-----------

Called when a map starts.

.. code-block:: python

    from listeners import OnLevelInit

    @OnLevelInit
    def on_level_init(map_name):
        pass


OnLevelShutdown
---------------

Called when a map ends e.g. due to a manual map change or time limit.

.. note:: Can be called multiple times per map change! Use :class:`listeners.OnLevelEnd` if you want to make sure it's only getting called once per map change.

.. code-block:: python

    from listeners import OnLevelShutdown

    @OnLevelShutdown
    def on_level_shutdown():
        pass


OnLevelEnd
----------

Called when a map ends e.g. due to a manual map change or time limit.

.. code-block:: python

    from listeners import OnLevelEnd

    @OnLevelEnd
    def on_level_end():
        pass


OnNetworkidValidated
--------------------

Called when a client has been authenticated.

.. code-block:: python

    from listeners import OnNetworkidValidated

    @OnNetworkidValidated
    def on_networkid_validated(name, steamid):
        pass

.. note::

    If your server can't establish a connection to the Steam servers, this
    listener will never get called.


OnButtonStateChanged
--------------------

Called when the button state of a player changed.

.. code-block:: python

    from listeners import OnButtonStateChanged

    @OnButtonStateChanged
    def on_button_state_changed(player, old_buttons, new_buttons):
        pass

.. seealso::

    Use :func:`listeners.get_button_combination_status` to check if a specific
    button or button combination has been pressed or released.


OnPlayerCollision
-----------------

Called when a player is about to collide with an entity.

.. note::

    This listener can be extremely noisy. Use :class:`entities.collisions.CollisionHash`,
    :class:`entities.collisions.CollisionMap`, or :class:`entities.collisions.CollisionSet`
    if you don't have dynamic conditions to test for.

.. code-block:: python

    from listeners import OnPlayerCollision

    @OnPlayerCollision
    def on_player_collision(player, entity):
        # Disable teammates collisions
        if not entity.is_player():
            return
        return player.team_index != entity.team_index


OnPlayerTransmit
----------------

Called when a player is about to be transmitted to another.

.. note::

    This listener can be extremely noisy. Use :class:`entities.transmit.TransmitHash`,
    :class:`entities.transmit.TransmitMap`, or :class:`entities.transmit.TransmitSet`
    if you don't have dynamic conditions to test for.

.. code-block:: python

    from listeners import OnPlayerTransmit

    @OnPlayerTransmit
    def on_player_transmit(player, other):
        # Don't transmit teammates
        return player.team != other.team


OnPlayerRunCommand
--------------------

Called when a player runs a command.

.. code-block:: python

    from listeners import OnPlayerRunCommand

    @OnPlayerRunCommand
    def on_player_run_command(player, user_cmd):
        pass


OnPlayerPostRunCommand
----------------------

Called after a player ran a command.

.. code-block:: python

    from listeners import OnPlayerPostRunCommand

    @OnPlayerPostRunCommand
    def on_player_post_run_command(player, user_cmd):
        pass


OnPluginLoaded
--------------

Called when a plugin has been loaded successfully.

.. code-block:: python

    from listeners import OnPluginLoaded

    @OnPluginLoaded
    def on_plugin_loaded(plugin):
        pass


OnPluginLoading
---------------

Called right before a plugin is imported and loaded. All checks (e.g. plugin
file exists, etc.) have been done at this point.

.. code-block:: python

    from listeners import OnPluginLoading

    @OnPluginLoading
    def on_plugin_loading(plugin):
        pass


OnPluginUnloaded
----------------

Called when a plugin has been unloaded successfully.

.. code-block:: python

    from listeners import OnPluginUnloaded

    @OnPluginUnloaded
    def on_plugin_unloaded(plugin):
        pass


OnPluginUnloading
-----------------

Called right before a loaded plugin is unloaded.

.. code-block:: python

    from listeners import OnPluginUnloading

    @OnPluginUnloading
    def on_plugin_unloading(plugin):
        pass


OnQueryCvarValueFinished
------------------------

Called when a client cvar query finished. Cvar queries need to be started with
:meth:`engines.server._EngineServer.start_query_cvar_value`.

.. code-block:: python

    from listeners import OnQueryCvarValueFinished

    @OnQueryCvarValueFinished
    def on_query_cvar_value_finished(cookie, index, status, cvar_name, cvar_value):
        pass


OnServerActivate
----------------

Called when a map starts and the server is ready to accept clients.

.. code-block:: python

    from listeners import OnServerActivate

    @OnServerActivate
    def on_server_activate(edicts, edict_count, max_clients):
        pass


OnServerOutput
----------------

Called when something is printed to the console. You can decide whether the
message is logged/printed or not.

.. warning::

    Be careful when printing something within this listener. It can easily
    result into a recursion, which results into a crash of the server.


.. code-block:: python

    from listeners import OnServerOutput
    from core import OutputReturn

    @OnServerOutput
    def on_server_output(severity, msg):
        # Block everything starting with 'sv_' or 'mp_' from being logged.
        # This keeps the console clean in CS:GO.
        if msg.startswith(('sv_', 'mp_')):
            return OutputReturn.BLOCK

        return OutputReturn.CONTINUE


OnServerHibernating
-------------------

Called when the server starts hibernating.

.. code-block:: python

    from listeners import OnServerHibernating

    @OnServerHibernating
    def on_server_hibernating():
        ...


OnServerWakingUp
----------------

Called when the server is waking up from hibernation.

.. code-block:: python

    from listeners import OnServerWakingUp

    @OnServerWakingUp
    def on_server_waking_up():
        ...


OnTick
------

Called once per server frame.

.. code-block:: python

    from listeners import OnTick

    @OnTick
    def on_tick():
        pass

.. warning::

    Be careful when using this listeners. Depending on your implementation
    this can cause a performance impact on your server.


OnVersionUpdate
---------------

Called when a new Source.Python version is available. Version updates are
checked once per map.

.. code-block:: python

    from listeners import OnVersionUpdate

    @OnVersionUpdate
    def on_version_update(old_version, new_version):
        pass

.. note::

    This requires ``sp_check_for_update`` to be set to ``1`` and the current
    installation has to be versioned (no build from source).
