@SET GAME=%1
@SET PROTOFILE=%2
@SET BPL_BINDINGS=%3
@IF "%1"=="" (
@	SET /P GAME=Enter the game name: 
)
@IF "%2"=="" (
@	SET /P PROTOFILE=Enter protobuf file path within games dir: 
)
@IF "%3"=="" (
@	SET /P BPL_BINDINGS=Use BPL Bindings boost-python exports:
)

@SET SRC_DIR=F:\sourcepython\src
@SET CORE_DIR=%SRC_DIR%\core
@SET ENGINE_DIR=%CORE_DIR%\modules\engine
@SET USERMESSAGE_DIR=%ENGINE_DIR%\usermessage
@SET USERMESSAGE_GAME_DIR=%USERMESSAGE_DIR%\%GAME%
@SET MODULE_DIR=%CORE_DIR%\modules
@SET PROTOBUF_DIR=%SRC_DIR%\thirdparty\protobuf
@SET PROTOBUF_SRC_DIR=%PROTOBUF_DIR%\protobuf-2.3.0\src
@SET PROTOFILE_DIR=%PROTOBUF_SRC_DIR%

@echo -----------------------------------------------------
@echo GAME:			%GAME%
@echo PROTOFILE:		%PROTOFILE%
@echo SRC_DIR:		%SRC_DIR%
@echo CORE_DIR:		%CORE_DIR%
@echo MODULE_DIR:		%MODULE_DIR%
@echo ENGINE_DIR:		%ENGINE_DIR%
@echo USERMESSAGE_DIR:	%USERMESSAGE_DIR%
@echo USERMESSAGE_GAME_DIR:	%USERMESSAGE_GAME_DIR%
@echo PROTOBUF_DIR		%PROTOBUF_DIR%
@echo PROTOBUF_SRC_DIR	%PROTOBUF_SRC_DIR%
@echo PROTOFILE_DIR 		%PROTOFILE_DIR%
@echo BPL_BINDINGS:	%BPL_BINDINGS%
@echo -----------------------------------------------------

@IF "%BPL_BINDINGS%"=="y" (
@	echo Compiling protobuf messages and running boost python generation %GAME%\usermessages.proto...
@	.\protoc.exe --proto_path=%PROTOFILE_DIR% --bplbindings_out=%MODULE_DIR%\usermessages %PROTOFILE_DIR%\%GAME%\%PROTOFILE%
) ELSE (
@	echo Compiling protobuf messages on %GAME%\usermessages.proto...
@	.\protoc.exe --proto_path=%PROTOFILE_DIR% --cpp_out=%USERMESSAGE_GAME_DIR% %PROTOFILE_DIR%\%GAME%\%PROTOFILE%
)

@echo Renaming file as the boost python exporter doesn't name them correctly
@xcopy %USERMESSAGE_GAME_DIR%\%GAME% %USERMESSAGE_GAME_DIR% /y /i /v
@rd %USERMESSAGE_GAME_DIR%\%GAME% /S /Q


pause
