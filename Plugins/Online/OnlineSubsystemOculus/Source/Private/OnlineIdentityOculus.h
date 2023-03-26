// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemOculus.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemOculusTypes.h"
#include "OVR_Platform.h"

/**
* Info associated with an user account generated by this online service
*/
class FUserOnlineAccountOculus :
	public FUserOnlineAccount
{

public:

	// FOnlineUser

	virtual FUniqueNetIdRef GetUserId() const override { return UserId; }
	virtual FString GetRealName() const override { return Name; }
	virtual FString GetDisplayName(const FString& Platform = FString()) const override { return Name; }
	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override;
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override;

	// FUserOnlineAccount

	virtual FString GetAccessToken() const override { return TEXT("ACCESSTOKEN"); }
	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override;

	// FUserOnlineAccountOculus

	FUserOnlineAccountOculus(const FUniqueNetIdRef& InUserId, const FString& InName)
		: UserId(InUserId),
		Name(InName)
	{ }

	virtual ~FUserOnlineAccountOculus()
	{
	}

	/** User Id represented as a FUniqueNetId */
	FUniqueNetIdRef UserId;

	/** Additional key/value pair data related to auth */
	TMap<FString, FString> AdditionalAuthData;
	/** Additional key/value pair data related to user attribution */
	TMap<FString, FString> UserAttributes;

private:
	FString Name;
};

/**
* Oculus service implementation of the online identity interface
*/
class FOnlineIdentityOculus : public IOnlineIdentity
{
public:

	// IOnlineIdentity

	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
	virtual bool Logout(int32 LocalUserNum) override;
	virtual bool AutoLogin(int32 LocalUserNum) override;
	virtual TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	virtual TArray<TSharedPtr<FUserOnlineAccount> > GetAllUserAccounts() const override;
	virtual FUniqueNetIdPtr GetUniquePlayerId(int32 LocalUserNum) const override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(const FString& Str) override;
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
	virtual ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override;
	virtual FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	virtual FString GetAuthToken(int32 LocalUserNum) const override;
	virtual void RevokeAuthToken(const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	virtual void GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate) override;
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const override;
	virtual FString GetAuthType() const override;

	// FOnlineIdentityOculus

	/**
	* Constructor
	*
	* @param InSubsystem online subsystem being used
	*/
	FOnlineIdentityOculus(FOnlineSubsystemOculus& InSubsystem);

	/**
	* Default destructor
	*/
	virtual ~FOnlineIdentityOculus() = default;

PACKAGE_SCOPE:

	/**
	* ** INTERNAL **
	* Called when we get the results back from the MessageQueue
	*/
	void OnLoginComplete(ovrMessageHandle Message, bool bIsError, int32 LocalUserNum);

private:

	/** Reference to the owning subsystem */
	FOnlineSubsystemOculus& OculusSubsystem;
	
	/** Ids mapped to locally registered users */
	TMap<int32, FUniqueNetIdRef> UserIds;

	/** Ids mapped to locally registered users */
	TUniqueNetIdMap<TSharedRef<FUserOnlineAccountOculus>> UserAccounts;
};

typedef TSharedPtr<FOnlineIdentityOculus, ESPMode::ThreadSafe> FOnlineIdentityOculusPtr;