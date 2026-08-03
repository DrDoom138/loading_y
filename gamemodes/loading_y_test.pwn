/*
    Loading Y Test Gamemode
    Press-Y prompt include demo

    Credits: Habibi / Janzzzz

    Commands:
      /craft     - show "PRESS Y TO CRAFTING COMPONENT..."
      /heal      - show "PRESS Y TO USE MEDKIT"
      /hidey     - hide the prompt early
*/

#include <open.mp>
#include <loading_y>

enum
{
    ACTION_NONE,
    ACTION_CRAFT,
    ACTION_HEAL
}

main()
{
    print("----------");
    print("Loading Y Test loaded.");
    print("Credits: Habibi / Janzzzz");
    print("----------");
}

public OnGameModeInit()
{
    SetGameModeText("Loading Y Test");
    AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, WEAPON:0, 0, WEAPON:0, 0, WEAPON:0, 0);
    return 1;
}

public OnPlayerConnect(playerid)
{
    LoadingY_OnPlayerConnect(playerid);
    SendClientMessage(playerid, 0xFFFFFFFF, "Welcome! /craft or /heal - press Y when blue reaches the grey marker.");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    LoadingY_OnPlayerDisconnect(playerid, reason);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SendClientMessage(playerid, 0x33CCFFFF, "Tip: /craft - press Y when the blue fill reaches the grey marker.");
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/craft", true))
    {
        LoadingY_Show(playerid, "CRAFTING COMPONENT...", ACTION_CRAFT);
        SendClientMessage(playerid, 0xAAAAAAFF, "Press Y when blue reaches the grey marker (loops until you hit it).");
        return 1;
    }

    if (!strcmp(cmdtext, "/heal", true))
    {
        LoadingY_Show(playerid, "USE MEDKIT", ACTION_HEAL);
        SendClientMessage(playerid, 0xAAAAAAFF, "Press Y when blue reaches the grey marker (loops until you hit it).");
        return 1;
    }

    if (!strcmp(cmdtext, "/hidey", true))
    {
        if (!LoadingY_IsActive(playerid))
            SendClientMessage(playerid, 0xFFFF00AA, "No prompt is active.");
        else
        {
            LoadingY_Hide(playerid);
            SendClientMessage(playerid, 0xFFFFFFFF, "Prompt hidden.");
        }
        return 1;
    }

    return 0;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (LoadingY_OnPlayerKeyStateChange(playerid, newkeys, oldkeys))
        return 1;
    return 1;
}

public OnPlayerLoadingY(playerid, actionid)
{
    switch (actionid)
    {
        case ACTION_CRAFT:
        {
            SendClientMessage(playerid, 0x33CC33FF, "Perfect timing! Crafted a component.");
            GivePlayerMoney(playerid, 100);
        }
        case ACTION_HEAL:
        {
            SetPlayerHealth(playerid, 100.0);
            SendClientMessage(playerid, 0x33CC33FF, "Perfect timing! You used a medkit.");
        }
        default:
        {
            SendClientMessage(playerid, 0xFFFFFFFF, "Action completed.");
        }
    }
    return 1;
}

public OnPlayerLoadingYFail(playerid, actionid)
{
    #pragma unused actionid
    SendClientMessage(playerid, 0xFF6666FF, "Missed - wait for the next loop.");
    return 1;
}
