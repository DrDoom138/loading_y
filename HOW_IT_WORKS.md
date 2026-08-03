# How loading_y works

**Credits:** Habibi / Janzzzz

Back to: [README.md](README.md)

This page explains the **player experience**, what moves on screen, and how your gamemode should use it — with **sample code** you can copy.

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

## Sample 1 — Full minimal gamemode

Copy this into a `.pwn` to see the whole flow.

```pawn
#include <open.mp>
#include <loading_y>

enum
{
    ACTION_CRAFT,
    ACTION_HEAL
}

public OnPlayerConnect(playerid)
{
    LoadingY_OnPlayerConnect(playerid);
    SendClientMessage(playerid, -1, "Type /craft or /heal - press Y on the marker.");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    LoadingY_OnPlayerDisconnect(playerid, reason);
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    // Required: without this, Y does nothing
    if (LoadingY_OnPlayerKeyStateChange(playerid, newkeys, oldkeys))
        return 1;
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/craft", true))
    {
        // Opens bar. Text becomes: PRESS Y TO CRAFTING COMPONENT...
        LoadingY_Show(playerid, "CRAFTING COMPONENT...", ACTION_CRAFT);
        return 1;
    }

    if (!strcmp(cmdtext, "/heal", true))
    {
        LoadingY_Show(playerid, "USE MEDKIT", ACTION_HEAL);
        return 1;
    }

    if (!strcmp(cmdtext, "/hidey", true))
    {
        if (LoadingY_IsActive(playerid))
            LoadingY_Hide(playerid); // cancel early
        return 1;
    }
    return 0;
}

// HIT - blue fill was on the marker when they pressed Y
public OnPlayerLoadingY(playerid, actionid)
{
    switch (actionid)
    {
        case ACTION_CRAFT:
        {
            GivePlayerMoney(playerid, 100);
            SendClientMessage(playerid, 0x33CC33FF, "Crafted! +$100");
        }
        case ACTION_HEAL:
        {
            SetPlayerHealth(playerid, 100.0);
            SendClientMessage(playerid, 0x33CC33FF, "Healed!");
        }
    }
    return 1;
}

// MISS - wrong timing. Bar keeps looping.
public OnPlayerLoadingYFail(playerid, actionid)
{
    #pragma unused actionid
    SendClientMessage(playerid, 0xFF6666FF, "Missed - wait and try again.");
    return 1;
}
```

What happens in-game:

1. `/craft` → bar appears  
2. Blue fill loops left ↔ right  
3. Press **Y** on marker → money + success message → bar closes  
4. Press **Y** off marker → “Missed” → bar stays  

---

## Sample 2 — Action ids (one UI, many features)

```pawn
enum
{
    ACTION_CRAFT = 1,
    ACTION_HEAL,
    ACTION_LOCKPICK
}

// Different commands, same UI
LoadingY_Show(playerid, "CRAFT STEEL", ACTION_CRAFT);
LoadingY_Show(playerid, "USE MEDKIT", ACTION_HEAL);
LoadingY_Show(playerid, "LOCKPICK DOOR", ACTION_LOCKPICK);

public OnPlayerLoadingY(playerid, actionid)
{
    switch (actionid)
    {
        case ACTION_CRAFT:
        {
            // give crafted item
        }
        case ACTION_HEAL:
        {
            SetPlayerHealth(playerid, 100.0);
        }
        case ACTION_LOCKPICK:
        {
            // unlock door
        }
    }
    return 1;
}
```

`actionid` is stored when you call `Show` and returned unchanged in both success and fail callbacks.

---

## Sample 3 — Don’t open twice

```pawn
if (!strcmp(cmdtext, "/craft", true))
{
    if (LoadingY_IsActive(playerid))
    {
        SendClientMessage(playerid, -1, "Finish the current prompt first.");
        return 1;
    }
    LoadingY_Show(playerid, "CRAFTING...", ACTION_CRAFT);
    return 1;
}
```

Calling `Show` again while active restarts that player’s prompt (new random marker). Blocking with `IsActive` is usually cleaner.

---

## Sample 4 — Limited fails (close after 3 misses)

By default miss keeps the UI open forever. Use this if you want a limit:

```pawn
new g_LyFails[MAX_PLAYERS];

public OnPlayerConnect(playerid)
{
    LoadingY_OnPlayerConnect(playerid);
    g_LyFails[playerid] = 0;
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/lockpick", true))
    {
        g_LyFails[playerid] = 0;
        LoadingY_Show(playerid, "LOCKPICK", 1);
        return 1;
    }
    return 0;
}

public OnPlayerLoadingYFail(playerid, actionid)
{
    #pragma unused actionid
    g_LyFails[playerid]++;

    if (g_LyFails[playerid] >= 3)
    {
        LoadingY_Hide(playerid);
        SendClientMessage(playerid, 0xFF6666FF, "Lockpick broken. Too many misses.");
        return 1;
    }

    new msg[64];
    format(msg, sizeof(msg), "Missed (%d/3). Try again.", g_LyFails[playerid]);
    SendClientMessage(playerid, 0xFF6666FF, msg);
    return 1;
}

public OnPlayerLoadingY(playerid, actionid)
{
    #pragma unused actionid
    g_LyFails[playerid] = 0;
    SendClientMessage(playerid, 0x33CC33FF, "Door unlocked!");
    return 1;
}
```

---

## Sample 5 — Easier / harder difficulty

Put defines **before** the include:

```pawn
// Easier: slower fill + bigger hit zone
#define LOADING_Y_SPEED         (1.0)
#define LOADING_Y_HIT_TOLERANCE (10.0)

#include <open.mp>
#include <loading_y>
```

```pawn
// Harder: faster fill + tighter hit zone
#define LOADING_Y_SPEED         (2.4)
#define LOADING_Y_HIT_TOLERANCE (4.0)

#include <open.mp>
#include <loading_y>
```

---

## Sample 6 — Read current action while open

```pawn
if (LoadingY_IsActive(playerid))
{
    new actionid = LoadingY_GetActionId(playerid);
    new text[64];
    LoadingY_GetActionText(playerid, text, sizeof(text));

    new msg[96];
    format(msg, sizeof(msg), "Busy: id=%d text=%s", actionid, text);
    SendClientMessage(playerid, -1, msg);
}
```

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

---

## Marker positions

There are **6** possible X positions. On each `LoadingY_Show`, one is picked with `random(6)` and shown.

They sit along the bar from rightish to leftish (approx):

`335` · `323` · `309` · `298` · `283` · `273`

So each prompt feels slightly different — players cannot memorize one fixed spot forever.

---

## What does **not** happen

- It does **not** auto-succeed when the bar passes the marker — they must press **Y**.
- Miss does **not** close the UI (unless your Fail callback calls `LoadingY_Hide`).
- Success **does** destroy textdraws (clean; next Show rebuilds).
- Other players cannot see this prompt (player textdraws only).

---

## Wiring checklist

| Hook | Why |
|------|-----|
| `LoadingY_OnPlayerConnect` | Reset state / TD ids |
| `LoadingY_OnPlayerDisconnect` | Destroy TDs + kill timer |
| `LoadingY_OnPlayerKeyStateChange` | Detect **Y** press |

Without the key hook, the bar animates but **Y** does nothing. See Sample 1.

---

## Try the included demo

`gamemodes/loading_y_test.pwn`:

| Command | What happens |
|---------|----------------|
| `/craft` | Opens craft prompt → success gives $100 |
| `/heal` | Opens heal prompt → success sets 100 HP |
| `/hidey` | Cancels early |

---

## Related

- [README.md](README.md) — install, API, defines
- `include/loading_y.inc` — implementation
- `gamemodes/loading_y_test.pwn` — demo commands
