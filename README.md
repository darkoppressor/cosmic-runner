# cosmic-runner

[![Build Status](https://wells-family.xyz/jenkins/buildStatus/icon?job=cosmic-runner)](https://wells-family.xyz/jenkins/job/cosmic-runner/)

A game in development by Cheese and Bacon Games
https://cheeseandbacon.org/

This is the source code for a game currently being developed by Cheese and Bacon Games.

Cosmic Runner is a top-down, 2D action game in an arcade-y style. The game world is procedurally generated.
The player controls a spaceship, and smuggles cargo from planet to planet. The player must avoid police ships,
which periodically scan the cargo of nearby ships. The player must also avoid pirates, who will attack without warning,
and may prey upon civilian ships. Causing too much havoc can lead to police ships attempting to arrest the player,
or even destroy them outright. Becoming truly infamous leads to the spawning of bounty hunters that seek only the
player's destruction. As the player completes cargo runs and increases their score and score multiplier, they obtain
upgrades for their ship.

Ship upgrades can be one of three categories:

    Passive: simply improves some feature of the ship
    Active: usable with the active skill key, and causes some specific event to occur
    Weapon: fires automatically at nearby ships if weapons systems are enabled

## For players

[System requirements](docs/systemRequirements.md)
[Changelog](docs/changelog.md)
[Android permissions](docs/androidPermissions.md)

## For developers

[Ideas](development/ideas.md)
[To do](development/toDo.md)

[Redesign](development/design/redesign.md)
[Design](development/design/design.md)
[Passives](development/design/passives.ods)
[Ships](development/design/ships.ods)

### Updating the version

    ~/build-server/cheese-engine/tools/version <PROJECT-DIRECTORY> <VERSION-COMPONENT> [STATUS]
    ~/build-server/cheese-engine/tools/build-date <PROJECT-DIRECTORY>

In version.cpp:

        Update the version series in Engine_Version::populate_version_series to include the new version

When an updated version has been pushed to main, tag the commit locally and remotely:

    git tag <VERSION> -m "<VERSION>"
    git push origin tag <VERSION>

## License

This game's source code is licensed under the MIT License. See [LICENSE.md](docs/LICENSE.md) for the full license text.

This game's data is not under the same license as the source code. See [contentLicense.md](docs/contentLicense.md) for
game data licensing details.

For third party licenses for dependencies, see [the thirdPartyLicenses directory](docs/thirdPartyLicenses).
