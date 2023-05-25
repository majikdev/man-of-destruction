### Man of Destruction

**Man of Destruction** is a two-dimensional puzzle game with emphasis on speed. Cause destruction and create a path to the end of the level while avoiding your own explosions and the debris caused by them.

**In-game controls:**

* **W** - move up;
* **A** - move left;
* **S** - move down;
* **D** - move right;
* **Left mouse button** - throw dynamite;
* **Esc** - pause the game;
* **F5** - retry the level;
* **F11** - toggle fullscreen.

**Installation steps:**

1. Download the latest [release](/releases);
2. Extract the downloaded archive;
3. Run `ManOfDestruction.exe`.

### Custom levels

Levels included in the base game can be replaced and custom levels can be added to the game.
To create a level, use the level editor found in `tools/level_editor.py`.
The level editor requires _Python 3.10_ or newer to be installed.

**The following must be specified for a custom level:**

1. **Name** - the name of the level (up to 24 characters, default: `output`);
2. **Biome** - the biome of the level (up to 24 characters, default: `grass`)<br>_This corresponds to the name of a file in the `assets/sprites/level` directory;_
3. **Width** - the width of the level in tiles (integer from 4 to 32);
4. **Height** - the height of the level in tiles (integer from 4 to 32);
5. **Tilemap** - the text representation of the level's tiles (_width_ columns, _height_ rows):
   * ` ` - ground;
   * `S` - start tile;
   * `F` - finish tile;
   * `D` - dynamite pickup;
   * `1` - stone block;
   * `2` - wood block;
   * `3` - dynamite block;
   * `#` - indestructible block.

### Custom resources

The sprite sheets and sounds included in the game can be modified and replaced with custom ones.
For these modifications to work, follow the requirements for both resource types below.

**Sprite sheet requirements:**

* Bitmap (`.bmp`) file format;
* 32-bit (A8 R8 G8 B8) colour;
* Keep the original resolution.

**Sound requirements:**

* Waveform (`.wav`) file format.