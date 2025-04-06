/* appearance */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
static const int sloppyfocus               = 0;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 5;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x222222ff);
static const float bordercolor[]           = COLOR(0x444444ff);
static const float focuscolor[]            = COLOR(0x005577ff);
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
	{ "[%]",      NULL },    /* no layout function means floating behavior */
	{ "[+]",      monocle },
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
static const char *termcmd[]     = { "foot", NULL };
static const char *launchercmd[] = { "fuzzel", NULL };
static const char *browsercmd[]  = { "qutebrowser", NULL };
static const char *screenshot[]  = { "menushot", NULL };
static const char *upvol[]       = { "/usr/local/bin/tctl", "audio", "up", NULL };
static const char *downvol[]     = { "/usr/local/bin/tctl", "audio", "down", NULL };
static const char *mutevol[]     = { "/usr/local/bin/tctl", "audio", "mute", NULL };
static const char *mutemic[]     = { "/usr/local/bin/tctl", "audio", "micmute", NULL };
static const char *brightup[]    = { "/usr/local/bin/tctl", "brightness", "up", NULL };
static const char *brightdown[]  = { "/usr/local/bin/tctl", "brightness", "down", NULL };
static const char *kbup[]        = { "/usr/local/bin/tctl", "brightness", "keyup", NULL };
static const char *kbdown[]      = { "/usr/local/bin/tctl", "brightness", "keydown", NULL };

static const Key keys[] = {
	/* modifier                  key                 function        argument */
  /* apps */
	{ MODKEY,                    XKB_KEY_Return,     spawn,          {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_q,          spawn,          {.v = launchercmd} },
	{ MODKEY,                    XKB_KEY_w,          spawn,          {.v = browsercmd} },
	{ MODKEY,                    XKB_KEY_s,          spawn,          {.v = screenshot} },
  /* stack */
	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
	{ MODKEY,                    XKB_KEY_i,          incnmaster,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_o,          incnmaster,     {.i = -1} },
  /* client */
	{ MODKEY,                    XKB_KEY_z,          zoom,           {0} },
	{ MODKEY,                    XKB_KEY_x,          togglefullscreen, {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_X,          togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_c,          killclient,     {0} },
  /* tags */
	{ MODKEY,                    XKB_KEY_Tab,        view,           {0} },
	{ MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright, tag,            {.ui = ~0} },
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
	{ MODKEY,                    XKB_KEY_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
  /* monitors */
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },

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
