SET(SOURCEPYTHON_MODULE_USERMESSAGES_GAME_SOURCES
    # As these are not in any LIB files for the SDK, I think this is the best way
    # of compiling them for the time being until Allied modders puts them in a .lib
    sdks/hl2sdk-csgo/public/game/shared/csgo/protobuf/cstrike15_usermessages.pb.cc
    sdks/hl2sdk-csgo/public/game/shared/csgo/protobuf/cstrike15_usermessage_helpers.cpp
    sdks/hl2sdk-csgo/public/engine/protobuf/netmessages.pb.cc    
    #---------------------------------------------------------------------------
)