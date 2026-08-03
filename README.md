# loading_y

Timing prompt for **open.mp** / SA-MP: a blue fill loops across a bar and the player must press **Y** when it reaches a single random marker.

Textdraw layout comes from Nickk's TextDraw editor (`loading_y.pwn`).

**Credits:** Habibi / Janzzzz

---

## Repo layout (add your files here)

After you upload, this folder should look like:

```
loading_y/
├── README.md                 ← this file
├── include/
│   └── loading_y.inc         ← drop your include here
└── gamemodes/
    └── loading_y_test.pwn    ← drop your test gamemode here
```

Optional: add `LICENSE`, `.gitignore`, screenshots.

---

## Mechanic

1. Show the prompt with `LoadingY_Show`.
2. Blue fill grows left to right, then shrinks back (loops).
3. Only **one** grey/white marker is shown (picked randomly from the NTD marker slots).
4. Press **Y** when the blue fill reaches that marker.
5. Hit → `OnPlayerLoadingY` and UI is destroyed.
6. Miss → `OnPlayerLoadingYFail`, UI stays and keeps looping.

---

## Install

1. Copy `include/loading_y.inc` into your server’s includes folder (e.g. `qawno/include/`).
2. Include it after open.mp:

```pawn
#include <open.mp>
#include <loading_y>
```

3. Wire the hooks in your gamemode:

```pawn
public OnPlayerConnect(playerid)
{
    LoadingY_OnPlayerConnect(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    LoadingY_OnPlayerDisconnect(playerid, reason);
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (LoadingY_OnPlayerKeyStateChange(playerid, newkeys, oldkeys))
        return 1;
    return 1;
}
```

4. Handle results:

```pawn
public OnPlayerLoadingY(playerid, actionid)
{
    // success
    return 1;
}

public OnPlayerLoadingYFail(playerid, actionid)
{
    // wrong timing — prompt still open
    return 1;
}
```

Use ASCII-only characters in `SendClientMessage` strings (no em dashes). SA-MP chat can turn them into garbage glyphs.

---

## API

### `LoadingY_Show(playerid, const action[], actionid = 0)`

Shows the bar and starts the loop.

- `action` — text after `PRESS Y TO `
- `actionid` — passed back to the callbacks

```pawn
LoadingY_Show(playerid, "CRAFTING COMPONENT...", ACTION_CRAFT);
```

### `LoadingY_Hide(playerid)`

Stops the timer and destroys all prompt textdraws.

### `LoadingY_IsActive(playerid)`

Returns whether the prompt is open for that player.

### `LoadingY_GetActionId(playerid)`

Current `actionid`, or `-1` if inactive.

### `LoadingY_GetActionText(playerid, dest[], len = sizeof(dest))`

Copies the current action string into `dest`.

---

## Callbacks

| Callback | When |
|----------|------|
| `OnPlayerLoadingY(playerid, actionid)` | Pressed Y on the marker |
| `OnPlayerLoadingYFail(playerid, actionid)` | Pressed Y off the marker (keeps looping) |

---

## Optional defines

Define these **before** `#include <loading_y>` to override defaults:

```pawn
#define LOADING_Y_MAX_TEXT       (64)
#define LOADING_Y_TICK_MS        (30)
#define LOADING_Y_SPEED          (1.6)
#define LOADING_Y_MARKER_WIDTH   (4.5)
#define LOADING_Y_HIT_TOLERANCE  (6.5)
#define LOADING_Y_LEFT_X         (268.0)
#define LOADING_Y_MIN_X          (272.0)
#define LOADING_Y_MAX_X          (343.0)
```

| Define | Meaning |
|--------|---------|
| `LOADING_Y_SPEED` | Fill edge speed per tick |
| `LOADING_Y_HIT_TOLERANCE` | How close the blue edge must be to the marker |
| `LOADING_Y_MIN_X` / `MAX_X` | Fill travel range inside the base bar |

---

## Test gamemode

1. Put `loading_y_test.pwn` in your server `gamemodes/`.
2. Point the include path at this repo’s `include/` (or copy `loading_y.inc` into `qawno/include/`).
3. In `config.json`:

```json
"main_scripts": [
    "loading_y_test 1"
]
```

4. Compile (example with qawno):

```bash
qawno/pawncc.exe gamemodes/loading_y_test.pwn -iqawno/include -ogamemodes/loading_y_test -d3 -Z+
```

5. Start the server and join.

### Commands

| Command | Effect |
|---------|--------|
| `/craft` | Prompt: `PRESS Y TO CRAFTING COMPONENT...` |
| `/heal` | Prompt: `PRESS Y TO USE MEDKIT` |
| `/hidey` | Cancel / hide prompt early |

---

## Notes

- Uses **player textdraws** so each player can have their own prompt text.
- On success/hide, textdraws are fully destroyed (not only hidden).
- Empty slots use `INVALID_PLAYER_TEXT_DRAW` (not `0`), because ID `0` is a valid textdraw.

---

## Upload to GitHub

From this folder:

```bash
cd loading_y
git init
git add .
# copy include/ + gamemodes/ here first, then:
git commit -m "Initial loading_y release"
gh repo create loading_y --public --source=. --remote=origin --push
```

Or create an empty repo on GitHub and drag-drop `include/`, `gamemodes/`, and this `README.md`.
