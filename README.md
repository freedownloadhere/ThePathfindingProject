# The Pathfinding Project

🔡 Status: Partially functional

❗ This is meant to be injected in MC Forge 1.8.9!

🤔 The purpose of this project is to develop a Minecraft external cheat that can do all sorts of pathfinding.

# How to use

❗ When injecting, make sure you are in-game.

🧰 To use the pathfinder, type the following line in chat:

```
.tpp goto x y z -setblock blockName

x, y, z = coordinates to pathfind to
blockName = the internal name (e.g. stone) of the block to make the path out of
  (this works only on your own worlds, of course)
```

Example:
```
.tpp goto 12 65 33 -setblock sand
```

# Goals:

- 2D pathfinding ✔️
- 3D pathfinding 👷‍♂️ (WIP)
- Smart pathfinding (Be able to jump, go through gaps etc) 👷‍♂️ (WIP)
- Create and traverse a path automatically 👷‍♂️ (WIP)
