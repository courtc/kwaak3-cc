//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#define QK_TAB				9
#define QK_ENTER			13
#define QK_ESCAPE			27
#define QK_SPACE			32
#define QK_BACKSPACE		127
#define QK_COMMAND			128
#define QK_CAPSLOCK			129
#define QK_POWER			130
#define QK_PAUSE			131
#define QK_UPARROW			132
#define QK_DOWNARROW		133
#define QK_LEFTARROW		134
#define QK_RIGHTARROW		135
#define QK_ALT				136
#define QK_CTRL				137
#define QK_SHIFT			138
#define QK_INS				139
#define QK_DEL				140
#define QK_PGDN				141
#define QK_PGUP				142
#define QK_HOME				143
#define QK_END				144
#define QK_F1				145
#define QK_F2				146
#define QK_F3				147
#define QK_F4				148
#define QK_F5				149
#define QK_F6				150
#define QK_F7				151
#define QK_F8				152
#define QK_F9				153
#define QK_F10				154
#define QK_F11				155
#define QK_F12				156
#define QK_F13				157
#define QK_F14				158
#define QK_F15				159
#define QK_KP_HOME			160
#define QK_KP_UPARROW		161
#define QK_KP_PGUP			162
#define QK_KP_LEFTARROW		163
#define QK_KP_5				164
#define QK_KP_RIGHTARROW	165
#define QK_KP_END			166
#define QK_KP_DOWNARROW		167
#define QK_KP_PGDN			168
#define QK_KP_ENTER			169
#define QK_KP_INS			170
#define QK_KP_DEL			171
#define QK_KP_SLASH			172
#define QK_KP_MINUS			173
#define QK_KP_PLUS			174
#define QK_KP_NUMLOCK		175
#define QK_KP_STAR			176
#define QK_KP_EQUALS		177
#define QK_MOUSE1			178
#define QK_MOUSE2			179
#define QK_MOUSE3			180
#define QK_MOUSE4			181
#define QK_MOUSE5			182
#define QK_MWHEELDOWN		183
#define QK_MWHEELUP			184
#define QK_JOY1				185
#define QK_JOY2				186
#define QK_JOY3				187
#define QK_JOY4				188
#define QK_JOY5				189
#define QK_JOY6				190
#define QK_JOY7				191
#define QK_JOY8				192
#define QK_JOY9				193
#define QK_JOY10			194
#define QK_JOY11			195
#define QK_JOY12			196
#define QK_JOY13			197
#define QK_JOY14			198
#define QK_JOY15			199
#define QK_JOY16			200
#define QK_JOY17			201
#define QK_JOY18			202
#define QK_JOY19			203
#define QK_JOY20			204
#define QK_JOY21			205
#define QK_JOY22			206
#define QK_JOY23			207
#define QK_JOY24			208
#define QK_JOY25			209
#define QK_JOY26			210
#define QK_JOY27			211
#define QK_JOY28			212
#define QK_JOY29			213
#define QK_JOY30			214
#define QK_JOY31			215
#define QK_JOY32			216
#define QK_AUX1				217
#define QK_AUX2				218
#define QK_AUX3				219
#define QK_AUX4				220
#define QK_AUX5				221
#define QK_AUX6				222
#define QK_AUX7				223
#define QK_AUX8				224
#define QK_AUX9				225
#define QK_AUX10			226
#define QK_AUX11			227
#define QK_AUX12			228
#define QK_AUX13			229
#define QK_AUX14			230
#define QK_AUX15			231
#define QK_AUX16			232
#define QK_WORLD_0			233
#define QK_WORLD_1			234
#define QK_WORLD_2			235
#define QK_WORLD_3			236
#define QK_WORLD_4			237
#define QK_WORLD_5			238
#define QK_WORLD_6			239
#define QK_WORLD_7			240
#define QK_WORLD_8			241
#define QK_WORLD_9			242
#define QK_WORLD_10			243
#define QK_WORLD_11			244
#define QK_WORLD_12			245
#define QK_WORLD_13			246
#define QK_WORLD_14			247
#define QK_WORLD_15			248
#define QK_WORLD_16			249
#define QK_WORLD_17			250
#define QK_WORLD_18			251
#define QK_WORLD_19			252
#define QK_WORLD_20			253
#define QK_WORLD_21			254
#define QK_WORLD_22			255
#define QK_WORLD_23			256
#define QK_WORLD_24			257
#define QK_WORLD_25			258
#define QK_WORLD_26			259
#define QK_WORLD_27			260
#define QK_WORLD_28			261
#define QK_WORLD_29			262
#define QK_WORLD_30			263
#define QK_WORLD_31			264
#define QK_WORLD_32			265
#define QK_WORLD_33			266
#define QK_WORLD_34			267
#define QK_WORLD_35			268
#define QK_WORLD_36			269
#define QK_WORLD_37			270
#define QK_WORLD_38			271
#define QK_WORLD_39			272
#define QK_WORLD_40			273
#define QK_WORLD_41			274
#define QK_WORLD_42			275
#define QK_WORLD_43			276
#define QK_WORLD_44			277
#define QK_WORLD_45			278
#define QK_WORLD_46			279
#define QK_WORLD_47			280
#define QK_WORLD_48			281
#define QK_WORLD_49			282
#define QK_WORLD_50			283
#define QK_WORLD_51			284
#define QK_WORLD_52			285
#define QK_WORLD_53			286
#define QK_WORLD_54			287
#define QK_WORLD_55			288
#define QK_WORLD_56			289
#define QK_WORLD_57			290
#define QK_WORLD_58			291
#define QK_WORLD_59			292
#define QK_WORLD_60			293
#define QK_WORLD_61			294
#define QK_WORLD_62			295
#define QK_WORLD_63			296
#define QK_WORLD_64			297
#define QK_WORLD_65			298
#define QK_WORLD_66			299
#define QK_WORLD_67			300
#define QK_WORLD_68			301
#define QK_WORLD_69			302
#define QK_WORLD_70			303
#define QK_WORLD_71			304
#define QK_WORLD_72			305
#define QK_WORLD_73			306
#define QK_WORLD_74			307
#define QK_WORLD_75			308
#define QK_WORLD_76			309
#define QK_WORLD_77			310
#define QK_WORLD_78			311
#define QK_WORLD_79			312
#define QK_WORLD_80			313
#define QK_WORLD_81			314
#define QK_WORLD_82			315
#define QK_WORLD_83			316
#define QK_WORLD_84			317
#define QK_WORLD_85			318
#define QK_WORLD_86			319
#define QK_WORLD_87			320
#define QK_WORLD_88			321
#define QK_WORLD_89			322
#define QK_WORLD_90			323
#define QK_WORLD_91			324
#define QK_WORLD_92			325
#define QK_WORLD_93			326
#define QK_WORLD_94			327
#define QK_WORLD_95			328
#define QK_SUPER			329
#define QK_COMPOSE			330
#define QK_MODE				331
#define QK_HELP				332
#define QK_PRINT			333
#define QK_SYSREQ			334
#define QK_SCROLLOCK		335
#define QK_BREAK			336
#define QK_MENU				337
#define QK_EURO				338
#define QK_UNDO				339
#define QK_CONSOLE			340

/* Function pointers to Quake3 code */
int  (*q3main)(int argc, char **argv);
void (*drawFrame)();
void (*queueKeyEvent)(int key, int state);
void (*queueMotionEvent)(int action, float x, float y, float pressure);
void (*queueTrackballEvent)(int action, float x, float y);
void (*queueJoystickEvent)(int axis, int value);
void (*requestAudioData)();
void (*setAudioCallbacks)(void *func, void *func2, void *func3);
void (*setResolution)(int width, int height);

/* Containts the path to /data/data/(package_name)/libs */
static char* lib_dir=NULL;

static jboolean audioEnabled=1;
static jboolean benchmarkEnabled=0;
static jboolean lightmapsEnabled=0;
static jboolean showFramerateEnabled=0;
static jobject audioBuffer=0;
static jobject kwaakAudioObj=0;
static void *libdl;
static int init=0;

typedef unsigned char BOOL;
#define FALSE 0
#define TRUE 1

static BOOL neon_support()
{
    char buf[80];
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(!fp)
    {
        __android_log_print(ANDROID_LOG_DEBUG, "Quake", "Unable to open /proc/cpuinfo\n");
        return FALSE;
    }

    while(fgets(buf, 80, fp) != NULL)
    {
        char *features = strstr(buf, "Features");

        if(features)
        {
            char *feature;
            features += strlen("Features");
            feature = strtok(features, ": ");
            while(feature)
            {
                if(!strcmp(feature, "neon"))
                    return TRUE;

                feature = strtok(NULL, ": ");
            }
            return FALSE;
        }
    }
    return FALSE;
}

const char *get_quake3_library()
{
    /* We ship a library with Neon FPU support. This boosts performance a lot but it only works on a few CPUs. */
    if(neon_support())
        return "libquake3_neon.so";

    return "libquake3.so";
}

void get_quake3_library_path(char *path)
{
    const char *libquake3 = get_quake3_library();
    if(lib_dir)
    {
        sprintf(path, "%s/%s", lib_dir, libquake3);
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, "Quake_JNI", "Library path not set, trying /data/data/org.kwaak3/lib");
        sprintf(path, "/data/data/org.kwaak3/lib/%s", libquake3);
    }
}

static void load_libquake3()
{
    char libquake3_path[80];
    get_quake3_library_path(libquake3_path);

#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake", "Attempting to load %s\n", libquake3_path);
#endif

    libdl = dlopen(libquake3_path, RTLD_NOW);
    if(!libdl)
    {
        __android_log_print(ANDROID_LOG_ERROR, "Quake", "Unable to load libquake3.so: %s\n", dlerror());
        return;
    }

    q3main = dlsym(libdl, "main");
    drawFrame = dlsym(libdl, "nextFrame");
    queueKeyEvent = dlsym(libdl, "queueKeyEvent");
    queueMotionEvent = dlsym(libdl, "queueMotionEvent");
    queueTrackballEvent = dlsym(libdl, "queueTrackballEvent");
	queueJoystickEvent = dlsym(libdl, "queueJoystickEvent");
    requestAudioData = dlsym(libdl, "requestAudioData");
    setAudioCallbacks = dlsym(libdl, "setAudioCallbacks");
    setResolution = dlsym(libdl, "setResolution");
    init=1;
}

int GetPos()
{
    /*JNIEnv *env;
    (*jVM)->GetEnv(jVM, (void**) &env, JNI_VERSION_1_4);
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "getPos");
#endif
    return (*env)->CallIntMethod(env, kwaakAudioObj, android_getPos);
	*/
	return 0;
}

void InitAudio(void *buffer, int size)
{
/*
    JNIEnv *env;
    jobject tmp;
    (*jVM)->GetEnv(jVM, (void**) &env, JNI_VERSION_1_4);
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "initAudio");
#endif
    tmp = (*env)->NewDirectByteBuffer(env, buffer, size);
    audioBuffer = (jobject)(*env)->NewGlobalRef(env, tmp);

    if(!audioBuffer) __android_log_print(ANDROID_LOG_ERROR, "Quake_JNI", "yikes, unable to initialize audio buffer");

    return (*env)->CallVoidMethod(env, kwaakAudioObj, android_initAudio);
	*/
}

void WriteAudio(int offset, int length)
{
/*
    JNIEnv *env;
    (*jVM)->GetEnv(jVM, (void**) &env, JNI_VERSION_1_4);
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "writeAudio audioBuffer=%p offset=%d length=%d", audioBuffer, offset, length);
#endif

    (*env)->CallVoidMethod(env, kwaakAudioObj, android_writeAudio, audioBuffer, offset, length);
	*/
}

void OnLoad()
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
#endif
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "JNI_OnLoad called");
    if(!init) load_libquake3();
}

void EnableAudio(jboolean enable)
{
    audioEnabled = enable;
}

void EnableBenchmark(jboolean enable)
{
    benchmarkEnabled = enable;
}

void EnableLightmaps(jboolean enable)
{
    lightmapsEnabled = enable;
}

void ShowFramerate(jboolean enable)
{
    showFramerateEnabled = enable;
}

void InitGame(int width, int height)
{
    char *argv[4];
    int argc=0;

    /* TODO: integrate settings with quake3, right now there is no synchronization */

    if(!audioEnabled)
    {
        argv[argc] = strdup("+set s_initsound 0");
        argc++;
    }

    if(lightmapsEnabled)
        argv[argc] = strdup("+set r_vertexlight 0");
    else
        argv[argc] = strdup("+set r_vertexlight 1");
    argc++;

    if(showFramerateEnabled)
        argv[argc] = strdup("+set cg_drawfps 1");
    else
        argv[argc] = strdup("+set cg_drawfps 0");
    argc++;

    if(benchmarkEnabled)
    {
        argv[argc] = strdup("+demo four +timedemo 1");
        argc++;
    }

#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "initGame(%d, %d)", width, height);
#endif

    setAudioCallbacks(&GetPos, &WriteAudio, &InitAudio);
    setResolution(width, height);

    /* In the future we might want to pass arguments using argc/argv e.g. to start a benchmark at startup, to load a mod or whatever */
    q3main(argc, argv);
}

void DrawFrame()
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "nextFrame()");
#endif
    if(drawFrame) drawFrame();
}

void QueueKeyEvent(jint key, jint state)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "queueKeyEvent(%d, %d)", key, state);
#endif
    if(queueKeyEvent) queueKeyEvent(key, state);
}

void QueueMotionEvent(jint action, jfloat x, jfloat y, jfloat pressure)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "queueMotionEvent(%d, %f, %f, %f)", action, x, y, pressure);
#endif
    if(queueMotionEvent) queueMotionEvent(action, x, y, pressure);
}

void QueueTrackballEvent(jint action, jfloat x, jfloat y)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "queueTrackballEvent(%d, %f, %f)", action, x, y);
#endif
    if(queueTrackballEvent) queueTrackballEvent(action, x, y);
}

void QueueJoystickEvent(jint nAxis, jint nValue)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "queueJoystickEvent(%d, %d)", nAxis, nValue);
#endif
    if(queueJoystickEvent) queueJoystickEvent(nAxis, nValue);
}

void RequestAudioData()
{
    if(requestAudioData) requestAudioData();
}

void SetLibraryDirectory(const char* szPath)
{
    jboolean iscopy;
    //const jbyte *path = (*env)->GetStringUTFChars(env, jpath, &iscopy);
    lib_dir = strdup(szPath);
    //(*env)->ReleaseStringUTFChars(env, jpath, path);

#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Quake_JNI", "path=%s\n", lib_dir);
#endif
}











/**
 * Shared state for our app.
 */
struct engine 
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
};

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) 
{
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = 
	{
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
	{
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->animating = 1;

    // Initialize GL state.
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    //glEnable(GL_CULL_FACE);
    //glShadeModel(GL_SMOOTH);
    //glDisable(GL_DEPTH_TEST);

    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) 
{
    if (engine->display == NULL) 
	{
        // No display.
        return;
    }

    // Just fill the screen with a color.
    //glClearColor(1,0,0,1);
    //glClear(GL_COLOR_BUFFER_BIT);

	DrawFrame();
	
    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) 
{
    if (engine->display != EGL_NO_DISPLAY) 
	{
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) 
		{
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) 
		{
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

static int androidKeyCodeToQuake(AInputEvent* event)
{	
	int32_t a_keycode = (AKeyEvent_getKeyCode)(event);
	/* Convert non-ASCII keys by hand */
	switch(a_keycode)
	{
		case AKEYCODE_FOCUS:
			return QK_F1;
		case AKEYCODE_VOLUME_DOWN:
			return QK_F2;
		case AKEYCODE_VOLUME_UP:
			return QK_F3;
		case AKEYCODE_DPAD_UP:
			return QK_UPARROW;
		case AKEYCODE_DPAD_DOWN:
			return QK_DOWNARROW;
		case AKEYCODE_DPAD_LEFT:
			return QK_LEFTARROW;
		case AKEYCODE_DPAD_RIGHT:
			return QK_RIGHTARROW;
		case AKEYCODE_DPAD_CENTER:
			return QK_ENTER;
		case AKEYCODE_BUTTON_L1:
		case AKEYCODE_BUTTON_START:
			return QK_SPACE;
		case AKEYCODE_BUTTON_R1:
			return QK_CTRL;
		case AKEYCODE_SEARCH:
			return QK_CONSOLE;
		case AKEYCODE_BACK:
			return QK_ESCAPE;
		case AKEYCODE_DEL:
			return QK_BACKSPACE;
		case AKEYCODE_ALT_LEFT:
			return QK_CTRL;
		case AKEYCODE_SHIFT_LEFT:
			return QK_SHIFT;
	}
	return 0;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
    struct engine* engine = (struct engine*)app->userData;
    switch (AInputEvent_getType(event))
	{
		case AINPUT_EVENT_TYPE_KEY:
		{
			int32_t nKeyAction = (AKeyEvent_getAction)(event);
			int32_t a_keycode = (AKeyEvent_getKeyCode)(event);
			int32_t a_scancode = (AKeyEvent_getScanCode)(event);
			int32_t a_metastate = (AKeyEvent_getMetaState)(event);
			int bAltPressed = (a_metastate & AMETA_ALT_ON)!=0;
			int nValue = 0;
			if( nKeyAction == AKEY_EVENT_ACTION_DOWN )
				nValue = 1;
			int nQuakeKey = androidKeyCodeToQuake(event);
			if (nQuakeKey != 0)
				QueueKeyEvent(nQuakeKey, nValue);
		}
		return 1;
		
		case AINPUT_EVENT_TYPE_MOTION:
		{
			int32_t nSourceId = (AInputEvent_getSource)(event);
			if (nSourceId == AINPUT_SOURCE_TOUCHPAD)
			{
				//QueueJoystickEvent(0, AMotionEvent_getX(event,0));
				float fX = AMotionEvent_getX(event, 0);
				float fY = AMotionEvent_getY(event, 0);
				fX = fX/966.0f*854.0f;
				fY = 480.0f-fY/360.0f*480.0f;
				QueueMotionEvent((AKeyEvent_getAction)(event), fX, fY, 1);
			}
			else
			{
				//engine->animating = 1;
				//engine->state.x = AMotionEvent_getX(event, 0);
				//engine->state.y = AMotionEvent_getY(event, 0);
				QueueMotionEvent((AKeyEvent_getAction)(event), AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0), 1);
			}
		}
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
	static int bInit = 0;
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) 
	{
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            //engine->app->savedState = malloc(sizeof(struct saved_state));
            //*((struct saved_state*)engine->app->savedState) = engine->state;
            //engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) 
			{
                engine_init_display(engine);
				if (!bInit)
				{
					InitGame(engine->width, engine->height);
					bInit = 1;
				}
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
			exit(0);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) 
			{
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
				engine->animating = 1;
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) 
			{
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) 
{
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

	// Load library
	OnLoad();
	
    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) 
	{
        // We are starting with a previous saved state; restore from it.
        //engine.state = *(struct saved_state*)state->savedState;
    }

    // loop waiting for stuff to do.

    while (1) 
	{
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) 
		{
			// Process this event.
            if (source != NULL) 
			{
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) 
			{
                if (engine.accelerometerSensor != NULL) 
				{
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) 
					{
                        /*LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);
								*/
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) 
			{
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) 
		{
            // Done with events; draw next animation frame.
            /*engine.state.angle += .01f;
            if (engine.state.angle > 1) 
			{
                engine.state.angle = 0;
            }*/

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
