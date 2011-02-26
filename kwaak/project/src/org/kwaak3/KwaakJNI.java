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

/*
 * This file contains the interface to the native C code.
 */

package org.kwaak3;

public class KwaakJNI {
	public static native void enableAudio(boolean enable);
	public static native void enableBenchmark(boolean enable);
	public static native void enableLightmaps(boolean enable);
	public static native void setAudio(KwaakAudio audio);
	public static native void showFramerate(boolean enable);
	
	/* Initialize the game engine */
	public static native void initGame(int width, int height);

	public static native void setLibraryDirectory(String path);

	/* Compute and draw a new frame */
	public static native void drawFrame();

	/* Keyboard and motion input */
	public static native void queueKeyEvent(int key, int state);
	public static native void queueMotionEvent(int action, float x, float y, float pressure);
	public static native void queueTrackballEvent(int action, float x, float y);

	public static native void requestAudioData();

	static {
		System.loadLibrary("kwaakjni");
	}
}

