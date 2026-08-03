#if defined _INC_LOADING_Y
	#endinput
#endif
#define _INC_LOADING_Y

#if !defined LOADING_Y_MAX_TEXT
	#define LOADING_Y_MAX_TEXT (64)
#endif

#define LY_TD_COUNT     (9)
#define LY_MARKER_COUNT (6)

#if !defined LOADING_Y_TICK_MS
	#define LOADING_Y_TICK_MS (30)
#endif

#if !defined LOADING_Y_SPEED
	#define LOADING_Y_SPEED (1.6)
#endif

#if !defined LOADING_Y_MARKER_WIDTH
	#define LOADING_Y_MARKER_WIDTH (4.5)
#endif

#if !defined LOADING_Y_HIT_TOLERANCE
	#define LOADING_Y_HIT_TOLERANCE (6.5)
#endif

#if !defined LOADING_Y_LEFT_X
	#define LOADING_Y_LEFT_X (268.000000)
#endif

#if !defined LOADING_Y_MIN_X
	#define LOADING_Y_MIN_X (272.0)
#endif

#if !defined LOADING_Y_MAX_X
	#define LOADING_Y_MAX_X (343.0)
#endif

#if !defined PRESSED
	#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#endif

static const Float:g_LyMarkerX[LY_MARKER_COUNT] =
{
	335.000000,
	323.000000,
	309.000000,
	298.000000,
	283.000000,
	273.000000
};

static PlayerText:PublicTD[MAX_PLAYERS][LY_TD_COUNT];
static bool:g_LyBuilt[MAX_PLAYERS];
static bool:g_LyActive[MAX_PLAYERS];
static g_LyActionId[MAX_PLAYERS];
static g_LyActionText[MAX_PLAYERS][LOADING_Y_MAX_TEXT];
static g_LyTimer[MAX_PLAYERS];
static Float:g_LyBarX[MAX_PLAYERS];
static Float:g_LyBarDir[MAX_PLAYERS];
static g_LyTarget[MAX_PLAYERS];

forward OnPlayerLoadingY(playerid, actionid);
forward OnPlayerLoadingYFail(playerid, actionid);
forward LoadingY_Tick(playerid);

static stock LoadingY_KillTimer(playerid)
{
	if (g_LyTimer[playerid] != 0)
	{
		KillTimer(g_LyTimer[playerid]);
		g_LyTimer[playerid] = 0;
	}
	return 1;
}

static stock LoadingY_DestroyTD(playerid)
{
	LoadingY_KillTimer(playerid);

	for (new i = 0; i < LY_TD_COUNT; i++)
	{
		if (PublicTD[playerid][i] != INVALID_PLAYER_TEXT_DRAW)
		{
			PlayerTextDrawHide(playerid, PublicTD[playerid][i]);
			PlayerTextDrawDestroy(playerid, PublicTD[playerid][i]);
			PublicTD[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
		}
	}
	g_LyBuilt[playerid] = false;
	return 1;
}

static stock LoadingY_BuildTD(playerid)
{
	if (g_LyBuilt[playerid])
		return 1;

	PublicTD[playerid][0] = CreatePlayerTextDraw(playerid, 306.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][0], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][0], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][0], 298.500000, 75.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][0], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][0], -1);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][0], 255);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][0], 135);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][0], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][0], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][0], false);

	PublicTD[playerid][1] = CreatePlayerTextDraw(playerid, 268.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][1], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][1], 0.500000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][1], 315.500000, -25.500000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][1], TEXT_DRAW_ALIGN:1);
	PlayerTextDrawColour(playerid, PublicTD[playerid][1], -1);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][1], 255);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][1], -2016478585);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][1], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][1], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][1], false);

	PublicTD[playerid][2] = CreatePlayerTextDraw(playerid, 335.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][2], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][2], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][2], 299.500000, 0.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][2], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][2], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][2], -1061109505);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][2], -741092353);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][2], -741092473);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][2], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][2], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][2], false);

	PublicTD[playerid][3] = CreatePlayerTextDraw(playerid, 323.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][3], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][3], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][3], 299.500000, 0.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][3], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][3], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][3], -1061109505);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][3], -741092353);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][3], -741092473);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][3], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][3], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][3], false);

	PublicTD[playerid][4] = CreatePlayerTextDraw(playerid, 309.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][4], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][4], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][4], 299.500000, 0.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][4], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][4], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][4], -1061109505);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][4], -741092353);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][4], -741092473);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][4], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][4], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][4], false);

	PublicTD[playerid][5] = CreatePlayerTextDraw(playerid, 298.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][5], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][5], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][5], 299.500000, 0.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][5], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][5], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][5], -1061109505);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][5], -741092353);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][5], -741092473);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][5], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][5], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][5], false);

	PublicTD[playerid][6] = CreatePlayerTextDraw(playerid, 283.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][6], TEXT_DRAW_FONT:1);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][6], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][6], 299.500000, 0.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][6], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][6], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][6], -1061109505);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][6], -741092353);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][6], -741092473);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][6], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][6], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][6], false);

	PublicTD[playerid][7] = CreatePlayerTextDraw(playerid, 273.000000, 336.000000, "_");
	PlayerTextDrawFont(playerid, PublicTD[playerid][7], TEXT_DRAW_FONT:0);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][7], 0.600000, 0.649999);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][7], 299.500000, 0.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][7], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][7], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][7], -1061109505);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][7], -741092353);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][7], -741092473);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][7], true);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][7], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][7], false);

	PublicTD[playerid][8] = CreatePlayerTextDraw(playerid, 306.000000, 348.000000, "Press Y to Name of Action");
	PlayerTextDrawFont(playerid, PublicTD[playerid][8], TEXT_DRAW_FONT:2);
	PlayerTextDrawLetterSize(playerid, PublicTD[playerid][8], 0.170000, 0.850000);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][8], 400.000000, 90.000000);
	PlayerTextDrawSetOutline(playerid, PublicTD[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, PublicTD[playerid][8], 0);
	PlayerTextDrawAlignment(playerid, PublicTD[playerid][8], TEXT_DRAW_ALIGN:2);
	PlayerTextDrawColour(playerid, PublicTD[playerid][8], -1);
	PlayerTextDrawBackgroundColour(playerid, PublicTD[playerid][8], 255);
	PlayerTextDrawBoxColour(playerid, PublicTD[playerid][8], 50);
	PlayerTextDrawUseBox(playerid, PublicTD[playerid][8], false);
	PlayerTextDrawSetProportional(playerid, PublicTD[playerid][8], true);
	PlayerTextDrawSetSelectable(playerid, PublicTD[playerid][8], false);

	g_LyBuilt[playerid] = true;
	return 1;
}

static stock LoadingY_ApplyBarPos(playerid)
{
	if (!g_LyActive[playerid] || !g_LyBuilt[playerid])
		return 0;

	PlayerTextDrawSetPos(playerid, PublicTD[playerid][1], LOADING_Y_LEFT_X, 336.000000);
	PlayerTextDrawTextSize(playerid, PublicTD[playerid][1], g_LyBarX[playerid], -25.500000);
	PlayerTextDrawShow(playerid, PublicTD[playerid][1]);
	return 1;
}

static stock LoadingY_SetupMarkers(playerid)
{
	new m, td;

	g_LyTarget[playerid] = random(LY_MARKER_COUNT);

	for (m = 0; m < LY_MARKER_COUNT; m++)
	{
		td = 2 + m;

		if (m == g_LyTarget[playerid])
		{
			PlayerTextDrawTextSize(playerid, PublicTD[playerid][td], 299.500000, LOADING_Y_MARKER_WIDTH);
			PlayerTextDrawBoxColour(playerid, PublicTD[playerid][td], -1);
			PlayerTextDrawShow(playerid, PublicTD[playerid][td]);
		}
		else
		{
			PlayerTextDrawHide(playerid, PublicTD[playerid][td]);
		}
	}
	return 1;
}

static stock bool:LoadingY_IsOnMarker(playerid)
{
	new Float:mx = g_LyMarkerX[g_LyTarget[playerid]];
	return bool:(floatabs(g_LyBarX[playerid] - mx) <= LOADING_Y_HIT_TOLERANCE);
}

static stock LoadingY_Complete(playerid, bool:success)
{
	new actionid = g_LyActionId[playerid];

	g_LyActive[playerid] = false;
	LoadingY_Hide(playerid);

	if (success)
		CallLocalFunction("OnPlayerLoadingY", "ii", playerid, actionid);
	else
		CallLocalFunction("OnPlayerLoadingYFail", "ii", playerid, actionid);
	return 1;
}

stock bool:LoadingY_IsActive(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return false;
	return g_LyActive[playerid];
}

stock LoadingY_GetActionId(playerid)
{
	if (!LoadingY_IsActive(playerid))
		return -1;
	return g_LyActionId[playerid];
}

stock LoadingY_GetActionText(playerid, dest[], len = sizeof(dest))
{
	if (!LoadingY_IsActive(playerid))
	{
		dest[0] = EOS;
		return 0;
	}
	return format(dest, len, "%s", g_LyActionText[playerid]);
}

stock LoadingY_Hide(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	g_LyActive[playerid] = false;
	LoadingY_KillTimer(playerid);
	LoadingY_DestroyTD(playerid);

	g_LyActionId[playerid] = 0;
	g_LyActionText[playerid][0] = EOS;
	g_LyTarget[playerid] = 0;
	return 1;
}

stock LoadingY_Show(playerid, const action[], actionid = 0)
{
	new string[LOADING_Y_MAX_TEXT + 16];

	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	LoadingY_KillTimer(playerid);
	LoadingY_BuildTD(playerid);

	format(g_LyActionText[playerid], LOADING_Y_MAX_TEXT, "%s", action);
	g_LyActionId[playerid] = actionid;
	g_LyActive[playerid] = true;

	format(string, sizeof(string), "PRESS Y TO %s", action);
	PlayerTextDrawSetString(playerid, PublicTD[playerid][8], string);

	g_LyBarX[playerid] = LOADING_Y_MIN_X;
	g_LyBarDir[playerid] = LOADING_Y_SPEED;

	PlayerTextDrawShow(playerid, PublicTD[playerid][0]);
	LoadingY_ApplyBarPos(playerid);
	LoadingY_SetupMarkers(playerid);
	PlayerTextDrawShow(playerid, PublicTD[playerid][8]);

	g_LyTimer[playerid] = SetTimerEx("LoadingY_Tick", LOADING_Y_TICK_MS, true, "i", playerid);
	return 1;
}

public LoadingY_Tick(playerid)
{
	if (!IsPlayerConnected(playerid) || !g_LyActive[playerid])
	{
		LoadingY_KillTimer(playerid);
		return 0;
	}

	g_LyBarX[playerid] += g_LyBarDir[playerid];

	if (g_LyBarX[playerid] >= LOADING_Y_MAX_X)
	{
		g_LyBarX[playerid] = LOADING_Y_MAX_X;
		g_LyBarDir[playerid] = -LOADING_Y_SPEED;
	}
	else if (g_LyBarX[playerid] <= LOADING_Y_MIN_X)
	{
		g_LyBarX[playerid] = LOADING_Y_MIN_X;
		g_LyBarDir[playerid] = LOADING_Y_SPEED;
	}

	if (!g_LyActive[playerid])
		return 0;

	LoadingY_ApplyBarPos(playerid);
	return 1;
}

stock LoadingY_OnPlayerConnect(playerid)
{
	for (new i = 0; i < LY_TD_COUNT; i++)
		PublicTD[playerid][i] = INVALID_PLAYER_TEXT_DRAW;

	g_LyBuilt[playerid] = false;
	g_LyActive[playerid] = false;
	g_LyActionId[playerid] = 0;
	g_LyActionText[playerid][0] = EOS;
	g_LyTimer[playerid] = 0;
	g_LyBarX[playerid] = LOADING_Y_MIN_X;
	g_LyBarDir[playerid] = LOADING_Y_SPEED;
	g_LyTarget[playerid] = 0;
	return 1;
}

stock LoadingY_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	LoadingY_DestroyTD(playerid);
	g_LyActive[playerid] = false;
	g_LyActionId[playerid] = 0;
	g_LyActionText[playerid][0] = EOS;
	g_LyTimer[playerid] = 0;
	return 1;
}

stock LoadingY_OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if (!g_LyActive[playerid])
		return 0;

	if (!PRESSED(KEY_YES))
		return 0;

	if (LoadingY_IsOnMarker(playerid))
	{
		LoadingY_Complete(playerid, true);
		return 1;
	}

	CallLocalFunction("OnPlayerLoadingYFail", "ii", playerid, g_LyActionId[playerid]);
	return 1;
}
