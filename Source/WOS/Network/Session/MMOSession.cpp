﻿#include "MMOSession.h"

#include "generated/account/ClientPacketHandler.gen.hpp"
#include "generated/mmo/ClientPacketHandler.gen.hpp"
#include "Managers/Manager.h"

void FMMOSession::OnConnected()
{
	FSession::OnConnected();
}

void FMMOSession::OnDisconnected()
{
	FSession::OnDisconnected();
}

void FMMOSession::OnReceive(std::span<char> buffer, int32 length)
{
	FSession::OnReceive(buffer, length);
	
	const auto Handler = gen::mmo::PacketHandler::getHandler(buffer);
	const TSharedPtr<FSession> SharedThis = AsShared();
	
	PushJob(Handler, SharedThis);
}