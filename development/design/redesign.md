# Redesign

Disable spawns for ships, junk, and items
Make game world larger
Make planets farther apart
Planets have a minimum distance between them
Ensure there is room between planets for belts
There should be some space beyond the planets as well
Implement belts
Maybe add belts rendering on the minimap and / or in the world when in dev mode?
Reenable junk spawns
Redo junk
Redo junk spawns
Reenable item spawns
Redo items
Redo item spawns
Reenable ship spawns
Redo ships
Redo ship spawns

Belts are defined by a pair of distances from a planet's center
Different planet types have different belt generation stats
Belts cannot overlap other belts, from their planet or any other
The farthest distance of any belt cannot exceed the minimum distance between planets

Asteroids should be generalized to Junk
Junk can be roughly round or rectangular and should use corresponding collision shapes
Other space junk could be metal space wrecks, etc.
Space junk should be prevalent near planets and rare farther out
Asteroids should be the opposite, rare near planets and more likely farther out
Additionally, asteroids should become rare again at a certain distance from planets, to create a vague "asteroid belt"
There are size classes of each type of junk
The size of a junk is also based on belt

The spawn rates for junk, ships, and items are affected by the player's belt
The type chances for junk, ships, and items are affected by the belt they are in (if in multiple belts, choose at random)
The player's belt affects their background and music
When the player changes belts, the background and music fade in / out
When the player has cargo, it should be shown being carried















The idea is to make the game feel more like cosmic heist.
Lots of flying, lots of dodging enemy ships, lots of ships crashing into each other and destroying each other
Ships dying should give score like in heist maybe
Narrowly dodging ships gives score just like junk
Maybe doing things with flare, like a spin, gives score?
The whole game world needs to start out much less dense with stuff
It should be larger with the planets spread out farther
Player and maybe other ships should be faster and more maneuverable
Some kind of alert level for cops like in GTA
Get everything balanced and feeling good and then look at upgrades again
Some ships have weapons, some ships are weapons

The warp drive can make the player invulnerable briefly when turned off
The faster the player was going, the longer the invulnerability window

Thoughts on upgrades:
Player ship can three weapons at once, one per weapon damage type
Might need to shuffle what's available, but the player chooses the weapon from a few choices
There should be enough weapon choices that they aren't all available at a given shop
Once a weapon is picked, it's locked in
A weapon for an empty slot is one possible choice at shops
Each weapon has an upgrade path
The upgrade path has a max level, and each level for the weapon gives it specific improvements
This might just be stats going up, but could also add special effects like homing for missiles
The next upgrade for an equipped weapon is another possible choice at shops
Replenishing health and repairing armor are also choices at shops
Shop choices have a level, 1-3, chosen at random
The higher the choice level, the better
Each ship can equip up to one addon
This is where cloak etc go
Works like weapons, once chosen it is locked in, appears as choice in shop, need enough available that they aren't all
in each shop, has an upgrade level
For several ship stats, stat upgrades exist
These appear in shops and upgrade the corresponding stat level
Stat levels improve the corresponding stat
One more slot, much like the addon slot
However, this slot can be emptied by using the equipped item
These do not have upgrade levels, but they do have 3 possible levels, determined by choice level

Events could happen, triggered by time, randomly, or other stuff
An event has some effect on the whole game world while it's in progress
Maybe dodging things is worth more, maybe all ships are sped up, maybe ship spawns are different, etc
Need good feedback when an event starts, is in progress, and ends
Events could end after a timer or maybe some other triggers

Events:
Two planets go to war with each other
They send warships against each other
These only attack their enemies and anyone who gets in their way

Wormholes can spawn as an item
Wormholes don't last long and then they despawn
Only one wormhole can exist at a time
They are not affected by the player's item vacuum
They can't move at all
They have a direction that is easy to see
If the player collects one, they enter a bonus stage
The main game world is paused when the player is in a bonus stage
A bonus stage is completely separate from and unaffected the main game world
There are various bonus stage types, and one is selected randomly
The type determines the background and music
It also determines spawns
It has a goal
The goal could be "reach the other side of the stage",
kill X ships, etc.
Once the goal is met, the player is warped instantly a random distance in the wormhole's direction
When entering and exiting a bonus stage, a fading in / out transition is used for both worlds

Ideas for addons:
* Time dilator
Slow down everything in the game world, giving the player more time to react
* Shield overcharger
Greatly increases shield stats
* Adaptive armor
Greatly increases armor stats
* Hull reinforcer
Greatly increases health stats

Less cops
Maybe less other enemies
New enemy types that just charge at the player, like in cosmic heist, these should probably be the most prevalent
enemies

With the ships being rebalanced, I think the weapons and other upgrades should be looked at again
Can all upgrades be used by enemies?
Can all weapons?
Might want to look into that...
Should enemies be able to have ANY upgrades?
But I should make sure every upgrade the player can use is specifically helpful in some situations
There might be too many weapons currently

Meta progression? I've been really loving rogue legacy 2...
Maybe some upgrades could instead become meta progression unlocks
Get the rest of the game working well before worrying about this

Ships and junk could change based on the nearest planet
So a planet could be taken over by zombies and then the ships and junk would be zombified

I need to play some heist and runner to compare
Heist has more fun flying I think, but runner has a whole game around it
I need to tweak the flying gameplay in runner to feel more exciting like heist
Once I have done this, look over design docs for both games and compare
Maybe look into Galaxy Chomp after this is done? Maybe...
