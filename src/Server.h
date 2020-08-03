// Copyright 2017-2019 Paul Nettle
//
// This file is part of Gobbledegook.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file in the root of the source tree.

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// >>
// >>>  INSIDE THIS FILE
// >>
//
// This is the top-level interface for the server. There is only one of these stored in the global `TheServer`. Use this object
// to configure your server's settings (there are surprisingly few of them.) It also contains the full server description and
// implementation.
//
// >>
// >>>  DISCUSSION
// >>
//
// See the discussion at the top of Server.cpp
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

#include <gio/gio.h>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "../include/Gobbledegook.h"
#include "DBusObject.h"

namespace ggk {

//
// Forward declarations
//

struct GattProperty;
struct GattCharacteristic;
struct DBusInterface;
struct DBusObjectPath;

//
// Implementation
//

struct Server
{
	//
	// Types
	//

	// Our server is a collection of D-Bus objects
	typedef std::list<DBusObject> Objects;

	//
	// Accessors
	//

	// Returns the set of objects that each represent the root of an object tree describing a group of services we are providing
	const Objects &getObjects() const { return objects; }

	// Returns the requested setting for BR/EDR (true = enabled, false = disabled)
	bool getEnableBREDR() const { return enableBREDR; }

	// Returns the requested setting for secure connections (true = enabled, false = disabled)
	bool getEnableSecureConnection() const { return enableSecureConnection; }

	// Returns the requested setting the connectable state (true = enabled, false = disabled)
	bool getEnableConnectable() const { return enableConnectable; }

	// Returns the requested setting the discoverable state (true = enabled, false = disabled)
	bool getEnableDiscoverable() const { return enableDiscoverable; }

	// Returns the requested setting the LE advertising state (true = enabled, false = disabled)
	bool getEnableAdvertising() const { return enableAdvertising; }

	// Returns the requested setting the bondable state (true = enabled, false = disabled)
	bool getEnableBondable() const { return enableBondable; }

	// Returns our registered data getter
	GGKServerDataGetter getDataGetter() const { return dataGetter; }

	// Returns our registered data setter
	GGKServerDataSetter getDataSetter() const { return dataSetter; }

	// Returns our advertising data
	const std::vector<uint8_t> &getAdvertisingData() const { return advertisingData; }

	// serviceName: The name of our server (collectino of services)
	//
	// This is set from the constructor.
	//
	// This is used to build the path for our Bluetooth services (and we'll go ahead and use it as the owned name as well for
	// consistency.)
	const std::string &getServiceName() const { return serviceName; }

	// Our owned name
	//
	// D-Bus uses owned names to locate servers on the bus. Think of this as a namespace within D-Bus. We building this with the
	// server name to keep things simple.
	std::string getOwnedName() const { return std::string("com.") + getServiceName(); }

	//
	// Initialization
	//

	// Our constructor builds our entire server description
	//
	// serviceName: The name of our server (collectino of services)
	//
	//     This is used to build the path for our Bluetooth services. It also provides the base for the D-Bus owned name (see
	//     getOwnedName.)
	//
	//     This value will be stored as lower-case only.
	//
	//     Retrieve this value using the `getName()` method.
	//
	// advertisingData: The data to be written into the advertising frame
	Server(const std::string &serviceName, const std::vector<uint8_t>& advertisingData,
		GGKServerDataGetter getter, GGKServerDataSetter setter);

	//
	// Utilitarian
	//

	// Find and call a D-Bus method within the given D-Bus object on the given D-Bus interface
	//
	// If the method was called, this method returns true, otherwise false.  There is no result from the method call itself.
	std::shared_ptr<const DBusInterface> findInterface(const DBusObjectPath &objectPath, const std::string &interfaceName) const;

	// Find a D-Bus method within the given D-Bus object on the given D-Bus interface
	//
	// If the method was found, it is returned, otherwise nullptr is returned
	bool callMethod(const DBusObjectPath &objectPath, const std::string &interfaceName, const std::string &methodName, GDBusConnection *pConnection, GVariant *pParameters, GDBusMethodInvocation *pInvocation, gpointer pUserData) const;

	// Find a GATT Property within the given D-Bus object on the given D-Bus interface
	//
	// If the property was found, it is returned, otherwise nullptr is returned
	const GattProperty *findProperty(const DBusObjectPath &objectPath, const std::string &interfaceName, const std::string &propertyName) const;

private:

	// Our server's objects
	Objects objects;

	// BR/EDR requested state
	bool enableBREDR;

	// Secure connection requested state
	bool enableSecureConnection;

	// Connectable requested state
	bool enableConnectable;

	// Discoverable requested state
	bool enableDiscoverable;

	// LE advertising requested state
	bool enableAdvertising;

	// Bondable requested state
	bool enableBondable;

	// The getter callback that is responsible for returning current server data that is shared over Bluetooth
	GGKServerDataGetter dataGetter;

	// The setter callback that is responsible for storing current server data that is shared over Bluetooth
	GGKServerDataSetter dataSetter;

	// Advertising frame data
	std::vector<uint8_t> advertisingData;

	// serviceName: The name of our server (collectino of services)
	//
	// This is set from the constructor.
	//
	// This is used to build the path for our Bluetooth services (and we'll go ahead and use it as the owned name as well for
	// consistency.)
	std::string serviceName;
};

// Our one and only server. It's a global.
extern std::shared_ptr<Server> TheServer;

}; // namespace ggk
