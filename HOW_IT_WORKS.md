# How loading_y works

**Credits:** Habibi / Janzzzz

Back to: [README.md](README.md)

This page explains the **player experience**, what moves on screen, and how your gamemode should use it.

---

## What the player sees

```
                    PRESS Y TO CRAFTING COMPONENT...
        ┌──────────────────────────────────────────┐
        │████████████░░░░░░░░░░░░  │               │
        │            ^             │  white marker │
        │         blue fill                     │
        └──────────────────────────────────────────┘
              left ←──────────────→ right
```

| Piece | What it is |
|-------|------------|
| Dark base bar | Background track |
| Blue bar | Moving “fill” that grows / shrinks |
| One white marker | Target zone (random each time the prompt opens) |
| Top text | `PRESS Y TO` + your action string |

Only **one** marker is visible. The other marker slots stay hidden.

---

## Player flow (simple)

```
Script calls LoadingY_Show
        │
        ▼
  Bar appears + blue fill starts looping
        │
        ├── Player presses Y on the marker ──► SUCCESS
        │         • OnPlayerLoadingY(playerid, actionid)
        │         • UI destroyed
        │
        └── Player presses Y off the marker ─► MISS
                  • OnPlayerLoadingYFail(playerid, actionid)
                  • UI stays open, fill keeps looping
                  • They can try again
```

Cancel anytime with `LoadingY_Hide(playerid)` (no success callback).

---

## How the blue bar moves

Every `LOADING_Y_TICK_MS` (default **30 ms**):

1. The fill edge moves by `LOADING_Y_SPEED` (default **1.6**).
2. When it hits the **right** end (`LOADING_Y_MAX_X`) → direction flips left.
3. When it hits the **left** end (`LOADING_Y_MIN_X`) → direction flips right.
4. Loop forever until success or hide.

So the bar **pings back and forth**. The player waits for the fill edge to line up with the white marker, then taps **Y**.

---

## How a hit is decided

On **Y** press (`KEY_YES`):

```
distance = |blue_edge_X - marker_X|

if distance <= LOADING_Y_HIT_TOLERANCE (default 6.5)
    → HIT (success)
else
    → MISS (fail callback, keep looping)
```

| Easier | Harder |
|--------|--------|
| Raise `LOADING_Y_HIT_TOLERANCE` | Lower it |
| Lower `LOADING_Y_SPEED` | Raise speed |

Define these **before** `#include <loading_y>`.

---

## Marker positions

There are **6** possible X positions. On each `LoadingY_Show`, one is picked with `random(6)` and shown.

They sit along the bar from rightish to leftish (approx):

`335` · `323` · `309` · `298` · `283` · `273`

So each prompt feels slightly different — players cannot memorize one fixed spot forever.

---

## Script flow (for developers)

### 1) Open a prompt

```pawn
LoadingY_Show(playerid, "CRAFTING COMPONENT...", ACTION_CRAFT);
```

What the include does:

1. Builds player textdraws (if needed)
2. Sets title text to `PRESS Y TO CRAFTING COMPONENT...`
3. Picks a random marker and shows only that one
4. Starts the fill at the left, moving right
5. Starts the tick timer

### 2) Wait for callbacks

```pawn
public OnPlayerLoadingY(playerid, actionid)
{
    // They timed it correctly
    if (actionid == ACTION_CRAFT)
    {
        // give item / finish craft / etc.
    }
    return 1;
}

public OnPlayerLoadingYFail(playerid, actionid)
{
    // Wrong timing — prompt is STILL open
    // You can warn them, or do nothing
    return 1;
}
```

Use `actionid` so one UI can mean craft, heal, lockpick, etc.

### 3) Optional helpers while active

| Function | Use |
|----------|-----|
| `LoadingY_IsActive(playerid)` | Is the bar open? |
| `LoadingY_GetActionId(playerid)` | Current action id (`-1` if inactive) |
| `LoadingY_GetActionText(...)` | Current action string |
| `LoadingY_Hide(playerid)` | Force close (no success) |

---

## Example use cases

| Idea | Show call | On success |
|------|-----------|------------|
| Crafting | `LoadingY_Show(playerid, "CRAFT...", ACTION_CRAFT)` | Give crafted item |
| Heal / medkit | `LoadingY_Show(playerid, "USE MEDKIT", ACTION_HEAL)` | Set health, remove medkit |
| Lockpick | `LoadingY_Show(playerid, "LOCKPICK", ACTION_LOCK)` | Unlock vehicle/door |
| Robbery mini-game | `LoadingY_Show(playerid, "CRACK SAFE", ...)` | Give money / progress |

Miss = they keep trying until they hit or you call `LoadingY_Hide`.

---

## What does **not** happen

- It does **not** auto-succeed when the bar passes the marker — they must press **Y**.
- Miss does **not** close the UI (unless your Fail callback calls `LoadingY_Hide`).
- Success **does** destroy textdraws (clean; next Show rebuilds).
- Other players cannot see this prompt (player textdraws only).

---

## Wiring checklist

Your gamemode must call these (see [README.md](README.md#install)):

| Hook | Why |
|------|-----|
| `LoadingY_OnPlayerConnect` | Reset state / TD ids |
| `LoadingY_OnPlayerDisconnect` | Destroy TDs + kill timer |
| `LoadingY_OnPlayerKeyStateChange` | Detect **Y** press |

Without the key hook, the bar animates but **Y** does nothing.

---

## Try it

In the test gamemode:

| Command | What happens |
|---------|----------------|
| `/craft` | Opens craft prompt |
| `/heal` | Opens heal prompt |
| `/hidey` | Cancels early |

Watch the blue fill, wait for the white marker, press **Y**.

---

## Related

- [README.md](README.md) — install, API, defines
- `include/loading_y.inc` — implementation
- `gamemodes/loading_y_test.pwn` — demo commands
