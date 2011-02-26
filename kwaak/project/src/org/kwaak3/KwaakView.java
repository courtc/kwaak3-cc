/*
 * Kwaak3
 * Copyright (C) 2010 Roderick Colenbrander
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package org.kwaak3;

import java.io.IOException;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

class KwaakView extends GLSurfaceView {
	private KwaakRenderer mKwaakRenderer;
	
	public KwaakView(Context context){
		super(context);

		/* We need the path to the library directory for dlopen in our JNI library */
		String cache_dir, lib_dir;
		try {
			cache_dir = context.getCacheDir().getCanonicalPath();
			lib_dir = cache_dir.replace("cache", "lib");
		} catch (IOException e) {
			e.printStackTrace();
			lib_dir = "/data/data/org.kwaak3/lib";
		}
		KwaakJNI.setLibraryDirectory(lib_dir);
		
		mKwaakRenderer = new KwaakRenderer();
		setRenderer(mKwaakRenderer);

		setFocusable(true);
		setFocusableInTouchMode(true);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		int qKeyCode = androidKeyCodeToQuake(keyCode, event);
		//Log.d("Quake_JAVA", "onKeyDown=" + keyCode + " " + qKeyCode + " " + event.getDisplayLabel() + " " + event.getUnicodeChar() + " " + event.getNumber());
		return queueKeyEvent(qKeyCode, 1);
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		int qKeyCode = androidKeyCodeToQuake(keyCode, event);
		//Log.d("Quake_JAVA", "onKeyUp=" + keyCode + " " + qKeyCode + " shift=" + event.isShiftPressed() + " =" + event.getMetaState());
		return queueKeyEvent(qKeyCode, 0);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		//Log.d("Quake_JAVA", "onTouchEvent action=" + event.getAction() + " x=" + event.getX() + " y=" + event.getY() + " pressure=" + event.getPressure() + "size = " + event.getSize());
		/* Perhaps we should pass integers down to reduce the number of float computations */
		return queueMotionEvent(event.getAction(), event.getX(), event.getY(), event.getPressure());
	}

	@Override
	public boolean onTrackballEvent(MotionEvent event) {
		//Log.d("Quake_JAVA", "onTrackballEvent action=" + event.getAction() + " x=" + event.getX() + " y=" + event.getY());
		return queueTrackballEvent(event.getAction(), event.getX(), event.getY());
	}

	public boolean queueKeyEvent(final int qKeyCode, final int state)
	{
		if(qKeyCode == 0) return true;

		/* Make sure all communication with Quake is done from the Renderer thread */
        queueEvent(new Runnable(){
            public void run() {
        		KwaakJNI.queueKeyEvent(qKeyCode, state);
            }});
        return true;
	}

	private boolean queueMotionEvent(final int action, final float x, final float y, final float pressure)
	{
		/* Make sure all communication with Quake is done from the Renderer thread */
        queueEvent(new Runnable(){
            public void run() {
        		KwaakJNI.queueMotionEvent(action, x, y, pressure);
            }});
        return true;
	}
	
	private boolean queueTrackballEvent(final int action, final float x, final float y)
	{
		/* Make sure all communication with Quake is done from the Renderer thread */
        queueEvent(new Runnable(){
            public void run() {
        		KwaakJNI.queueTrackballEvent(action, x, y);
            }});
        return true;
	}

	private static final int QK_TAB			=   9;
	private static final int QK_ENTER		=  13;
	private static final int QK_ESCAPE		=  27;
	private static final int QK_SPACE		=  32;
	private static final int QK_BACKSPACE		= 127;
	private static final int QK_COMMAND		= 128;
	private static final int QK_CAPSLOCK		= 129;
	private static final int QK_POWER		= 130;
	private static final int QK_PAUSE		= 131;
	private static final int QK_UPARROW		= 132;
	private static final int QK_DOWNARROW		= 133;
	private static final int QK_LEFTARROW		= 134;
	private static final int QK_RIGHTARROW		= 135;
	private static final int QK_ALT			= 136;
	private static final int QK_CTRL		= 137;
	private static final int QK_SHIFT		= 138;
	private static final int QK_INS			= 139;
	private static final int QK_DEL			= 140;
	private static final int QK_PGDN		= 141;
	private static final int QK_PGUP		= 142;
	private static final int QK_HOME		= 143;
	private static final int QK_END			= 144;
	private static final int QK_F1			= 145;
	private static final int QK_F2			= 146;
	private static final int QK_F3			= 147;
	private static final int QK_F4			= 148;
	private static final int QK_F5			= 149;
	private static final int QK_F6			= 150;
	private static final int QK_F7			= 151;
	private static final int QK_F8			= 152;
	private static final int QK_F9			= 153;
	private static final int QK_F10			= 154;
	private static final int QK_F11			= 155;
	private static final int QK_F12			= 156;
	private static final int QK_F13			= 157;
	private static final int QK_F14			= 158;
	private static final int QK_F15			= 159;
	private static final int QK_KP_HOME		= 160;
	private static final int QK_KP_UPARROW		= 161;
	private static final int QK_KP_PGUP		= 162;
	private static final int QK_KP_LEFTARROW	= 163;
	private static final int QK_KP_5		= 164;
	private static final int QK_KP_RIGHTARROW	= 165;
	private static final int QK_KP_END		= 166;
	private static final int QK_KP_DOWNARROW	= 167;
	private static final int QK_KP_PGDN		= 168;
	private static final int QK_KP_ENTER		= 169;
	private static final int QK_KP_INS		= 170;
	private static final int QK_KP_DEL		= 171;
	private static final int QK_KP_SLASH		= 172;
	private static final int QK_KP_MINUS		= 173;
	private static final int QK_KP_PLUS		= 174;
	private static final int QK_KP_NUMLOCK		= 175;
	private static final int QK_KP_STAR		= 176;
	private static final int QK_KP_EQUALS		= 177;
	private static final int QK_MOUSE1		= 178;
	private static final int QK_MOUSE2		= 179;
	private static final int QK_MOUSE3		= 180;
	private static final int QK_MOUSE4		= 181;
	private static final int QK_MOUSE5		= 182;
	private static final int QK_MWHEELDOWN		= 183;
	private static final int QK_MWHEELUP		= 184;
	private static final int QK_JOY1		= 185;
	private static final int QK_JOY2		= 186;
	private static final int QK_JOY3		= 187;
	private static final int QK_JOY4		= 188;
	private static final int QK_JOY5		= 189;
	private static final int QK_JOY6		= 190;
	private static final int QK_JOY7		= 191;
	private static final int QK_JOY8		= 192;
	private static final int QK_JOY9		= 193;
	private static final int QK_JOY10		= 194;
	private static final int QK_JOY11		= 195;
	private static final int QK_JOY12		= 196;
	private static final int QK_JOY13		= 197;
	private static final int QK_JOY14		= 198;
	private static final int QK_JOY15		= 199;
	private static final int QK_JOY16		= 200;
	private static final int QK_JOY17		= 201;
	private static final int QK_JOY18		= 202;
	private static final int QK_JOY19		= 203;
	private static final int QK_JOY20		= 204;
	private static final int QK_JOY21		= 205;
	private static final int QK_JOY22		= 206;
	private static final int QK_JOY23		= 207;
	private static final int QK_JOY24		= 208;
	private static final int QK_JOY25		= 209;
	private static final int QK_JOY26		= 210;
	private static final int QK_JOY27		= 211;
	private static final int QK_JOY28		= 212;
	private static final int QK_JOY29		= 213;
	private static final int QK_JOY30		= 214;
	private static final int QK_JOY31		= 215;
	private static final int QK_JOY32		= 216;
	private static final int QK_AUX1		= 217;
	private static final int QK_AUX2		= 218;
	private static final int QK_AUX3		= 219;
	private static final int QK_AUX4		= 220;
	private static final int QK_AUX5		= 221;
	private static final int QK_AUX6		= 222;
	private static final int QK_AUX7		= 223;
	private static final int QK_AUX8		= 224;
	private static final int QK_AUX9		= 225;
	private static final int QK_AUX10		= 226;
	private static final int QK_AUX11		= 227;
	private static final int QK_AUX12		= 228;
	private static final int QK_AUX13		= 229;
	private static final int QK_AUX14		= 230;
	private static final int QK_AUX15		= 231;
	private static final int QK_AUX16		= 232;
	private static final int QK_WORLD_0		= 233;
	private static final int QK_WORLD_1		= 234;
	private static final int QK_WORLD_2		= 235;
	private static final int QK_WORLD_3		= 236;
	private static final int QK_WORLD_4		= 237;
	private static final int QK_WORLD_5		= 238;
	private static final int QK_WORLD_6		= 239;
	private static final int QK_WORLD_7		= 240;
	private static final int QK_WORLD_8		= 241;
	private static final int QK_WORLD_9		= 242;
	private static final int QK_WORLD_10		= 243;
	private static final int QK_WORLD_11		= 244;
	private static final int QK_WORLD_12		= 245;
	private static final int QK_WORLD_13		= 246;
	private static final int QK_WORLD_14		= 247;
	private static final int QK_WORLD_15		= 248;
	private static final int QK_WORLD_16		= 249;
	private static final int QK_WORLD_17		= 250;
	private static final int QK_WORLD_18		= 251;
	private static final int QK_WORLD_19		= 252;
	private static final int QK_WORLD_20		= 253;
	private static final int QK_WORLD_21		= 254;
	private static final int QK_WORLD_22		= 255;
	private static final int QK_WORLD_23		= 256;
	private static final int QK_WORLD_24		= 257;
	private static final int QK_WORLD_25		= 258;
	private static final int QK_WORLD_26		= 259;
	private static final int QK_WORLD_27		= 260;
	private static final int QK_WORLD_28		= 261;
	private static final int QK_WORLD_29		= 262;
	private static final int QK_WORLD_30		= 263;
	private static final int QK_WORLD_31		= 264;
	private static final int QK_WORLD_32		= 265;
	private static final int QK_WORLD_33		= 266;
	private static final int QK_WORLD_34		= 267;
	private static final int QK_WORLD_35		= 268;
	private static final int QK_WORLD_36		= 269;
	private static final int QK_WORLD_37		= 270;
	private static final int QK_WORLD_38		= 271;
	private static final int QK_WORLD_39		= 272;
	private static final int QK_WORLD_40		= 273;
	private static final int QK_WORLD_41		= 274;
	private static final int QK_WORLD_42		= 275;
	private static final int QK_WORLD_43		= 276;
	private static final int QK_WORLD_44		= 277;
	private static final int QK_WORLD_45		= 278;
	private static final int QK_WORLD_46		= 279;
	private static final int QK_WORLD_47		= 280;
	private static final int QK_WORLD_48		= 281;
	private static final int QK_WORLD_49		= 282;
	private static final int QK_WORLD_50		= 283;
	private static final int QK_WORLD_51		= 284;
	private static final int QK_WORLD_52		= 285;
	private static final int QK_WORLD_53		= 286;
	private static final int QK_WORLD_54		= 287;
	private static final int QK_WORLD_55		= 288;
	private static final int QK_WORLD_56		= 289;
	private static final int QK_WORLD_57		= 290;
	private static final int QK_WORLD_58		= 291;
	private static final int QK_WORLD_59		= 292;
	private static final int QK_WORLD_60		= 293;
	private static final int QK_WORLD_61		= 294;
	private static final int QK_WORLD_62		= 295;
	private static final int QK_WORLD_63		= 296;
	private static final int QK_WORLD_64		= 297;
	private static final int QK_WORLD_65		= 298;
	private static final int QK_WORLD_66		= 299;
	private static final int QK_WORLD_67		= 300;
	private static final int QK_WORLD_68		= 301;
	private static final int QK_WORLD_69		= 302;
	private static final int QK_WORLD_70		= 303;
	private static final int QK_WORLD_71		= 304;
	private static final int QK_WORLD_72		= 305;
	private static final int QK_WORLD_73		= 306;
	private static final int QK_WORLD_74		= 307;
	private static final int QK_WORLD_75		= 308;
	private static final int QK_WORLD_76		= 309;
	private static final int QK_WORLD_77		= 310;
	private static final int QK_WORLD_78		= 311;
	private static final int QK_WORLD_79		= 312;
	private static final int QK_WORLD_80		= 313;
	private static final int QK_WORLD_81		= 314;
	private static final int QK_WORLD_82		= 315;
	private static final int QK_WORLD_83		= 316;
	private static final int QK_WORLD_84		= 317;
	private static final int QK_WORLD_85		= 318;
	private static final int QK_WORLD_86		= 319;
	private static final int QK_WORLD_87		= 320;
	private static final int QK_WORLD_88		= 321;
	private static final int QK_WORLD_89		= 322;
	private static final int QK_WORLD_90		= 323;
	private static final int QK_WORLD_91		= 324;
	private static final int QK_WORLD_92		= 325;
	private static final int QK_WORLD_93		= 326;
	private static final int QK_WORLD_94		= 327;
	private static final int QK_WORLD_95		= 328;
	private static final int QK_SUPER		= 329;
	private static final int QK_COMPOSE		= 330;
	private static final int QK_MODE		= 331;
	private static final int QK_HELP		= 332;
	private static final int QK_PRINT		= 333;
	private static final int QK_SYSREQ		= 334;
	private static final int QK_SCROLLOCK		= 335;
	private static final int QK_BREAK		= 336;
	private static final int QK_MENU		= 337;
	private static final int QK_EURO		= 338;
	private static final int QK_UNDO		= 339;
	private static final int QK_CONSOLE		= 340;
	
	private int androidKeyCodeToQuake(int aKeyCode, KeyEvent event)
	{	
		/* Convert non-ASCII keys by hand */
		switch(aKeyCode)
		{
			case KeyEvent.KEYCODE_FOCUS:
				return QK_F1;
			case KeyEvent.KEYCODE_VOLUME_DOWN:
				return QK_F2;
			case KeyEvent.KEYCODE_VOLUME_UP:
				return QK_F3;
			case KeyEvent.KEYCODE_DPAD_UP:
				return QK_UPARROW;
			case KeyEvent.KEYCODE_DPAD_DOWN:
				return QK_DOWNARROW;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				return QK_LEFTARROW;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				return QK_RIGHTARROW;
			case KeyEvent.KEYCODE_DPAD_CENTER:
				return QK_ENTER;
			case KeyEvent.KEYCODE_BUTTON_L1:
				return QK_SPACE;
			case KeyEvent.KEYCODE_BUTTON_R1:
				return QK_CTRL;
			case KeyEvent.KEYCODE_SEARCH:
				return QK_CONSOLE;
			case KeyEvent.KEYCODE_BACK:
				return QK_ESCAPE;
			case KeyEvent.KEYCODE_DEL:
				return QK_BACKSPACE;
			case KeyEvent.KEYCODE_ALT_LEFT:
				return QK_CTRL;
			case KeyEvent.KEYCODE_SHIFT_LEFT:
				return QK_SHIFT;
		}

		/* Let Android do all the character conversion for us. This way we don't have
		 * to care about modifier keys and specific keyboard layouts.
		 * TODO: add some more filtering
		 */
		int uchar = event.getUnicodeChar();
		if (uchar < 127)
			return uchar;
		return QK_WORLD_0 + aKeyCode;
	}
}
