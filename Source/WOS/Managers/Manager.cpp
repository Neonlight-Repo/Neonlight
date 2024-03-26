// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager.h"

#include "UISystem.h"
#include "generated/mmo/ClientPacketHandler.gen.hpp"
#include "Kismet/GameplayStatics.h"
#include "Network/Session/Session.h"
#include "Managers/Network.h"
#include "Session/MMOSession.h"

UManager::UManager() : NetworkObject(nullptr)
{
	NetworkClass = UNetwork::StaticClass();
	UISystemClass = UUISystem::StaticClass();
}

UManager::~UManager()
{
}

void UManager::ConnectToServer(ServerType Type, SessionFactoryFunc SessionFactory) const
{
	if (!NetworkObject->Connect(Type, net::Endpoint(net::IpAddress::Loopback, static_cast<uint16>(Type)), SessionFactory))
	{
		UE_LOG(LogNet, Warning, TEXT("Can't connect with server."));
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Type::Quit, false);
	}
}

void UManager::HandlePacket() const
{
	if (!NetworkObject)
		return;
	const auto& Sessions = NetworkObject->GetSessions();
	for (const auto& Session : Sessions)
		Session->Flush();
}

void UManager::HandleLogin(gen::account::LoginRes* Packet) const
{
	auto World = GetWorld();
	if (Packet->success)
	{
		UISystemObject->ShowPopup(World, TEXT("알림"), TEXT("로그인 성공."));
		
		NetworkObject->SetUUID(Packet->uuid);
		ConnectToServer(ServerType::MMO, [](TSharedPtr<net::Socket> Socket)
		{
			auto Session = MakeShared<FMMOSession>(Socket);
			return Session;
		});
	}
	else
	{
		switch (Packet->cause)
		{
		case gen::account::EXIST:
			UI()->ShowPopup(World, TEXT("알림"), TEXT("이미 접속하고 있는 유저가 있습니다."));
			break;
		case gen::account::INVALID:
			UI(GetWorld())->ShowPopup(World, TEXT("알림"), TEXT("닉네임이나 비밀번호가 잘못되었습니다."));
			break;
		default:
			break;;
		}
	}
}

void UManager::HandleRegister(gen::account::RegisterRes* Packet)
{
	auto World = GetWorld();
	if (Packet->success)
	{
		UI()->ShowPopup(World, TEXT("알림"), TEXT("회원등록에 성공했습니다."));
	}
	else
	{
		UI()->ShowPopup(World, TEXT("알림"), TEXT("이미 존재하는 닉네임입니다."));
	}	
}

TObjectPtr<UNetwork> UManager::Net(const UWorld* World)
{
	return Get(World)->NetworkObject;
}

TObjectPtr<UUISystem> UManager::UI(const UWorld* World)
{
	return Get(World)->UISystemObject;
}

UManager* UManager::Get(const UWorld* World)
{
	if (World == nullptr)
		return nullptr;
	const auto Manager = CastChecked<UManager>(UGameplayStatics::GetGameInstance(World));
	if (Manager != nullptr)
	{
		Manager->Initialize();
		return Manager;
	}
#if WITH_EDITOR
	UE_LOG(LogTemp, Error, TEXT("Invalid Game Instance"));
#endif

	const auto Instance = NewObject<UManager>();
	Instance->Initialize();
	return Instance;
}

void UManager::Initialize()
{
	INIT_MANAGER(Network);
	INIT_MANAGER(UISystem);
}
