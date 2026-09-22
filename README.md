# <h2 align=center>*Generic Minesweeper*</h2>

<p align="center">
   <img src= "https://github.com/edin-aj/GenericMinesweeper/blob/main/models/m_face_clicked.texture?raw=true">
</p>

I built this a while back as a classic Minesweeper clone, using C++ and SFML. It's a simple project but it covers a good chunk of game logic: recursive block expansion, mine placement, flagging, scoring and a timer, all wrapped in a small rendering loop.

## <h2 align=center>*How it works*</h2>

The board is generated with a set of hidden mines placed randomly, and every other block is empty. Left-clicking an empty block reveals it, and if it has no neighboring mines, it recursively expands and opens up all the connected empty blocks around it, just like the original game.

Right-clicking a block flags/unflags it, which is tracked against the number of mines on the board so you can't over-flag. There's also a face button at the top *(neutral, surprised, clicked)* that reacts to your actions and lets you restart the board at any time.

On the side, the game keeps track of your score and a running timer, which starts the moment you make your first move and stops the instant you hit a mine.

## <h2 align=center>*Concept vs. Release*</h2>

The game was never fully finished *(an opening/menu screen was planned but never built)*, so here's a look at where it started versus where it ended up:

<p align="center">
   <img src= "https://github.com/edin-aj/GenericMinesweeper/blob/main/samples/plan_2023-11-27.png?raw=true" width="45%">
   <img src= "https://github.com/edin-aj/GenericMinesweeper/blob/main/samples/release.png?raw=true" width="45%">
</p>
<p align="center">
   <i>Left: original concept &nbsp;|&nbsp; Right: released state</i>
</p>

## <h2 align=center>*Dependencies*</h2>

This project is built with:
+ **[SFML](https://www.sfml-dev.org/)** *(Simple and Fast Multimedia Library)* — handles the window, rendering, and input.
+ **OpenAL** — used internally by SFML's audio module.

To build and run the project, you'll need to download SFML yourself and link it in the project, then place the required DLLs *(`sfml-graphics`, `sfml-window`, `sfml-system`, `sfml-audio`, `openal32`)* next to the compiled executable.

## <h2 align=center>*Tools*</h2>

`convert_binary_data.py` is a small script I wrote to embed the font file directly into the binary as a byte array *(`font_data.hpp`)*, instead of shipping it as a loose `.ttf` next to the executable. It's not the cleanest approach, but it kept the release self-contained.

## <h2 align=center>*License*</h2>

+ Source code is licensed under **MIT** — see [LICENSE](LICENSE).
+ Sprites/textures in `models/` are original work © edin-aj, licensed under **[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)** — free to reuse, just credit me.
+ `font_text.ttf` and `font_title.ttf` are third-party fonts, sourced from Fontspace *([Emotion Engine](https://www.fontspace.com/emotion-engine-font-f18002), [All The Way To The Sun](https://www.fontspace.com/all-the-way-to-the-sun-font-f29812))* — check their individual license terms before reuse.

## <h2 align=center>*Changelog:*</h2>

	2026 update:
	    - Initial Release (unfinished/abandoned project, uploaded as-is)

<table align="center">
  <tr>
    <td align="center">
      <h2>Issues, pull requests and repo</h2>
      <a href="https://github.com/edin-aj/GenericMinesweeper/issues">
        <img src="https://img.shields.io/github/issues/edin-aj/GenericMinesweeper" alt="GitHub issues">
      </a>
      <a href="https://github.com/edin-aj/GenericMinesweeper/issues?q=is%3Aissue+is%3Aclosed">
        <img src="https://img.shields.io/github/issues-closed/edin-aj/GenericMinesweeper" alt="GitHub issues closed">
      </a>
      <a href="https://github.com/edin-aj/GenericMinesweeper/pulls">
        <img src="https://img.shields.io/github/issues-pr/edin-aj/GenericMinesweeper" alt="GitHub pull requests">
      </a>
      <a href="https://github.com/edin-aj/GenericMinesweeper/pulls?q=is%3Apr+is%3Aclosed">
        <img src="https://img.shields.io/github/issues-pr-closed/edin-aj/GenericMinesweeper" alt="GitHub pull requests closed">
      </a>
      <br>
      <a href="https://github.com/edin-aj/GenericMinesweeper/network/members">
        <img src="https://img.shields.io/github/forks/edin-aj/GenericMinesweeper?style=for-the-badge&color=lighgreen" alt="Forks">
      </a>
      <a href="https://github.com/edin-aj/GenericMinesweeper/watchers">
        <img src="https://img.shields.io/github/watchers/edin-aj/GenericMinesweeper?style=for-the-badge&color=lighgreen" alt="Watchers">
      </a>
      <a href="https://github.com/edin-aj/GenericMinesweeper/commits/main">
        <img src="https://img.shields.io/github/last-commit/edin-aj/GenericMinesweeper?style=for-the-badge&color=lighgreen" alt="Last Commit">
      </a>
      <br>
      <h2>You can give me a star!</h2>
      <a href="https://github.com/edin-aj/GenericMinesweeper/stargazers">
        <img src="https://i.imgur.com/FyVXkZL.png" alt="Built with Love">
      </a>
    </td>
  </tr>
</table>