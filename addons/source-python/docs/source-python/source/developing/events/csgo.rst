Counter-Strike: Global Offensive
================================

This is a summary of all known Counter-Strike: Global Offensive events.


achievement_earned
------------------
This event is fired any time a player earns an achievement while on the server.

===========  ==========================================  =====
Name         Description                                 Type
===========  ==========================================  =====
achievement  The achievement id that the player earned.  short
player       The player that earned the achievement.     byte
===========  ==========================================  =====


announce_phase_end
------------------
This event is fired when a specific phase is ended.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


begin_new_match
---------------
This event is fired when a match is started.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


bomb_abortdefuse
----------------
This event is fired when a player aborts defusing the bomb.

======  ========================================================  =====
Name    Description                                               Type
======  ========================================================  =====
userid  The userid of the player that aborted defusing the bomb.  short
======  ========================================================  =====


bomb_abortplant
---------------
This event is fired when a player aborts planting the bomb.

======  ========================================================  =====
Name    Description                                               Type
======  ========================================================  =====
site    The index of the site that the abort took place.          short
userid  The userid of the player that aborted planting the bomb.  short
======  ========================================================  =====


bomb_beep
---------
This event is fired when the bomb emits a beeping sound.

========  ===========================================  ====
Name      Description                                  Type
========  ===========================================  ====
entindex  The index of the c4 entity that is beeping.  long
========  ===========================================  ====


bomb_begindefuse
----------------
This event is fired when a player starts to defuse the bomb.

======  ===================================================  =====
Name    Description                                          Type
======  ===================================================  =====
haskit  True if the player has a defusal kit.                bool
userid  The userid of the player that is defusing the bomb.  short
======  ===================================================  =====


bomb_beginplant
---------------
This event is fired when a player starts to plant the bomb.

======  ========================================================  =====
Name    Description                                               Type
======  ========================================================  =====
site    The index of the site that the bomb is being planted at.  short
userid  The userid of the player that is planting the bomb.       short
======  ========================================================  =====


bomb_defused
------------
This event is fired when the bomb is successfully defused.

======  =================================================  =====
Name    Description                                        Type
======  =================================================  =====
site    The index of the site where the bomb was defused.  short
userid  The userid of the player that defused the bomb.    short
======  =================================================  =====


bomb_dropped
------------
This event is fired when a player drops the bomb.

========  ===============================================  =====
Name      Description                                      Type
========  ===============================================  =====
entindex  The index of the c4 entity that was dropped.     long
userid    The userid of the player that dropped the bomb.  short
========  ===============================================  =====


bomb_exploded
-------------
This event is fired when the bomb is successfully exploded.

======  ==================================================  =====
Name    Description                                         Type
======  ==================================================  =====
site    The index of the site where the bomb was exploded.  short
userid  The userid of the player that planted the bomb.     short
======  ==================================================  =====


bomb_pickup
-----------
This event is fired when a player picks up the bomb.

======  =================================================  =====
Name    Description                                        Type
======  =================================================  =====
userid  The userid of the player that picked up the bomb.  short
======  =================================================  =====


bomb_planted
------------
This event is fired when a player plants the bomb.

======  =================================================  =====
Name    Description                                        Type
======  =================================================  =====
site    The index of the site where the bomb was planted.  short
userid  The userid of the player that planted the bomb.    short
======  =================================================  =====


bot_takeover
------------
This event is fired each time a player takes control of a bot.

======  ===========================================  =====
Name    Description                                  Type
======  ===========================================  =====
botid   The userid of the bot being taken over.      short
index                                                short
userid  The userid of the player taking over a bot.  short
======  ===========================================  =====


break_breakable
---------------
This event is fired when a player breaks a breakable prop.

========  ==================================================  =====
Name      Description                                         Type
========  ==================================================  =====
entindex  The index of the breakable.                         long
material                                                      byte
userid    The userid of the player that broke the breakable.  short
========  ==================================================  =====


break_prop
----------
This event is fired when a player breaks a prop that has an entity index.

========  =============================================  =====
Name      Description                                    Type
========  =============================================  =====
entindex  The index of the prop entity that was broken.  long
userid    The userid of the player that broke the prop.  short
========  =============================================  =====


bullet_impact
-------------
This event is fired when a player shoots their weapon and the bullet impacts a surface.

======  ========================================================  =====
Name    Description                                               Type
======  ========================================================  =====
userid  The userid of the player that fired the bullet.           short
x       The x coordinate on the map where the impact took place.  float
y       The y coordinate on the map where the impact took place.  float
z       The z coordinate on the map where the impact took place.  float
======  ========================================================  =====


buytime_ended
-------------
This event is fired when players are no longer allowed to purchase weapons during a round.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


cs_pre_restart
--------------
This event is fired prior to a match restarting.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


cs_win_panel_match
------------------
This event is fired when the win panel is shown at the end of a match (map).

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


cs_win_panel_round
------------------
This event is fired when the win panel is shown at the end of each round.

=================  ====================================================  ======
Name               Description                                           Type
=================  ====================================================  ======
final_event                                                              byte
funfact_data1      Data for use by the funfact.                          long
funfact_data2      Data for use by the funfact.                          long
funfact_data3      Data for use by the funfact.                          long
funfact_player     The player whom the funfact is about.                 short
funfact_token      Token used for the funfact.                           string
show_timer_attack  True if the attack time is supposed to be displayed.  bool
show_timer_defend  True if the defend time is supposed to be displayed.  bool
timer_time         The amount of time that the round took.               short
=================  ====================================================  ======


decoy_detonate
--------------
This event is fired when a decoy grenade detonates.

========  ======================================================  =====
Name      Description                                             Type
========  ======================================================  =====
entityid  The index of the decoy that detonated.                  short
userid    The userid of the player that threw the decoy.          short
x         The x coordinate on the map where the decoy detonated.  float
y         The y coordinate on the map where the decoy detonated.  float
z         The z coordinate on the map where the decoy detonated.  float
========  ======================================================  =====


decoy_firing
------------
This event is fired each time a decoy grenade fires.

========  ==================================================  =====
Name      Description                                         Type
========  ==================================================  =====
entityid  The index of the decoy that fired.                  short
userid    The userid of the player that threw the decoy.      short
x         The x coordinate on the map where the decoy fired.  float
y         The y coordinate on the map where the decoy fired.  float
z         The z coordinate on the map where the decoy fired.  float
========  ==================================================  =====


decoy_started
-------------
This event is fired when a decoy grenade starts to fire.

========  ===========================================================  =====
Name      Description                                                  Type
========  ===========================================================  =====
entityid  The index of the decoy that started to fire.                 short
userid    The userid of the player that threw the decoy.               short
x         The x coordinate on the map where the decoy started firing.  float
y         The y coordinate on the map where the decoy started firing.  float
z         The z coordinate on the map where the decoy started firing.  float
========  ===========================================================  =====


door_moving
-----------
This event is fired when a door is opened or closed.

========  ============================================================  =====
Name      Description                                                   Type
========  ============================================================  =====
entindex  The index of the door.                                        long
userid    The userid of the player that activated the door's movement.  short
========  ============================================================  =====


enter_bombzone
--------------
This event is fired each time a player enters a bomb zone.

=========  ================================================  =====
Name       Description                                       Type
=========  ================================================  =====
hasbomb    True if the player is carrying a bomb.            bool
isplanted  True if a bomb is planted at any site.            bool
userid     The userid of the player entering the bomb zone.  short
=========  ================================================  =====


enter_buyzone
-------------
This event is fired each time a player enters a buy zone.

======  ===============================================  =====
Name    Description                                      Type
======  ===============================================  =====
canbuy  True if buy time has not expired.                bool
userid  The userid of the player entering the buy zone.  short
======  ===============================================  =====


entity_visible
--------------
This event is fired any time a player sees an entity.

==========  ==============================================  ======
Name        Description                                     Type
==========  ==============================================  ======
classname   The classname of the entity.                    string
entityname  The name of the entity.                         string
subject     The index of the entity that is seen.           short
userid      The userid of the player that sees the entity.  short
==========  ==============================================  ======


exit_bombzone
-------------
This event is fired each time a player exits a bomb zone.

=========  ===============================================  =====
Name       Description                                      Type
=========  ===============================================  =====
hasbomb    True if the player is carrying a bomb.           bool
isplanted  True if a bomb is planted at any site.           bool
userid     The userid of the player exiting the bomb zone.  short
=========  ===============================================  =====


exit_buyzone
------------
This event is fired each time a player exits a buy zone.

======  ==============================================  =====
Name    Description                                     Type
======  ==============================================  =====
canbuy  True if buy time has not expired.               bool
userid  The userid of the player exiting the buy zone.  short
======  ==============================================  =====


flashbang_detonate
------------------
This event is fired when a flashbang detonates.

========  ==========================================================  =====
Name      Description                                                 Type
========  ==========================================================  =====
entityid  The index of the flashbang that detonated.                  short
userid    The userid of the player that threw the flashbang.          short
x         The x coordinate on the map where the flashbang detonated.  float
y         The y coordinate on the map where the flashbang detonated.  float
z         The z coordinate on the map where the flashbang detonated.  float
========  ==========================================================  =====


gg_bonus_grenade_achieved
-------------------------
This event is fired when a player earns a bonus grenade in Arsenal: Demolition mode.

======  =======================================================  =====
Name    Description                                              Type
======  =======================================================  =====
userid  The userid of the player that achieved a bonus grenade.  short
======  =======================================================  =====


gg_final_weapon_achieved
------------------------
This event is fired when a player gets to the final weapon in Arsenal: Arms Race mode.

========  ======================================================  =====
Name      Description                                             Type
========  ======================================================  =====
playerid  The userid of the player that got to the final weapon.  short
========  ======================================================  =====


gg_killed_enemy
---------------
This event is fired each time a player kills an enemy during an Arsenal match.

==========  ===================================================================  =====
Name        Description                                                          Type
==========  ===================================================================  =====
attackerid  The userid of the player killer.                                     short
bonus       True if the killer killed with a bonus weapon.                       bool
dominated   True (1) if the kill caused the killer to be dominating the victim.  short
revenge     True (1) if the victim was dominating the killer.                    short
victimid    The userid of the victim.                                            short
==========  ===================================================================  =====


gg_player_impending_upgrade
---------------------------
This event is fired when a player is impending a weapon upgrade from leveling up.

======  ==================================================  =====
Name    Description                                         Type
======  ==================================================  =====
userid  The userid of the player that will be leveling up.  short
======  ==================================================  =====


gg_reset_round_start_sounds
---------------------------
This event is fired when a player's round start sounds are reset.

======  ============================================================  =====
Name    Description                                                   Type
======  ============================================================  =====
userid  The userid of the player whose round start sounds are reset.  short
======  ============================================================  =====


gg_team_leader
--------------
This event ...

.. todo:: Update description of CS:GO gg_team_leader event.

========  =======================================================  =====
Name      Description                                              Type
========  =======================================================  =====
playerid  The userid of the player that is currently in the lead.  short
========  =======================================================  =====


ggprogressive_player_levelup
----------------------------
This event is fired when a player levels up.

==========  =========================================================  ======
Name        Description                                                Type
==========  =========================================================  ======
userid      The userid of the player who leveled up.                   short
weaponname  The type of weapon the player is being leveled up to.      string
weaponrank  The rank of the weapon the player is being leveled up to.  short
==========  =========================================================  ======


ggtr_player_levelup
-------------------
This event is fired when a player levels up.

==========  =========================================================  ======
Name        Description                                                Type
==========  =========================================================  ======
userid      The userid of the player who leveled up.                   short
weaponname  The type of weapon the player is being leveled up to.      string
weaponrank  The rank of the weapon the player is being leveled up to.  short
==========  =========================================================  ======


grenade_bounce
--------------
This event is fired each time a grenade bounces off of a surface.

======  ======================================================  =====
Name    Description                                             Type
======  ======================================================  =====
userid  The userid of the player that threw the grenade.        short
x       The x coordinate on the map where the grenade bounced.  float
y       The y coordinate on the map where the grenade bounced.  float
z       The z coordinate on the map where the grenade bounced.  float
======  ======================================================  =====


hegrenade_detonate
------------------
This event is fired when a high explosive grenade detonates.

========  ========================================================  =====
Name      Description                                               Type
========  ========================================================  =====
entityid  The index of the grenade that detonated.                  short
userid    The userid of the player that threw the grenade.          short
x         The x coordinate on the map where the grenade detonated.  float
y         The y coordinate on the map where the grenade detonated.  float
z         The z coordinate on the map where the grenade detonated.  float
========  ========================================================  =====


hostage_follows
---------------
This event is fired each time a hostage is picked up by a player.

=======  ==========================================================  =====
Name     Description                                                 Type
=======  ==========================================================  =====
hostage  The index of the hostage entity that is now being carried.  short
userid   The userid of the player that picked up the hostage.        short
=======  ==========================================================  =====


hostage_hurt
------------
This event is fired when a hostage is hurt.

=======  ======================================================  =====
Name     Description                                             Type
=======  ======================================================  =====
hostage  The index of the hostage entity that sustained damage.  short
userid   The userid of the player that hurt the hostage.         short
=======  ======================================================  =====


hostage_killed
--------------
This event is fired when a hostage is killed.

=======  =================================================  =====
Name     Description                                        Type
=======  =================================================  =====
hostage  The index of the hostage entity that was killed.   short
userid   The userid of the player that killed the hostage.  short
=======  =================================================  =====


hostage_rescued
---------------
This event is fired each time a hostage reaches a hostage rescue zone.

=======  ===================================================================  =====
Name     Description                                                          Type
=======  ===================================================================  =====
hostage  The index of the hostage entity that was rescued.                    short
site     The index of the hostage rescue zone where the hostage was rescued.  short
userid   The userid of the player that led the hostage to the rescue zone.    short
=======  ===================================================================  =====


hostage_rescued_all
-------------------
This event is fired when all hostages have been rescued during a round.  In CS:GO, only one hostage needs to be rescued for the Counter-Terrorists to win the round.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


inferno_expire
--------------
This event is fired when the inferno caused by a molotov or incendiary grenade expires.  This event will also be fired if the inferno is [[Event-csgo:inferno_extinguish|extinguished by a smoke grenade]].

========  =======================================  =====
Name      Description                              Type
========  =======================================  =====
entityid  The index of the inferno entity.         short
x         The x coordinate of the inferno entity.  float
y         The y coordinate of the inferno entity.  float
z         The z coordinate of the inferno entity.  float
========  =======================================  =====


inferno_extinguish
------------------
This event is fired when a molotov or incendiary grenade's inferno is extinguished by a smoke grenade.

========  =======================================  =====
Name      Description                              Type
========  =======================================  =====
entityid  The index of the inferno entity.         short
x         The x coordinate of the inferno entity.  float
y         The y coordinate of the inferno entity.  float
z         The z coordinate of the inferno entity.  float
========  =======================================  =====


inferno_startburn
-----------------
This event is fired when a molotov or incendiary grenade explodes into an inferno.

========  =======================================  =====
Name      Description                              Type
========  =======================================  =====
entityid  The index of the inferno entity.         short
x         The x coordinate of the inferno entity.  float
y         The y coordinate of the inferno entity.  float
z         The z coordinate of the inferno entity.  float
========  =======================================  =====


inspect_weapon
--------------
This event is fired when a player uses the +lookatweapon client command to inspect their weapon.

======  =========================================================  =====
Name    Description                                                Type
======  =========================================================  =====
userid  The userid of the player that is inspecting their weapon.  short
======  =========================================================  =====


item_equip
----------
This event is fired each time a player equips a new weapon.

===========  ===========================================================  ======
Name         Description                                                  Type
===========  ===========================================================  ======
canzoom      True if the weapon has a zoom feature.                       bool
hassilencer  True if the weapon has a silencer available.                 bool
hastracers   True if the weapon has tracer bullets that show when fired.  bool
ispainted    True if the weapon is painted.                               bool
issilenced   True if the weapon has a silencer and it is on.              bool
item         The type of item/weapon that the player equipped.            string
userid       The userid of the player that equipped the item.             short
weptype      The weapon type of the item equipped (more below).           short
===========  ===========================================================  ======


item_pickup
-----------
This event is fired each time a player picks an item up.

======  =================================================  ======
Name    Description                                        Type
======  =================================================  ======
item    The index of the item the player picked up.        string
silent  True if the item is a weapon that has a silencer.  bool
userid  The userid of the player that picked up the item.  short
======  =================================================  ======


item_purchase
-------------
This event is fired each time a player purchases an item.

======  =====================================================  ======
Name    Description                                            Type
======  =====================================================  ======
team    The team number of the player that purchased an item.  short
userid  The userid of the player that purchased an item.       short
weapon  The type of item that the player purchased.            string
======  =====================================================  ======


molotov_detonate
----------------
This event is fired when a molotov or incendiary grenade detonates.

======  ===========================================================================  =====
Name    Description                                                                  Type
======  ===========================================================================  =====
userid  The userid of the player that threw the molotov/incendiary grenade.          short
x       The x coordinate on the map where the molotov/incendiary grenade detonated.  float
y       The y coordinate on the map where the molotov/incendiary grenade detonated.  float
z       The z coordinate on the map where the molotov/incendiary grenade detonated.  float
======  ===========================================================================  =====


other_death
-----------
This event is fired each time a non-player entity is killed.

=========================  ====================================================================  ======
Name                       Description                                                           Type
=========================  ====================================================================  ======
attacker                   The userid of the killer.                                             short
headshot                   True if the killshot was to the entity's head hitbox.                 bool
otherid                    The index of the entity that died.                                    short
othertype                  The classname of the entity that died.                                string
penetrated                 The number of objects the killshot penetrated before killing entity.  short
weapon                     The type of weapon used to kill the entity.                           string
weapon_fauxitemid          Faux item id of weapon killer used.                                   string
weapon_itemid              Inventory item id of weapon killer used.                              string
weapon_originalowner_xuid                                                                        string
=========================  ====================================================================  ======


player_activate
---------------
This event is fired when a player is fully connected during a map.  This event is fired each map change while the player is still on the map.

======  ===========================================  =====
Name    Description                                  Type
======  ===========================================  =====
userid  The userid of the player that is connected.  short
======  ===========================================  =====


player_avenged_teammate
-----------------------
This event is fired when a player kills the killer of a teammate.

=================  =============================================================  =====
Name               Description                                                    Type
=================  =============================================================  =====
avenged_player_id  The userid of the teammate whose killer was killed.            short
avenger_id         The userid of the player that killed their teammate's killer.  short
=================  =============================================================  =====


player_connect
--------------
This event is fired each time a player is first connected to the server.

=========  =========================================================  ======
Name       Description                                                Type
=========  =========================================================  ======
address    The IP address and port of the player that is connecting.  string
index      The index given to the player that connected.              byte
name       The name of the connecting player.                         string
networkid  The SteamID of the connecting player.                      string
userid     The userid given to the connecting player.                 short
=========  =========================================================  ======


player_connect_full
-------------------
This event is fired when a client has fully connected to the server.

======  ============================================================  =====
Name    Description                                                   Type
======  ============================================================  =====
index   The index of the player that fully connected to the server.   byte
userid  The userid of the player that fully connected to the server.  short
======  ============================================================  =====


player_death
------------
This event is fired each time a player dies.

=========================  =====================================================================================  ======
Name                       Description                                                                            Type
=========================  =====================================================================================  ======
assister                   The userid of the player that assisted in the kill (if any).                           short
attacker                   The userid of the killer.                                                              short
dominated                  True (1) if the kill caused the killer to be dominating the victim.                    short
headshot                   True if the killshot was to the victim's head hitbox.                                  bool
penetrated                 The number of objects that were penetrated by the bullet before it struck the victim.  short
revenge                    True (1) if the victim was dominating the killer.                                      short
userid                     The userid of the victim.                                                              short
weapon                     The type of weapon used to kill the victim.                                            string
weapon_fauxitemid          Faux item id of weapon killer used.                                                    string
weapon_itemid              Inventory item id of weapon killer used.                                               string
weapon_originalowner_xuid                                                                                         string
=========================  =====================================================================================  ======


player_disconnect
-----------------
This event is fired when a player disconnects from the server.

=========  ============================================  ======
Name       Description                                   Type
=========  ============================================  ======
name       The name of the player that disconnected.     string
networkid  The SteamID of the player that disconnected.  string
reason     The reason why the player was disconnected.   string
userid     The userid of the player that disconnected.   short
=========  ============================================  ======


player_falldamage
-----------------
This event is fired each time a player sustains damage from falling.

======  ==========================================  =====
Name    Description                                 Type
======  ==========================================  =====
damage  The amount of damage the player sustained.  float
userid  The userid of the player that fell.         short
======  ==========================================  =====


player_footstep
---------------
This event is fired each time a player makes an audible footstep.  Using walk mode or crouch will cause the player to move silently.

======  ==========================================  =====
Name    Description                                 Type
======  ==========================================  =====
userid  The userid of the player that took a step.  short
======  ==========================================  =====


player_given_c4
---------------
This event is fired when a player is given C4 at the start of a round.

======  ===============================================  =====
Name    Description                                      Type
======  ===============================================  =====
userid  The userid of the player that was given the C4.  short
======  ===============================================  =====


player_hurt
-----------
This event is fired each time a player is hurt.

==========  ===============================================================  ======
Name        Description                                                      Type
==========  ===============================================================  ======
armor       The remaining amount of armor the victim has after the damage.   byte
attacker    The userid of the attacking player.                              short
dmg_armor   The amount of damage sustained by the victim's armor.            byte
dmg_health  The amount of health the victim lost in the attack.              short
health      The remaining amount of health the victim has after the damage.  byte
hitgroup    The hitgroup that was damaged in the attack.                     byte
userid      The userid of the victim.                                        short
weapon      The type of weapon used in the attack.                           string
==========  ===============================================================  ======


player_radio
------------
This event is fired each time a player uses a radio command.

======  =====================================================  =====
Name    Description                                            Type
======  =====================================================  =====
slot    The index of the command used.                         short
userid  The userid of the player that used the radio command.  short
======  =====================================================  =====


player_say
----------
This event is fired each time a player says something via chat.

======  ====================================================  ======
Name    Description                                           Type
======  ====================================================  ======
text    The text that the player sent in the chat message.    string
userid  The userid of the player that sent the chat message.  short
======  ====================================================  ======


player_spawn
------------
This event is fired each time a player spawns on the server.

=======  ===========================================  =====
Name     Description                                  Type
=======  ===========================================  =====
teamnum  The team number of the player that spawned.  short
userid   The userid of the player that spawned.       short
=======  ===========================================  =====


player_spawned
--------------
This event is fired when a player has been spawned.

=========  ========================================  =====
Name       Description                               Type
=========  ========================================  =====
inrestart  True if spawning during a match restart.  bool
userid     The userid of the player that spawned.    short
=========  ========================================  =====


player_team
-----------
This event is fired each time a player changes teams.

==========  =======================================================  =====
Name        Description                                              Type
==========  =======================================================  =====
autoteam    True if the player auto-picked a team.                   bool
disconnect  True if the player is disconnecting.                     bool
isbot       True if the player is a bot.                             bool
oldteam     The team that the player is changing from.               byte
silent      True if the event is to not be executed on each client.  bool
team        The team that the player is changing to.                 byte
userid      The userid of the player that is changing teams.         short
==========  =======================================================  =====


round_announce_last_round_half
------------------------------
This event is fired when the announcement is made that the currently starting round is the last round before half-time (team switch).

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


round_announce_match_point
--------------------------
This event is fired when the announcement is made that the currently starting round could be the last if the leading team wins the round.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


round_announce_match_start
--------------------------
This event is fired when the announcement is made that the match is starting.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


round_announce_warmup
---------------------
This event is fired when the announcement is made that warmup is starting.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


round_end
---------
This event is fired at the end of every round.

=======  ================================================================  ======
Name     Description                                                       Type
=======  ================================================================  ======
message  The message that corresponds to the reason for the round ending.  string
reason   The reason that the round ended.                                  byte
winner   The team number of the winning team.                              byte
=======  ================================================================  ======


round_freeze_end
----------------
This event is fired when players are first allowed to move each round.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


round_mvp
---------
This event is fired at the end of each round when the round MVP is displayed.

======  =======================================================  =====
Name    Description                                              Type
======  =======================================================  =====
reason  The reason why the player is the MVP of the round.       short
userid  The userid of the player that was the MVP of the round.  short
======  =======================================================  =====


round_start
-----------
This event is fired at the very beginning of each round.

=========  ==================================================  ======
Name       Description                                         Type
=========  ==================================================  ======
fraglimit  The number of kills required for the round to end.  long
objective  The round objective.                                string
timelimit  The time limit (in seconds) for the round to last.  long
=========  ==================================================  ======


server_cvar
-----------
This event is fired each time a :class:`cvars.ConVar` with the :attr:`cvars.flags.ConVarFlags.NOTIFY` flag set is changed.

=========  ===================================================================  ======
Name       Description                                                          Type
=========  ===================================================================  ======
cvarname   The name of the :class:`cvars.ConVar` whose value was changed.       string
cvarvalue  The value that the :class:`cvars.ConVar` was changed to.             string
=========  ===================================================================  ======


server_spawn
------------
This event is fired each time a map is loaded.

==========  =======================================================  ======
Name        Description                                              Type
==========  =======================================================  ======
address     The IP address/port of the server.                       string
dedicated   True if the server is a dedicated server.                bool
game        The directory path of the server.                        string
hostname    The value of the 'hostname' :class:`cvars.ConVar`.       string
mapname     The name of the map that is loaded on the server.        string
maxplayers  The maximum number of players allowed on the server.     long
official    True if the server is an official Valve server.          bool
os          The operating system that the server is on.              string
password    True if the server is password protected.                bool
port        The port of the server.                                  short
==========  =======================================================  ======


smokegrenade_detonate
---------------------
This event is fired when a smoke grenade detonates.

========  ==============================================================  =====
Name      Description                                                     Type
========  ==============================================================  =====
entityid  The index of the smoke grenade that detonated.                  short
userid    The userid of the player that threw the smoke grenade.          short
x         The x coordinate on the map where the smoke grenade detonated.  float
y         The y coordinate on the map where the smoke grenade detonated.  float
z         The z coordinate on the map where the smoke grenade detonated.  float
========  ==============================================================  =====


smokegrenade_expired
--------------------
This event is fired when a smoke grenade's smoke has expired.

========  ============================================================  =====
Name      Description                                                   Type
========  ============================================================  =====
entityid  The index of the smoke grenade entity.                        short
userid    The userid of the player that threw the smoke grenade.        short
x         The x coordinate on the map where the smoke grenade expired.  float
y         The y coordinate on the map where the smoke grenade expired.  float
z         The z coordinate on the map where the smoke grenade expired.  float
========  ============================================================  =====


switch_team
-----------
This event is fired when a player switches teams.

==============  =================================================================================  =====
Name            Description                                                                        Type
==============  =================================================================================  =====
avg_rank        The average rank of human players on the server.                                   short
numCTSlotsFree  The number of Counter-Terrorist slots that are free.                               short
numPlayers      The total number of active players on both Terrorist and Counter-Terrorist teams.  short
numSpectators   The number of players that are spectating.                                         short
numTSlotsFree   The number of Terrorist slots that are free.                                       short
==============  =================================================================================  =====


teamchange_pending
------------------
This event is fired when a player is going to be switching teams.

======  ================================================  =====
Name    Description                                       Type
======  ================================================  =====
toteam  The team number that the player is switching to.  byte
userid  The userid of the player switching teams.         short
======  ================================================  =====


vote_cast
---------
This event is fired when a player casts a vote.

===========  =========================================  =====
Name         Description                                Type
===========  =========================================  =====
entityid     The index of the player that voted.        long
team         The team number of the player that voted.  short
vote_option  The option the player voted for.           byte
===========  =========================================  =====


vote_options
------------
This event is fired when a vote starts.

=======  ========================================  ======
Name     Description                               Type
=======  ========================================  ======
count    The total number of options in the vote.  byte
option1  The first option.                         string
option2  The second option.                        string
option3  The third option (if available).          string
option4  The fourth option (if available).         string
option5  The fifth option (if available).          string
=======  ========================================  ======


weapon_fire
-----------
This event is fired each time a bullet is fired, or a projectile thrown, by a player.

========  ===============================================  ======
Name      Description                                      Type
========  ===============================================  ======
silenced  True if the weapon has a silencer active.        bool
userid    The userid of the player that fired the weapon.  short
weapon    The type of weapon that was fired.               string
========  ===============================================  ======


weapon_fire_on_empty
--------------------
This event is fired when a player attempts to fire a weapon that is completely out of ammo.

======  ============================================================  ======
Name    Description                                                   Type
======  ============================================================  ======
userid  The userid of the player attempting to fire an empty weapon.  short
weapon  The type of weapon that the player is trying to fire.         string
======  ============================================================  ======


weapon_reload
-------------
This event is fired when a player reloads their weapon by pressing their 'reload' button.  Automatic reloading does not fire this event.

======  ====================================================  =====
Name    Description                                           Type
======  ====================================================  =====
userid  The userid of the player that reloaded their weapon.  short
======  ====================================================  =====


weapon_zoom
-----------
This event is fired each time a player zooms in (or out) their weapon.  This only fires on sniper rifles.  This event is notifies bots and has a zoom sound.

======  ==================================================  =====
Name    Description                                         Type
======  ==================================================  =====
userid  The userid of the player that zoomed their weapon.  short
======  ==================================================  =====


weapon_zoom_rifle
-----------------
This event is fired when a player zooms in with non-sniper rifles.  This event does not notify bots and is not accompanied by a zoom sound.

======  ==================================================  =====
Name    Description                                         Type
======  ==================================================  =====
userid  The userid of the player that zoomed their weapon.  short
======  ==================================================  =====
