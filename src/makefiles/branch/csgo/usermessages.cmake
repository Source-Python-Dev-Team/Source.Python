SET(SOURCEPYTHON_MODULE_MESSAGES_GAME_SOURCES
    # As these are not in any LIB files for the SDK, I think this is the best way
    # of compiling them for the time being until Allied modders puts them in a .lib
    hl2sdk/public/game/shared/csgo/protobuf/cstrike15_usermessages.pb.cc
    hl2sdk/public/game/shared/csgo/protobuf/cstrike15_usermessage_helpers.cpp
    hl2sdk/public/engine/protobuf/netmessages.pb.cc    
    #---------------------------------------------------------------------------
)