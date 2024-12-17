# dungeonGame


# Dungeon Adventure Game

Dungeon Adventure is a simple text-based dungeon exploration and combat game. The player must defeat creatures in different rooms and collect items to progress. The game allows the player to manage health and inventory.

## Features

- **Dungeon Rooms**: Different rooms are connected to form a dungeon map. The player can move between rooms.
- **Items**: Items found in rooms can be added to the player's inventory.
- **Combat**: The player can fight creatures in the dungeon.
- **Save and Load**: The player can save progress and load it later to continue from where they left off.
- **Inventory**: The player can collect and view items in their inventory.

## Game Commands

The game supports the following commands:

- **look**: The player can view the description of the current room and the items in it.
- **move <direction>**: The player can move in a given direction (up, down, left, right).
- **pickup**: The player can pick up items in the current room.
- **attack**: The player can attack creatures in the current room.
- **inventory**: The player can view the items in their inventory.
- **save <filepath>**: The player can save the game to a specified file.
- **load <filepath>**: The player can load a saved game from a specified file.
- **list**: The player can list saved games.
- **exit**: The game exits.

## Starting the Game

To start the game, run the program in a terminal or command prompt. At the beginning, the player will be given information about the commands. Then, they can use the commands to play the game.

## Save and Load Game

The game can be saved and loaded. Saved games are stored with the `.save` extension in the "saves" folder.

- To save the game, use the `save <filepath>` command.
- To load the game, use the `load <filepath>` command.

## Directory Structure

- **saves/**: Folder containing saved games.
- **DungeonAdventure.c**: The source code of the game.

## Compilation and Running

Since the game is written in C, you can compile and run it with the following commands:

1. **Compile**:
   ```bash
   gcc DungeonAdventure.c -o DungeonAdventure
