SET(SOURCEPYTHON_MESSAGES_MODULE_GAME_SOURCES
    # As these are not in any LIB files for the SDK, I think this is the best way
    # of compiling them for the time being until Allied modders puts them in a .lib
    hl2sdk/blade/public/game/shared/berimbau/protobuf/berimbau_usermessages.pb.cc
    hl2sdk/blade/public/game/shared/berimbau/protobuf/berimbau_usermessage_helpers.cpp
    hl2sdk/blade/public/engine/protobuf/netmessages.pb.cc    
    #---------------------------------------------------------------------------
)