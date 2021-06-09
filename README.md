# far-horizons-engine
Specifications and reference engine for Far Horizons engine.

This specification does not document ...

# Key Concepts
A CLUSTER

A PLAYER

A SPECIES

A SYSTEM

## Objects and Units
The key difference between UNITs and OBJECTs is that UNITs can be created, moved between locations, and destroyed.
Most UNITS can be transfers as CARGO in a SHIP.
The exceptions are SHIPS, which can never be treated as CARGO.

|Object|Plural|Description|
|-|-|-|
|CLUSTER|CLUSTERS|The object that owns all data in a single game.|
|SYSTEM|SYSTEMS|?|
|SPECIES|SPECIES|?|
|PLANET|PLANETS|A PLANET occupies an ORBIT in a SYSTEM. PLANETS can be named by SPECIES. They can be the HOMEWORLD of a SPECIES, but they are never owned by any SPECIES.|
|COLONY|COLONIES|A collection of colonists on a PLANET that can produce RM or PC. The difference between a COLONY and a PLANET is that a PLANET can have many COLONIES. COLONIES can be destroyed by attrition or by combat.|
|SHUTTLE|SHUTTLES|Shuttles are game constructs that are used to move items within a system or to tow ships between systems. They are not SHIPS; they can't be created, destroyed, named, or managed by any SPECIES.|

|Unit|Plural|Description|
|-|-|-|
|ITEM|ITEMS|ITEMS can be transfered as CARGO in a SHIP|
|SHIP|SHIPS|?|
|TRANSPORT|TRANSPORTS|A TRANSPORT is a type of SHIP.|
|WARSHIP|

# Orbit
An ORBIT represents a spot in a SYSTEM that can be developed by any SPECIES in the game.

The orbit could be a planet (in the traditional sense), a moon orbiting a planet, or an asteroid belt.

For game purposes, the actual items in the orbit aren't important.
We use an abstraction to represent the resources that can be exploited and how hard it is to do so.

# Planet
A PLANET represents an ORBIT in a SYSTEM.

A SCAN of the SYSTEM will reveal the minimum life support level ("LSN") that the SPECIES must have to colonize the PLANET.
That number will be different for each SPECIES.

Players may share the results of a scan; please be aware that the LSN is for their species.

# Colony
A SPECIES may build a colony on any PLANET that meets the following conditions:
1. The PLANET is not the SPECIES HOMEWORLD.
1. The PLANET has been named by the SPECIES.
1. The SPECIES's LS is at least equal to the PLANET's LSN.
1. The PLANET has no COLONIES on it that are owned by this SPECIES.
1. The PLANET has no COLONIES on it or at least one of owners of any of those COLONIES has allowed this SPECIES to colonize the PLANET.

There are three types of COLONIES that may be created.
The type is determined by the units used to build the colony.

Type|Units Required|Mining Base|Manufacturing Base
GENERAL|CU + AU + IU|min(CU,IU) / 10|min(CU,AU)/10
MINING|CU + IU|min(CU,IU) / 10|0
RESORT|CU + AU|0|min(CU,AU) / 10

The key difference between the types is that a GENERAL colony will experience population growth.
Population will never increate on MINING and RESORT colonies.

This document will try to use COLONY when referring to any of the three types.

# Manufacturing Base
## Home World
TODO

## Colony
The Manufacturing Base of a COLONY is determined by the number of CU and AU units used to build the colony.
The formula is the lesser of the CU or AU units, divided by 10.

For example, if 142 CU and 159 AU were used, the Mining Base would be 142/10, giving 14.2.

# Mining Base
## Home World
The Mining Base of a HOMEWORLD is assigned when the CLUSTER is created.
It increases by about 2% each turn.
There is nothing that a SPECIES can do to increase the Mining Base of their HOMEWORLD.

## Colony
The Mining Base of a COLONY is determined by the number of CU and IU units used to build the colony.
The formula is the lesser of the CU or IU units divided by 10.

For example, if 142 CU and 139 IU were used, the Mining Base would be 139/10, giving 13.9.

# Cargo
Items (non-SHIP units) that need to be transferred between systems must be transported as CARGO on a SHIP.

A SPECIES may use any SHIP to transport CARGO that has the CAPACITY to store it.

ITEMS that need to be transferred between SHIPS, PLANETS, or COLONIES within a system do not need to be transported as CARGO.
The game engine allows an unlimited number of ITEMS to be transferred via SHUTTLES within a SYSTEM.

# License

## Source Code

## Documentation

# Glossary

