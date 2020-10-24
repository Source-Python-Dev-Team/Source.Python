/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "modules/memory/memory_tools.h"
#include "inetchannel.h"
#include "inetmsghandler.h"
#include "inetchannelinfo.h"
#include "inetmessage.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_net_channel_handler(scope);
void export_net_channel(scope);
void export_net_channel_info(scope);
void export_net_message(scope);


//-----------------------------------------------------------------------------
// Declare the _net_channel module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_net_channel)
{
	export_net_channel_handler(_net_channel);
	export_net_channel(_net_channel);
	export_net_channel_info(_net_channel);
	export_net_message(_net_channel);
}


//-----------------------------------------------------------------------------
// Exports INetChannelHandler.
//-----------------------------------------------------------------------------
void export_net_channel_handler(scope _net_channel)
{
	class_<INetChannelHandler, INetChannelHandler*, boost::noncopyable> NetChannelHandler("NetChannelHandler", no_init);
	NetChannelHandler ADD_MEM_TOOLS(INetChannelHandler);

	BEGIN_CLASS_INFO(INetChannelHandler)
		FUNCTION_INFO(ConnectionStart)
		FUNCTION_INFO(ConnectionClosing)
		FUNCTION_INFO(ConnectionCrashed)
		FUNCTION_INFO(PacketStart)
		FUNCTION_INFO(PacketEnd)
		FUNCTION_INFO(FileRequested)
		FUNCTION_INFO(FileReceived)
		FUNCTION_INFO(FileDenied)
		FUNCTION_INFO(FileSent)
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports INetChannelHandler.
//-----------------------------------------------------------------------------
void export_net_channel(scope _net_channel)
{
	class_<INetChannel, INetChannel*, boost::noncopyable> NetChannel("NetChannel", no_init);

	NetChannel.add_property(
		"msg_handler",
		make_function(&INetChannel::GetMsgHandler, reference_existing_object_policy())
	);

	NetChannel.def(
		"set_data_rate",
		&INetChannel::SetDataRate
	);

#if defined(ENGINE_BLADE) || defined(ENGINE_CSGO) || defined(ENGINE_LEFT4DEAD2)
	NetChannel.def(
		"send_file",
		&INetChannel::SendFile,
		(arg("file_name"), arg("transfer_id"), arg("is_replay_demo")=false)
	);

	NetChannel.def(
		"deny_file",
		&INetChannel::DenyFile,
		(arg("file_name"), arg("transfer_id"), arg("is_replay_demo")=false)
	);

	NetChannel.def(
		"request_file",
		&INetChannel::RequestFile,
		(arg("file_name"), arg("is_replay_demo")=false)
	);
#else
	NetChannel.def(
		"send_file",
		&INetChannel::SendFile,
		(arg("file_name"), arg("transfer_id"))
	);

	NetChannel.def(
		"deny_file",
		&INetChannel::DenyFile,
		(arg("file_name"), arg("transfer_id"))
	);

	NetChannel.def(
		"request_file",
		&INetChannel::RequestFile,
		(arg("file_name"))
	);
#endif

	NetChannel.def(
		"send_data",
		&INetChannel::SendData,
		("buffer", arg("reliable")=true)
	);

	// TODO: Export more

	NetChannel ADD_MEM_TOOLS(INetChannel);
}


//-----------------------------------------------------------------------------
// Exports INetChannelInfo.
//-----------------------------------------------------------------------------
void export_net_channel_info(scope _net_channel)
{
	_net_channel.attr("FLOW_OUTGOING") = FLOW_OUTGOING;
	_net_channel.attr("FLOW_INCOMING") = FLOW_INCOMING;
	_net_channel.attr("FLOW_IN_OUT") = MAX_FLOWS;

	class_<INetChannelInfo, INetChannelInfo*, boost::noncopyable> NetChannelInfo("NetChannelInfo", no_init);

	// TODO: Some methods require int pointers
	NetChannelInfo.add_property(
		"name",
		&INetChannelInfo::GetName,
		"Return the channel's name."
	);
	
	NetChannelInfo.add_property(
		"address",
		&INetChannelInfo::GetAddress,
		"Returns the net address of the player."
	);

	NetChannelInfo.add_property(
		"time",
		&INetChannelInfo::GetTime,
		"Return the current net time."
	);

	NetChannelInfo.add_property(
		"time_connected",
		&INetChannelInfo::GetTimeConnected,
		"Returns the amount of time the player has been connected."
	);

	NetChannelInfo.add_property(
		"buffer_size",
		&INetChannelInfo::GetBufferSize,
		"Return the packet history size."
	);

	NetChannelInfo.add_property(
		"data_rate",
		&INetChannelInfo::GetDataRate,
		"Return the send data rate in bytes/second."
	);

	NetChannelInfo.def(
		"is_loopback",
		&INetChannelInfo::IsLoopback,
		"Return True if loopback."
	);

	NetChannelInfo.def(
		"is_timing_out",
		&INetChannelInfo::IsTimingOut,
		"Return True if the channel is timing out."
	);

	NetChannelInfo.def(
		"is_playback",
		&INetChannelInfo::IsPlayback,
		"Return True if it's a demo playback."
	);

	NetChannelInfo.def(
		"get_latency",
		&INetChannelInfo::GetLatency,
		"Return the current latency (RTT). It's more accurate, but jittering."
	);

	NetChannelInfo.def(
		"get_avg_latency",
		&INetChannelInfo::GetAvgLatency,
		"Return the average latency in seconds."
	);

	NetChannelInfo.def(
		"get_avg_loss",
		&INetChannelInfo::GetAvgLoss,
		"Return the average packet loss."
	);

	NetChannelInfo.def(
		"get_avg_choke",
		&INetChannelInfo::GetAvgChoke,
		"Return the average packet choke."
	);

	NetChannelInfo.def(
		"get_avg_data",
		&INetChannelInfo::GetAvgData,
		"Return the average data flow in bytes/second."
	);

	NetChannelInfo.def(
		"get_avg_packets",
		&INetChannelInfo::GetAvgPackets,
		"Return the average packets/second."
	);

	NetChannelInfo.def(
		"get_total_data",
		&INetChannelInfo::GetTotalData,
		"Return the total flow in/out in bytes."
	);

	NetChannelInfo.def(
		"get_sequence_number",
		&INetChannelInfo::GetSequenceNr,
		"Return the last sent sequence number."
	);

	NetChannelInfo.def(
		"is_valid_packet",
		&INetChannelInfo::IsValidPacket,
		"Return True if the packet was not lost, dropped, choked or flushed."
	);

	NetChannelInfo.def(
		"get_packet_time",
		&INetChannelInfo::GetPacketTime,
		"Return the time when the packet was sent."
	);

	NetChannelInfo.def(
		"get_packet_bytes",
		&INetChannelInfo::GetPacketBytes,
		"Return the group size of this packet."
	);

	NetChannelInfo.def(
		"get_stream_progress",
		&INetChannelInfo::GetStreamProgress,
		"Return TCP progress if transmitting."
	);

	NetChannelInfo.add_property(
		"time_since_last_received",
		&INetChannelInfo::GetTimeSinceLastReceived,
		"Return the time since the last received packet in seconds."
	);

	NetChannelInfo.def(
		"get_command_interpolation_amount",
		&INetChannelInfo::GetCommandInterpolationAmount
	);

	NetChannelInfo.def(
		"get_packet_response_latency",
		&INetChannelInfo::GetPacketResponseLatency
	);

	NetChannelInfo.def(
		"get_remote_frame_rate",
		&INetChannelInfo::GetRemoteFramerate
	);

	NetChannelInfo.add_property(
		"timeout_seconds",
		&INetChannelInfo::GetTimeoutSeconds
	);
		
	NetChannelInfo ADD_MEM_TOOLS(INetChannelInfo);
}


//-----------------------------------------------------------------------------
// Exports INetMessage.
//-----------------------------------------------------------------------------
void export_net_message(scope _net_channel)
{
	class_<INetMessage, INetMessage*, boost::noncopyable> NetMessage("NetMessage", no_init);

	NetMessage.def(
		"process",
		&INetMessage::Process,
		"Call the recently set handler to process this message."
	);

	NetMessage.def(
		"read_from_buffer",
		&INetMessage::ReadFromBuffer,
		"Return True if parsing was OK."
	);

	NetMessage.def(
		"write_to_buffer",
		&INetMessage::WriteToBuffer,
		"Return True if writing was OK."
	);

	NetMessage.add_property(
		"reliable",
		&INetMessage::IsReliable,
		&INetMessage::SetReliable,
		"Return True if the message needs reliable handling."
	);

	NetMessage.add_property(
		"type",
		&INetMessage::GetType,
		"Return the module specific header tag. E.g. svc_serverinfo."
	);

	NetMessage.add_property(
		"group",
		&INetMessage::GetGroup,
		"Return the net message group."
	);

	NetMessage.add_property(
		"name",
		&INetMessage::GetName,
		"Return the network message name. E.g. svc_serverinfo."
	);

	NetMessage.add_property(
		"net_channel",
		make_function(
			&INetMessage::GetNetChannel,
			reference_existing_object_policy()
		),
		&INetMessage::SetNetChannel,
		"Return the net channel this message is from/for."
	);

	NetMessage.def(
		"to_string",
		&INetMessage::ToString,
		"Return a human readable string about the message content."
	);

	NetMessage ADD_MEM_TOOLS(INetMessage);
}