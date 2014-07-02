
package net.homeip.ofn.androix;

import android.util.Log;

public class AndroiXLib implements Runnable {
    public native void init(int screenWidth, int screenHeight);
    public native void keyDown(int kbd, int keyCode);
    public native void keyUp(int kbd, int keyCode);
    public native void touchDown(int mouse, int x, int y);
    public native void touchMove(int mouse, int x, int y);
    public native void touchUp(int mouse, int x, int y);
    public native void trackballNormalizedMotion(int ball, float x, float y);
    public native void trackballPress(int ball);
    public native void trackballRelease(int ball);

    private int screenWidth;
    private int screenHeight;
    
    public AndroiXLib(int width, int height) {
    	screenWidth = width;
    	screenHeight = height;
    }
    
    public void run() {
    	Log.i("AndroiX","in AndroiXLib main");
    	init(screenWidth, screenHeight);
    }

    static
    {
        System.loadLibrary("androix");
    }
}
