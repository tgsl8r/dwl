/* appearance */
static const int sloppyfocus               = 0;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 4;  /* border pixel of windows */
static const int enablegaps                = 1;  /* 1 means gaps are enabled */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static const int monoclegaps               = 1;  /* 1 means outer gaps in monocle layout */
static const unsigned int gappih           = 10; /* horiz inner gap between windows */
static const unsigned int gappiv           = 10; /* vert inner gap between windows */
static const unsigned int gappoh           = 10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov           = 10; /* vert outer gap between windows and screen edge */
static const int cursor_timeout            = 5; /* seconds of mouse inactivity before hiding cursor */

/* colours */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
static const float rootcolor[]             = COLOR(0x222222ff);
static const float bordercolor[]           = COLOR(0x444444ff);
static const float focuscolor[]            = COLOR(0xd4be98ff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
        "pipewire", NULL,
        "yambar", NULL,
        "wl-paste", "--watch", "cliphist", "store", NULL,
        "swaybg", "-m", "tile", "-i", "/home/toby/.src/tiles-and-such/Floral/ret.jpg", NULL,
        NULL /* terminate */
};


/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	/* examples: */
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "=M=",      centeredmaster },
	{ "[+]",      monocle },
	{ "[%]",      NULL },    /* no layout function means floating behavior */
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* rules, model, layout, variant, options */
	.layout = "us",
	.options = "caps:escape_shifted_capslock,altwin:prtsc_rwin,compose:ins",
};
static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 0;
static const int tap_and_drag = 0;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/*
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/*
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER;

/*
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/*
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/*
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }


/* commands */
static const char *termcmd[]       = { "foot", NULL };
static const char *tmuxcmd[]       = { "foot", "tmux", NULL };
static const char *launchercmd[]   = { "fuzzel", NULL };
static const char *pathcmd[]       = { "fuzzel", "--list-executables-in-path", NULL };
static const char *browsercmd[]    = { "qutebrowser", NULL };
static const char *browsertwocmd[] = { "firefox", NULL };
static const char *explorercmd[]   = { "foot", "yazi", NULL };
static const char *explorertwocmd[]= { "thunar", NULL };
static const char *lockcmd[]       = { "lock", NULL };

/* Menus */
static const char *menusys[]  = { "menusys", NULL };
static const char *menunm[]   = { "menunm", NULL };
static const char *menubt[]   = { "menubt", NULL };
static const char *menushot[] = { "menushot", NULL };
static const char *menuclip[] = { "menuclip", NULL };

/* System keys */
static const char *upvol[]      = { "/usr/local/bin/tctl", "audio", "up", NULL };
static const char *downvol[]    = { "/usr/local/bin/tctl", "audio", "down", NULL };
static const char *mutevol[]    = { "/usr/local/bin/tctl", "audio", "mute", NULL };
static const char *mutemic[]    = { "/usr/local/bin/tctl", "audio", "micmute", NULL };
static const char *brightup[]   = { "/usr/local/bin/tctl", "brightness", "up", NULL };
static const char *brightdown[] = { "/usr/local/bin/tctl", "brightness", "down", NULL };
static const char *kbup[]       = { "/usr/local/bin/tctl", "brightness", "keyup", NULL };
static const char *kbdown[]     = { "/usr/local/bin/tctl", "brightness", "keydown", NULL };

static const Key keys[] = {
	/* modifier                  key                 function        argument */
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_Escape,     quit,           {0} },
	{ MODKEY,                    XKB_KEY_grave,      spawn,          {.v = lockcmd} },
  /* apps */
	{ MODKEY,                    XKB_KEY_Return,     spawn,          {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_t,          spawn,          {.v = tmuxcmd} },
	{ MODKEY,                    XKB_KEY_w,          spawn,          {.v = browsercmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_W,          spawn,          {.v = browsertwocmd} },
	{ MODKEY,                    XKB_KEY_e,          spawn,          {.v = explorercmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_E,          spawn,          {.v = explorertwocmd} },
  /* menus */
	{ MODKEY,                    XKB_KEY_a,          spawn,          {.v = launchercmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_A,          spawn,          {.v = pathcmd} },
	{ MODKEY,                    XKB_KEY_Escape,     spawn,          {.v = menusys} },
  { MODKEY,                    XKB_KEY_n,          spawn,          {.v = menunm} },
	{ MODKEY,                    XKB_KEY_b,          spawn,          {.v = menubt} },
	{ MODKEY,                    XKB_KEY_s,          spawn,          {.v = menushot} },
	{ MODKEY,                    XKB_KEY_v,          spawn,          {.v = menuclip} },
  /* stack */
	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,          pushdown,       {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,          pushup,         {0} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_k,          incnmaster,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_j,          incnmaster,     {.i = -1} },
  /* client */
	{ MODKEY,                    XKB_KEY_z,          zoom,           {0} },
	{ MODKEY,                    XKB_KEY_x,          togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_c,          togglefullscreen,{0} },
	{ MODKEY,                    XKB_KEY_q,          killclient,     {0} },
  /* tags */
	{ MODKEY,                    XKB_KEY_Tab,        view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Tab,        tag,            {.ui = ~0} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                         1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                     3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                  6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                   7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                  8),
  /* layouts */
	{ MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_g,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_f,          setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
  /* monitors */
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
  /* gaps */
  { MODKEY,                    XKB_KEY_bracketleft, incgaps,       {.i = -1 } },
  { MODKEY,                    XKB_KEY_bracketright,incgaps,       {.i = +1 } },
  { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_braceleft,   incigaps,      {.i = -1 } },
  { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_braceright,  incigaps,      {.i = +1 } },
  { MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_bracketleft, incogaps,      {.i = -1 } },
  { MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_bracketright,incogaps,      {.i = +1 } },

  /* Media keys */
  { 0,                         XKB_KEY_XF86AudioRaiseVolume,  spawn, {.v = upvol   } },
  { 0,                         XKB_KEY_XF86AudioLowerVolume,  spawn, {.v = downvol } },
  { 0,                         XKB_KEY_XF86AudioMute,         spawn, {.v = mutevol } },
  { 0,                         XKB_KEY_XF86AudioMicMute,      spawn, {.v = mutemic } },
  { 0,                         XKB_KEY_XF86MonBrightnessUp,   spawn, {.v = brightup   } },
  { 0,                         XKB_KEY_XF86MonBrightnessDown, spawn, {.v = brightdown } },
  { WLR_MODIFIER_SHIFT,        XKB_KEY_XF86MonBrightnessUp,   spawn, {.v = kbup   } },
  { WLR_MODIFIER_SHIFT,        XKB_KEY_XF86MonBrightnessDown, spawn, {.v = kbdown } },

	/* Ctrl-Alt-Backspace to quit DWL */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
  /* Ctrl-Alt-Fn to switch TTYs */
  #define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
