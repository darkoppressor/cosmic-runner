# Design

Arcade style, you play until you quit or lose
Single player
A game takes place in a pcg planetary system of set size
Game world is static (world objects don't move)
Player is a smuggler
Player controls a single spaceship

Ships have a collision box
Ships can thrust in any direction
The player's ship can thrust in 8 directions
Ships do not decelerate naturally
Ships have a brake that decelerates them to a stop
Ships have an acceleration thrust
Ships have a separate deceleration thrust, used for braking and "maneuvering"
Maneuvering occurs when a ship is thrusting at an angle very different from its current velocity direction
Ships have a velocity
Ships have a maximum velocity

Player begins at a planet, with an initial smuggling contract
A contract is completed by landing on the contract planet
When completing a contract, a new contract is assigned
A new contract fills the player's cargo hold with cargo
Player always has either one or zero contracts
Player can drop cargo at any time
Dropping cargo abandons the current contract
Dropped cargo cannot be picked back up
Player can land on a planet if he has a contract targeting it
Player can also land on a planet if he has no contract, and he will be assigned a new one
Landing ships are invisible to other ships and shots for AI and collision purposes, and do not move

Player can toggle a map overlay displaying the planetary system
The player's current location is shown on the map
A directional indicator pointing from the player to the contract target is shown on the map

A directional indicator is also shown in the game world
A notifier is shown when no contract is held
A notification should pop up briefly when a contract is completed
A notification should pop up briefly when a contract is lost

Player has a score
Score increases when:
Destroying ships, each ship has a point value
Picking up an item, each item has a point value
Completing a contract, this is a set number, and points are added before increasing the score multiplier
Dodging debris (explained below)

When a piece of debris is in dodge range and is not in the cooldown list or being tracked, begin tracking it
If the debris is hit, add it to the cooldown list
If tracked debris leaves the dodge range, add it to the cooldown list
If the player's speed is over the threshold when debris stops being tracked, add score

Player has a score multiplier
The score multiplier cannot drop below 1
Score multiplier increases when a contract is completed
Score multiplier decreases when a contract is lost via the player being caught by police
Score multiplier decreases when a contract is abandoned

If a ship is chasing or fleeing from the player and is damage/killed by debris, the player is blamed

When a contract is completed, the player may select one upgrade for their ship
The upgrade list is chosen completely at random from all upgrades not currently on the player's ship
There are three kinds of upgrades: active, passive, and weapon
All upgrades must be unique on a ship, no duplicates
Passive upgrades are permanent modifications to the ship
A ship may have up to one active upgrade at a time
If the player selects an active upgrade, and already has one, the old active upgrade is replaced with the new one
A ship can use its active upgrade
Active upgrades have a cooldown

Planets are dotted about

Ships have hull and shields
When a ship takes damage, it loses shields first, if it has any, and then hull
If a ship loses its hull entirely, it is destroyed, causing an explosion
If the player's ship is destroyed, it is a game over
Shields regen over time
Hull is restored from some kind of item

Items exist in the world, and are collected when touched
Only the player can collect items
If an item restores a stat, and the player has that stat at max, the item is not collected
Items can be generated in the world, or dropped by a destroyed ship
Items start out moving when dropped by a ship
Items' velocity and angular velocity quickly reduce to 0
Items near the player's ship are sucked towards it

There are three types of damage:
Solid: x2 to Shields, /2 to Hull
Explosive: x1.5 to Hull, x1.5 to Shields
Energy: x2 to Hull, /2 to Shields

A ship may have up to one weapon upgrade at a time
If the player selects a weapon upgrade, and already has one, the old weapon upgrade is replaced with the new one
Weapons fire automatically on a cooldown
Ships can enable/disable their weapon's firing
Weapons continue cooling down when disabled
Each weapon fires one kind of shot: bullet, missile, guided missile, or plasma blast
Bullets fly straight very fast and do solid damage on contact
Missiles fly straight and explode on contact
Guided missiles are just like missiles, but they follow the nearest target
Plasma blasts fly straight extremely fast (cannot miss any ship) and do energy damage on contact
Bullets and plasma blasts are destroyed when they deal damage, and thus can only damage one target, once
Shots despawn if they are far from the player
Explosions can hit any number of targets, once each, and deal explosive damage
Explosions despawn if they are far from the player

Player's ship has power (and only the player's ship)
Power decreases over time
If power runs out, player's ship is disabled and it is game over
Power is replenished with an item
Power is also replenished when completing a contract
A disabled ship stops responding to all controls, weapons do not fire (but weapons enabled status is unchanged),
any active upgrade ability is disabled, shields do not recharge, and the following passive upgrade abilities
are disabled if present: Point Defense

Player has notoriety
Notoriety decreases over time
Notoriety increases when player does damage to civilian or police ships
(this applies even if the effective damage done is 0)

NPC ships are spawned "near" the player
NPC ships far away from the player are despawned
Every set number of seconds, a ship spawn occurs
A number of ships to try to place around the player (minus any existing ships) is determined using the score multiplier
The type of ships spawned are determined by the player's current area, score multiplier, and notoriety

Items are spawned near the player in much the same way that NPC ships are
The number of items to try to place around the player is determined using the score multiplier
The type of items spawned are determined by the player's needs

For the purposes of NPC ships reacting to another ship, they always only look at the closest ship not of their faction
Ships and shots have a faction
Factions are: player, civilian, police, pirate, bounty_hunter, world (a special faction for damage done by world objects)
Faction is used by ships and shots to determine target to follow
The player is manually controlled obviously
Civilians spawn with a nearby target planet randomly selected, and try to fly there
Civilians can spawn anywhere
If they reach their target, they "land" and despawn
If they see a pirate ship, or a player ship and the player has tier 1 notoriety or greater, they flee from it
If they are fleeing and no longer have a reason to, they resume their course for their target
Civilians have no weapons
Pirates spawn wandering at random
Pirates only spawn when player is in deep space
Pirates flee from police and the player if they have tier 2 notoriety
Pirates chase civilians and the player if their notoriety is below tier 2
Bounty Hunters spawn wandering at random
Bounty Hunters can spawn anywhere
Bounty Hunters flee from no one
Bounty Hunters chase the player
Police spawn patrolling between their spawn point and a target point in a straight line
Police only spawn when player is near a planet
Police flee from no one
Police chase pirates
Police chase the player if their notoriety is tier 1 or higher
However, Police treatment of player is variable when they are chasing him
If the player's notoriety is tier 1, police will attempt to catch player in tractor beam
If player's notoriety is tier 2, police will attempt to kill player
Tractor beam is shortish range special police ability
Once active, it cannot be escaped by normal flight (unless maybe engines are very upgraded?)
A tractored player is ignored by all other ships
A tractoring ship brakes
The tractor beam pulls the player in towards the tractoring ship
When the player reaches the tractoring ship, the player is caught
If the player leaves tractoring range, the tractoring ship stops tractoring and resumes normal behavior
When the player is caught, any contract is lost, and notoriety is reset to 0

All non-player ships use their active upgrade (if any) as often as they can,
unless they are a police ship currently tractoring the player

Asteroids are scattered around the game world
Asteroids do not move
Asteroids do solid damage when colliding with a ship

No collision between ships
No collision between shots
No collision with planets

When near a planet, background should have planety stuff
Otherwise, player is in deep space, and background is just stars

Weapon Upgrades:
Shotgun Cannon: Fires bullets in a shotgun pattern, centered on target
Sniper Cannon: Fires a single bullet at the target
Quad Cannon: Fires 4 bullets in the 4 "cardinal directions", with the target being north
Single Missile: Fires a single missile at the target
Spread Missile: Fires a spread of missiles centered on the target
Circle Missile: Fires a complete circle of evenly spaced missiles centered on the ship
Single Guided Missile: Fires a single slow guided missile at the target
Triple Guided Missile: Fires a spread of 3 slow guided missiles centered on the target
Swarm Guided Missile: Fires a large spread of several fast guided missiles centered on the target
Plasma Gun: Locks onto the target briefly with a laser, then fires a plasma blast at the target

Active Upgrades:
Scanner: Only available to police ships. Has a brief startup animation during which time it is ineffective.
Once it has started up, it scans an area around the ship, in an obvious way. If this scan sees the player's ship,
the player's ship's cloak is disabled (if any), and if the player has a contract, his notoriety increases.
This upgrade consumes a set amount of power per use
EMP: Fires a circular pulse centered on the ship that expands to a set radius and then vanishes. Any ships caught in
this pulse (ignoring the firing ship) are disabled for a set period of time
This upgrade consumes a set amount of power per use
Cloak: While this is active, the ship is invisible (the player's own ship will still be shown, but semi-transparent)
No ships or shots can see the ship at all for AI purposes
Weapon and point defense firing is disabled while this is enabled
This upgrade begins cooling down when the cloak is disabled
Also, when the cloak is disabled, the weapon and point defense cooldowns (if any) are restarted
This upgrade consumes power at a set rate while it is active
This upgrade is disabled if the ship takes any damage (this applies even if the effective damage done is 0)
Warp Drive: While this is active, all collision is ignored
No ships or shots can see the ship at all for AI purposes
Ship controls do not respond while this is active, with the exception of disabling the Warp Drive
Weapon and point defense firing is disabled while this is enabled
Separate "warp" acceleration/deceleration thrust and max velocity are used,
and the ship automatically thrusts continually in the direction it was facing when the warp drive was activated
This upgrade begins cooling down when the warp drive is disabled
This upgrade consumes power at a set rate while it is active

Passive Upgrade Effects:
Max Shields Up: Maximum shields increased
Max Shields Down: Maximum shields decreased
Shield Recharge Rate Up: Shield recharge rate increased
Shield Recharge Rate Down: Shield recharge rate decreased
Max Hull Up: Maximum hull increased
Max Hull Down: Maximum hull decreased
Acceleration Thrust Up: Increases acceleration thrust
Acceleration Thrust Down: Decreases acceleration thrust
Deceleration Thrust Up: Increases deceleration thrust
Deceleration Thrust Down: Decreases deceleration thrust
Max Speed Up: Increases maximum speed
Max Speed Down: Decreases maximum speed
Cooldown Reduction: Cooldowns on all upgrades that have them are reduced
Cooldown Increase: Cooldowns on all upgrades that have them are increased
Solid Damage Up: Increases solid damage dealt by weapons
Solid Damage Down: Decreases solid damage dealt by weapons
Explosive Damage Up: Increases explosive damage dealt by weapons
Explosive Damage Down: Decreases explosive damage dealt by weapons
Energy Damage Up: Increases energy damage dealt by weapons
Energy Damage Down: Decreases energy damage dealt by weapons

Passive Upgrades:
Point Defense: A laser fires automatically on a cooldown at any nearby hostile missile or guided missile. Missiles and
guided missiles hit by this laser instantly explode
All other passive upgrades are described in the file passives.ods

--Stat Balancing--

Stat base values:
Max Hull: 0
Max Shields: 0
Shield Recharge Rate (ms): 1000
Acceleration Thrust: 0
Deceleration Thrust: 0
Max Speed: 0
Cooldown (ms): varies
Solid Damage: 10 damage for a single shot (rounded normally, minimum of 2)
Explosive Damage: 7 damage for a single shot (rounded normally, minimum of 2)
Energy Damage: 10 damage for a single shot (rounded normally, minimum of 2)

Stat tier values:
Max Hull: 20
Max Shields: 5
Shield Recharge Rate (ms): -150
Acceleration Thrust: 500
Deceleration Thrust: 1000
Max Speed: 50
Cooldown (ms): -250
Solid Damage: 3 damage for a single shot (rounded down, minimum of 1)
Explosive Damage: 2 damage for a single shot (rounded down, minimum of 1)
Energy Damage: 3 damage for a single shot (rounded down, minimum of 1)
