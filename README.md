# The Pathfinding Project

🔡 Status: Functional, mostly

❗ This is meant to be injected in MC Forge 1.8.9!

🤔 The purpose of this project is to develop a Minecraft external cheat that can do all sorts of pathfinding.

# How to use

❗ When injecting, make sure you are in a world.

🧰 Available console commands:
  1. goto
  2. makepath
  3. print
  4. send
  5. end

## 1. goto

Pathfind from your location to the specified coordinates.

If mid-air, the starting location is the first block below you.

The target must be a solid block.

Does not check for invalid block names!

```
goto x y z blockName

x, y, z = coordinates to pathfind to
(optional) blockName = the internal name (default is stone) of the block to make the path out of
```

Example:
```
goto 12 65 -33 redstone_block  // Will make a path out of redstone blocks
goto 0 55 103                  // Will make a stone path
```

## 2. makepath

Pathfind from a location to another location.

Both locations must be solid blocks.

Does not check for invalid block names!

```
makepath x1 y1 z1 x2 y2 z2 blockName
x1, y1, z1 = coordinates of the starting location
x2, y2, z2 = coordinates of the target location
(optional) blockName = the internal name (default is stone) of the block to make the path out of
```

Example:
```
makepath -10 3 20 50 12 45  // Will make a path from (-10, 3, 20) to (50, 12, 45) out of stone
```

## 3. print

Shows your input as an in-game message only you can see.

```
print message
(optional) message = the string which will be displayed in-game
```

Example:
```
print hey mom  // Will show the message "hey mom" in your chat
```

## 4. send

Sends a message from the player.

```
send message
(optional) message = the string which will be sent in chat
```

Example:
```
send Hi guys!  // Will send "Hi guys!" in chat.
```

## 5. end

Ends the execution of the DLL.

```
end
```

# Goals:

- 2D pathfinding ✔️
- 3D pathfinding 👷‍♂️ (WIP)
- Smart pathfinding (Be able to jump, go through gaps etc) 👷‍♂️ (WIP)
- Create and traverse a path automatically 👷‍♂️ (WIP)
