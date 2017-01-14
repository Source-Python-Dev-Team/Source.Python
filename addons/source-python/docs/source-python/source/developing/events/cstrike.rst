Counter-Strike: Source
======================

This is a summary of all known Counter-Strike: Source events.


achievement_earned
------------------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
achievement                                                        short 
player                                                             byte  
===========  ====================================================  ======


achievement_earned_local
------------------------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
achievement                                                        short 
===========  ====================================================  ======


achievement_event
-----------------

================  ====================================================  ======
Name              Description                                           Type  
================  ====================================================  ======
achievement_name                                                        string
cur_val                                                                 short 
max_val                                                                 short 
================  ====================================================  ======


achievement_increment
---------------------

==============  ====================================================  ======
Name            Description                                           Type  
==============  ====================================================  ======
achievement_id                                                        long  
cur_val                                                               short 
max_val                                                               short 
==============  ====================================================  ======


base_player_teleported
----------------------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
entindex                                                        short 
========  ====================================================  ======


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

======  ===============================================  =====
Name    Description                                      Type
======  ===============================================  =====
userid  The userid of the player that dropped the bomb.  short
======  ===============================================  =====


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
posx    Position x of the bomb.                            short
posy    Position y of the bomb.                            short
site    The index of the site where the bomb was planted.  short
userid  The userid of the player that planted the bomb.    short
======  =================================================  =====


bonus_updated
-------------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
numadvanced                                                        short 
numbronze                                                          short 
numgold                                                            short 
numsilver                                                          short 
===========  ====================================================  ======


break_breakable
---------------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
entindex                                                        long  
material                                                        byte  
userid                                                          short 
========  ====================================================  ======


break_prop
----------
This event is fired when a player breaks a prop that has an entity index.

========  =============================================  =====
Name      Description                                    Type
========  =============================================  =====
entindex  The index of the prop entity that was broken.  long
userid    The userid of the player that broke the prop.  short
========  =============================================  =====


browse_replays
--------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


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


cart_updated
------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


christmas_gift_grab
-------------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
userid                                                        short 
======  ====================================================  ======


client_beginconnect
-------------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
address                                                        string
ip                                                             long  
port                                                           short 
source                                                         string
=======  ====================================================  ======


client_connected
----------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
address                                                        string
ip                                                             long  
port                                                           short 
=======  ====================================================  ======


client_disconnect
-----------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
message                                                        string
=======  ====================================================  ======


client_fullconnect
------------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
address                                                        string
ip                                                             long  
port                                                           short 
=======  ====================================================  ======


cs_win_panel_match
------------------
This event is fired when the win panel is shown at the end of a match (map).

==================  =======================================================================================  =====
Name                Description                                                                              Type
==================  =======================================================================================  =====
ct_kd               The kill/death ratio of the Counter-Terrorist team for the match.                        float
ct_money_earned     The total money earned by the Counter-Terrorist team for the match.                      long
ct_objectives_done  The number of objectives that the Counter-Terrorist team accomplished during the match.  short
ct_score            The final score of the Counter-Terrorist team for the match.                             short
t_kd                The kill/death ratio of the Terrorist team for the match.                                float
t_money_earned      The total money earned by the Terrorist team for the match.                              long
t_objectives_done   The number of objectives that the Terrorist team accomplished during the match.          short
t_score             The final score of the Terrorist team for the match.                                     short
==================  =======================================================================================  =====


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


door_moving
-----------
This event is fired when a door is opened or closed.

========  ============================================================  =====
Name      Description                                                   Type
========  ============================================================  =====
entindex  The index of the door.                                        long
userid    The userid of the player that activated the door's movement.  short
========  ============================================================  =====


entered_performance_mode
------------------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


entity_killed
-------------
This event is fired when an entity is killed.

==================  ======================================================================  ====
Name                Description                                                             Type
==================  ======================================================================  ====
damagebits          The amount of damage done to kill the entity.                           long
entindex_attacker   The index of the entity that caused the kill.                           long
entindex_inflictor  The index of the entity that infliced the damage that caused the kill.  long
entindex_killed     The index of the entity that was killed.                                long
==================  ======================================================================  ====


flare_ignite_npc
----------------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
entindex                                                        long  
========  ====================================================  ======


flashbang_detonate
------------------
This event is fired when a flashbang detonates.

======  ==========================================================  =====
Name    Description                                                 Type
======  ==========================================================  =====
userid  The userid of the player that threw the flashbang.          short
x       The x coordinate on the map where the flashbang detonated.  float
y       The y coordinate on the map where the flashbang detonated.  float
z       The z coordinate on the map where the flashbang detonated.  float
======  ==========================================================  =====


freezecam_started
-----------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


game_end
--------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
winner                                                        byte  
======  ====================================================  ======


game_init
---------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


game_message
------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
target                                                        byte  
text                                                          string
======  ====================================================  ======


game_newmap
-----------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
mapname                                                        string
=======  ====================================================  ======


game_start
----------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
fraglimit                                                          long  
objective                                                          string
roundslimit                                                        long  
timelimit                                                          long  
===========  ====================================================  ======


gc_connected
------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


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

======  ========================================================  =====
Name    Description                                               Type
======  ========================================================  =====
userid  The userid of the player that threw the grenade.          short
x       The x coordinate on the map where the grenade detonated.  float
y       The y coordinate on the map where the grenade detonated.  float
z       The z coordinate on the map where the grenade detonated.  float
======  ========================================================  =====


helicopter_grenade_punt_miss
----------------------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


hide_freezepanel
----------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


hltv_cameraman
--------------

=====  ====================================================  ======
Name   Description                                           Type  
=====  ====================================================  ======
index                                                        short 
=====  ====================================================  ======


hltv_changed_mode
-----------------

==========  ====================================================  ======
Name        Description                                           Type  
==========  ====================================================  ======
newmode                                                           short 
obs_target                                                        short 
oldmode                                                           short 
==========  ====================================================  ======


hltv_changed_target
-------------------

==========  ====================================================  ======
Name        Description                                           Type  
==========  ====================================================  ======
mode                                                              short 
obs_target                                                        short 
old_target                                                        short 
==========  ====================================================  ======


hltv_chase
----------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
distance                                                        short 
inertia                                                         byte  
ineye                                                           byte  
phi                                                             short 
target1                                                         short 
target2                                                         short 
theta                                                           short 
========  ====================================================  ======


hltv_chat
---------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
text                                                        string
====  ====================================================  ======


hltv_fixed
----------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
fov                                                           float 
offset                                                        short 
phi                                                           short 
posx                                                          long  
posy                                                          long  
posz                                                          long  
target                                                        short 
theta                                                         short 
======  ====================================================  ======


hltv_message
------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
text                                                        string
====  ====================================================  ======


hltv_rank_camera
----------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
index                                                         byte  
rank                                                          float 
target                                                        short 
======  ====================================================  ======


hltv_rank_entity
----------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
index                                                         short 
rank                                                          float 
target                                                        short 
======  ====================================================  ======


hltv_status
-----------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
clients                                                        long  
master                                                         string
proxies                                                        short 
slots                                                          long  
=======  ====================================================  ======


hltv_title
----------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
text                                                        string
====  ====================================================  ======


host_quit
---------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


hostage_call_for_help
---------------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
hostage                                                        short 
=======  ====================================================  ======


hostage_follows
---------------
This event is fired each time a hostage is activated to follow a player.

=======  ===================================================================  =====
Name     Description                                                          Type
=======  ===================================================================  =====
hostage  The index of the hostage entity that is now following the player.    short
userid   The userid of the player that activated the hostage to follow them.  short
=======  ===================================================================  =====


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
This event is fired when all hostages have been rescued during a round.

====  ===========  ====
Name  Description  Type
====  ===========  ====
====  ===========  ====


hostage_stops_following
-----------------------
This event is fired when a hostage stops following a player.  This can happen when the hostage is killed, the player commands the hostage to stay, or the player gets too far away from the hostage.

=======  ============================================================  =====
Name     Description                                                   Type
=======  ============================================================  =====
hostage  The index of the hostage that stopped following the player.   short
userid   The userid of the player that the hostage stopped following.  short
=======  ============================================================  =====


inventory_updated
-----------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


item_pickup
-----------
This event is fired each time a player picks an item up.

======  =================================================  ======
Name    Description                                        Type
======  =================================================  ======
item    The index of the item the player picked up.        string
userid  The userid of the player that picked up the item.  short
======  =================================================  ======


item_schema_initialized
-----------------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


match_end_conditions
--------------------

==========  ====================================================  ======
Name        Description                                           Type  
==========  ====================================================  ======
frags                                                             long  
max_rounds                                                        long  
time                                                              long  
win_rounds                                                        long  
==========  ====================================================  ======


nav_blocked
-----------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
area                                                           long  
blocked                                                        bool  
=======  ====================================================  ======


nav_generate
------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


physgun_pickup
--------------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
entindex                                                        long  
========  ====================================================  ======


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


player_blind
------------
This event is fired when a player is blinded by a flashbang.

======  ==========================================  =====
Name    Description                                 Type
======  ==========================================  =====
userid  The userid of the player that was blinded.  short
======  ==========================================  =====


player_changename
-----------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
newname                                                        string
oldname                                                        string
userid                                                         short 
=======  ====================================================  ======


player_chat
-----------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
teamonly                                                        bool  
text                                                            string
userid                                                          short 
========  ====================================================  ======


player_class
------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
class                                                         string
userid                                                        short 
======  ====================================================  ======


player_connect
--------------
This event is fired each time a player is first connected to the server.

=========  =========================================================  ======
Name       Description                                                Type
=========  =========================================================  ======
address    The IP address and port of the player that is connecting.  string
bot        1/0 for whether the player is a bot.                       short
index      The index given to the player that connected.              byte
name       The name of the connecting player.                         string
networkid  The SteamID of the connecting player.                      string
userid     The userid given to the connecting player.                 short
=========  =========================================================  ======


player_connect_client
---------------------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
bot                                                              short 
index                                                            byte  
name                                                             string
networkid                                                        string
userid                                                           short 
=========  ====================================================  ======


player_death
------------
This event is fired each time a player dies.

=========  ===================================================================  ======
Name       Description                                                          Type
=========  ===================================================================  ======
attacker   The userid of the killer.                                            short
dominated  True (1) if the kill caused the killer to be dominating the victim.  short
headshot   True if the killshot was to the victim's head hitbox.                bool
revenge    True (1) if the victim was dominating the killer.                    short
userid     The userid of the victim.                                            short
weapon     The type of weapon used to kill the victim.                          string
=========  ===================================================================  ======


player_decal
------------
This event is fired each time a player sprays their decal on a surface.

======  ====================================  =====
Name    Description                           Type
======  ====================================  =====
userid  The userid that sprayed their decal.  short
======  ====================================  =====


player_disconnect
-----------------
This event is fired when a player disconnects from the server.

=========  ==============================================  ======
Name       Description                                     Type
=========  ==============================================  ======
bot        True (1) if the disconnecting player is a bot.  short
name       The name of the player that disconnected.       string
networkid  The SteamID of the player that disconnected.    string
reason     The reason why the player was disconnected.     string
userid     The userid of the player that disconnected.     short
=========  ==============================================  ======


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


player_hintmessage
------------------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
hintmessage                                                        string
===========  ====================================================  ======


player_hurt
-----------
This event is fired each time a player is hurt.

==========  ===============================================================  ======
Name        Description                                                      Type
==========  ===============================================================  ======
armor       The remaining amount of armor the victim has after the damage.   byte
attacker    The userid of the attacking player.                              short
dmg_armor   The amount of damage sustained by the victim's armor.            byte
dmg_health  The amount of health the victim lost in the attack.              byte
health      The remaining amount of health the victim has after the damage.  byte
hitgroup    The hitgroup that was damaged in the attack.                     byte
userid      The userid of the victim.                                        short
weapon      The type of weapon used in the attack.                           string
==========  ===============================================================  ======


player_info
-----------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
bot                                                              bool  
index                                                            byte  
name                                                             string
networkid                                                        string
userid                                                           short 
=========  ====================================================  ======


player_jump
-----------
This event is fired each time a player jumps.

======  =====================================  =====
Name    Description                            Type
======  =====================================  =====
userid  The userid of the player that jumped.  short
======  =====================================  =====


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


player_score
------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
deaths                                                        short 
kills                                                         short 
score                                                         short 
userid                                                        short 
======  ====================================================  ======


player_shoot
------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
mode                                                          byte  
userid                                                        short 
weapon                                                        byte  
======  ====================================================  ======


player_spawn
------------
This event is fired each time a player spawns on the server.

======  ======================================  =====
Name    Description                             Type
======  ======================================  =====
userid  The userid of the player that spawned.  short
======  ======================================  =====


player_stats_updated
--------------------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
forceupload                                                        bool  
===========  ====================================================  ======


player_team
-----------
This event is fired each time a player changes teams.

==========  =======================================================  ======
Name        Description                                              Type
==========  =======================================================  ======
autoteam    True if the player auto-picked a team.                   bool
disconnect  True if the player is disconnecting.                     bool
name        The name of the player that is changing teams.           string
oldteam     The team that the player is changing from.               byte
silent      True if the event is to not be executed on each client.  bool
team        The team that the player is changing to.                 byte
userid      The userid of the player that is changing teams.         short
==========  =======================================================  ======


player_use
----------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
entity                                                        short 
userid                                                        short 
======  ====================================================  ======


ragdoll_dissolved
-----------------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
entindex                                                        long  
========  ====================================================  ======


replay_saved
------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


replay_youtube_stats
--------------------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
favorited                                                        long  
likes                                                            long  
views                                                            long  
=========  ====================================================  ======


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


server_addban
-------------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
by                                                               string
duration                                                         string
ip                                                               string
kicked                                                           bool  
name                                                             string
networkid                                                        string
userid                                                           short 
=========  ====================================================  ======


server_changelevel_failed
-------------------------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
levelname                                                        string
=========  ====================================================  ======


server_cvar
-----------
This event is fired each time a :class:`cvars.ConVar` with the :attr:`cvars.flags.ConVarFlags.NOTIFY` flag set is changed.

=========  ===================================================================  ======
Name       Description                                                          Type
=========  ===================================================================  ======
cvarname   The name of the :class:`cvars.ConVar` whose value was changed.       string
cvarvalue  The value that the :class:`cvars.ConVar` was changed to.             string
=========  ===================================================================  ======


server_message
--------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
text                                                        string
====  ====================================================  ======


server_removeban
----------------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
by                                                               string
ip                                                               string
networkid                                                        string
=========  ====================================================  ======


server_shutdown
---------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
reason                                                        string
======  ====================================================  ======


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
ip          The IP address of the server.                            long
mapname     The name of the map that is loaded on the server.        string
maxplayers  The maximum number of players allowed on the server.     long
os          The operating system that the server is on.              string
password    True if the server is password protected.                bool
port        The port of the server.                                  short
==========  =======================================================  ======


show_freezepanel
----------------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
killer                                                        short 
======  ====================================================  ======


smokegrenade_detonate
---------------------
This event is fired when a smoke grenade detonates.

======  ==============================================================  =====
Name    Description                                                     Type
======  ==============================================================  =====
userid  The userid of the player that threw the smoke grenade.          short
x       The x coordinate on the map where the smoke grenade detonated.  float
y       The y coordinate on the map where the smoke grenade detonated.  float
z       The z coordinate on the map where the smoke grenade detonated.  float
======  ==============================================================  =====


spec_target_updated
-------------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


store_pricesheet_updated
------------------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


team_info
---------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
teamid                                                          byte  
teamname                                                        string
========  ====================================================  ======


team_score
----------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
score                                                         short 
teamid                                                        byte  
======  ====================================================  ======


teamplay_broadcast_audio
------------------------

=====  ====================================================  ======
Name   Description                                           Type  
=====  ====================================================  ======
sound                                                        string
team                                                         byte  
=====  ====================================================  ======


teamplay_round_start
--------------------

==========  ====================================================  ======
Name        Description                                           Type  
==========  ====================================================  ======
full_reset                                                        bool  
==========  ====================================================  ======


user_data_downloaded
--------------------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


vip_escaped
-----------

======  ====================================================  ======
Name    Description                                           Type  
======  ====================================================  ======
userid                                                        short 
======  ====================================================  ======


vip_killed
----------

========  ====================================================  ======
Name      Description                                           Type  
========  ====================================================  ======
attacker                                                        short 
userid                                                          short 
========  ====================================================  ======


vote_cast
---------

===========  ====================================================  ======
Name         Description                                           Type  
===========  ====================================================  ======
entityid                                                           long  
team                                                               short 
vote_option                                                        byte  
===========  ====================================================  ======


vote_changed
------------

==============  ====================================================  ======
Name            Description                                           Type  
==============  ====================================================  ======
potentialVotes                                                        byte  
vote_option1                                                          byte  
vote_option2                                                          byte  
vote_option3                                                          byte  
vote_option4                                                          byte  
vote_option5                                                          byte  
==============  ====================================================  ======


vote_ended
----------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
====  ====================================================  ======


vote_failed
-----------

====  ====================================================  ======
Name  Description                                           Type  
====  ====================================================  ======
team                                                        byte  
====  ====================================================  ======


vote_options
------------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
count                                                          byte  
option1                                                        string
option2                                                        string
option3                                                        string
option4                                                        string
option5                                                        string
=======  ====================================================  ======


vote_passed
-----------

=======  ====================================================  ======
Name     Description                                           Type  
=======  ====================================================  ======
details                                                        string
param1                                                         string
team                                                           byte  
=======  ====================================================  ======


vote_started
------------

=========  ====================================================  ======
Name       Description                                           Type  
=========  ====================================================  ======
initiator                                                        long  
issue                                                            string
param1                                                           string
team                                                             byte  
=========  ====================================================  ======


weapon_fire
-----------
This event is fired each time a bullet is fired, or a projectile thrown, by a player.

======  ===============================================  ======
Name    Description                                      Type
======  ===============================================  ======
userid  The userid of the player that fired the weapon.  short
weapon  The type of weapon that was fired.               string
======  ===============================================  ======


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
This event is fired each time a player zooms in (or out) their weapon.  This only fires on sniper rifles.  Even though the Aug and SG552 'zoom', the event is not fired.

======  ==================================================  =====
Name    Description                                         Type
======  ==================================================  =====
userid  The userid of the player that zoomed their weapon.  short
======  ==================================================  =====
