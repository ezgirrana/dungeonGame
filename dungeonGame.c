#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <sys/types.h> 

#define MAX_INVENTORY 5
#define MAX_DESC 256
#define MAX_ITEMS 10

typedef struct Room {
    char *description;
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
    char *items[MAX_ITEMS];
    int item_count;
    int creature_health;
} Room;

typedef struct Player {
    int health;
    int strength;
    char *inventory[MAX_INVENTORY];
    int inventory_count;
    Room *current_room;
} Player;

void look(Player *player);
void move(Player *player, const char *direction);
void pickup(Player *player);
void attack(Player *player);
void display_inventory(Player *player);
Room *createRoom(const char *description);
void linkRooms(Room *room1, Room *room2, const char *direction);
void freeRoom(Room *room);
void addItem(Room *room, const char *item);
void save_game(Player *player, const char *filepath);
void load_game(Player *player, const char *filepath, Room *rooms[]);
void list_saved_games();



int main() {
   
    Room *room1 = createRoom("A dark, damp dungeon room.");
    Room *room2 = createRoom("A bright room with a mysterious chest.");
    Room *room3 = createRoom("A narrow hallway with strange carvings on the walls.");
    Room *room4 = createRoom("Room full of golden treasures.");
    Room *room5 = createRoom("Dark dungeon with a 7-headed basilisk.");

   
    linkRooms(room1, room2, "right");
    linkRooms(room2, room3, "down");
    linkRooms(room3, room4, "right");
    linkRooms(room4, room5, "up");

    
    addItem(room1, "Sword");
    addItem(room4, "Golden Necklace");
    addItem(room4, "Magic Wand");
    addItem(room1, "Armor");
    room2->creature_health = 20;
    room5->creature_health = 120;

    
    Player player = {50, 10, {NULL}, 0, room1};

   
    Room *rooms[] = {room1, room2, room3, room4, room5};

    char command[64];
    printf("Welcome to the dungeon adventure game!\n");
    printf("To start the game, select:\n - look\n - move\n - pickup\n - attack\n - inventory\n - list\n - save\n -load\n");

    while (1) {
        printf("\n> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; 

        if (strncmp(command, "look", 4) == 0) {
            look(&player);
        } else if (strncmp(command, "move", 4) == 0) {
            char direction[16];
            sscanf(command + 5, "%s", direction);
            move(&player, direction);
        } else if (strncmp(command, "pickup", 6) == 0) {
            pickup(&player);
        } else if (strncmp(command, "attack", 6) == 0) {
            attack(&player);
        } else if (strncmp(command, "inventory", 9) == 0) {
            display_inventory(&player);
        } else if (strncmp(command, "list", 4) == 0) {
            list_saved_games();
        } else if (strncmp(command, "save", 4) == 0) {
            char filepath[64];
            sscanf(command + 5, "%s", filepath);
            save_game(&player, filepath);
        } else if (strncmp(command, "load", 4) == 0) {
            char filepath[64];
            sscanf(command + 5, "%s", filepath);
            load_game(&player, filepath, rooms);
        } else if (strncmp(command, "exit", 4) == 0) {
            printf("Exiting the game. Goodbye!\n");
            break;
        } else {
            printf("Unknown command. Try: look, move <direction>, pickup, attack, inventory, list, save <filepath>, load <filepath>, or exit.\n");
        }
    }

    
    freeRoom(room1);
    freeRoom(room2);
    freeRoom(room3);
    freeRoom(room4);
    freeRoom(room5);

    return 0;
}

Room *createRoom(const char *description) {
    Room *room = (Room *)malloc(sizeof(Room));
    if (!room) {
        perror("Failed to allocate memory for room");
        exit(EXIT_FAILURE);
    }
    room->description = strdup(description);
    room->up = room->down = room->left = room->right = NULL;
    room->item_count = 0;
    room->creature_health = 0;
    return room;
}

void addItem(Room *room, const char *item) {
    if (room->item_count < MAX_ITEMS) {
        room->items[room->item_count++] = strdup(item);
    } else {
        printf("Room cannot hold any more items!\n");
    }
}

void linkRooms(Room *room1, Room *room2, const char *direction) {
    if (strcmp(direction, "up") == 0) {
        room1->up = room2;
        room2->down = room1;
    } else if (strcmp(direction, "down") == 0) {
        room1->down = room2;
        room2->up = room1;
    } else if (strcmp(direction, "left") == 0) {
        room1->left = room2;
        room2->right = room1;
    } else if (strcmp(direction, "right") == 0) {
        room1->right = room2;
        room2->left = room1;
    }
}

void freeRoom(Room *room) {
    if (room) {
        free(room->description);
        for (int i = 0; i < room->item_count; i++) {
            free(room->items[i]);
        }
        free(room);
    }
}

void look(Player *player) {
    printf("%s\n", player->current_room->description);
    if (player->current_room->item_count > 0) {
        printf("You see the following items:\n");
        for (int i = 0; i < player->current_room->item_count; i++) {
            printf("- %s\n", player->current_room->items[i]);
        }
    }
    if (player->current_room->creature_health > 0) {
        printf("A creature with %d health is here!\n", player->current_room->creature_health);
    }
}

void move(Player *player, const char *direction) {
    Room *next_room = NULL;
    if (strcmp(direction, "up") == 0) {
        next_room = player->current_room->up;
    } else if (strcmp(direction, "down") == 0) {
        next_room = player->current_room->down;
    } else if (strcmp(direction, "left") == 0) {
        next_room = player->current_room->left;
    } else if (strcmp(direction, "right") == 0) {
        next_room = player->current_room->right;
    }

    if (next_room) {
        player->current_room = next_room;
        printf("You moved %s.\n", direction);
    } else {
        printf("You can't move %s from here.\n", direction);
    }
}

void pickup(Player *player) {
    if (player->current_room->item_count > 0) {
        printf("Which item would you like to pick up?\n");
        for (int i = 0; i < player->current_room->item_count; i++) {
            printf("%d. %s\n", i + 1, player->current_room->items[i]);
        }
        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice > 0 && choice <= player->current_room->item_count) {
            if (player->inventory_count < MAX_INVENTORY) {
                player->inventory[player->inventory_count++] = player->current_room->items[choice - 1];
                printf("You picked up a %s.\n", player->current_room->items[choice - 1]);

                for (int i = choice - 1; i < player->current_room->item_count - 1; i++) {
                    player->current_room->items[i] = player->current_room->items[i + 1];
                }
                player->current_room->item_count--;
            } else {
                printf("Your inventory is full!\n");
            }
        } else {
            printf("Invalid choice.\n");
        }
    } else {
        printf("There are no items to pick up here.\n");
    }
}

void attack(Player *player) {
      if (player->current_room->creature_health > 0) {
        while (player->health > 0 && player->current_room->creature_health > 0) {
            
            player->current_room->creature_health -= player->strength;
            printf("You attack the creature! It has %d health left.\n", player->current_room->creature_health);

            if (player->current_room->creature_health <= 0) {
                printf("You defeated the creature!\n");
                player->current_room->creature_health = 0; 
                return;
            }

           
            player->health -= 5;
            printf("The creature attacks you! You have %d health left.\n", player->health);

            if (player->health <= 0) {
                printf("You have been defeated by the creature. Game over.\n");
                exit(EXIT_FAILURE); // Oyundan çýk
            }
        }
    } else {
        printf("There's no creature to attack here.\n");
    }
}

void display_inventory(Player *player) {
    if (player->inventory_count > 0) {
        printf("Your inventory contains:\n");
        for (int i = 0; i < player->inventory_count; i++) {
            printf("- %s\n", player->inventory[i]);
        }
    } else {
        printf("Your inventory is empty.\n");
    }
}

void save_game(Player *player, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to save the game");
        return;
    }

   
    fprintf(file, "%d %d\n", player->health, player->strength);
    fprintf(file, "%d\n", player->inventory_count);
    for (int i = 0; i < player->inventory_count; i++) {
        fprintf(file, "%s\n", player->inventory[i]);
    }

    
    fprintf(file, "%s\n", player->current_room->description);

    fclose(file);
    printf("Game saved to %s\n", filepath);
}

void load_game(Player *player, const char *filepath, Room *rooms[]) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to load the game");
        return;
    }

    
    fscanf(file, "%d %d\n", &player->health, &player->strength);
    fscanf(file, "%d\n", &player->inventory_count);
    for (int i = 0; i < player->inventory_count; i++) {
        char item[64];
        fscanf(file, "%s\n", item);
        player->inventory[i] = strdup(item);
    }

    
    char room_description[MAX_DESC];
    fscanf(file, "%[^\n]\n", room_description);

    for (int i = 0; i < 5; i++) {
        if (strcmp(room_description, rooms[i]->description) == 0) {
            player->current_room = rooms[i];
            break;
        }
    }

    fclose(file);
    printf("Game loaded from %s\n", filepath);
}

void list_saved_games() {
     const char* folder = "saves"; 

    
    DIR* dir = opendir(folder);   
    if (dir == NULL) { 
       
        printf("'%s' Folder not found, being created...\n", folder);
        mkdir(folder); 
        dir = opendir(folder);
        if (dir == NULL) {
            perror("Could not create folder");
            return;
        }
    }

    struct dirent* entry;
    printf("Saved games:\n");
    int gameCount = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".save")) { 
            printf("- %s\n", entry->d_name);
            gameCount++;
        }
    }

    if (gameCount == 0) {
        printf("Saved game not found.\n");
    }

    closedir(dir);
}
