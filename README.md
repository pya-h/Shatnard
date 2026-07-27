# ♟️🎲 Shatnard

**Shatnard** is a two‑player console board game written in **C** — a mash‑up of two of the
world's most famous board games: **Chess**&nbsp;+&nbsp;**Backgammon**

The board and the movement rules are mostly **backgammon**, with a handful of twists borrowed
from **chess**: the pieces are *not* all identical — they carry chess names (Soldier, Elephant,
Horse, Castle, Queen, King). You still win the backgammon way: **be the first to bear all of
your pieces off the board.**

---

## 🖼️ Gameplay

Everything runs right in your terminal:

```
Player2's Castle              Road number 2                Player1 Cell | Player2 Cell
 1   2   3   4   5   6  | 1   2   3   4   5   6                         |
K_1     Q_1               C_2 |   S_2         S_1                       |
                          S_2 |   E_2         H_1                       |
                          H_2 |   S_2         S_1                       |
                          S_2 |               S_1                       |
                          C_2 |               E_1                       |
                              |                                         |
                              |                                         |
                          C_1 |               E_2                       |
                          S_1 |               S_2                       |
                          H_1 |   S_1         S_2                       |
Q_2                       S_1 |   E_1         H_2                       |
K_2                       C_1 |   S_1         S_2                       |
 1   2   3   4   5   6  | 1   2   3   4   5   6                         |
Player1's Castle              Road number 1

Dices: 2 , 3
Nice move Player 1 , now please enter your second source and destination: C2_1 C2_4
```

<!-- Drop the real screenshot at assets/gameplay.png and it will render below. -->
![Shatnard gameplay](assets/gameplay.png)

---

## 🗺️ The Board

The board is split into **four quadrants**, each with **6 rooms**:

| Region  | Address prefix | Belongs to |
|---------|----------------|------------|
| Player 1's Castle | `C1` | Player 1 |
| Player 1's Road   | `R1` | Player 1 |
| Player 2's Castle | `C2` | Player 2 |
| Player 2's Road   | `R2` | Player 2 |

On the right side, two extra columns — **Player 1 Cell** and **Player 2 Cell** — show pieces
that have been **captured** and sent to the "cell" (backgammon's *bar*). They must re‑enter the
board before that player can make any other move.

### The pieces

Every piece is written as `X_p` — a **type** letter and the **player** number (`1` or `2`):

| Letter | Piece |
|--------|-------|
| `S` | Soldier |
| `E` | Elephant |
| `H` | Horse |
| `C` | Castle (Rook) |
| `Q` | Queen |
| `K` | King |

Each player starts with **15 pieces**.

---

## 🎯 How to Play

### Addressing a room

An address is the **region prefix** + `_` + the **room number (1–6)**, and it is
**case‑insensitive**:

```
C2_1    →  Player 2's Castle, room 1
R1_3    →  Player 1's Road, room 3
O       →  bear a piece OFF the board (the goal!)
```

### A turn, step by step

1. Press **`d`** (or `D`) to **throw the dice**.
2. You get **two moves** — or **four moves** if you roll **doubles**.
3. For **each** move, type **two addresses**: a **source** and a **destination**, separated by a
   space. For example, to move a piece from Player 2's Castle room 1 forward by a 3:
   ```
   C2_1 C2_4
   ```
4. To **bear a piece off**, send it to `O`:
   ```
   C1_1 O
   ```
5. When your turn ends you may **save** (press `s`) or continue to the next player.

### Core rules (backgammon‑style)

- 🎲 Each die value is a **step count**; the two dice can be spent on one or two pieces.
- 🥊 Landing on a room that holds a **single** enemy piece **captures** it — it goes to that
  player's **Cell**. A room stacked with **2+** enemy pieces is **blocked**.
- ⛓️ If any of your pieces are in the **Cell**, you **must** bring them back onto the board
  first before making any other move.
- 🏁 You can only start **bearing pieces off** (`O`) once **all** of your remaining pieces are
  home in your Castle.
- 🏆 The **winner** is the first player to move **all** of their pieces off the board.

### Twists that make it Shatnard 🌀

- 🕳️ **Holes** — some rooms hide a hole. Step into one and that piece is **trapped: you are
  banned for 2 turns.**
- 🪙 **Coins / Gold** — some rooms hide gold. Collect it, and you can spend a coin to **rescue a
  captured piece** from your Cell straight back onto the board.
- Holes and coins are placed **randomly** at the start of every game, so no two matches feel the
  same.

---

## ✨ Two Implementations, Two Approaches

This project ships **two independent implementations** of the same game, each built with a
different data model, algorithms and feature set.

### 🟢 [`main.c`](main.c) — the full‑featured version (primary)

The polished, more professional build. Internally it models the board as flat integer arrays
and layers a full feature set on top:

- 💾 **Save & load** — games are written to `gameSave.dat`; each save is numbered
  (`Shatnard1`, `Shatnard2`, …) and tracked in `numberOfSavedGames.dat`, so **multiple games /
  multiple users** can be kept side by side.
- 📊 **Live cell table & scores** — an on‑screen panel shows which pieces are captured (in the
  Cell) for **both** players at all times.
- 🗺️ **Custom boards from a map file** — start a game from an external **map file** and the UI
  rebuilds itself around whatever (rule‑valid) arrangement you describe.
- 🙂 A friendlier, more readable interface overall.

On launch it offers a menu:

```
1. Continue              (load a saved game)
2. New Game
3. Load From Map File
```

### 🔵 [`Shatnard.var2.c`](Shatnard.var2.c) — the basic version *(the "second approach")*

An **alternative, simpler implementation** of the same game, built with a completely different
approach and algorithms — it uses C **`struct`s** (pieces, players and rooms as structured
records) instead of the flat integer arrays used by `main.c`.

It is a **direct, no‑frills** build: it jumps straight to the board and focuses purely on the
gameplay loop — **no** save/load, map files or scoreboards. The trade‑off is a **much smaller,
simpler codebase** that is easy to read and follow.

> 📌 *This basic "second approach" file is the subject of a separate follow‑up task — it's only
> introduced here for reference.*

| | `main.c` (full) | `Shatnard.var2.c` (basic) |
|---|:---:|:---:|
| Data model | flat integer arrays | C `struct`s |
| Save / load games | ✅ | ❌ |
| Multiple saved games | ✅ | ❌ |
| Live cell table & scores | ✅ | ❌ |
| Load board from map file | ✅ | ❌ |
| Holes & coins | ✅ | ✅ |
| Code size / complexity | larger, more professional | smaller, simpler |

---

## 🛠️ Build & Run

You need a C compiler (`gcc`/`clang`). A small cross‑platform helper,
[`include/console.h`](include/console.h), provides `getch()` / `getche()` (via `termios` on
Linux/macOS) and a `CLEAR_SCREEN` macro, so no external console library is required.

Compile and run the full version:

```bash
gcc main.c -o shatnard
./shatnard
```

Or the basic version:

```bash
gcc Shatnard.var2.c -o shatnard-basic
./shatnard-basic
```

---

## ⌨️ Controls Reference

| Key | Action |
|-----|--------|
| `1` / `2` / `3` | Menu: Continue · New Game · Load From Map File |
| `d` / `D` | Throw the dice |
| `<source> <destination>` | Make a move (e.g. `C2_1 C2_4`) |
| `<source> O` | Bear a piece off the board |
| `y` / `Y` | Use a collected coin to rescue a captured piece |
| `s` / `S` | Save the current game (end of turn) |